#include "qfile.h"
#include "mdl.h"
#include "Lexicon.h"
#include "WordCollection.h"
#include  "log2.h"


// ==============================  MDL ============================================  //


double CLexicon::compute_MDL(){

    /*
    Grammar:
    List of affixes, each with a pointer to it and each with phonologicla content;
    List of signatures:
        List of pointers to affixes
        List of stems, with each stem's content

    List of words, with token counts
    Total number of word topens

    Derived:
    Number of word types
    Number of word tokens
    Count of analyzed word types
    Count of analyzed word tokens


    */

    MDL MDL(this);

    QFile distributions("MDL_distributions.txt");
    distributions.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out_distributions(&distributions);

    QFile summaries("MDL_summary_reports.txt");
    summaries.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out_summaries(&summaries);

    QFile corpus_costs("MDL_report_on_corpus_costs.txt");
    corpus_costs.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out_corpus(&corpus_costs);

    QFile grammar_file("MDL_grammar.txt");
    grammar_file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out_grammar(&grammar_file);



    CSignatureCollection * pSigs;
    if (m_suffix_flag){
        pSigs = m_Signatures;
    } else{
        pSigs = m_PrefixSignatures;
    }
    //------------------------------------------------------//


    pSigs->sort(SIGS);
    MDL.reports(out_summaries);
    MDL.process_signatures(pSigs, out_distributions, out_summaries);
    MDL.compute_stem_pointer_lengths_within_signature_Choice(out_distributions, out_summaries);
    MDL.compute_signature_pointer_lengths_Choice(out_distributions, out_summaries);
    MDL.process_stems_Content( out_distributions, out_summaries);
    MDL.process_unanalyzed_words(out_distributions, out_summaries);
    MDL.weigh_analyzed_and_unanalyzed_words(out_distributions, out_summaries);
    MDL.process_affixes(out_distributions, out_summaries);
    MDL.process_signatures_bis_Architecture(out_distributions, out_summaries);
    MDL.corpus_description(out_corpus, out_summaries);
    out_summaries << "Final report." << Qt::endl;
    MDL.reports(out_summaries);
    MDL.print_grammar(out_grammar);

    grammar_file.close();

    return MDL.this_MDL;
}


