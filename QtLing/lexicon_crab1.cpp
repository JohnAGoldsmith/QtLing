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

#include "Lexicon.h"
#include "SignatureCollection.h"
#include "StemCollection.h"
#include "SuffixCollection.h"
#include "WordCollection.h"
#include "Word.h"
#include "evaluation.h"
#include "cparse.h"
#include "compound.h"


void SortQStringListFromRight(QStringList& ThisStringList);
bool reverse_string_compare(QString,QString);

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
 */

void CLexicon::Crab_1()
{


    step1_from_words_to_protostems();

    step2_from_protostems_to_parses();

    step3_from_parses_to_stem_to_sig_maps(QString("crab_1"));

    step4_create_signatures(QString("Crab1"));

    //collect_parasuffixes(); // these are suffixes found in a signature with only one stem

    m_SuffixesFlag?
                m_Signatures->compute_containment_list():
                m_PrefixSignatures->compute_containment_list();
    m_SuffixesFlag?
                m_Signatures->calculate_sig_robustness():
                m_PrefixSignatures->calculate_sig_robustness();

}


void CLexicon::Crab_2()
{

    ReSignaturizeWithKnownAffixes();

    m_SuffixesFlag?
                m_Signatures->compute_containment_list():
                m_PrefixSignatures->compute_containment_list();
    m_SuffixesFlag?
                m_Signatures->calculate_sig_robustness():
                m_PrefixSignatures->calculate_sig_robustness();

    replace_parse_pairs_from_current_signature_structure();

    //step10_find_compounds();
}


/**
 * @brief CLexicon::Crab_3
 *
 */
void CLexicon::Crab_3()
{    m_StatusBar->showMessage("Crab 3: Find good signatures inside bad.");

    //collect_parasuffixes();

    step7_FindGoodSignaturesInsideParaSignatures();
    m_StatusBar->showMessage("Crab 3: Find good signatures inside bad, completed.");
    m_SuffixesFlag?
        m_Signatures->calculate_sig_robustness():
        m_PrefixSignatures->calculate_sig_robustness();
    m_SuffixesFlag ?
        m_Signatures->calculate_stem_entropy():
        m_PrefixSignatures->calculate_stem_entropy();

    replace_parse_pairs_from_current_signature_structure();

}


/**
 * @brief CLexicon::Crab_4
 *
 */
void CLexicon::Crab_4()
{
    m_StatusBar->showMessage("Crab 4: eliminate low entropy sigs.");

    replace_parse_pairs_from_current_signature_structure();

    repair_low_entropy_signatures();

    // Temporarily stop these, June 2021
    //step8a_compute_sig_graph_edges();
    //step8b_compute_sig_graph_edge_map();
    //step9_from_sig_graph_edges_map_to_hypotheses();

    // step10_find_compounds();

    m_SuffixesFlag?
                m_Signatures->calculate_sig_robustness():
                m_PrefixSignatures->calculate_sig_robustness();

    test_json_functionality();

     m_StatusBar->showMessage("Crab 4: eliminate low entropy sigs completed.");

}

/**
 * @brief CLexicon::Crab_5
 *
 */
void CLexicon::Crab_5()
{   m_StatusBar->showMessage("Crab 5: split complex morphemes...");
    replace_parse_pairs_from_current_signature_structure();



    step8a_compute_word_sig_pairs();
    //step8b_compute_sig_pair_map(); // this definitely isn't needed anymore. I put its functionality into step8a.

    step8c_from_sig_pairs_to_parses();

    //step9_from_sig_pair_map_to_hypotheses(); //NOTE: this step works but only if we do not first run step 8c; 8c will mess up step 9.


    m_SuffixesFlag?
                m_Signatures->calculate_sig_robustness():
                m_PrefixSignatures->calculate_sig_robustness();

    test_json_functionality();

   m_StatusBar->showMessage("Crab 5: split complex morphemes completed.");
}


/**
 * @brief CLexicon::Crab_6
 *
 */
