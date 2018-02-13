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
#include "Lexicon.h"

#include "SignatureCollection.h"
#include "StemCollection.h"
#include "SuffixCollection.h"
#include "WordCollection.h"
#include "Word.h"

CLexicon::CLexicon( CLexicon* lexicon, bool suffix_flag)
{
    m_Signatures            = new CSignatureCollection(this, true);
    m_PrefixSignatures      = new CSignatureCollection(this,false);
    m_Words                 = new CWordCollection(this);
    m_prefixal_stems          = new CStemCollection(this);
    m_suffixal_stems          = new CStemCollection(this);
    m_Suffixes              = new CSuffixCollection(this);
    m_Prefixes              = new CPrefixCollection(this);
    m_Compounds             = new CWordCollection(this);
    m_Parses                = new QList<QPair<QString,QString>>();
    m_ParaSignatures        =  new CSignatureCollection(this, true);
    m_ParaSuffixes          = new CSuffixCollection(this);
    m_ResidualStems         = new CStemCollection(this);
    m_PassiveSignatures     = new CSignatureCollection(this, true);
    m_SuffixesFlag          = suffix_flag;
    m_Hypotheses            = new QList<CHypothesis*>;
    m_Hypothesis_map        = new QMap<QString, CHypothesis*>;
    m_entropy_threshold_for_stems = 1.2;
    m_parent_lexicon        = lexicon;

//  This is part of an experiment.
    m_category_types["Words"]               = CT_word;
    m_category_types["Suffixal stems"]      = CT_stem;
    m_category_types["Prefixal stems"]      = CT_stem;
    m_category_types["Suffixes"]            = CT_affix;
    m_category_types["Prefixes"]            = CT_affix;
    m_category_types["Suffix signatures"]   = CT_signature;
    m_category_types["Prefix signatures"]   = CT_signature;
    m_category_types["Compounds"]           = CT_compound;
    m_category_types["Parses"]              = CT_parse;
    m_category_types["Protostems"]          = CT_stem;
    m_category_types["Parasignatures"]      = CT_signature;
    m_category_types["Parasuffixes"]        = CT_affix;
    m_category_types["Hypotheses"]          = CT_hypothesis;
    m_category_types["Hypotheses 2"]          = CT_hypothesis2;
    // add component 2
}


QListIterator<simple_sig_graph_edge*> * CLexicon::get_sig_graph_edge_list_iter()
{
    QListIterator<simple_sig_graph_edge*> * iter = new QListIterator<simple_sig_graph_edge*>(m_SigGraphEdgeList);
    return iter;
}

QMapIterator<QString, sig_graph_edge*> * CLexicon::get_sig_graph_edge_map_iter()
{
    QMapIterator<QString, sig_graph_edge*> * iter = new QMapIterator<QString, sig_graph_edge*>(m_SigGraphEdgeMap);
    return iter;
}

CLexicon::~CLexicon()
{
    delete m_Signatures;
    delete m_PrefixSignatures;
    delete m_ParaSignatures;
    delete m_PassiveSignatures;
}

void CLexicon::clear_lexicon(){
    delete m_Signatures;
    m_Signatures = new CSignatureCollection(this, true);
    delete m_PrefixSignatures;
    m_PrefixSignatures = new CSignatureCollection(this, false);
    delete m_prefixal_stems;
    m_prefixal_stems = new CStemCollection(this);
    delete m_suffixal_stems;
    m_suffixal_stems = new CStemCollection(this);
    delete m_Suffixes;
    m_Suffixes = new CSuffixCollection(this);
    delete m_Prefixes;
    m_Prefixes = new CPrefixCollection(this);
    delete m_ParaSignatures;
    m_ParaSignatures = new CSignatureCollection(this, true);
    delete m_ParaSuffixes;
    m_ParaSuffixes = new CSuffixCollection(this);
    delete m_ResidualStems;
    m_ResidualStems = new CStemCollection(this);
    delete m_PassiveSignatures;
    m_PassiveSignatures = new CSignatureCollection(this);
    delete m_Hypotheses;
    m_Hypotheses = new QList<CHypothesis*>;




}

