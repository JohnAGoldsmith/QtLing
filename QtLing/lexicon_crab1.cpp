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

    time_stamp("crab 1");
    // time_stamp does not work here because the stem collection is empty before step 4.

    step3_from_parses_to_stem_to_sig_maps(QString("crab_1"));

    step4_create_signatures(QString("Crab1"));

    step5a_replace_parse_pairs_from_current_signature_structure(); // from_

    step5b_find_full_signatures();

    collect_parasuffixes();

    m_SuffixesFlag?
                m_Signatures->compute_containment_list():
                m_PrefixSignatures->compute_containment_list();

}

//  <-------------->

////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
 * This is the first of the three initial parts of finding signatures.
 * This makes a cut at every point in a word where the successor frequency
 * is greater than 1.
 * It is divided into two parts;  the first finds protostems, by detecting
 * successfor frequency greater than 1; the second breaks a word after a protostem.
 */
void CLexicon::step1_from_words_to_protostems()
{   word_t          this_word, previous_word;
    bool            StartFlag = true;
    bool            DifferenceFoundFlag = false;
    stem_t          potential_stem;
    int             this_word_length(0), previous_word_length(0);
    //int             MINIMUM_STEM_LENGTH = 4;

    QStringList *  Words;
    if (m_SuffixesFlag){
        Words = get_word_collection()->GetSortedStringArray();
    }else{
        Words = get_word_collection()->get_reverse_sort_list();
    }

    m_ProgressBar->reset();
    m_ProgressBar->setMinimum(0);
    m_ProgressBar->setMaximum(Words->size());
    m_StatusBar->showMessage("1. Find proto-stems.");
    m_Parses->clear();
    //m_Parse_map.clear();


    int temp_j = 0;
    for (int wordno=0; wordno<Words->size(); wordno ++) {
        temp_j++;
        if (temp_j++ == 5000) {
            temp_j = 0;
            m_ProgressBar->setValue(wordno);
            qApp->processEvents();
        }

        this_word = Words->at(wordno);
        this_word_length = this_word.length();
        if (StartFlag){
            StartFlag = false;
            previous_word = this_word;
            previous_word_length = this_word_length;
            continue;
        }
        DifferenceFoundFlag = false;
        int end = qMin(this_word_length, previous_word_length);
        if (m_SuffixesFlag){   // -->   Suffix case   <-- //
            for (int i=0; i <end; i++){
                if (i < M_MINIMUM_STEM_LENGTH ) continue;
                if (previous_word[i] != this_word[i]){
                    potential_stem = previous_word.left(i);
                    if (potential_stem.length()== 0){continue;}
                    DifferenceFoundFlag = true;
                    if (!m_suffix_protostems.contains(potential_stem)){
                        m_suffix_protostems[potential_stem] = new protostem(potential_stem, true);
                    }
                    break;
                } // end of having found a difference.
            }// end of loop over this word;
            if (DifferenceFoundFlag == false){
                if (!m_suffix_protostems.contains(previous_word)
                        && previous_word_length >= M_MINIMUM_STEM_LENGTH){
                    m_suffix_protostems[previous_word] = new protostem(previous_word, true);
                }
            }
        }  // end of suffix case.
        else
        {       // -->   Prefix case   <-- //
            for (int i=1; i <=end; i++){
                if (previous_word.right(i) != this_word.right(i)){
                    if (i-1 < M_MINIMUM_STEM_LENGTH) continue;
                    potential_stem = previous_word.right(i-1);
                    if (potential_stem.length() == 0) {continue;}
                    DifferenceFoundFlag = true;
                    if (!m_prefix_protostems.contains(potential_stem)) {
                        m_prefix_protostems[potential_stem] = new protostem(potential_stem, false);
                    }
                    break;
                }// end of having found a difference.
            } // end of loop over this word;
            if (DifferenceFoundFlag == false){
                if (!m_prefix_protostems.contains(previous_word)
                        && previous_word_length >= M_MINIMUM_STEM_LENGTH){
                    m_prefix_protostems[previous_word] = new protostem(previous_word, false);
                }
            }
        } // end of prefix case.
        previous_word = this_word;
        previous_word_length = this_word_length;
    } // word loop.
    //Second half:  finding protostem objects, which mark their beginning and their end on the alphabetized word list.
    if (m_SuffixesFlag){
        m_ProgressBar->reset();
        m_ProgressBar->setMinimum(0);
        m_ProgressBar->setMaximum(m_suffix_protostems.size());
        QStringList alphabetized_protostems = m_suffix_protostems.keys();
        alphabetized_protostems.sort();
        int alpha = 0;
        //int omega = 0;
        //int start_index = 0;

        for (int p = 0; p<alphabetized_protostems.length(); p++){
            QString this_protostem_t = alphabetized_protostems[p];
            int protostem_length = this_protostem_t.length();
            int i = alpha;
            while (Words->at(i).left(protostem_length) != this_protostem_t){
                i++;
            }
            int j = i;
            alpha = i;
            while (j < Words->length() && Words->at(j).left(protostem_length) == this_protostem_t){
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
        int alpha = 0;
        //int omega = 0;
        //int start_index = 0;
        for (int p = 0; p<alphabetized_protostems.length(); p++){
            QString this_protostem_t = alphabetized_protostems[p];
            int protostem_length = this_protostem_t.length();
            int i = alpha;
            while (Words->at(i).right(protostem_length) != this_protostem_t){
                i++;
            }
            int j = i;
            alpha = i;
            while (j < Words->length() && Words->at(j).right(protostem_length) == this_protostem_t){
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
    m_StatusBar->showMessage("2: find stem-affix pairs.");
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
                    CParse* this_parse = new CParse(stem, suffix);
                    m_Parses->append(this_parse);
                    if (m_Words->contains(stem)){
                        CParse* that_parse = new CParse(stem, QString("NULL"), true );
                        m_Parses->append(that_parse);
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
                    CParse* this_parse = new CParse(prefix, stem);
                    m_Parses->append(this_parse);
                    if (m_Words->contains(stem)){
                        CParse * that_parse = new CParse(QString("NULL"), stem, false);
                        m_Parses->append(that_parse);
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

    Stem_to_sig_map                this_stem_to_sig_map;
    // morph_set *                 pSet;
    // CWord*                      pWord;
    m_ProgressBar->reset();
    m_ProgressBar->setMinimum(0);
    m_ProgressBar->setMaximum(m_Parses->size());
    m_StatusBar->showMessage("3: assign affixes to stems.");
    qApp->processEvents();
    clear_lexicon();

    //--> We establish a temporary map from stems to sets of affixes as we iterate through parses. <--//

    step3a_from_parses_to_stem_to_sig_map(m_Parses, m_SuffixesFlag);

    //-----------------------------------------------------------------------------------------------//

    //--> We iterate through these stems and for each stem, create QStringLists of their affixes. <--//
    //--> then we create a "pre-signature" in a map that points to lists of stems. <--//
     step3b_from_stem_to_sig_map_to_sig_to_stem_map();

     //-----------------------------------------------------------------------------------------------//'
    m_StatusBar->showMessage("3: completed.");
}

/*!
 * \brief step3a_from_parses_to_stem_to_sig_map
 * \param parses
 * \param suffix_flag
 * \param these_protosigs
 * This function is called by step3 (just below)
 */
void CLexicon::step3a_from_parses_to_stem_to_sig_map(QList<CParse*> * parses, bool suffix_flag )
{
    QString this_stem_t, this_affix_t;
    int i = 0;
    for (int parseno = 0; parseno < parses->size(); parseno++){
        CParse * this_pair = parses->at(parseno);
        if (suffix_flag){
            this_stem_t = this_pair->get_string1();
            this_affix_t = this_pair->get_string2();
        } else{
            this_stem_t = this_pair->get_string2();
            this_affix_t = this_pair->get_string1();
        }
        i++;
        if (i == 10000) {
            i = 0;
            m_StatusBar->showMessage("3a: "+ this_stem_t + this_affix_t);
            qApp->processEvents();
        }
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
        if (i == 100000) {
            i = 0;
            m_StatusBar->showMessage("3b: stem"+ this_stem_t);
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


void CLexicon::step4_create_signatures(const QString& name_of_calling_function,
                                       eMinimumStemCountFlag min_stem_count_flag)
{
    sigstring_t             this_signature_string;
    stem_t                  this_stem_t;
    affix_t                 this_affix_t;
    word_t                  this_word_t;
    //CWord*                  pWord;

    //const int               MINIMUM_NUMBER_OF_STEMS = 2;
    CSignature*             pSig;

    //-->  create signatures, stems, affixes:  <--//
    m_ProgressBar->reset();
    m_ProgressBar->setMinimum(0);
    m_ProgressBar->setMaximum(m_intermediate_sig_to_stem_map.m_core.count());
    m_StatusBar->showMessage("4: create signatures.");

    // this is an experiment
    map_string_to_word_ptr_iter word_iter (*m_Words->get_map());
    while(word_iter.hasNext()){
       CWord* pWord = word_iter.next().value();
       pWord->clear_signatures();
       pWord->clear_parse_triple_map();
   } // moved this from step 7b to here, Hanson, 7.31

    if (m_SuffixesFlag){
        m_Suffixes->clear();
        m_Signatures->clear();
    }
    else {
        m_Prefixes->clear();
        m_PrefixSignatures->clear();
    }
    // -->  Iterate through tentative signatures.    <-- //
    int temp_i = 0;
    int count (0);
    foreach (this_signature_string, m_intermediate_sig_to_stem_map.m_core.keys() )
    {
        qApp->processEvents();
        count++;
        m_ProgressBar->setValue(count);
        temp_i++;
        if (temp_i == 1000){
            temp_i = 0;
            m_StatusBar->showMessage("4: create signatures: " + this_signature_string);
            qApp->processEvents();
        }

        QSet<stem_t>* this_stem_set = m_intermediate_sig_to_stem_map.get_stem_set(this_signature_string);
        affix_list this_affix_list = this_signature_string.split("=");

        if (min_stem_count_flag == MS_ignore_minimum_stem_count
                || this_stem_set->size() >= M_MINIMUM_STEM_COUNT)
        {
            if( m_SuffixesFlag) {
                // CSignature* pSig;
                pSig = *m_Signatures       << this_signature_string;
            } else {
                pSig = *m_PrefixSignatures << this_signature_string;
                pSig->set_suffix_flag(false);
            }
            foreach (QString this_affix_t, this_affix_list){
                step4a_link_signature_and_affix(pSig,this_affix_t);
            }
            foreach (this_stem_t, *this_stem_set){
                step4b_link_signature_and_stem_and_word(this_stem_t,pSig, this_signature_string, name_of_calling_function);
            }

        } // end of condition of having enough stems in the signature.
        else
        {   // if there are not enough stems for this signature: this is here just for words ability to remember where they were and how they were analyzed.

            foreach(this_stem_t, *this_stem_set){
                QString message = this_affix_list.size() <= 50 ?
                            this_signature_string:
                            QString("Super long list of affixes");
                add_to_stem_autobiographies(this_stem_t,
                                            QString("[%1]=Set of affixes"
                                                    " not qualified as a signature=%2")
                                            .arg(name_of_calling_function)
                                            .arg(message));

                foreach (this_affix_t,this_affix_list){
                    if (this_affix_t == "NULL"){
                        this_affix_t = "";
                    }

                    m_SuffixesFlag?
                            this_word_t = this_stem_t + this_affix_t:
                            this_word_t = this_affix_t + this_stem_t;
                    add_to_word_autobiographies(this_word_t,
                                                QString("[%1]=Set of affixes"
                                                        " not qualified as signature=Stem: %2=%3")
                                                .arg(name_of_calling_function)
                                                .arg(this_stem_t)
                                                .arg(message));

                    // old autobiography code
                    /*
                    pWord = m_Words->find_or_fail(this_word_t);
                    pWord->add_to_autobiography("failure of " + name_of_calling_function + "=" + this_stem_t + "=" + this_signature_string );
                    */
                }
            }
        }
    }

    m_Suffixes->sort_by_count();
    m_Prefixes->sort_by_count();
    m_SuffixesFlag ?
                m_Signatures->calculate_stem_entropy():
                m_PrefixSignatures->calculate_stem_entropy();
    m_StatusBar->showMessage("Computation of Crab 1 completed.");
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

void CLexicon::step4b_link_signature_and_stem_and_word
(stem_t this_stem_t, CSignature* pSig, QString this_signature_string, const QString& name_of_calling_function)
{
    CStem* pStem;
    QString this_affix, this_word;
    m_SuffixesFlag ?
            pStem = m_suffixal_stems->find_or_add(this_stem_t):
            pStem = m_prefixal_stems->find_or_add(this_stem_t);

    // check if new signature already exists in the stem's list of signatures
    /*
    bool duplicate_flag = false;
    foreach (CSignature* p_sig, *(pStem->GetSignatures())) {
        if (p_sig->get_key() == this_signature_string) {
            duplicate_flag = true;
            break;
        }
    }
    if (!duplicate_flag)
        pStem->add_signature (pSig);
    // Added by Hanson 7.30,
    // moved to definition of CStem::add_signature, 7.31
    */
    pStem->add_signature(pSig);
    pSig->add_stem_pointer(pStem);

    add_to_stem_autobiographies(this_stem_t,
                                QString("[%1]=Found signature=%2")
                                .arg(name_of_calling_function)
                                .arg(this_signature_string));

    int stem_count = 0;
    affix_list this_affix_list = this_signature_string.split("=");
    foreach (this_affix, this_affix_list){
        // added this condition to detect if affix is associated with a signature
        // -- added by Hanson 7.30
        if (!this_affix.contains('[')) {
            // word does not contain secondary affixes, same as before
            if (this_affix == "NULL"){
                this_word = this_stem_t;
            } else {
                m_SuffixesFlag ?
                    this_word = this_stem_t + this_affix :
                    this_word = this_affix + this_stem_t ;
            }

            // connecting word and signature
            CWord* pWord = m_Words->get_word(this_word);
            if (pWord == NULL){
                qDebug() << this_word <<  "Step4: this_word not found among words."
                         << this_stem_t  << this_affix;
            } else {
                stem_count += pWord->get_word_count();
                pWord->add_parse_triple(this_stem_t, this_affix, pSig->get_key());
                QString message = this_affix_list.size() <= 50 ?
                            this_signature_string:
                            QString("Super long signature");
                add_to_word_autobiographies(this_word,
                                            QString("[%1]=Found signature=Stem: %2=%3")
                                            .arg(name_of_calling_function)
                                            .arg(this_stem_t)
                                            .arg(message));
                /*pWord->add_to_autobiography(QString("Found signature")
                                            + "=" + this_stem_t + "=" + message); */
            }
        } else {
            // Iterating through the list of affixes in the associated signatrue,
            // e.g. iterating through the list {NULL, s} in ment[NULL~s]
            // I call these "secondary signatures"
            int bracket_start_i = this_affix.indexOf('[');
            const QString reduced_affix = this_affix.left(bracket_start_i);
            const QString str_secondary_affixes =
                    this_affix.mid(bracket_start_i+1, this_affix.length()-bracket_start_i-2);
            const QStringList secondary_affixes = str_secondary_affixes.split('~');
            foreach (QString secondary_affix, secondary_affixes) {
                // iterate through list of secondary suffixes, and
                // get string of whole word using concatenation
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
                    qDebug() << this_word <<  "Step4: this_word not found among words."
                             << this_stem_t  << this_affix;
                } else {
                    stem_count += pWord->get_word_count();
                    pWord->add_parse_triple(this_stem_t, this_affix, pSig->get_key());
                    QString message = this_affix_list.size() <= 50 ?
                                this_signature_string:
                                QString("Super long signature");
                    add_to_word_autobiographies(this_word,
                                                QString("[%1]=Found signature=Stem: %2=%3")
                                                .arg(name_of_calling_function)
                                                .arg(this_stem_t)
                                                .arg(message));

                    /* // removing old autobiography record system
                    pWord->add_to_autobiography(QString("Found signature")
                                              + "=" + this_stem_t + "=" + message); */
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

void CLexicon::step5a_replace_parse_pairs_from_current_signature_structure()
{
    clear_parses();

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
    /*
    while (sig_iter->hasNext()){
        pSig = sig_iter->next().value();
        affixes = pSig->display().split("=");

        stem_list =  pSig->get_stems();
        foreach (CStem* p_this_stem, *stem_list){
            foreach (this_affix, affixes){
                //if (this_affix == "NULL") this_affix = "";
                m_SuffixesFlag?
                        this_parse = new CParse(this_stem, this_affix, true):
                        this_parse = new CParse(this_affix, this_stem, false);
            }
        }
    }*/
}


void CLexicon::step5b_find_full_signatures()
{
/*  1. iterate through signatures.
        a. if the edge has zero-entropy, make a shorter stem, add the stem-affix pair to Parses, for all stems in sig.
        b. keep pairs of (new stem, whole word) and see if the set of new stems has zero-entropy. if so, repeat. If not, exit.
    2. Call AssignSuffixesToStems.
*/
    int temp_i = 0;
    int progress_bar_count = 0;
    m_ProgressBar->reset();
    m_ProgressBar->setMinimum(0);
    m_ProgressBar->setMaximum(m_intermediate_sig_to_stem_map.m_core.count());
    m_StatusBar->showMessage("5: Find full signatures inside hollow signatures.");

    qApp->processEvents();
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
        m_ProgressBar->setValue(progress_bar_count++);
        pSig = sig_iter->next().value();

        temp_i++;
        if (temp_i == 10000){
            temp_i = 0;
            m_StatusBar->showMessage("5: Find full signatures inside hollow signatures: " + pSig->display() );
            qApp->processEvents();
        }

        if (pSig->get_stem_entropy() > 0){
            continue; // skip if the stem-entropy of the stem is greater than 0
        }
        // get last letter of stems; the last letter of all corresponding
        // stems of a signature with 0 entropy should be the same
        m_SuffixesFlag?
                    letter = pSig->get_stems()->at(0)->get_key().right(1):
                letter = pSig->get_stems()->at(0)->get_key().left(1);
        stem_list.clear();
        pSig->get_stem_strings(stem_list); // save list of stems into stem_list
        remove_final_letter(stem_list, m_SuffixesFlag);
        pSig->get_string_list(affix_list);
        add_initial_letter (affix_list, letter, m_SuffixesFlag);

        for (int i = 0; i < stem_list.size(); i++){
            stem = stem_list.at(i);
            for (int affno = 0; affno < affix_list.size(); affno++){
                affix = affix_list.at(affno);
                if (m_SuffixesFlag){
                    CParse* pParse = new CParse(stem, affix, true);
                    m_Parses->append(pParse);
                } else{
                    CParse* pParse = new CParse(affix, stem, false);
                    m_Parses->append(pParse);
                    if (stem.length() == 0 ){
                        qDebug() << 523 << "null stem";
                    }
                }
            }

            // create new protostem object for that stem
            QMap<QString, protostem*>& ref_protostem_map = m_SuffixesFlag ?
                        m_suffix_protostems : m_prefix_protostems;
            QStringList* words = m_SuffixesFlag?
                        get_word_collection()->GetSortedStringArray():
                        get_word_collection()->get_reverse_sort_list();
            int stem_length = stem.length();
            int words_length = words->length();
            if (!ref_protostem_map.contains(stem)) {
                protostem* p_protostem = new protostem(stem);
                ref_protostem_map[stem] = p_protostem;
                int i, j;
                if (m_SuffixesFlag) {
                    i = 0;
                    while (words->at(i).left(stem_length) != stem)
                        i++;
                    j = i;
                    while (words->at(j).left(stem_length) == stem && j < words_length)
                        j++;
                } else {
                    i = 0;
                    while (words->at(i).right(stem_length) != stem)
                        i++;
                    j = i;
                    while (words->at(j).right(stem_length) == stem && j < words_length)
                        j++;
                }
                p_protostem->set_start_and_end_word(i, j-1);
            }
            // finish creating new protostem object
        }
    }

    step3_from_parses_to_stem_to_sig_maps(QString("Finding full signatures"));
    step4_create_signatures(QString("Finding full signatures"));
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
        suffixes = sigstring.split("=");
        foreach (suffix, suffixes){
            pSuffix = *m_ParaSuffixes <<  suffix;
            pSuffix->increment_sig_count();
        }
    }
    m_ParaSuffixes->sort_by_count();

}