void CLexicon::Crab_6()
{
   step10_find_compounds();
   m_StatusBar->showMessage("Crab 6:simple compounds completed.");
}




//  <-------------->

QString commonsuffix (QString string1, QString string2)
{
    int l1 = string1.length();
    int l2 = string2.length();
    int shorterlength = l1;
    if (shorterlength > l2 ){
         shorterlength = l2;}
    for (int i =1; i  <= shorterlength; i++){
        if (string1[l1 - i] != string2[l2 - i])
            return string1.right(i-1);
    }
    return string1.right(shorterlength);
}
QString commonprefix (QString string1, QString string2)
{   int l1 = string1.length();
    int l2 = string2.length();
    int shorterlength = l1;
    if (shorterlength > l2 ){
         shorterlength = l2;}
    for (int i =0; i  < shorterlength; i++){
        if (string1[i] != string2[i]){
            return string1.left(i);
        }
    }
    return string1.right(shorterlength);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
 * This is the first of the three initial parts of finding signatures.
 * This makes a cut at every point in a word where the successor frequency
 * is greater than 1.
 * It is divided into two parts;  the first finds protostems, by detecting
 * successfor frequency greater than 1; the second breaks a word after a protostem.
 */
void CLexicon::step1_from_words_to_protostems()
{   word_t          this_word, previous_word, next_word;
    stem_t          potential_stem, this_protostem;

    QStringList *  Words;
    if (m_SuffixesFlag){
        Words = get_word_collection()->GetSortedStringArray();
    }else{
        Words = get_word_collection()->get_reverse_sort_list();
    }

    m_ProgressBar->reset();
    m_ProgressBar->setMinimum(0);
    m_ProgressBar->setMaximum(Words->size());
    m_ParseMap.clear();
 
    int temp_j = 0;
    for (int wordno=0; wordno<Words->size()-1; wordno ++) {
        temp_j++;
        if (temp_j++ == 5000) {
            temp_j = 0;
            m_ProgressBar->setValue(wordno);
            qApp->processEvents();
        }

        this_word = Words->at(wordno);
        next_word = Words->at(wordno+1);
        if (m_SuffixesFlag) { // ==> Suffix case <== //
            this_protostem = commonprefix (this_word, next_word);
            if ( this_protostem.length() >= M_MINIMUM_STEM_LENGTH)
                if ( ! m_suffix_protostems.contains(this_protostem) )
                    m_suffix_protostems[this_protostem] = new protostem(this_protostem, true);
        } else {  // ==> Prefix case <== //
            this_protostem = commonsuffix(this_word,next_word);
            if (this_protostem.length() >= M_MINIMUM_STEM_LENGTH)
                 if ( ! m_prefix_protostems.contains(this_protostem))
                     m_prefix_protostems[this_protostem] = new protostem(this_protostem,false);
        }
    }

      //Second half:  finding protostem objects, which mark their beginning and their end  on the alphabetized word list.
    if (m_SuffixesFlag){
        m_ProgressBar->reset();
        m_ProgressBar->setMinimum(0);
        m_ProgressBar->setMaximum(m_suffix_protostems.size());
        QStringList alphabetized_protostems = m_suffix_protostems.keys();
        alphabetized_protostems.sort();

        int i = 0;
        for (int p = 0; p<alphabetized_protostems.length(); p++){
            QString this_protostem_t = alphabetized_protostems[p];

            while ( ! Words->at(i).startsWith(this_protostem_t))
                i++;
            int j = i;
            while (j < Words->length() && Words->at(j).startsWith(this_protostem_t)){
                j++;
            }
            m_suffix_protostems[this_protostem_t]->set_start_and_end_word(i,j-1);

        }
    } // end of suffix case
    else { // beginning of prefix case
        m_ProgressBar->reset();
        m_ProgressBar->setMinimum(0);
        m_ProgressBar->setMaximum(m_prefix_protostems.size());
        QStringList alphabetized_protostems = m_prefix_protostems.keys();
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
            m_prefix_protostems[this_protostem_t]->set_start_and_end_word(i,j-1);
        }
    } //end of prefix case.
    return;

}

 

