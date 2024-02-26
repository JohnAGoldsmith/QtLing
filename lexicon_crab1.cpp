#include <QPair>
#include <QList>
#include <QMap>
#include <QMapIterator>
#include <QMultiMap>
#include <QSet>
#include <QtDebug>
#include <QProgressBar>
#include <QApplication>
#include <algorithm>
#include <QChar>
#include <QSet>
#include <QRegularExpression>
#include <QMessageBox>

#include <QtGlobal>
#include <QFile>
#include <QTextStream>



#include "Lexicon.h"
#include "SignatureCollection.h"
#include "StemCollection.h"
#include "SuffixCollection.h"
#include "WordCollection.h"
#include "Word.h"
#include "evaluation.h"
#include "cparse.h"
#include "compound.h"
#include "Stem.h"
#include "log2.h"
#include "mdl.h"
#include "mainwindow.h"
#include "latex.h"

void SortQStringListFromRight(QStringList& ThisStringList);
//bool reverse_string_compare(QString,QString);
extern QString QStringList2QString(QStringList);

QString combine_stem_and_prefix(QString stem, QString prefix){
    if (prefix == "NULL")
        return stem;
    else
        return prefix + stem;
}
QString combine_stem_and_prefix_with_gap(QString stem, QString prefix){
    if (prefix == "NULL")
        return stem;
    else
        return prefix + "  " + stem;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//linguistic methods
/**
 * @brief CLexicon::Crab_1
 * The division of the Crab algorithm into two parts (as I have done here) is only temporary.
 * I have done it so that I can do code development more quickly; sometimes I only need to
 * get part of the algorithm going to test what I am doing.
b */

void CLexicon::Crab_1()
{


    step1_from_words_to_protostems();

    step2_from_protostems_to_parses();

    // at this point, there are some protostems with only one parse --
    // this will happen if a word occurs alone and also with a hyphen after it.
    // this will create a signature with just the NULL suffix!
    // This should be fixed here.


    step3_from_parses_to_stem2sig_maps(QString("crab_1"));

    step4_create_signatures(QString("Crab1"));

    //collect_parasuffixes(); // these are suffixes found in a signature with only one stem

    m_suffix_flag?
       m_Signatures->calculate_sig_robustness():
       m_PrefixSignatures->calculate_sig_robustness(); 
}


//  <-------------->

QString commonsuffix (QString string1, QString string2)
{
    int l1 = string1.length();
    int l2 = string2.length();
    int shorterlength = std::min (l1,l2);
    for (int i =1; i  <= shorterlength; i++){
        if (string1[l1 - i] != string2[l2 - i])
            return string1.right(i-1);
    }
    return string1.right(shorterlength);
}
QString commonprefix (QString string1, QString string2)
{   int l1 = string1.length();
    int l2 = string2.length();
    int shorterlength = std::min (l1,l2);
    for (int i =0; i  < shorterlength; i++){
        if (string1[i] != string2[i]){
            return string1.left(i);
        }
    }
    return string1.right(shorterlength);
}

// ==================== step 1    ======================================================  //
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
 * This is the first of the three initial parts of finding signatures.
 * This makes a cut at every point in a word where the successor frequency
 * is greater than 1.
 * It is divided into two parts;  the first finds protostems, by detecting
 * successfor frequency greater than 1; the second breaks a word after a protostem.
 */
//helper functions:
void CLexicon::step1b_make_protostem_objects_suffix_case(const QStringList* Words){
    initialize_progress(m_suffix_protostems->get_count());
    QStringList alphabetized_protostems = m_suffix_protostems->get_list_of_stem_strings();
    alphabetized_protostems.sort();
    int i = 0;
    for (int p = 0; p<alphabetized_protostems.length(); p++){
        QString protostem = alphabetized_protostems[p];
        if (protostem=="montreal"){
            int i = 1;
        }
        while ( ! Words->at(i).startsWith(protostem))
            i++;
        int j = i;
        while (j < Words->length() && Words->at(j).startsWith(protostem)){
            j++;
        }
        m_suffix_protostems->get_stem(protostem)->set_start_and_end_word(i,j-1);
    }
}
void CLexicon::step1b_make_protostem_objects_prefix_case(const QStringList* Words){
    initialize_progress(m_prefix_protostems->get_count());
    QStringList alphabetized_protostems = m_prefix_protostems->get_list_of_stem_strings();
    SortQStringListFromRight(alphabetized_protostems);
    int i = 0;
    for (int p = 0; p<alphabetized_protostems.length(); p++){
        QString this_protostem_t = alphabetized_protostems[p];
        while (! Words->at(i).endsWith(this_protostem_t) ){
            i++;
        }
        int j = i;
        while (j < Words->length() && Words->at(j).endsWith( this_protostem_t ) ){
            j++;
        }
        m_prefix_protostems->get_stem(this_protostem_t)->set_start_and_end_word(i,j-1);
    }
}
void CLexicon::step1a_make_protostem_if_appropriate(QString this_word, QString next_word){
    QString this_protostem;
    if (m_suffix_flag){
        this_protostem = commonprefix (this_word, next_word);
        if ( this_protostem.length() >= M_MINIMUM_STEM_LENGTH ){
            m_suffix_protostems->find_or_add(this_protostem);
        }
    } else
    {
        this_protostem = commonsuffix(this_word,next_word);
        if (this_protostem.length() >= M_MINIMUM_STEM_LENGTH)
            m_prefix_protostems->find_or_add(this_protostem);
    }
}
/*...............................................................................//
 * ...............................................................................*/
void CLexicon::step1_from_words_to_protostems()
{   word_t          this_word, next_word;
    const QStringList *  Words;
    if (m_suffix_flag){
        Words = get_word_collection()->get_sorted_list();
    }else{
        Words = get_word_collection()->get_end_sorted_list();
    }
    initialize_progress(Words->size());
    m_ParseMap.clear();


    /* this created a  bug, signatures with just one suffix, if a word occurred with a hyphen and no other suffixes
    for (int wordno=0; wordno<Words->size()-1; wordno ++) {
        if (wordno % 500 == 0) { mark_progress(wordno);}
        this_word = Words->at(wordno);
        next_word = Words->at(wordno+1);
        // ideally this would skip words with a hyphen in them...
        step1a_make_protostem_if_appropriate(this_word, next_word);
    }
    */

    int wordno = 0;
    int next_wordno = 0;
    while (wordno < Words->size()-1){
        if (wordno % 1000 == 0) { mark_progress(wordno);}
        this_word = Words->at(wordno);
        next_wordno = wordno + 1;
        while (Words->at(next_wordno).contains('-')){
            next_wordno++;
        }
        next_word = Words->at(next_wordno);
        step1a_make_protostem_if_appropriate(this_word, next_word);
        wordno = next_wordno;
    }

    if (m_suffix_flag)
        step1b_make_protostem_objects_suffix_case(Words);
    else
        step1b_make_protostem_objects_prefix_case(Words);
    return;
}

// ==================== step 2    ======================================================  //

/*!
 * This is the second of the three initial parts of finding signatures.
 * This creates "parses", which are a class of stem/affix/suffix-flag triples, which are put in a long list of "Parses":
 */
//helper functions:
void CLexicon::step2a_scan_word_for_protostems_suffix_case(QString word){
    int suffix_length;
    for (int letterno = M_MINIMUM_STEM_LENGTH; letterno <= word.length(); letterno++){
        QString stem = word.left(letterno);
        if(m_suffix_protostems->find_or_fail(stem))  {
            suffix_length = word.length() - letterno;
            if (suffix_length > M_MAXIMUM_AFFIX_LENGTH){
                continue;
            }
            QString suffix = word.right(suffix_length);
            if (suffix.length() == 0) { suffix = "NULL";}
            CParse* this_parse = new CParse(stem, suffix, true);
            add_parse(this_parse);
        }
    }
}
void CLexicon::step2a_scan_word_for_protostems_prefix_case(QString word){
    int prefix_length;
    for (int letterno = M_MINIMUM_STEM_LENGTH; letterno <= word.length(); letterno++){
         QString stem = word.right(letterno);
         if (m_prefix_protostems->find_or_fail(stem)){
              prefix_length = word.length() - letterno;
              if (prefix_length > M_MAXIMUM_AFFIX_LENGTH){
                   continue;
              }
              QString prefix = word.left(prefix_length);
              if (prefix.length() == 0) {prefix = "NULL";}
              CParse* this_parse = new CParse(stem,  prefix, false);
              add_parse(this_parse);
         }
    }
}
void CLexicon::step2_from_protostems_to_parses()
{   initialize_progress(m_Words->get_count());
    QString                     word;
    int                         wordno (0);
    const QStringList *  Words;
    if (m_suffix_flag){
        Words = get_word_collection()->get_sorted_list();
    }else{
        Words = get_word_collection()->get_end_sorted_list();
    }

    foreach (QString word, *Words){
        if (word.contains('-')){
            continue;
        }
        if (m_suffix_flag)
            step2a_scan_word_for_protostems_suffix_case(word);
        else
            step2a_scan_word_for_protostems_prefix_case(word);
        mark_progress(wordno++);
     }
}
/*...............................................................................//
 * ...............................................................................*/


// ==================== step 3    ======================================================  //
/*!
 * This is the third of the four initial parts of finding signatures.
 * This creates signatures, which in turn creates stems and affixes.
 */
void   CLexicon::step3_from_parses_to_stem2sig_maps(QString name_of_calling_function)
{   initialize_progress(m_ParseMap.size());

    /* Since this is going to be used several times, and all but the first time
     * there will be a previous set of stems, it would make sense to contrast
     * the stems made here with the stems in the preceding step -- so we can
     * keep track of what is gained and what is discarded.
     */

    //name_of_calling_function = " " ; // TODO is this correct? We are not using it here?

    step3a_clear_lexicon_except_protostems();
    m_temp_stem2sig_map.clear();
    step3b_from_parses_to_stem2sig_map(  );
    step3d_from_stem2sig_map_to_sig2stem_map();
}


// =================   3a    =========================================================  //
/*!
 * \brief step3a_from_parses_to_stem_to_sig_map
 * \param parses
 * \param suffix_flag
 * \param these_protosigs
 * This function is called by step3 (just below)
 */
//helper function:
void CLexicon::step3c_attach_affix_to_stem(CParse* parse){
    QString stem = parse->get_stem();
    if (!m_temp_stem2sig_map.contains(stem)){
        m_temp_stem2sig_map[stem] = QSet<affix_t>();
    }
    m_temp_stem2sig_map[stem].insert(parse->get_affix());
}
void CLexicon::step3b_from_parses_to_stem2sig_map(  )
{   foreach (CParse* this_parse, m_ParseMap){
        step3c_attach_affix_to_stem(this_parse);
    }
}
// ===================  3b  =======================================================  //
//helper functions:
QString convert_set_to_qstring(affix_set& this_affix_set){
    QStringList temp_presignature;
    foreach (QString affix, this_affix_set){
        temp_presignature.append(affix);
    }
    temp_presignature.sort();
    return temp_presignature.join("=");
}
// no longer used; get rid of this:
void CLexicon::attach_stem_to_intermediate_signature(QString stem){
    const QString this_signature_string = convert_set_to_qstring(m_temp_stem2sig_map[stem]);
    m_temp_sig2stem_map.attach_stem_to_signature(stem, this_signature_string);
}
void CLexicon::step3d_from_stem2sig_map_to_sig2stem_map()
{   initialize_progress(m_temp_stem2sig_map.count());
    m_temp_sig2stem_map.clear();
    int i=0;
    foreach(QString stem, m_temp_stem2sig_map.keys()){
        if (++i % 500) { mark_progress(i);}
        const QString sigstring = convert_set_to_qstring(m_temp_stem2sig_map[stem]);
        m_temp_sig2stem_map.attach_stem_to_signature(stem, sigstring);
    }
}
// ===============  step 4    ===========================================================  //
// helper functions:
void stem_autobiography_negative_notice_not_enough_stems(CLexicon* lexicon,
                                                         QString stem,                                                        
                                                         QString signature_string,
                                                         QString calling_function){
    QString message = signature_string.count("=") <= 40 ?
                signature_string:
                QString("many=many=affixes");
    lexicon->add_to_stem_autobiographies(stem,
                                QString("[%1]=[singleton stem]=%2")
                                .arg(calling_function)
                                .arg(message));
}
void word_autobiography_negative_notice_not_enough_stems(CLexicon* lexicon,
                                                         QString word,
                                                         QString stem,
                                                         QString signature_string,
                                                         QString calling_function){
    QString message = signature_string.count("=") <= 40 ?
                signature_string:
                QString("many=many=affixes");
    lexicon->add_to_word_autobiographies(word,
                                QString("[%1]=[singleton stem]=stem: %2 = %3")
                                .arg(calling_function)
                                .arg(stem)
                                .arg(message));
}
//helper function:
void stem_autobiography_positive_notice (CLexicon* lexicon, QString stem, QString calling_function, QString sig_string){
    lexicon->add_to_stem_autobiographies(stem,
                                         QString("[%1]==%2")
                                             .arg(calling_function)
                                             .arg(sig_string));
}
void CLexicon::step4_create_signatures(const QString& name_of_calling_function,
                                       eMinimumStemCountFlag min_stem_count_flag)
{   initialize_progress(m_temp_sig2stem_map.m_core.count());
    QString                 sigstring;
    QString                 stem;
    QString                 affix;
    QString                 word;
    CSignature*             pSig;
    int                     count (0);

    m_Words->clear_all_words_parse_triple_maps();
    step4a_clear_active_signatures_and_affixes();

    foreach (sigstring, m_temp_sig2stem_map.m_core.keys() ) {
        if (sigstring == "NULL"){
            qDebug() << 368 << sigstring;
        }
        if (count++ % 100 == 0){ mark_progress(count);}
        QSet<QString>* this_stem_set = m_temp_sig2stem_map.get_stem_set(sigstring);
        Affix_list affix_list = sigstring.split("=");
        if (min_stem_count_flag == MS_ignore_minimum_stem_count
                || this_stem_set->count() >= M_MINIMUM_STEM_COUNT){
            pSig = add_signature(sigstring);
            step4b_link_sig_with_stems_and_affixes(pSig, this_stem_set, name_of_calling_function);
        }
        else
        {   // if there are not enough stems for this signature:
            foreach(stem, *this_stem_set){
                stem_autobiography_negative_notice_not_enough_stems(this, stem, sigstring, name_of_calling_function);
                foreach (affix, affix_list){
                    if (affix == "NULL"){ affix = ""; }
                    CParse parse (stem, affix, m_suffix_flag);
                    word = parse.display();
                    word_autobiography_negative_notice_not_enough_stems(this, word, stem, sigstring, name_of_calling_function);
                }
            }
        }
    }
    step4e_link_all_words_to_signatures(name_of_calling_function);

    m_suffix_flag?
                m_suffixal_stems->sort_alphabetically():
                m_prefixal_stems->sort_alphabetically();
    m_Suffixes->sort_by_count();
    m_Prefixes->sort_by_count();
    m_suffix_flag ?
                m_Signatures->calculate_stem_entropy():
                m_PrefixSignatures->calculate_stem_entropy();
    print_signature_transforms();
}


// ===============  step 4b-4e    ===========================================================  //

void CLexicon::step4b_link_sig_with_stems_and_affixes(CSignature* pSig, QSet<stem_t>* this_stem_set, QString name_of_calling_function){ // step 4a of crab 1
    Affix_list affix_list = pSig->get_affix_string_list();
    foreach (QString stem, *this_stem_set){         
        step4c_create_stem_then_link_sig_and_stem(pSig, stem, name_of_calling_function);
        foreach (QString affix, affix_list){
            step4d_link_signature_with_affix(pSig, stem, affix, name_of_calling_function);
        }
    }
    if (m_suffix_flag){
        foreach (QString affix, affix_list){
            CSuffix * suffix = m_Suffixes->find_or_fail(affix);
            suffix->increment_sig_count();
            suffix->increment_word_count(1 );
        }
    } else{
        foreach (QString affix, affix_list){
            CPrefix * prefix = m_Prefixes->find_or_fail(affix);
            prefix->increment_sig_count();
            prefix->increment_word_count(1 );
        }
    }
}
void CLexicon::step4c_create_stem_then_link_sig_and_stem(CSignature * pSig,  QString stem, QString name_of_calling_function){
    CStem* pStem;
    m_suffix_flag ?
        pStem = m_suffixal_stems->find_or_add(stem):
        pStem = m_prefixal_stems->find_or_add(stem);
    pStem->add_signature(pSig);
    pStem->increment_word_type_count(pSig->get_number_of_affixes());
    pSig->add_stem_pointer(pStem);
    stem_autobiography_positive_notice(this, stem, name_of_calling_function, pSig->get_key());
}
void CLexicon::step4d_link_signature_with_affix(CSignature * pSig, stem_t this_stem, affix_t this_affix, QString name_of_calling_function)
{   QString this_signature_string = pSig->get_key();
    //qDebug() << 424 << pSig->display() << this_stem << this_affix;
    add_affix_to_signature(this_affix, pSig);
    stem_autobiography_positive_notice(this, this_stem, name_of_calling_function, this_signature_string);
}
//helper function:
void CLexicon::add_affix_to_signature(QString affix, CSignature*  pSig){
    if (m_suffix_flag){
        CSuffix* pSuffix = m_Suffixes->find_or_add(affix);
        pSig->add_affix_ptr(pSuffix);
    } else {
        CPrefix* pPrefix = m_Prefixes->find_or_add(affix);
        //pPrefix->increment_word_count(pSig->get_number_of_stems());
        pSig->add_affix_ptr(pPrefix);
    }
}
//helper function:
QString clean(QString string){
    QStringList parts = string.split(":");
    if (parts.length() == 1) return string;
    if (parts.length() > 2) return QString();
    bool test0, test1;
    int i = parts[0].toInt(&test0);
    i = parts[1].toInt(&test1);
    if (!test0 && test1) {return parts[0];}
    if (test0 && !test1) {return parts[1];}
    return QString();
}

// ===============  step 4e    ===========================================================  //

void CLexicon::step4e_link_all_words_to_signatures(QString name_of_calling_function){
    QString word, word_split, stem, sig_string;
    // latex this_latex;
    // this_latex.tabular_beginning(QString ("l l l l "));
    if (m_suffix_flag){
        foreach (CSignature* pSig, *m_Signatures->get_signature_list()){
            //  this_latex.tabular(pSig);
            sig_string = pSig->get_key();
            foreach (CStem* pStem, *pSig->get_stems()){
                stem = pStem->get_key();
                foreach (QString affix, sig_string.split("=")){
                    CParse parse(stem,affix, m_suffix_flag);
                    step4f_add_analysis_to_word_for_suffix_case(parse, pSig, name_of_calling_function);
                }
            }
        }
        sort_parse_triples_on_all_words(m_suffix_flag);
        return;
    }
    // prefix case:
    foreach (CSignature* pSig, *m_PrefixSignatures->get_signature_list()){
            sig_string = pSig->get_key();
            foreach (CStem* pStem, *pSig->get_stems()){
                stem = pStem->get_key();
                foreach (QString affix, sig_string.split("=")){
                    CParse parse(stem,affix, m_suffix_flag);
                    step4f_add_analysis_to_word_for_prefix_case(parse, pSig, name_of_calling_function);
                }
            }
    sort_parse_triples_on_all_words(m_suffix_flag);
    }
}

//helper function:
void CLexicon::step4f_add_analysis_to_word_for_suffix_case(CParse& Parse, CSignature* pSig, QString name_of_calling_function ){
    QString sig_string = pSig->get_key();
    QString word, word_split;
    CWord* pWord;
    if (Parse.get_affix() == "NULL"){
        word = Parse.display();
        pWord = m_Words->find_or_fail(word);
        if (pWord){
            add_suffix_analysis_to_word(pWord, Parse,sig_string, name_of_calling_function);
        }
        return;
    }
    if (Parse.get_affix().contains(":")){
        CStem* inner_suffix = m_suffixal_stems->find_or_fail(Parse.get_affix());
        if (inner_suffix) {
            CSignature* pLocalSig = inner_suffix->get_last_signature();
            foreach (QString suffix, pLocalSig->get_affix_string_list())
            {  if (Parse.get_affix() == "NULL") suffix = "";
                word =       Parse.get_stem()       + clean(Parse.get_affix())         + Parse.get_affix();
                word_split = Parse.get_stem() + " " + clean(Parse.get_affix())  + " "  + Parse.get_affix();
                pWord = m_Words->find_or_fail(word);
                if (pWord){
                    add_suffix_analysis_to_word(pWord,Parse, sig_string, name_of_calling_function);
                }
            }
        }
        return;
    }
    word = Parse.display();
    pWord = m_Words->find_or_fail(word);
    if (pWord){
        add_suffix_analysis_to_word(pWord,Parse, sig_string,  name_of_calling_function);
    }
    return;
}
void CLexicon::step4f_add_analysis_to_word_for_prefix_case(CParse& parse, CSignature* pSig,QString name_of_calling_function){
    QString sig_string = pSig->get_key();
    QString word, word_split;
    CWord* pWord;

    if (parse.get_affix()=="NULL"){
        word = parse.display();
        pWord = m_Words->find_or_fail(word);
        if (pWord){
            //stem_count += pWord->get_word_count();  TODO
            add_prefix_analysis_to_word(pWord, parse, sig_string, name_of_calling_function);
        }
        return;
    }
    if (parse.get_affix().contains(":")){
        CStem* inner_prefix = m_prefixal_stems->find_or_fail(parse.get_affix());
        if (inner_prefix) {
            CSignature* pLocalSig = inner_prefix->get_last_signature();
            foreach (QString prefix, pLocalSig->get_affix_string_list()) {
                if (prefix == "NULL") prefix = "";
                //word =  stem + clean(affix) + suffix;
                // is this right?:
                word = parse.get_affix() + clean(parse.get_affix()) + parse.get_stem();

                CWord* pWord = m_Words->find_or_fail(word);
                if (pWord)
                {   //stem_count += pWord->get_word_count();  TODO
                    add_prefix_analysis_to_word(pWord, parse, sig_string, name_of_calling_function);
                }
            }
        }
        return;
    }
    //normal case
    word = parse.display();
    pWord = m_Words->find_or_fail(word);
    if (pWord){
         add_prefix_analysis_to_word(pWord, parse, sig_string, name_of_calling_function);
        //stem_count += pWord->get_word_count();  TODO
    }
}

//helper functions to above:
void CLexicon::add_suffix_analysis_to_word(CWord* pWord, CParse& parse, QString sig_string,  QString name_of_calling_function){
    //stem_count += pWord->get_word_count();  TODO
    pWord->add_suffixal_parse_triple(parse.get_stem(), parse.get_affix(), sig_string);
    pWord->add_morphemic_split(parse.display_with_gap());
    word_autobiography_positive_notice(pWord->get_key(), parse.get_stem(), sig_string, name_of_calling_function);
    m_word2suffix_sigs.insert(pWord->get_key(),sig_string);

}
void CLexicon::add_prefix_analysis_to_word(CWord* pWord,  CParse& parse, QString sigstring, QString name_of_calling_function){
    //stem_count += pWord->get_word_count();  TODO
    pWord->add_prefixal_parse_triple(parse.get_stem(), parse.get_affix(), sigstring);
    pWord->add_morphemic_split(parse.display_with_gap());
    word_autobiography_positive_notice(pWord->get_key(), parse.get_stem(), sigstring, name_of_calling_function);
    m_word2prefix_sigs.insert(pWord->get_key(), sigstring);
}


void CLexicon::sort_parse_triples_on_all_words(bool suffixes_flag){
    for(int i = 0; i < m_Words->get_word_count(); i++){
        CWord* pWord = m_Words->get_word(i);
        if (suffixes_flag){
            pWord->sort_suffixal_parse_triple_list();
        }else{
           pWord->sort_prefixal_parse_triple_list();
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//      End of Crab 1
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
///
///