////////////////////////////////////////////////////////////////////////////////////////////////////
//linguistic methods
/**
 * @brief CLexicon::Crab_1
 * The division of the Crab algorithm into two parts (as I have done here) is only temporary.
 * I have done it so that I can do code development more quickly; sometimes I only need to
 * get part of the algorithm going to test what I am doing.
 */


void CLexicon::dump_signatures_to_debug()
{
    qDebug() << "********  signature dump ********************* ";
    int i = 0;
    foreach (CSignature* pSig, *m_Signatures->get_signature_list()){
        i++;
        qDebug()<< i << pSig->get_key();
    }
    qDebug() << "******** end of signature dump ********************* ";

}

void CLexicon::Crab_1()
{
    FindProtostems();

    CreateStemAffixPairs();

    AssignSuffixesToStems();

    find_full_signatures();

    collect_parasuffixes();

    m_SuffixesFlag?
        m_Signatures->compute_containment_list():
        m_PrefixSignatures->compute_containment_list();


    qDebug() << "finished crab 1.";

 }

//  <-------------->

////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
 * This is the first of the three initial parts of finding signatures.
 * This makes a cut at every point in a word where the successor frequency
 * is greater than 1.
 */
void CLexicon::FindProtostems()
{   word_t          this_word, previous_word;
    QStringList *   Words = get_word_collection()->GetSortedStringArray();
    bool            StartFlag = true;
    bool            DifferenceFoundFlag = false;
    stem_t          stem;
    int             this_word_length(0), previous_word_length(0);
    m_ProgressBar->reset();
    m_ProgressBar->setMinimum(0);
    m_ProgressBar->setMaximum(Words->size());
    m_StatusBar->showMessage("Proto-stems.");
//    m_prefix_protostems.clear();
//    m_suffix_protostems.clear();
    m_Parses->clear();
    m_Parse_map.clear();
    for (int wordno=0; wordno<Words->size(); wordno ++){
        m_ProgressBar->setValue(wordno);
        if (m_SuffixesFlag){
            this_word = Words->at(wordno);
        } else{
            this_word = get_words()->get_reverse_sort_list()->at(wordno);
        }
        this_word_length = this_word.length();
        if (StartFlag){
            StartFlag = false;
            previous_word = this_word;
            previous_word_length = this_word_length;
            continue;
        }
        DifferenceFoundFlag = false;
        int end_word;
        int wordno2;
        if (m_SuffixesFlag){
            int end = qMin(this_word_length, previous_word_length);
            for (int i=0; i <end; i++){
                if (previous_word[i] != this_word[i]){
                    stem = previous_word.left(i);
                    if (stem.length()== 0){continue;}
                    DifferenceFoundFlag = true;
                    if (!m_suffix_protostems_2.contains(stem)){
                        for (wordno2 = wordno; wordno2 < m_Words->get_count(); wordno2++ ){
                            if ( ! Words->at(wordno2).startsWith(stem) ){
                                end_word = wordno2 - 1;
                                break;                            }
                        } // add case for last word on list here.
                        protostem * this_protostem = new protostem(stem, wordno-1, end_word );
                        m_suffix_protostems_2[stem] = this_protostem;
                    }
                    break;
                }
            }
        }  // end of suffix case.
        else
        {       // -->   Prefix case   <-- //
            this_word_length = this_word.length();
            previous_word_length = previous_word.length();
            if (this_word == "kitabu") {qDebug()<< 213 << previous_word;}

            int end = qMin(this_word_length, previous_word_length);
            for (int i=1; i <=end; i++){
                if (previous_word.right(i) != this_word.right(i)){
                    if (this_word=="kitabu"){
                        qDebug() << 217 <<  i << this_word.right(i);
                    }
                    stem = previous_word.right(i-1);
                    DifferenceFoundFlag = true;
                    if (this_word == "kitabu"){qDebug() << stem << previous_word << 221;}
                    if (!m_prefix_protostems.contains(stem))                {
                            m_prefix_protostems[stem] = 1;
                     }
                     break;
                }
            }
        }
        if (DifferenceFoundFlag == true){
            previous_word = this_word;
            continue;
        }
        else {
            if (previous_word.length() < this_word.length()) {
                m_prefix_protostems[previous_word] = 1;
            }
        }
        previous_word = this_word;
        previous_word_length = this_word_length;
    }
    return;
}