/*!
 * This is the second of the three initial parts of finding signatures.
 * This creates stem/affix pairs, which are put in a long list of "Parses":
 * QList<QPair<QString,QString>>* m_Parses
 *
 */
void CLexicon::step2_from_protostems_to_parses()
{
    m_ProgressBar->reset();
    m_ProgressBar->setMinimum(0);
    m_ProgressBar->setMaximum(m_Words->get_count());
    //m_StatusBar->showMessage("2: find stem-affix pairs.");
    QString                     stem, suffix, word, prefix;
    int                         suffix_length, prefix_length, wordno (0);
    map_string_to_word_ptr_iter *   word_iter = m_Words->get_iterator();
    // bool                        DoNotParseCompoundsFlag = true;
    while (word_iter->hasNext())   {
        word = word_iter->next().value()->get_word();
        if (word.contains('-')){
            continue;
        }
        wordno++;
        qApp->processEvents();
        m_ProgressBar->setValue(wordno);
         for (int letterno = M_MINIMUM_STEM_LENGTH; letterno < word.length(); letterno++){
            if (m_SuffixesFlag){
                stem = word.left(letterno);
                if(m_suffix_protostems.contains(stem))  {
                    suffix_length = word.length() - letterno;
                    if (suffix_length > M_MAXIMUM_AFFIX_LENGTH){
                        continue;
                    }
                    suffix = word.right(suffix_length);
                    CParse* this_parse = new CParse(stem, suffix, true);
                    add_parse(this_parse);
                    if (m_Words->contains(stem)){
                        CParse* that_parse = new CParse(stem, QString("NULL"), true );                         
                        add_parse(that_parse);
                    }
                }
            }else{
                stem = word.right(letterno);
                if (m_prefix_protostems.contains(stem)){
                    prefix_length = word.length() - letterno;
                    if (prefix_length > M_MAXIMUM_AFFIX_LENGTH){
                        continue;
                    }
                    prefix = word.left(prefix_length);
                    CParse* this_parse = new CParse(stem,  prefix, false);
                    add_parse(this_parse);
                    if (m_Words->contains(stem)){
                        CParse * that_parse = new CParse(stem, QString("NULL"), false);
                        add_parse(that_parse);
                    }
                }
            } // end of prefixes.
        }
    }
}

// ==========================================================================  //

QString convert_set_to_qstring(affix_set& this_affix_set){
    QStringList temp_presignature = this_affix_set.toList();
    temp_presignature.sort();
    return temp_presignature.join("=");

}

// ==========================================================================  //



/*!
 * This is the third of the four initial parts of finding signatures.
 * This creates signatures, which in turn creates stems and affixes.
 */
void   CLexicon::step3_from_parses_to_stem_to_sig_maps(QString name_of_calling_function)
{  // const int MINIMUM_NUMBER_OF_STEMS = 2;

    /* Since this is going to be used several times, and all but the first time
     * there will be a previous set of stems, it would make sense to contrast
     * the stems made here with the stems in the preceding step -- so we can
     * keep track of what is gained and what is discarded.
     */
    //name_of_calling_function = " " ;
    Stem_to_sig_map                this_stem_to_sig_map;
    m_ProgressBar->reset();
    m_ProgressBar->setMinimum(0);
    m_ProgressBar->setMaximum(m_ParseMap.size());
    qApp->processEvents();
    clear_lexicon();

    //--> We establish a temporary map from stems to sets of affixes as we iterate through parses. <--//
    m_intermediate_stem_to_sig_map.clear();
    step3a_from_parses_to_stem_to_sig_map( m_SuffixesFlag);
    //--> We iterate through these stems and for each stem, create QStringLists of their affixes. <--//
    //--> then we create a "pre-signature" in a map that points to lists of stems. <--//

    step3b_from_stem_to_sig_map_to_sig_to_stem_map();

}