MDL::MDL(CLexicon* lexicon)
{
    m_Lexicon = lexicon;
    m_suffix_flag = lexicon->get_suffix_flag();

    total_type_count_of_words = 0;
    m_total_token_count_of_words = 0;

    m_type_count_of_unanalyzed_words = 0.0;
    m_token_count_of_unanalyzed_words= 0;

    m_token_word_count_in_all_signatures = 0;
    type_word_count_in_all_signatures = 0;

    m_total_flagging_of_analyzed_words=0.0;
    m_total_flagging_of_unanalyzed_words=0.0;

    this_MDL = 0.0;
    m_corpus_description_from_analyzed_words = 0.0;
    m_corpus_description_from_unanalyzed_words= 0.0;
    bits_per_letter = 0.0;

    total_letter_count_in_affixes = 0.0;
    total_letter_count_in_stems = 0.0;

    signature_to_affix_pointers_Architexture_8 = 0.0;
    stem_content_in_bits = 0.0;
    stem_plus_affix_content_in_bits_Architecture_1 = 0.0;
    grammar_length_in_bits = 0.0;

    choice_of_stems_within_sigs_Choice_4 = 0.0;

    total_cost_of_affix_choice_within_all_signatures_Choice_2 = 0.0;
}
/*
void MDL::analyze_all_words(){
    QString word, signature, affix;
    QMapIterator<QString, QString> iter1(word2sig);
    while (iter1.hasNext()){
    }
}
*/
double MDL::get_grammar_length_in_bits(){
    return  stem_plus_affix_content_in_bits_Architecture_1 +
            total_cost_of_affix_choice_within_all_signatures_Choice_2 +
            pointers_to_signatures_Choice_3 +
            choice_of_stems_within_sigs_Choice_4 +
            m_total_flagging_of_analyzed_words +
            m_total_flagging_of_unanalyzed_words +
            signature_to_affix_pointers_Architexture_8;
}
// #1
void MDL::process_signatures(CSignatureCollection* sigs, QTextStream & out_distributions, QTextStream & out_summaries){
    for (int signo =0; signo < sigs->get_count(); signo++){
       CSignature* sig =  sigs->get_at_sorted(signo);
       process_a_signature(sig,out_distributions, out_summaries);
    }
    out_summaries << "1 process signatures" << Qt::endl;
    reports(out_summaries);
}
// #1a
void MDL::process_a_signature(CSignature* sig,QTextStream & out_distributions, QTextStream& out_summaries){
    bool verbose_flag (true);
    QString sigstring = sig->get_key();
    if (!signatures.contains(sigstring)){
        signatures.insert(sigstring,signatures.count());
    }
    signature2corpus_count[sigstring] = 0;
    signature2stems[sigstring] = new QStringList;

    QStringList stem_list;
    m_affix2corpus_count_given_signature[sigstring] = new QMap<QString, double>;
    foreach(QString stem, sig->get_stem_strings(stem_list)){
        process_a_stem_given_a_signature_and_its_words(sig, stem, out_summaries);
    }
    if (verbose_flag){out_distributions <<  "1a Choice of affixes within a signature." << Qt::endl;}
    double running_total = 0;
    m_affix2pointer_length_affix_choice_within_signature[sigstring] = new QMap<QString, double>;
    out_distributions <<
           qSetFieldWidth(10) <<  sigstring <<
           qSetFieldWidth(10) <<  "count" <<
           qSetFieldWidth(10) <<  "ratio" <<
           qSetFieldWidth(15) <<  "running total" <<
           qSetFieldWidth(20) <<  "pointer length" <<
           qSetFieldWidth(0) << Qt::endl;

    foreach(QString affix, m_affix2corpus_count_given_signature[sigstring]->keys()){
        int count =  m_affix2corpus_count_given_signature[sigstring]->value(affix);
        double ratio =   double(count)/ double(signature2corpus_count[sigstring]);
        double pointer_length = -1.0 * base2log( ratio );
        m_affix2pointer_length_affix_choice_within_signature[sigstring]->insert(affix, pointer_length); // this variable is only affected here
        total_cost_of_affix_choice_within_all_signatures_Choice_2 += pointer_length;
        running_total += ratio;
        if (verbose_flag){
             QString affix1;
             if (affix == "") {affix1 = "NULL";} else {affix1 = affix;}
             out_distributions <<
                    qSetFieldWidth(10) <<  affix1 <<
                    qSetFieldWidth(10) <<  count <<
                    qSetFieldWidth(10) <<  ratio <<
                    qSetFieldWidth(15) <<  running_total <<
                    qSetFieldWidth(20) <<  pointer_length <<
                    qSetFieldWidth(0) << Qt::endl;
         }
    }
    if (verbose_flag){out_distributions << Qt::endl<< "    -------------------------------------------------" << Qt::endl;}
}
QString build_a_word(bool suffix_flag, QString stem, QString affix){
    if (affix == "NULL" ) {affix = "";}
    if (suffix_flag){
       return  stem + affix;
    } else{
       return affix + stem;
    }
}
void MDL::add_letters_to_alphabet(QString word){
    foreach(QString letter, word){
        if (!alphabet.contains(letter)){
            alphabet[letter]=1;
        }else{
            alphabet[letter] += 1;
        }
    }
}
// #1b
void MDL::process_a_stem_given_a_signature_and_its_words( CSignature * sig, QString stem, QTextStream& out_summaries){
    QString word, sigstring;
    sigstring = sig->get_key();
    foreach (QString affix, sig->get_affix_string_list()){
         QString word = build_a_word(m_suffix_flag, stem, affix);
         CWord * pWord = m_Lexicon->get_words()->get_word(word);
         if (!pWord){
             out_summaries << "process a stem given a signature:" << word << " doesn't exist!";
             continue;
         }
         if (m_word_token_counts.contains(word)){
             // this means we have already analyzed this word in a prior signature, and we ignore it now
             // As a consequence a word is only analyzed once.
             // This raises questions!! How can we ignore one word for a stem but not others for the same stem?
             // This is how we guarantee a word has only one analysis in this current implementation.
             // However I am not sure that this is not creating problems.
         } else{
             int this_word_token_count = pWord->get_word_count();
             adjust_word_token_count(word, this_word_token_count);                                           // stems is affected only here.
             adjust_stem_token_count(stem, this_word_token_count);
             adjust_affix_token_count_For_Choice(affix, word, this_word_token_count, sigstring);

             if (! signature2stems[sigstring]->contains(stem)){
                signature2stems[sigstring]->append(stem);
             }
             qDebug() << 157 << word << "analyzed";
             word2sig[word]   = sigstring;
             word2stem[word]  = stem;
             word2affix[word] = affix;
             signature2corpus_count[sigstring] += this_word_token_count;   // this variable is changed only here.
             add_letters_to_alphabet(word);
         }
    }
    total_letter_count_in_stems += stem.length() + 1;
}
// #1c
void MDL::adjust_stem_token_count(QString stem, int word_count )
{   if (!stem2corpus_count.contains(stem)){
        stem2corpus_count[stem] = 0;
    }
    stem2corpus_count[stem] += word_count;
    stems[stem] += 1;
}
// #1d
void MDL::adjust_affix_token_count_For_Choice(QString affix, QString word, int this_word_token_count, QString sigstring){
    if (!m_affix2corpus_count_given_signature.contains(sigstring)){
        m_affix2corpus_count_given_signature[sigstring]= new QMap<QString, double>;
    }
    if (!m_affix2corpus_count_given_signature[sigstring]->contains(affix)){
        m_affix2corpus_count_given_signature[sigstring]->insert(affix,0);
    }
    double old_value = m_affix2corpus_count_given_signature[sigstring]->value(affix);
    m_affix2corpus_count_given_signature[sigstring]->insert(affix,old_value + m_word_token_counts[word]);
    m_affixes[affix] += 1;

    if (!m_affix2corpus_count.contains(affix)){ m_affix2corpus_count[affix] = 0.0;}
    m_affix2corpus_count[affix] += this_word_token_count;

}
// #1e
void MDL::adjust_word_token_count(QString word, int word_token_count){
    type_word_count_in_all_signatures += 1;                        // this variable is changed only here.
    m_word_token_counts[word] = word_token_count;             // analyzed words are affected here; unanalyzed words are affected elsewhere.
    m_token_word_count_in_all_signatures += word_token_count; // this variable is changed only here
}
CSignatureCollection* MDL::get_signatures(){
    if (m_suffix_flag){
        return  m_Lexicon->get_signatures();
    }else {
        return m_Lexicon->get_prefix_signatures();
    }
}
// #2
void MDL::compute_stem_pointer_lengths_within_signature_Choice(QTextStream& out_distributions,  QTextStream& out_summaries){
    out_distributions << Qt::endl << "2. Compute stem pointer lengths (choice)." << Qt::endl;
    //bool verbose_flag(true);
    CSignatureCollection * sigs = get_signatures();
    out_distributions <<
           qSetFieldWidth(15)  << "" <<
           qSetFieldWidth(15)  << "stem"<<
           qSetFieldWidth(10 )  << "count"<<
           qSetFieldWidth(12)   << "ratio" <<
           qSetFieldWidth(15)   << "running total" <<
           qSetFieldWidth(20)   << "pointer length" <<
           qSetFieldWidth(0  )   << Qt::endl;
    for (int signo =0; signo< sigs->get_count(); signo++){
       CSignature* sig = sigs->get_at_sorted(signo);
       QString sigstring = sig->get_key();
       QStringList stem_list;
       double running_ratio = 0.0;
       foreach(QString stem, sig->get_stem_strings(stem_list)){
           if (signature2corpus_count[sigstring] == 0){continue;}
           int count = stem2corpus_count[stem];
           double ratio (count / signature2corpus_count[sigstring] );
           if (ratio == 0){continue;}
           running_ratio += ratio;
           double pointer_length = -1.0 * base2log(ratio);
           choice_of_stems_within_sigs_Choice_4 += pointer_length;
           stem2pointer_length_given_signature[stem]= pointer_length;
           //out.setFieldAlignment(QTextStream::AlignLeft);
           out_distributions <<
                  qSetFieldWidth(15)  <<  sigstring <<
                  qSetFieldWidth(15)  << stem <<
                  qSetFieldWidth(10 )  << count <<
                  qSetFieldWidth(12)   << ratio <<
                  qSetFieldWidth(15)   << running_ratio <<
                  qSetFieldWidth(20)   << pointer_length <<
                  qSetFieldWidth(0  )   << Qt::endl;
       }
       out_distributions << Qt::endl;
    }
    out_distributions << "----------------------------------------" << Qt::endl;

    out_summaries << "2 compute stem pointers within signatures (choice)" << Qt::endl;
    reports(out_summaries);
}
// #3
void MDL::compute_signature_pointer_lengths_Choice(QTextStream & out_distributions, QTextStream & out_summaries){
    bits_per_letter = base2log(alphabet.size());

    // 1b compute pointer length to each signature.
    // Add to running  the cost of each signature-pointer.
    // This means that the major loop is over signatures.
    // And that means only one occurrence of a pointer to each signature, rather than
    // One pointer to a signature for each analyzed word in the word-list.
    // Also, add an end mark for the list of signatures (sentry).

    double running_total = 0.0;
    out_distributions << "3. Compute signature pointer lengths (choice of signature)"<< Qt::endl<<Qt::endl;
    pointers_to_signatures_Choice_3 = 0;
    out_distributions <<
    qSetFieldWidth(20)  <<  "signature" <<
    qSetFieldWidth(10)  <<  "count" <<
    qSetFieldWidth(12)  <<  "ratio" <<
    qSetFieldWidth(15)  <<  "running total" <<
    qSetFieldWidth(20) <<  "pointer length" <<
    qSetFieldWidth(0) << Qt::endl;
    foreach (QString sigstring, signature2corpus_count.keys()){
       if (signature2corpus_count[sigstring] == 0){
           out_distributions << ">> No members! " << sigstring << Qt::endl<<Qt::endl; // TODO
           continue;
       }
       int count = signature2corpus_count[sigstring];
       double fraction = count / m_token_word_count_in_all_signatures;
       double this_sig_pointer_length = -1* base2log(fraction);
       signature_plogs[sigstring] = this_sig_pointer_length; // information content on signature pointer is based on corpus-counts of signatures
       running_total += fraction;
       pointers_to_signatures_Choice_3 += this_sig_pointer_length;
       out_distributions <<
               qSetFieldWidth(20)  <<  sigstring <<
               qSetFieldWidth(10)  <<  count <<
               qSetFieldWidth(12)  <<  fraction <<
               qSetFieldWidth(15)  <<  running_total <<
               qSetFieldWidth(20) <<  this_sig_pointer_length <<
               //qSetFieldWidth(10) << pointers_to_signatures_Choice_3 <<
               qSetFieldWidth(0) << Qt::endl;
    }
    out_distributions << Qt::endl <<  "-------------------------------------"<< Qt::endl;
    out_summaries << "3. compute signature pointer length (choice)" << Qt::endl;
    reports(out_summaries);
}
// #4
void MDL::process_stems_Content(QTextStream & out, QTextStream& out_summaries){
    Q_UNUSED(out);
    foreach (QString sigstring, signature2corpus_count.keys()) {
        //out << "Process stems " << sigstring << Qt::endl;
        foreach (QString stem, *signature2stems[sigstring]){
            double bits = (stem.length() + 1) * bits_per_letter;
            stem_content_in_bits += bits;
        }
    }
    stem_plus_affix_content_in_bits_Architecture_1 += stem_content_in_bits;
    out_summaries << "4. Process stems (content)" << Qt::endl;
    reports(out_summaries);
}
// # 5
void MDL::process_unanalyzed_words(QTextStream & out_distributions, QTextStream & out_summaries){
    bool verbose_flag = true;
    // Unanalyzed words
    //double unanalyzed_words_bits = 0.0;
    double running_word_bits = 0.0;
    out_distributions <<   Qt::endl;
    out_distributions << Qt::endl<< "5. Process unanalyzed words." << Qt::endl;
    m_token_count_of_unanalyzed_words = 0;
    m_type_count_of_unanalyzed_words =0;
    for (int i = 0; i < m_Lexicon->get_words()->get_count(); i++){
        CWord* pWord = m_Lexicon->get_words()->get_word(i);
        if (! word2sig.contains(pWord->get_key())){
            qDebug() << 321 << "unanalyzed";
            m_token_count_of_unanalyzed_words += pWord->get_word_count();
            m_type_count_of_unanalyzed_words++;
        }
    }

    double running_total = 0.0;
    out_distributions <<
            qSetFieldWidth(20)  <<  "word" <<

            qSetFieldWidth(10)  <<  "count" <<
            qSetFieldWidth(12)  <<  "ratio" <<
            qSetFieldWidth(15)  <<  "running total" <<
            qSetFieldWidth(20) <<  "word pointer length" <<
            qSetFieldWidth(0) << Qt::endl;
    for (int i = 0; i < m_Lexicon->get_words()->get_word_count(); i++){
        CWord* pWord = m_Lexicon->get_words()->get_word(i);
        //bool analyzed_flag = false;
        QString word = pWord->get_key();
        if (! word2sig.contains(word) ){
            int count = pWord->get_word_count();
            m_word_token_counts[word] = count; // unanalyzed words are affected here; analyzed words are affected elsewhere
            unanalyzed_words.insert(word, unanalyzed_words.count());
            double ratio = count / m_token_count_of_unanalyzed_words;
            double this_word_pointer_length = -1.0 * base2log(ratio);
            running_total += ratio;
            double word_bits = (word.length()+1) * bits_per_letter;
            running_word_bits += word_bits;

            if (verbose_flag){
                out_distributions <<
                        qSetFieldWidth(20)  <<  word <<
                        qSetFieldWidth(10)  <<  count <<
                        qSetFieldWidth(12)  <<  ratio <<
                        qSetFieldWidth(15)  <<  running_total <<
                        qSetFieldWidth(20) <<  this_word_pointer_length <<
                        qSetFieldWidth(0) << Qt::endl;
             }
         }
    }
    this_MDL += running_word_bits;

    out_summaries << "5. Process unanalyzed words"<<Qt::endl;
    reports(out_summaries);
}
// #6
void MDL::weigh_analyzed_and_unanalyzed_words(QTextStream& out_distributions, QTextStream& out_summaries){
    out_distributions << Qt::endl << "---------------------------------------"<< Qt::endl <<
                      "6. Weigh analyzed and unanalyzed words"<< Qt::endl;
    double ratio1, ratio2;
    ratio1= 0;
    ratio2= 0;
    m_total_flagging_of_analyzed_words=0.0;
    m_total_flagging_of_unanalyzed_words=0.0;
    if (m_total_token_count_of_words){
        ratio1 = m_token_word_count_in_all_signatures / m_total_token_count_of_words;
        ratio2 = m_token_count_of_unanalyzed_words / m_total_token_count_of_words;
    }
    if (ratio1 >  0.0 && ratio2 > 0.0){
        m_flag_to_mark_an_analyzed_word  = -1.0 * base2log(ratio1);
        m_flag_to_mark_an_unanalyzed_word = -1.0 * base2log(ratio2);
        m_total_flagging_of_analyzed_words   =  m_token_word_count_in_all_signatures * m_flag_to_mark_an_analyzed_word ;
        m_total_flagging_of_unanalyzed_words =  m_token_count_of_unanalyzed_words * m_flag_to_mark_an_unanalyzed_word;
    } else{
        m_flag_to_mark_an_analyzed_word = 0.0;
        m_flag_to_mark_an_analyzed_word = 0.0;
        m_total_flagging_of_analyzed_words = 0.0;
        m_total_flagging_of_unanalyzed_words  = 0.0;
    }

    if (true){
        out_distributions << Qt::left <<  Qt::endl << Qt::endl <<
              qSetFieldWidth(30)  << "Analyzed words " <<
              qSetFieldWidth(15)  << m_token_word_count_in_all_signatures <<
              qSetFieldWidth(15)  << ratio1 <<
              qSetFieldWidth(15)  << ratio1 <<
              qSetFieldWidth(15)  << m_flag_to_mark_an_analyzed_word <<
              qSetFieldWidth(15)  << m_total_flagging_of_analyzed_words <<
              qSetFieldWidth (0)  << Qt::endl;

              out_distributions   << Qt::left   <<
              qSetFieldWidth(30)  << "Unanalyzed words" <<
              qSetFieldWidth(15)  << m_token_count_of_unanalyzed_words <<
              qSetFieldWidth(15)  << ratio2 <<
              qSetFieldWidth(15)  << ratio1 + ratio2<<
              qSetFieldWidth(15)  << m_flag_to_mark_an_unanalyzed_word <<
              qSetFieldWidth(15)  << m_total_flagging_of_unanalyzed_words <<
              qSetFieldWidth (0)  << Qt::endl;


    }
    out_summaries << "6. weigh analyzed and unanalyzed words"<<Qt::endl;
    reports(out_summaries);
}