/*!
 * This is the second of the three initial parts of finding signatures.
 * This creates stem/affix pairs, which are put in a long list of "Parses".
 */
void CLexicon::CreateStemAffixPairs()
{
    m_ProgressBar->reset();
    m_ProgressBar->setMinimum(0);
    m_ProgressBar->setMaximum(m_Words->get_count());
    m_StatusBar->showMessage("Stem-affix pairs.");
    QString                     stem, suffix, word, prefix;
    int                         suffix_length, prefix_length, wordno (0);
    map_string_to_word_ptr_iter *   word_iter = m_Words->get_iterator();
    bool                        DoNotParseCompoundsFlag = true;
    while (word_iter->hasNext())   {
        word = word_iter->next().value()->GetWord();
        if (DoNotParseCompoundsFlag and word.contains("-")){
            *m_Compounds << word;
            continue;
        }
        wordno++;
        m_ProgressBar->setValue(wordno);
        int MinimumStemLength = 3;
        for (int letterno = MinimumStemLength; letterno < word.length(); letterno++){
            if (m_SuffixesFlag){
                stem = word.left(letterno);
                if(m_suffix_protostems_2.contains(stem))  {
                    suffix_length = word.length() - letterno;
                    suffix = word.right(suffix_length);
                    m_Parses->append(QPair<QString,QString>(stem,suffix));
                    if (m_Words->contains(stem)){
                        m_Parses->append(QPair<QString,QString>(stem,QString("NULL")));
                    }
                }
            }else{
                stem = word.right(letterno);
                if (m_prefix_protostems.contains(stem)){
                    prefix_length = word.length() - letterno;
                    prefix = word.left(prefix_length);
                    if (word == "kitabu") {
                            qDebug() << "kitabu"<< 282 << stem;
                    }
                    m_Parses->append(QPair<QString,QString>(prefix,stem));
                    if (m_Words->contains(stem)){
                        m_Parses->append(QPair<QString,QString>(QString("NULL"),stem));

                    }
                }
            } // end of prefixes.
        }
    }
}

/*!
 * This is the third of the three initial parts of finding signatures.
 * This creates signatures, which in turn creates stems and affixes.
 */