/*!
 * \brief step3a_from_parses_to_stem_to_sig_map
 * \param parses
 * \param suffix_flag
 * \param these_protosigs
 * This function is called by step3 (just below)
 */
void CLexicon::step3a_from_parses_to_stem_to_sig_map(bool suffix_flag )
{
    QString this_stem_t, this_affix_t;
    foreach (CParse* this_parse, m_ParseMap){
        this_stem_t = this_parse->get_stem();
        this_affix_t = this_parse->get_affix();
        if (!m_intermediate_stem_to_sig_map.contains(this_stem_t)){
            m_intermediate_stem_to_sig_map[this_stem_t] = QSet<affix_t>();
        }
        m_intermediate_stem_to_sig_map[this_stem_t].insert(this_affix_t);
    }

}

void CLexicon::step3b_from_stem_to_sig_map_to_sig_to_stem_map()
{
    m_intermediate_sig_to_stem_map.clear();   //replaces "this_signature_to_stems_map"
    m_ProgressBar->reset();
    m_ProgressBar->setMinimum(0);
    m_ProgressBar->setMaximum(m_intermediate_stem_to_sig_map.count());

    int count= 0;
    int i(0);
    foreach(QString this_stem_t, m_intermediate_stem_to_sig_map.keys()){
        i++;
        if (i % 100000) {
            qApp->processEvents();
        }
        m_ProgressBar->setValue(count++);
        QSet<affix_t>& ref_affix_set = m_intermediate_stem_to_sig_map[this_stem_t];

        /* check for repetition, prevent cases like "NULL=ed=er=er[NULL~s]"
         * or "...=li[a~wa]=ali=wali=..." by removing "er" in the former or
         * removing "ali" and "wali" in the latter
         */
        foreach (affix_t affix, ref_affix_set) {
            if (affix.contains('[')) {
                int bracket_start_i = affix.indexOf('[');
                const affix_t reduced_affix = affix.left(bracket_start_i);
                const sigstring_t str_secondary_affixes =
                        affix.mid(bracket_start_i+1, affix.length()-bracket_start_i-2);
                const QList<affix_t> secondary_affixes = str_secondary_affixes.split('~');
                foreach (affix_t secondary_affix, secondary_affixes) {
                    if (secondary_affix == "NULL")
                        secondary_affix = "";
                    const affix_t complete_affix = m_SuffixesFlag?
                                reduced_affix + secondary_affix:
                                secondary_affix + reduced_affix;
                    QSet<affix_t>::iterator iter = ref_affix_set.find(complete_affix);
                    if (iter != ref_affix_set.end())
                        ref_affix_set.erase(iter);
                }
            }
        } // -- added by Hanson 7.31

        const QString this_signature_string = convert_set_to_qstring(ref_affix_set);
        m_intermediate_sig_to_stem_map.attach_stem_to_signature(this_stem_t, this_signature_string);
    }
}
// ==========================================================================  //

void stem_autobiography_negative_notice_not_enough_stems(CLexicon* lexicon, QString stem, QStringList affix_list, QString signature_string, QString calling_function){
    QString message = affix_list.size() <= 40 ?
                signature_string:
                QString("many=many=affixes");
    lexicon->add_to_stem_autobiographies(stem,
                                QString("[%1]=[singleton stem]=%2")
                                .arg(calling_function)
                                .arg(message));

}
void word_autobiography_negative_notice_not_enough_stems(CLexicon* lexicon, QString word, QString stem, QStringList affix_list, QString signature_string, QString calling_function){
    QString message = affix_list.size() <= 40 ?
                signature_string:
                QString("many=many=affixes");
    lexicon->add_to_word_autobiographies(word,
                                QString("[%1]=[singleton stem]=stem: %2 = %3")
                                .arg(calling_function)
                               .arg(stem)
                                .arg(message));
    /*
    qDebug() << word << QString("[%1]=singleton=stem: %2 = %3")
                .arg(calling_function)
                .arg(stem)
                .arg(message);
                */
}