// #7

// todo : worry about whether type or token counts to be used here for pointer length --


void MDL::process_affixes(QTextStream & out_distributions, QTextStream& out_summaries){
    bool verbose_flag = true;
    if (verbose_flag){
        out_distributions << Qt::endl << "-------------------------------------" << Qt::endl
            <<"7. Process affixes (architectural) " << Qt::endl << Qt::endl;
    }
    affix_content_in_bits = 0.0;
    int index = 0;
    double running_total = 0.0;
    out_distributions <<
       qSetFieldWidth(15) << "affix" <<
       qSetFieldWidth(10) << "count" <<
       qSetFieldWidth(15) << "ratio" <<
       qSetFieldWidth(15) << "running total" <<
       qSetFieldWidth(20) << "affix pointer length"  <<
       qSetFieldWidth(0) << Qt::endl;
    for (auto iter = m_affixes.constBegin(); iter != m_affixes.constEnd(); ++iter){
        QString this_affix = iter.key();
        m_affix2index[this_affix] = index++;
        double this_affix_content_in_bits;
        double this_affix_use_count = iter.value();  // to do -- check this is the choice for variable
        //qDebug() << 438 << this_affix << this_affix_use_count;
        int this_letter_count;
        if (this_affix == "NULL") {
            this_letter_count = 1;
        } else{
            this_letter_count = this_affix.length() + 1;
        }
        total_letter_count_in_affixes += this_letter_count;
        double ratio =this_affix_use_count / type_word_count_in_all_signatures;
        running_total += ratio;
        m_affix_pointer_length[this_affix] = -1.0 * base2log( ratio );
        //qDebug() << 449 << this_affix << m_affix_pointer_length[this_affix];
        this_affix_content_in_bits = this_letter_count * bits_per_letter;
        affix_content_in_bits += this_affix_content_in_bits ;

        if (this_affix == "") {this_affix = "NULL";}
        if (verbose_flag){
            out_distributions <<
               qSetFieldWidth(15) << this_affix <<
               qSetFieldWidth(10) << this_affix_use_count <<
               qSetFieldWidth(15) << ratio <<
               qSetFieldWidth(15) << running_total <<
               qSetFieldWidth(20) << m_affix_pointer_length[this_affix]  <<
               qSetFieldWidth(0) << Qt::endl;

        }
    }
    stem_plus_affix_content_in_bits_Architecture_1 += affix_content_in_bits;
    out_distributions<< Qt::endl << "-----------------------------------------------------------";
    out_summaries << "7. Process affixes"<<Qt::endl;
    reports(out_summaries);
}
//  #8
void MDL::process_signatures_bis_Architecture(QTextStream & out_distributions, QTextStream& out_summaries){
    out_distributions << Qt::endl << "8. Process signatures part 2 (architectural). No new distributions." << Qt::endl <<Qt::endl;
    signature_to_affix_pointers_Architexture_8  = 0.0;
    foreach (QString sigstring, signature2corpus_count.keys()){
        out_distributions  << "# "<< sigstring  << Qt::endl;
        double this_signature_pointer_use_in_bits(0);
        QStringList affixes = sigstring.split("=");
        foreach (QString affix, affixes){
            this_signature_pointer_use_in_bits += m_affix_pointer_length[affix];
            out_distributions <<
                qSetFieldWidth(15) << affix <<
                qSetFieldWidth(20) << m_affix_pointer_length[affix] <<
                qSetFieldWidth(15) << ". Running total of affix pointer use: " <<
                qSetFieldWidth(15) << this_signature_pointer_use_in_bits <<
                qSetFieldWidth(0)  << Qt::endl;
        }
        signature_to_affix_pointers_Architexture_8 += this_signature_pointer_use_in_bits;
        out_distributions << qSetFieldWidth(15) <<  "running total" <<
                             qSetFieldWidth(20) << signature_to_affix_pointers_Architexture_8<<
                             qSetFieldWidth(0) <<Qt::endl << Qt::endl;
    }
    out_distributions << Qt::endl << " Running total of signature pointer use in bits " <<
           signature_to_affix_pointers_Architexture_8 <<Qt::endl << Qt::endl;

    out_summaries << "8. Process signatures, part 2"<<Qt::endl;
    reports(out_summaries);
}
bool MDL::word_is_analyzed(QString word){
    if (word2sig.contains(word)){return true;}
    return (false);
}
void MDL::corpus_description(QTextStream& out_corpus, QTextStream& out_summaries ){
    out_corpus.setFieldAlignment(QTextStream::AlignLeft);
    out_corpus << Qt::endl<< "Analyzed words"<< Qt::endl;
    out_corpus <<
           qSetFieldWidth(25) << "word" <<
           qSetFieldWidth(12)  << " sig: "<<
           qSetFieldWidth(10)  << " stem: "<<
           qSetFieldWidth(10)  <<" affix: " <<
           qSetFieldWidth(20)  << " flag as analyzed: " <<
           qSetFieldWidth(10)  <<" count: "  <<
           qSetFieldWidth(10)  <<" sum"  <<
           qSetFieldWidth(0) << Qt::endl;
    foreach( QString word, word2sig.keys() ) {
        compute_corpus_length(word, out_corpus);
    }
    out_corpus << "Total for analyzed words " << m_corpus_description_from_analyzed_words<< Qt::endl;

    out_corpus << Qt::endl<< "Unanalyzed words"<< Qt::endl<< Qt::endl;
    out_corpus <<
           qSetFieldWidth(25)  << "word" <<
           qSetFieldWidth(12)  << " pointer" <<
           qSetFieldWidth(20)  << " flag as unanalyzed" <<
           qSetFieldWidth(10)  << " count"  <<
           qSetFieldWidth(10)  << " sum"  <<
           qSetFieldWidth(0) << Qt::endl;

    foreach( QString word, unanalyzed_words.keys() ) {
        compute_corpus_length(word, out_corpus);
    }
    out_summaries<<"9. Corpus description"<< Qt::endl;
    out_corpus << "Total for unanalyzed words " << m_corpus_description_from_unanalyzed_words;



    reports(out_summaries);

}
void MDL::compute_corpus_length(QString word, QTextStream & out_corpus){
    bool verbose_flag (true);
    double bits = 0.0;
    QString sigstring;
    double count = 1.0;
    double signature_pointer;
    double per_word_cost;
    out_corpus.setFieldAlignment(QTextStream::AlignLeft);
    out_corpus.setRealNumberPrecision(5);
    if (word2sig.contains(word)){

        count = m_word_token_counts[word];
        sigstring =  word2sig[word];

        QString affix = word2affix[word];
        signature_pointer = signature_plogs[sigstring];
        double stem_pointer = stem2pointer_length_given_signature.value(word2stem[word]);
        double affix_pointer = m_affix2pointer_length_affix_choice_within_signature[sigstring]->value( word2affix[word]);
        per_word_cost = signature_pointer + stem_pointer + affix_pointer + m_flag_to_mark_an_analyzed_word;
        bits = count *  per_word_cost ;
        m_corpus_description_from_analyzed_words += bits;

        if (verbose_flag){
            out_corpus <<

                   qSetFieldWidth(25) << word <<
                   qSetFieldWidth(12) << QString::number(signature_pointer) <<
                   qSetFieldWidth(10) << QString::number(stem_pointer) <<
                   qSetFieldWidth(10)  << QString::number (affix_pointer) <<
                   qSetFieldWidth(20) <<m_flag_to_mark_an_analyzed_word  <<
                   qSetFieldWidth(10) << count  <<
                   qSetFieldWidth(10) <<per_word_cost  <<
                   qSetFieldWidth(0) << Qt::endl;
        }
    }else{
        double this_pointer;
        double count = m_word_token_counts[word];
        double ratio = count/m_token_count_of_unanalyzed_words;
        this_pointer = m_unanalyzed_word2pointer_length[word] = -1.0 * base2log(ratio);
        per_word_cost = this_pointer + m_flag_to_mark_an_unanalyzed_word;
        bits = count * per_word_cost;
        if (verbose_flag){
            out_corpus <<
                   qSetFieldWidth(25) << word <<
                   qSetFieldWidth(12) << QString::number(this_pointer)  <<
                   qSetFieldWidth(20) << m_flag_to_mark_an_unanalyzed_word <<
                   qSetFieldWidth(10) << count  <<
                   qSetFieldWidth(10) << per_word_cost  <<
                   qSetFieldWidth(0) << Qt::endl;
        }
        m_corpus_description_from_unanalyzed_words += bits;
    }

}
void MDL::print_grammar(QTextStream& out){
    QMap<QString, int> sigstring_to_row_number;
    QMap<QString, int> stem_to_row_number;
    int row = 0;
    for(auto sigstring : signatures.keys())
    {
        sigstring_to_row_number[sigstring] = row++;
    }

    //<=======           Start printing here:    ==============================>

    for(auto affix : m_affixes.keys())
    {
        out <<  QString::number(m_affix2index[affix])<< " " <<
                affix<< " " <<
                QString::number(m_affix_pointer_length.value(affix))  <<
                Qt::endl;
    }
    out << "---"<< Qt::endl;
    for(auto sigstring : signature2stems.keys())
    {   out <<  QString::number(sigstring_to_row_number[sigstring])<< " ";
        QStringList affixes = sigstring.split('=');
        out << "[ ";
        foreach (QString affix, affixes){
            out <<  QString::number(m_affix2index[affix]) << " ";
        }
        out << "[ ";
        bool first = true;
        foreach (QString stem, *signature2stems[sigstring]){            
            if (!first){
                out << ']';
            } else{first = false;}
            out << stem << " " <<
            QString::number(stem2pointer_length_given_signature[stem]);
        }
        out << "[ ";
        foreach (QString affix, affixes){
            //out << affix << " ";
            out << QString::number(m_affix2pointer_length_affix_choice_within_signature[sigstring]->value(affix)) << " ";
        }
        out <<  " [ " << signature_plogs[sigstring];
        out << Qt::endl;
    }
    out << "--"<< Qt::endl;
    for (auto word: unanalyzed_words.keys()){
        if (word.length() == 0) continue;
        out <<
               word  << " " <<
               m_unanalyzed_word2pointer_length[word] + m_flag_to_mark_an_unanalyzed_word <<
               Qt::endl;
    }

    out << "Plog analyzed word " << m_flag_to_mark_an_analyzed_word << Qt::endl;
    if (m_suffix_flag){
        out << "suffixes";
    } else{
        out << "prefixes";
    }

}
QString Format(double n){
    return QLocale(QLocale::English).toString(n, 'f', 2);
}