void   CLexicon::AssignSuffixesToStems()
{   const int MINIMUM_NUMBER_OF_STEMS = 2;

    QPair<QString,QString>      this_pair;
    CSignature*                 pSig;
    QString                     this_stem_t, this_suffix, this_prefix, this_affix, this_signature_string, this_word;
    stem_list *                 p_this_stem_list;
    affix_set *                 this_ptr_to_affix_set;
    CStem*                      pStem;
    map_sigstring_to_suffix_set temp_stems_to_affix_set;
    map_sigstring_to_stem_list  temp_signatures_to_stems;
    morph_set *                 pSet;
    m_ProgressBar->reset();
    m_ProgressBar->setMinimum(0);
    m_ProgressBar->setMaximum(m_Parses->size());
    m_StatusBar->showMessage("Form signatures: 1. temporary stems and affixes.");

    clear_lexicon();

    //delete m_PrefixSignatures;
    //m_PrefixSignatures = new CSignatureCollection(this, m_SuffixesFlag);

    //--> We establish a temporary map from stems to sets of affixes as we iterate through parses. <--//
    for (int parseno = 0; parseno < m_Parses->size(); parseno++){
        m_ProgressBar->setValue(parseno);
        this_pair = m_Parses->at(parseno);
        if (m_SuffixesFlag){
            this_stem_t = this_pair.first;
            this_affix = this_pair.second;
        } else{
            this_stem_t = this_pair.second;
            this_affix = this_pair.first;
        }
        if (! temp_stems_to_affix_set.contains(this_stem_t)){
            if (m_SuffixesFlag){
                pSet = new suffix_set();
            } else{
                pSet = new prefix_set();
            }
            temp_stems_to_affix_set.insert(this_stem_t,pSet);
        }
        temp_stems_to_affix_set.value(this_stem_t)->insert(this_affix);
    }
    //-----------------------------------------------------------------------------------------------//
    qDebug() << "Step 1.";
    //--> We iterate through these stems and for each stem, create QStringLists of their affixes. <--//
    //--> then we create a "pre-signature" in a map that points to lists of stems. <--//
    m_ProgressBar->reset();
    m_ProgressBar->setMinimum(0);
    m_ProgressBar->setMaximum(temp_stems_to_affix_set.count());
    m_StatusBar->showMessage("Form signatures: 2. tentative signatures.");

    int count= 0;
    QMapIterator<QString, morph_set*>   stem_iter(temp_stems_to_affix_set);    // part 1
    while (stem_iter.hasNext())                                                // make a presignature for each stem.
    {    qApp->processEvents();
         count ++;
         m_ProgressBar->setValue(count);
         stem_iter.next();
         this_stem_t            = stem_iter.key();
         this_ptr_to_affix_set  = stem_iter.value();
         QStringList temp_presignature;
         affix_set_iter affix_iter (*this_ptr_to_affix_set);
         while (affix_iter.hasNext()){
                 temp_presignature.append ( affix_iter.next() );
         }
         temp_presignature.sort();
         sigstring_t this_signature_string = temp_presignature.join("=");
         if ( ! temp_signatures_to_stems.contains(this_signature_string)){
            stem_list * pStemSet = new stem_list;
            temp_signatures_to_stems[this_signature_string] = pStemSet;
         }
         temp_signatures_to_stems.value(this_signature_string)->append(this_stem_t);
    }
    //-----------------------------------------------------------------------------------------------//
    //-->  create signatures, stems, affixes:  <--//
    m_ProgressBar->reset();
    m_ProgressBar->setMinimum(0);
    m_ProgressBar->setMaximum(temp_signatures_to_stems. count());
    m_StatusBar->showMessage("Form signatures: 3. final step.");
    count = 0;
    QMapIterator<sigstring_t, stem_list*> iter_sigstring_to_stems ( temp_signatures_to_stems);
     // -->  Iterate through tentative signatures.    <-- //
    while (iter_sigstring_to_stems.hasNext())
    {
        qApp->processEvents();
        count ++;
        m_ProgressBar->setValue(count);
        iter_sigstring_to_stems.next();
        this_signature_string    = iter_sigstring_to_stems.key();
        p_this_stem_list         = iter_sigstring_to_stems.value();

        affix_set this_affix_set = QSet<QString>::fromList( this_signature_string.split("="));
        if (p_this_stem_list->size() >= MINIMUM_NUMBER_OF_STEMS)
        {
            if( m_SuffixesFlag) {
                pSig = *m_Signatures       << this_signature_string;
            } else {
                pSig = *m_PrefixSignatures << this_signature_string;
                pSig->set_suffix_flag(false);
            }

            pSig->add_memo("Pass 1");
            QSetIterator<suffix_t> affix_iter(this_affix_set);
            while(affix_iter.hasNext()){
                  this_affix = affix_iter.next();
                  if (m_SuffixesFlag){
                      CSuffix* pSuffix = m_Suffixes->find_or_add(this_affix);
                      pSuffix->increment_count();
                      pSig->add_affix_ptr(pSuffix);
                  }else{
                      CPrefix* pPrefix = m_Prefixes->find_or_add(this_affix);
                      pPrefix->increment_count();
                      pSig->add_affix_ptr(pPrefix);
                  }
            }
            m_StatusBar->showMessage("Form signatures: 3b");
            // --> We go through this sig's stems and reconstitute its words. <--//
            stem_list_iterator stem_iter(*p_this_stem_list);
            while (stem_iter.hasNext()){
                this_stem_t = stem_iter.next();
                m_SuffixesFlag ?
                    pStem = m_suffixal_stems->find_or_add(this_stem_t):
                    pStem = m_prefixal_stems->find_or_add(this_stem_t);
                pStem->add_signature (pSig);
                pSig->add_stem_pointer(pStem);

                QSetIterator<suffix_t> affix_iter(this_affix_set);
                while(affix_iter.hasNext()){
                    this_affix = affix_iter.next();
                    if (this_affix == "NULL"){
                        this_word = this_stem_t;
                    } else{
                        m_SuffixesFlag ?
                            this_word = this_stem_t + this_affix :
                            this_word = this_affix + this_stem_t ;
                    }
                    CWord* pWord = m_Words->get_word(this_word);
                    if (!pWord){
                        qDebug() << this_word <<  "Error: this_word not found among words.";
                    } else{
                        pWord->add_parse_triple(this_stem_t, this_affix, pSig->get_key());
                        QString message = this_signature_string;
                        if (this_affix_set.size() > 50){message = "Super long signature";};
                        pWord->add_to_autobiography("Pass1= " + this_stem_t + "=" + message);
                    }
                 }
            }
        }
    }
    m_Suffixes->sort_by_count();
    m_SuffixesFlag ?
        m_Signatures->calculate_stem_entropy():
        m_PrefixSignatures->calculate_stem_entropy();
    m_StatusBar->showMessage("Computation of Crab 1 completed.");
}