void CLexicon::step4_create_signatures(const QString& name_of_calling_function,
                                       eMinimumStemCountFlag min_stem_count_flag)
{
    sigstring_t             this_signature_string;
    stem_t                  this_stem_t;
    affix_t                 this_affix_t;
    word_t                  this_word_t;
    CSignature*             pSig;
    int                     MINIMUM_NUMBER_OF_AFFIXES_IN_SIGNATURE (2);
    qDebug() << "Core function step4 create signature, SuffixFlag"<< get_suffix_flag();

    //-->  create signatures, stems, affixes:  <--//
    m_ProgressBar->reset();
    m_ProgressBar->setMinimum(0);
    m_ProgressBar->setMaximum(m_intermediate_sig_to_stem_map.m_core.count());

    map_string_to_word_ptr_iter word_iter (*m_Words->get_map());
    while(word_iter.hasNext()){
       CWord* pWord = word_iter.next().value();
       pWord->clear_signatures();
       pWord->clear_parse_triple_map();
   }

    if (m_SuffixesFlag){
        m_Suffixes->clear();
        m_Signatures->clear();
    }
    else {
        m_Prefixes->clear();
        m_PrefixSignatures->clear();
    }
    // -->  Iterate through tentative signatures.    <-- //
    int count (0);
    foreach (this_signature_string, m_intermediate_sig_to_stem_map.m_core.keys() )
    { //---------------------------------------------------------------------------------------------------------//
        qApp->processEvents();
        m_ProgressBar->setValue(count);
        if (count++ % 1000 == 0){
            qApp->processEvents();
        }
      //---------------------------------------------------------------------------------------------------------//
        QSet<stem_t>* this_stem_set = m_intermediate_sig_to_stem_map.get_stem_set(this_signature_string);
        affix_list this_affix_list = this_signature_string.split("=");

        if (this_affix_list.count() < MINIMUM_NUMBER_OF_AFFIXES_IN_SIGNATURE){
            continue;
        }
        if (min_stem_count_flag == MS_ignore_minimum_stem_count
                || this_stem_set->count() >= M_MINIMUM_STEM_COUNT)
        {   if( m_SuffixesFlag) {
                pSig = *m_Signatures       << this_signature_string;
            } else {
                pSig = *m_PrefixSignatures << this_signature_string;
                pSig->set_suffix_flag(false);
            }
            foreach (QString this_affix_t, this_affix_list){
                step4a_link_signature_and_affix(pSig,this_affix_t);
            }
            foreach (this_stem_t, *this_stem_set){

                if (m_ParseMap.contains("populari")){
                    qDebug() << 571 << "contains populari";
                } else{
                  //  qDebug() << 573 << " Doesn't contain populari" ;
                }

                step4b_link_signature_and_stem_and_word(this_stem_t,pSig, name_of_calling_function);
            }
        } // end of condition of having enough stems in the signature.
        else
        {   // if there are not enough stems for this signature: this is here just for words ability to remember where they were and how they were analyzed.
            foreach(this_stem_t, *this_stem_set){
                stem_autobiography_negative_notice_not_enough_stems(this, this_stem_t, this_affix_list, this_signature_string, name_of_calling_function);
                foreach (this_affix_t,this_affix_list){
                    if (this_affix_t == "NULL"){ this_affix_t = ""; }
                    m_SuffixesFlag?
                            this_word_t = this_stem_t + this_affix_t:
                            this_word_t = this_affix_t + this_stem_t;
                    word_autobiography_negative_notice_not_enough_stems(this, this_word_t, this_stem_t, this_affix_list, this_signature_string, name_of_calling_function);
                }
            }
        } // all stems in this set
    }

    m_SuffixesFlag?
                m_suffixal_stems->sort_alphabetically():
                m_prefixal_stems->sort_alphabetically();
    m_Suffixes->sort_by_count();
    m_Prefixes->sort_by_count();
    m_SuffixesFlag ?
                m_Signatures->calculate_stem_entropy():
                m_PrefixSignatures->calculate_stem_entropy();
}
void CLexicon::step4a_link_signature_and_affix(CSignature * pSig, affix_t this_affix)
{
    if (m_SuffixesFlag){
        CSuffix* pSuffix = m_Suffixes->find_or_add(this_affix);
        pSuffix->increment_sig_count();
        pSig->add_affix_ptr(pSuffix);
    } else {
        CPrefix* pPrefix = m_Prefixes->find_or_add(this_affix);
        pPrefix->increment_sig_count();
        pSig->add_affix_ptr(pPrefix);
    }
}
void stem_autobiography_positive_notice (CLexicon* lexicon, QString stem, QString calling_function, QString sig_string){
    lexicon->add_to_stem_autobiographies(stem,
                                         QString("[%1]==%2")
                                         .arg(calling_function)
                                         .arg(sig_string));
}