void MDL::reports(QTextStream & out){
    //QLocale::setDefault(QLocale(QLocale::UnitedStates));

    m_total_token_count_of_words = m_token_word_count_in_all_signatures + m_token_count_of_unanalyzed_words;
    total_type_count_of_words =  type_word_count_in_all_signatures + m_type_count_of_unanalyzed_words;


    int field = 50;
    int field2 = 15;
    //bool verbose_flag(true);
    out << "===========================================================" << Qt::endl;

    if(m_suffix_flag){
        out << "Suffixal analysis." <<qSetFieldWidth(0)<<  Qt::endl;
    }else{
        out << "Prefixal analysis." << qSetFieldWidth(0)<< Qt::endl;
    }

    out << qSetFieldWidth(field) << Qt::left
        << "Type count of words: " << Qt::right << qSetFieldWidth(field2)
        << Format(total_type_count_of_words)
        << qSetFieldWidth(0)<< Qt::endl;

    out << qSetFieldWidth(field) << Qt::left
        << "Type count of unanalyzed words: " << Qt::right << qSetFieldWidth(field2)
        << Format(m_type_count_of_unanalyzed_words) << Qt::endl;
    out << qSetFieldWidth(field) << Qt::left
        << "Type count of analyzed words: " << Qt::right<<  qSetFieldWidth(field2)
        << Format(type_word_count_in_all_signatures) << Qt::endl;
    out << qSetFieldWidth(field) << Qt::left
        << "Token count of all words:"<< Qt::right << qSetFieldWidth(field2)
        << Format(m_total_token_count_of_words)   << Qt::endl;
    out << qSetFieldWidth(field) << Qt::left
        << "Token count of unanalyzed words:"<< Qt::right << qSetFieldWidth(field2)
        << Format(m_token_count_of_unanalyzed_words) << Qt::endl;
    out << qSetFieldWidth(field) << Qt::left
        << "Token count of analyzed words: "<< Qt::right << qSetFieldWidth(field2)
        << Format(m_token_word_count_in_all_signatures  )  << Qt::endl;
    out << qSetFieldWidth(field) << Qt::left
        << "Total token count of words: "<< Qt::right << qSetFieldWidth(field2)
        << Format(m_token_word_count_in_all_signatures + m_token_count_of_unanalyzed_words )  << Qt::endl;
    out << qSetFieldWidth(field) << Qt::left
        << "Type count of words from Lxa: "<< Qt::right << qSetFieldWidth(field2)
        << Format(m_Lexicon->get_words()->get_count())  << Qt::endl;
    out << qSetFieldWidth(field) << Qt::left
        << "Token count of words from Lxa: "<< Qt::right << qSetFieldWidth(field2)
        << Format(m_Lexicon->get_words()->get_token_count())  << Qt::endl;
    out << qSetFieldWidth(field) << Qt::left
        << "Number of signatures: "<< Qt::right << qSetFieldWidth(field2)
        << Format(signatures.count() )  << Qt::endl;
    out << qSetFieldWidth(field) << Qt::left
        << "Number of stems: "<< Qt::right << qSetFieldWidth(field2)
        << Format(stems.count() )  << Qt::endl;
    out << qSetFieldWidth(field) << Qt::left
        << "Number of affixes: "<< Qt::right << qSetFieldWidth(field2)
        << Format(m_affixes.count() )  << Qt::endl;
    out << qSetFieldWidth(field) << Qt::left
        << "Total letter count in stems: " << Qt::right << qSetFieldWidth(field2)
        << Format(total_letter_count_in_stems) << Qt::endl;
    out << qSetFieldWidth(field) << Qt::left
        << "total letter count in affixes: " << Qt::right  << qSetFieldWidth(field2)
        << Format(total_letter_count_in_affixes) << Qt::endl;
    out << qSetFieldWidth(field) << Qt::left
        << "total letter count: "         << Qt::right  << qSetFieldWidth(field2)
        << Format(get_total_letter_count()) << Qt::endl;
    out << qSetFieldWidth(field) << Qt::left
        << "Size of alphabet: "             << Qt::right << qSetFieldWidth(field2)
        << alphabet.size() << Qt::endl;
    out << qSetFieldWidth(field) << Qt::left
        << "Bits per letter: "             << Qt::right << qSetFieldWidth(field2)
        <<  QLocale(QLocale::English).toString(bits_per_letter, 'f', 3) << Qt::endl;
    out << qSetFieldWidth(field) << Qt::left
        << "Running total of stem content (bits) : " << Qt::right << qSetFieldWidth(field2)
        << Format(stem_content_in_bits) << Qt::endl;
    out << qSetFieldWidth(field) << Qt::left
        << "Running total of affix content (bits): "    << Qt::right << qSetFieldWidth(field2)
        << Format(affix_content_in_bits) << Qt::endl;
    out << qSetFieldWidth(field) << Qt::left
        << "Stem plus affix content: ** "             << Qt::right << qSetFieldWidth(field2)
        << Format(stem_plus_affix_content_in_bits_Architecture_1) << Qt::endl;
    out << qSetFieldWidth(field) << Qt::left
        << "Cost of affix choices within signatures: **"<< Qt::right << qSetFieldWidth(field2)
        << Format(total_cost_of_affix_choice_within_all_signatures_Choice_2)  << Qt::endl;
   out << qSetFieldWidth(field) << Qt::left
       << "Cost of a set of pointers to each signature: **"<< Qt::right << qSetFieldWidth(field2)
       <<  Format( pointers_to_signatures_Choice_3)  << Qt::endl;
   out << qSetFieldWidth(field) << Qt::left
       << "Cost of stem-choice within signatures **"
       << Qt::right << qSetFieldWidth(field2)
       << Format(choice_of_stems_within_sigs_Choice_4) << Qt::endl;
   out << qSetFieldWidth(field) << Qt::left
       << "bit cost of flagging analyzed words: ** " << Qt::right << qSetFieldWidth(field2)
       << Format(m_total_flagging_of_analyzed_words) << Qt::endl;
   out << qSetFieldWidth(field) << Qt::left
       << "bit cost of flagging unanalyzed words: ** " << Qt::right << qSetFieldWidth(field2)
       << Format(m_total_flagging_of_unanalyzed_words) << Qt::endl;
   out << qSetFieldWidth(field) << Qt::left  << "Grammar length in bits: "<< Qt::right << qSetFieldWidth(field2)
       << Format(get_grammar_length_in_bits()) << Qt::endl;
    out << qSetFieldWidth(field) << Qt::left
        << "corpus description from analyzed words: "<< Qt::right << qSetFieldWidth(field2)
        << Format(m_corpus_description_from_analyzed_words) << Qt::endl;
    out << qSetFieldWidth(field) << Qt::left
        << "corpus description from unanalyzed words: " << Qt::right         << qSetFieldWidth(field2)
        << Format(m_corpus_description_from_unanalyzed_words) << Qt::endl;
    out << qSetFieldWidth(field) << Qt::left
        << "corpus description: " << Qt::right << qSetFieldWidth(field2)
        << Format(get_corpus_description()) << Qt::endl;
    out << qSetFieldWidth(field) << Qt::left
        << "Total description length: " << Qt::right         << qSetFieldWidth(field2)
        << Format(m_corpus_description_from_unanalyzed_words + m_corpus_description_from_analyzed_words + get_grammar_length_in_bits() ) << Qt::endl;




    out << "===========================================================" << Qt::endl;
}