bool contains(QList<QString> * list2, QList<QString> * list1){
    for (int i=0; i < list1->length();i++){
        bool success = false;
        for (int j = 0; j < list2->length();j++){
            if (list1->at(i) == list2->at(j)){
                success = true;
            }
        }
        if (success == false){
            return false;
        }
    }
    return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//      End of Crab 1
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//      Find full signatures

void remove_final_letter (QStringList & stem_list, bool suffix_flag){
    for (int i = 0; i < stem_list.size(); i++){
        if (suffix_flag){
            stem_list[i] = stem_list[i].left(stem_list[i].length()-1);
        }else{
            stem_list[i] = stem_list[i].mid(1);
        }
     }
}
void add_initial_letter (QStringList & affix_list, QString letter, bool suffix_flag){
    for (int i = 0; i < affix_list.size(); i++){
        if (affix_list[i]== "NULL"){
            affix_list[i] = letter;
        } else {
            if (suffix_flag){
                affix_list[i] = letter + affix_list[i];
            }else{
                affix_list[i] = affix_list[i] + letter;
            }
        }
     }
}
void CLexicon::find_full_signatures()
{
/*  1. iterate through signatures.
        a. if the edge has zero-entropy, make a shorter stem, and the stem-affix pair to Parses, for all stems in sig.
        b. keep pairs of (new stem, whole word) and see if the set of new stems has zero-entropy. if so, repeat. If not, exit.
    2. Call AssignSuffixesToStems.
*/
    m_StatusBar->showMessage("Find full signatures inside hollow signatures.");
    CStemCollection * stems;
    CSignature * pSig;
    QString letter, affix, stem;
    QStringList affix_list, stem_list;
    CSignatureCollection * signatures;
    m_SuffixesFlag ?
                stems = m_suffixal_stems:
                stems = m_prefixal_stems;
    m_SuffixesFlag ?
                signatures = m_Signatures:
                signatures = m_PrefixSignatures;
    m_ProgressBar->reset();
    m_ProgressBar->setMinimum(0);
    m_ProgressBar->setMaximum(signatures->get_count());
    QMapIterator<sigstring_t,CSignature*> * sig_iter = new QMapIterator<sigstring_t,CSignature*> (* signatures->get_map() );
    while (sig_iter->hasNext()){
        pSig = sig_iter->next().value();
        //qDebug() << 495 << pSig->get_key() << pSig->get_stem_entropy();
        //qDebug() << pSig->display();
        //if (pSig->display() == "k=v") {
        //    qDebug() << 496 << pSig->display() << pSig->get_stem_entropy();
        // }
        if (pSig->get_stem_entropy() > 0){
            continue;
        }
        m_SuffixesFlag?
               letter = pSig->get_stems()->at(0)->get_key().right(1):
               letter = pSig->get_stems()->at(0)->get_key().left(1);
        stem_list.clear();
        pSig->get_stem_strings(stem_list);
        remove_final_letter(stem_list, m_SuffixesFlag);
        pSig->get_string_list(affix_list);
        add_initial_letter (affix_list, letter, m_SuffixesFlag);

        for (int i = 0; i < stem_list.size(); i++){
            stem = stem_list.at(i);
            //qDebug() << 508 << stem;
            for (int affno = 0; affno < affix_list.size(); affno++){
                affix = affix_list.at(affno);
                if (m_SuffixesFlag){
                    m_Parses->append(QPair<QString, QString> (stem,affix));
                    //qDebug() << 534 << stem << " "<< affix ;
                } else{
                    //qDebug() << affix <<  stem << 521;
                    m_Parses->append(QPair<QString, QString> (affix ,stem));
                    if (stem.length() == 0 ){
                        //qDebug() << 523 << "null stem"   ;
                    }

                }
            }
            //stem_set.append(QString(pSig->get_stems()->at(i)->get_key()));
        }


    }


    AssignSuffixesToStems();
}




/*
struct{
    bool operator ()(CSignature* pSig_a, CSignature* pSig_b) const {
    // return pSig_a->number_of_true_suffixes() > pSig_b->number_of_true_suffixes();
    }
}custom_compare_residual_sig;
*/


/*!
 * Replace parse pairs from current signature structure. This allows us to
 * delete the old signature structure, and replace them with the set of
 * parse-pairs that exactly describe the current signature structure.
 */

void CLexicon::replace_parse_pairs_from_current_signature_structure(bool FindSuffixesFlag) {
    m_Parses->clear();
    m_Parse_map.clear();
    QString                         sig_string;
    CSignature*                     pSig;
    //CStem*                          pStem;
    QList<CStem*> *                 stem_list;
    map_sigstring_to_sig_ptr_iter  * sig_iter =   new map_sigstring_to_sig_ptr_iter(*  get_signatures()->get_map() );

    while (sig_iter->hasNext()){
        pSig = sig_iter->next().value();
        stem_list =  pSig->get_stems();
    }
}





/*!
 * We can build a graph whose nodes are the signatures, where an edge connects
 * any pair of signatures if there exists a word that they both analyze.
 * A sig_graph_edge has two flavors: this function uses the flavor that
 * contains the two signatures, the word, and the string-difference between
 * the stems of the word at the two signatures.
 * The edge has: signature1, signature2, a morph, and a pair of stems.
 *
 * This is Part 1 of the 3rd major function of Crab 2.
 */
void CLexicon::compute_sig_graph_edges()
{
    map_string_to_word *            WordMap = m_Words->GetMap();
    map_string_to_word_ptr_iter     word_iter(*WordMap);
    simple_sig_graph_edge *          p_SigGraphEdge;
    CSignature *                    pSig, * this_sig_ptr, *the_other_sig_ptr;
    CWord*                          pWord;
    morph_t                         difference;
    stem_t                          this_stem, the_other_stem;

    while (word_iter.hasNext())   {
        pWord = word_iter.next().value();
        QMapIterator<stem_t, Parse_triple*> iter_triple_1 (*pWord->get_parse_triple_map());
        while (iter_triple_1.hasNext())
        {
            this_stem = iter_triple_1.next().key();
            int this_stem_length = this_stem.length();
            Parse_triple * this_triple = iter_triple_1.value();
            sig_string this_sig = this_triple->p_sig_string;
            CSignature* this_sig_ptr;
            m_SuffixesFlag?
                this_sig_ptr = m_Signatures->find_or_fail(this_sig):
                this_sig_ptr = m_PrefixSignatures->find_or_fail(this_sig);
            QMapIterator<stem_t,Parse_triple*> iter_triple_2 = iter_triple_1;
            while (iter_triple_2.hasNext())
            {
                the_other_stem = iter_triple_2.next().key();
                int the_other_stem_length = the_other_stem.length();
                Parse_triple* the_other_triple = iter_triple_2.value();
                sig_string the_other_sig = the_other_triple->p_sig_string;
                m_SuffixesFlag?
                    the_other_sig_ptr = m_Signatures->find_or_fail(the_other_sig):
                    the_other_sig_ptr = m_PrefixSignatures->find_or_fail(the_other_sig);
                if (this_sig == the_other_sig){continue;}
                if (this_stem_length > the_other_stem_length ){     // this cannot happen now (it could in the previous formulation -- so fix this);
                            int length_of_difference = this_stem_length - the_other_stem_length;
                            m_SuffixesFlag?
                                difference = this_stem.mid(the_other_stem_length):
                                difference = this_stem.left(length_of_difference);
                            p_SigGraphEdge = new simple_sig_graph_edge (this, this_sig_ptr, the_other_sig_ptr, this_sig,the_other_sig,
                                                                        difference, pWord->get_key(), this_stem, the_other_stem);
                 } else{
                     int length_of_difference = the_other_stem_length - this_stem_length;
                     m_SuffixesFlag?
                           difference = the_other_stem.mid(this_stem_length):
                           difference = the_other_stem.left(length_of_difference);
                     p_SigGraphEdge = new simple_sig_graph_edge(this, the_other_sig_ptr, this_sig_ptr,  the_other_sig,this_sig,
                                                                difference, pWord->get_key(), the_other_stem, this_stem);
                }
                m_SigGraphEdgeList.append(p_SigGraphEdge);
                pWord->add_to_autobiography("multiple parse=" + this_stem + "=" +  this_sig + "=" + the_other_stem + "=" + the_other_sig);
            } // end of looking at stems longer than this_stem
        } //end of looking at each stem in this word.
    } // each word
}

/*!
 * This function takes the SigTreeEdge list, and makes a smaller list composed of
 * SigTreeEdges which share the same signature pair and string-difference. This
 * flavor of sig_graph_edge contains a list of all the words that participate in
 * this particular sig_graph_edge.
 *
 * This is Part 2 of the 3rd major function of Crab 2.
 */
void CLexicon::compute_sig_graph_edge_map() {
morph_t         edge_label;
word_t          this_word;
simple_sig_graph_edge * p_sig_graph_edge;
sig_graph_edge      //  * p_sig_graph_edge_2,
                     * p_sig_graph_edge_3;
lxa_sig_graph_edge_map* p_EdgeMap = & m_SigGraphEdgeMap;

QListIterator<simple_sig_graph_edge*> this_simple_sig_graph_edge_iter (m_SigGraphEdgeList);
while (this_simple_sig_graph_edge_iter.hasNext())
{
    p_sig_graph_edge = this_simple_sig_graph_edge_iter.next();
    edge_label = p_sig_graph_edge->label();
    this_word  = p_sig_graph_edge->word;
    // --> We iterate through the simple Edges contained in the TreeEdge List            <-- //
    // --> We build a map of larger TreeEdges, each containing multiple stems and words. <-- //
    if (p_EdgeMap->contains(edge_label)){
        p_sig_graph_edge_3 = p_EdgeMap->value(edge_label);
        word_stem_struct * this_word_stem_struct = new word_stem_struct;
        this_word_stem_struct->word = this_word;
        this_word_stem_struct->stem_1 = p_sig_graph_edge->stem_1;
        this_word_stem_struct->stem_2 = p_sig_graph_edge->stem_2;
        QString this_label = this_word_stem_struct->get_label();
        if ( ! p_sig_graph_edge_3->shared_word_stems.contains(this_label)){
               p_sig_graph_edge_3->shared_word_stems[this_label] = this_word_stem_struct;
        }
     } else {  // --> start a new sig_graph_edge with multiple stems <-- //
        sig_graph_edge * p_sig_graph_edge_2 = new sig_graph_edge(*p_sig_graph_edge);
        m_SigGraphEdgeMap.insert(p_sig_graph_edge_2->label(),p_sig_graph_edge_2);
     }
}
}

/*!
 * This function looks at pairs of signatures joined by a sig-tree-edge, where
 * the morpheme that separates them is a single letter. We look to see  how tight
 * the fit is between these two sets of signatures.
 */
void CLexicon::test_for_phonological_relations_between_signatures()
{
   lxa_sig_graph_edge_map_iter  sig_iter (m_SigGraphEdgeMap);
   QString difference;
   QSet<QString> differences_1_letter, differences_longer;
   while (sig_iter.hasNext()){
       sig_iter.next();
       QString morph = sig_iter.value()->morph;
       if (morph.length() == 1){
        differences_1_letter.insert(sig_iter.value()->morph);
       } else{
        differences_longer.insert(sig_iter.value()->morph);
       }
   }

   QStringList differences = differences_1_letter.toList();
   differences.sort();
   QSet<CSignature*> SignatureSet_1, SignatureSet_2;

   for (int i = 0; i < differences.size(); i++){
       difference = differences[i];
       //--> Pull out the sig_graph_edges with each particular difference (morpheme).
       sig_iter.toFront();
       while (sig_iter.hasNext()){
             sig_iter.next();
             if (sig_iter.value()->morph == difference){
                 CSignature* pSig1 = sig_iter.value()->m_sig_1;
                 CSignature* pSig2 = sig_iter.value()->m_sig_2;
                 SignatureSet_1.insert(pSig1);
                 SignatureSet_2.insert(pSig2);
                 *m_PassiveSignatures << pSig1;
             }
       }

       //--> Now these two sets have signatures which are from opposite sides of the phonological rule. <---//
       compare_opposite_sets_of_signatures(&SignatureSet_1, &SignatureSet_2, difference);

   }
}

/*!
 * This function takes two sets of signatures, each taken from the opposing signatures
 * in a sig-tree-edge, where the morph separating them is a specific morph of length = 1 letter.
 *
 * This is not currently being used.
 */

void CLexicon::compare_opposite_sets_of_signatures(QSet<CSignature*>* sig_set_1, QSet<CSignature*>* sig_set_2, QString morph)
{   sig_graph_edge * p_edge;
    CSignature* pSig_1, *pSig_2;
    QHash<QString,int> Counts;
    foreach(p_edge,  m_SigGraphEdgeMap){
        if (p_edge->morph == morph){
            pSig_1 = p_edge->m_sig_1;
            pSig_2 = p_edge->m_sig_2;
            QString code = pSig_1->get_key() + "@" + pSig_2->get_key();
            Counts[code] = p_edge->get_number_of_words();
        }
    }
}



void CLexicon::dump_suffixes(QList<QString> * pList)
{
    return m_Suffixes->get_suffixes(pList);
}

/**
 * @brief CLexicon::collect_parasuffixes
 * Parasignatures are signatures with only a single stem, hence are not used directly.
 * Parasuffixes are affixes found in a parasignature.
 *
 *
 */

void CLexicon::collect_parasuffixes()
{   sigstring_t     sigstring;
    suffix_t        suffix;
    CSignature*     pSig;
    CSuffix *       pSuffix;
    QStringList     suffixes;
    map_sigstring_to_sig_ptr_iter sig_iter (* m_ParaSignatures->get_map());
    while (sig_iter.hasNext())
    { pSig = sig_iter.next().value();
        sigstring = pSig->get_key();
        suffixes = sigstring.split("=");
        foreach (suffix, suffixes){
            pSuffix = *m_ParaSuffixes <<  suffix;
            pSuffix->increment_count();
        }
    }
    m_ParaSuffixes->sort_by_count();
};