void CLexicon::step4b_link_signature_and_stem_and_word
(stem_t this_stem_t, CSignature* pSig, const QString& name_of_calling_function)
{
    CStem* pStem;
    QString this_affix, this_word, this_word_split;
    QString this_signature_string = pSig->get_key();
    m_SuffixesFlag ?
            pStem = m_suffixal_stems->find_or_add(this_stem_t):
            pStem = m_prefixal_stems->find_or_add(this_stem_t);

    pStem->add_signature(pSig);
    pSig->add_stem_pointer(pStem);
    stem_autobiography_positive_notice(this, this_stem_t, name_of_calling_function, this_signature_string);
    int stem_count = 0;
    affix_list this_affix_list = this_signature_string.split("=");
    foreach (this_affix, this_affix_list){
        if (!this_affix.contains('[')) {
            // word does not contain secondary affixes, same as before
            if (this_affix == "NULL"){ this_word = this_stem_t; }
            else {
                if (m_SuffixesFlag){
                    this_word = this_stem_t + this_affix;
                    this_word_split = this_stem_t + " " + this_affix;
                   }else{
                        this_word = this_affix + this_stem_t ;
                        this_word_split = this_affix + " " + this_stem_t;
                   }
            }
            // connect word and signature
            CWord* pWord = m_Words->get_word(this_word);
            if (pWord == NULL){
            } else {
                stem_count += pWord->get_word_count();                
                pWord->add_parse_triple(this_stem_t, this_affix, pSig->get_key());
                pWord->add_morphemic_split(this_word_split);
                word_autobiography_positive_notice(this_word, this_stem_t, this_signature_string, name_of_calling_function);
            }
        } else {
            // Iterating through the list of affixes in the associated signatrue,
            // e.g. iterating through the list {NULL, s} in ment[NULL~s]
            int bracket_start_i = this_affix.indexOf('[');
            const QString reduced_affix = this_affix.left(bracket_start_i);
            const QString str_secondary_affixes =
                    this_affix.mid(bracket_start_i+1, this_affix.length()-bracket_start_i-2);
            const QStringList secondary_affixes = str_secondary_affixes.split('~');
            foreach (QString secondary_affix, secondary_affixes) {
                if (secondary_affix == "NULL")
                    secondary_affix = "";
                m_SuffixesFlag ?
                        this_affix = reduced_affix + secondary_affix:
                        this_affix = secondary_affix + reduced_affix;
                m_SuffixesFlag ?
                        this_word = this_stem_t + this_affix:
                        this_word = this_affix + this_stem_t;

                // connecting word and signature
                CWord* pWord = m_Words->get_word(this_word);
                if (!pWord){
                    qDebug() << 611<<  this_word <<  "Step4: this_word notfound among words."
                             << this_stem_t  << this_affix;
                } else {
                    stem_count += pWord->get_word_count();
                    pWord->add_parse_triple(this_stem_t, this_affix, pSig->get_key());
                    word_autobiography_positive_notice_2(this_word, this_stem_t, this_signature_string, name_of_calling_function);
                }
            } // end of iterating through each secondary affix in sig associated with an affix
        } // end of dealing with affixes with associated signature -- added by Hanson 7.30
    }
    pStem->set_count(stem_count);
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

void add_initial_letter (QStringList & this_affix_list, QString letter, bool suffix_flag){
    for (int i = 0; i < this_affix_list.size(); i++){
        if (this_affix_list[i]== "NULL"){
            this_affix_list[i] = letter;
        } else {
            if (suffix_flag){
                this_affix_list[i] = letter + this_affix_list[i];
            }else{
                this_affix_list[i] = this_affix_list[i] + letter;
            }
        }
    }
}

void CLexicon::replace_parse_pairs_from_current_signature_structure()
{
    //m_Raw_parses = new QMap<QString, CParse*> (m_ParseMap);
    m_ParseMap.clear();
    QList<CSignature*> *           these_signatures;
    m_SuffixesFlag?
            these_signatures = m_Signatures->get_signature_list():
            these_signatures = m_PrefixSignatures->get_signature_list();
    CParse*  this_parse;
    QStringList affix_string_list;
    foreach (CSignature* pSig, *these_signatures ){
        affix_string_list = pSig->get_affix_string_list(affix_string_list);
        foreach (CStem* pStem, *pSig->get_stems()){
            const QString& this_stem = pStem->display();
            foreach (QString this_affix, affix_string_list){
                this_parse = new CParse(this_stem, this_affix, m_SuffixesFlag);
                add_parse(this_parse);
            }
        }
    }
}


void CLexicon::repair_low_entropy_signatures()
{
/*  1. iterate through signatures.
        a. if the edge has zero-entropy, make a shorter stem, add the stem-affix pair to Parses, for all stems in sig.
        b. keep pairs of (new stem, whole word) and see if the set of new stems has zero-entropy. if so, repeat. If not, exit.
    2. Call AssignSuffixesToStems.
*/

    qApp->processEvents();
    CStemCollection * stems;
    CSignature * pSig;
    QString letter, affix, stem, stem2, this_morphemic_split;
    QStringList affix_list, stem_list;
    CSignatureCollection * signatures;
    m_SuffixesFlag ?
            stems = m_suffixal_stems:
            stems = m_prefixal_stems;
    m_SuffixesFlag ?
            signatures = m_Signatures:
            signatures = m_PrefixSignatures;

    //----------------------------------------------------------------------------//
    int progress_bar_count = 0;
    m_ProgressBar->reset();
    m_ProgressBar->setMinimum(0);
    m_ProgressBar->setMaximum(signatures->get_count());
    //----------------------------------------------------------------------------//
    QMapIterator<sigstring_t,CSignature*> * sig_iter = new QMapIterator<sigstring_t,CSignature*> (* signatures->get_map() );
    while (sig_iter->hasNext()){
        pSig = sig_iter->next().value();

        //----------------------------------------------------------------------------//
        m_ProgressBar->setValue(progress_bar_count++);
        if (progress_bar_count % 10000 == 0){
            //m_StatusBar->showMessage("5: Shift morpheme break towards root: " + pSig->display() );
            qApp->processEvents();
        }
        //----------------------------------------------------------------------------//

        // if this sig has low entropy ( == is not left-definite ), then consider each stem which ends in that high-freq letter L.
        // For each stem, consider each word generated by each suffix of this signature.
        // For each word, is there another analysis with the stem one-letter shorter? (We can find that by looking at the parses, actually)
        // If so, drop the parse that comes from this signature.
        // Keep track of this erasure of (part of) this signature.
        // put message in the word's biography.

        if (pSig->get_stem_entropy() > m_entropy_threshold_for_stems ){
            continue;
        }
        float EdgeLetterPredominanceThreshold = 0.8;
        letter = pSig->get_highfreq_edge_letters(EdgeLetterPredominanceThreshold);
        foreach (stem, pSig->get_stem_strings(stem_list)) {
            stem2 = stem.left(stem.length()-1);
            foreach (QString affix, pSig->get_affix_string_list(affix_list) ){
                   CParse parse (stem,affix,get_suffix_flag());
                   QString word = parse.display();
                   QString stem2 = stem.left(stem.length()-1);
                   CWord* pWord = get_words()->find_or_fail(word);
                   if (stem2=="abducti"){
                       qDebug() << 807 << " abducti" ;
                   }
                   if (pWord->contains_this_stem_among_parses(stem2)){
                          this_morphemic_split = parse.display_with_gap();
                          pWord->remove_morphemic_split(this_morphemic_split);
                          remove_parse(&parse);
                          qDebug() << 823 << this_morphemic_split;
                   }
            } //end of affixes in this sig
        } // end of stems in this sig;
    } // end of signatures loops
/*
    verify_parses(); //check complete list of parses in Lexicon with what's in the words;

*/
    step3_from_parses_to_stem_to_sig_maps(QString("Shift morpheme boundary leftward"));
    //MS_ignore_minimum_stem_count
    step4_create_signatures(QString("Shift morpheme boundary leftward"));


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
 /*
void CLexicon::replace_parse_pairs_from_current_signature_structure()
{
    m_Parses->clear();

    QList<CSignature*> *           these_signatures;
    m_SuffixesFlag?
            these_signatures = m_Signatures->get_signature_list():
            these_signatures = m_PrefixSignatures->get_signature_list();
    CSignature*                     pSig;
    foreach (pSig, *these_signatures ){
        const QStringList affixes = pSig->display().split("=");
        QList<CStem*>* stem_list = pSig->get_stems();
        CStem* pStem;
        foreach (pStem, *stem_list){
            const QString& this_stem = pStem->display();
            QString this_affix;
            foreach (this_affix, affixes){
                //if (this_affix == "NULL") this_affix = "";
                CParse*                         this_parse;
                m_SuffixesFlag?
                        this_parse = new CParse(this_stem, this_affix, true):
                        this_parse = new CParse(this_affix, this_stem, false);
                m_Parses->append(this_parse);

            }
        }

    }

   // while (sig_iter->hasNext()){
   //     pSig = sig_iter->next().value();
   //     affixes = pSig->display().split("=");
//
  //      stem_list =  pSig->get_stems();
  //      foreach (CStem* p_this_stem, *stem_list){
  //          foreach (this_affix, affixes){
  //              //if (this_affix == "NULL") this_affix = "";
  //              m_SuffixesFlag?
  //                      this_parse = new CParse(this_stem, this_affix, true):
  //                      this_parse = new CParse(this_affix, this_stem, false);
  //          }
  //      }
  //
}
 */




/*!
 * This function looks at pairs of signatures joined by a sig-tree-edge, where
 * the morpheme that separates them is a single letter. We look to see  how tight
 * the fit is between these two sets of signatures.
 */
void CLexicon::test_for_phonological_relations_between_signatures()
{
    sig_pair_iter  sig_iter (m_SigPairMap);
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
{   sig_pair * p_edge;
    //TEMP:
    (void) sig_set_1;
    (void) sig_set_2;
    CSignature* pSig_1, *pSig_2;
    QHash<QString,int> Counts;
    foreach(p_edge,  m_SigPairMap){
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
/*
void CLexicon::collect_parasuffixes()
{
    sigstring_t     sigstring;
    suffix_t        suffix;
    CSignature*     pSig;
    CSuffix *       pSuffix;
    QStringList     suffixes;
    map_sigstring_to_sig_ptr_iter sig_iter (* m_ParaSignatures->get_map());
    while (sig_iter.hasNext())
    {
        pSig = sig_iter.next().value();
        sigstring = pSig->get_key();
        qDebug() << sigstring;
        suffixes = sigstring.split("=");
        foreach (suffix, suffixes){
            pSuffix = *m_ParaSuffixes <<  suffix;
            pSuffix->increment_sig_count();
        }
    }
    m_ParaSuffixes->sort_by_count();
}
*/
