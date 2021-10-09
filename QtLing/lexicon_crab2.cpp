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
#include <QMessageBox>
#include "Lexicon.h"

#include "SignatureCollection.h"
#include "StemCollection.h"
#include "SuffixCollection.h"
#include "WordCollection.h"
#include "Word.h"
#include "cparse.h"
#include "compound.h"

extern bool contains(QStringList & container, QStringList & contained);
extern QString QStringList2signature(QStringList);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*!
* This is lke AssignSuffixesToStems, but crucially: with Suffixes known ahead of time.
* This creates signatures and stems: signatures that only have approved affixes.
* I need to refactorize this function.
*/
// this functionality is already present in FindGoodSignaturesInsideBad, below, so it's not needed.
// Not used, should be removed. Oct 9 2021. JG.
void CLexicon::ReSignaturizeWithKnownAffixes()  // step 2
{
    m_StatusBar->showMessage("Crab 2: Resignaturize with known affixes");
    m_ProgressBar->reset();
    m_ProgressBar->setMinimum(0);
    m_ProgressBar->setMaximum(m_ParseMap.size());

    CStemCollection*            stems;
    m_SuffixesFlag ?
               stems = m_suffixal_stems:
               stems = m_prefixal_stems;

   //--> We establish a temporary map from stems to sets of affixes as we iterate through parses. <--//
   //--> THIS is where the continuations that are not affixes are eliminated -- well, they are not
   //    eliminated, but they are not copied into ref_stems_to_affix_set. Changing that to a Protosigs.
    m_intermediate_sig_to_stem_map.clear();
    m_intermediate_stem_to_sig_map.clear();


    create_new_parse_set_from_known_affixes();
    step3_from_parses_to_stem_to_sig_maps("Resignaturize with known affixes");
    step4_create_signatures("Resignaturize with known affixes");
    replace_parse_pairs_from_current_signature_structure();


 }
/**
 * helper function for preceding function.
 *
 */
void CLexicon::create_new_parse_set_from_known_affixes()
{
    qApp->processEvents();

    int                         count = 0;
    QString                     this_stem_t, this_affix_t;
    m_Raw_parses  =             QMap<QString,CParse*>(m_ParseMap);
     m_ParseMap.clear();

    // iterate through parselist, and assign to stem and affix collections;
    foreach (CParse* this_parse, m_Raw_parses){
        count++;
        if (count % 10000 == 0){
            m_ProgressBar->setValue(count);
            qApp->processEvents();
        }
        this_affix_t = this_parse->get_affix();
        if (m_SuffixesFlag){
            if (! m_Suffixes->contains(this_affix_t)){
                    continue;
            }
        } else{
            if (! m_Prefixes->contains(this_affix_t)){
                continue;
            }
        }        
        add_parse(this_parse);
    }
}

bool Contains(QList<CSuffix*>* list1, QList<CSuffix*>* list2 ) {
    foreach (CSuffix* suffix, *list2){
        qDebug() << 94 << suffix;
        if (! list1->contains(suffix)){
            return false;
        }
    }
    return true;
}
bool Contains(QList<CPrefix*>* list1, QList<CPrefix*>* list2 ) {
    foreach (CPrefix* prefix, *list2){
        if (! list1->contains(prefix)){
            return false;
        }
    }
    return true;
}
int IntersectionCount(QList<CSuffix*>* list1, QList<CSuffix*>* list2){
    int count = 0;
    foreach (CSuffix* suffix, *list2){
        if (list1->contains(suffix)){
           count ++;
        }
    }
    return count;
}
int IntersectionCount(QList<CPrefix*>* list1, QList<CPrefix*>* list2){
    int count = 0;
    foreach (CPrefix* prefix, *list2){
        if (list1->contains(prefix)){
           count ++;
        }
    }
    return count;
}
int intersection_count(QStringList& list1, QStringList& list2){
    int count = 0;
    if (list1.count() > list2.count() ){
        foreach (QString item, list2){
            if (list1.contains(item)) count++;
        }
    }else{
        foreach (QString item, list1){
            if (list2.contains(item)){
                count++;
            }
        }
    }
   return count;
}

/*!
 *  This function looks at all the non-signatures that were rejected
 *  because they were associated with only one stem. For each one, it
 *  looks for the broadest signature that occurs inside it, and assigns
 *  its stem to that signature.
 *
 * 2nd function of Crab 2.
 */
void   CLexicon::step7_FindGoodSignaturesInsideParaSignatures()  // step 2
{   stem_t                      this_stem;
    word_t                      this_word;
    affix_t                     this_affix;
    affix_list                  affixes_of_residual_sig;
    CSignature*                 pBestSig, *this_sig, *pSig;
    CSignatureCollection*       signatures;
    //suffix_t                    Null_string ("NULL");
    CStemCollection*            stems;
    int                         best_intersection_count = 0;
    QStringList                 best_sig_affix_list;
    QStringList                 working_prefix_string_list;
    QStringList                 working_suffix_string_list;
    CSuffix*                    pSuffix;
    CPrefix*                    pPrefix;

    qApp->processEvents();

    replace_parse_pairs_from_current_signature_structure();
    generate_virtual_signatures();


    QMap<QString, protostem*> * these_protostems;
    if (m_SuffixesFlag) {
         signatures = m_Signatures;  // TODO remove this;
         signatures = m_VirtualSignatures;
         stems = m_suffixal_stems;
         these_protostems = & m_suffix_protostems;

    } else{
         signatures = m_PrefixSignatures;
         stems = m_prefixal_stems;
         these_protostems = & m_prefix_protostems;
    }
    signatures->sort(SIG_BY_REVERSE_ROBUSTNESS);


    //--------------------------------------------------------------------------------//
    m_ProgressBar->reset();
    m_ProgressBar->setMinimum(0);
    m_ProgressBar->setMaximum(these_protostems->count());
    //--------------------------------------------------------------------------------//

    int protostem_count = 0;
    int temp_i = 0;
    foreach (auto this_protostem, * these_protostems)
    {   //-----------------------------------------------------------------------------------------//
        m_ProgressBar->setValue(protostem_count++);
        qApp->processEvents();
        temp_i++;
        if (temp_i == 1000){
            temp_i = 0;
            m_StatusBar->showMessage("Crab 2: Find good signatures inside bad: " + this_stem);
            qApp->processEvents();
        } //---------------------------------------------------------------------------------------//
        affixes_of_residual_sig.clear();
        working_suffix_string_list.clear();
        working_prefix_string_list.clear();
        stem_t this_stem = this_protostem->get_stem();
        int stem_length = this_stem.length();
        if (stems->find_or_fail( this_stem )){
            continue;
        }
        for (int wordno= this_protostem->get_start_word(); wordno <= this_protostem->get_end_word(); wordno++){
            if (m_SuffixesFlag){
                this_word = m_Words->get_string_from_sorted_list(wordno);
                this_affix = this_word.mid( stem_length );
                if (this_affix.length() == 0){this_affix = "NULL";}               
                pSuffix =  m_Suffixes->find_or_fail(this_affix);
                if (! pSuffix ){
                    add_parasuffix(this_affix, this_word);
                    continue;
                }
                working_suffix_string_list.append(this_affix);
                if (this_affix.length()==0) qDebug() << 230 << "empty continuation"<< this_stem;
            } else{
                this_word = m_Words->get_reverse_sort_list()->at(wordno);
                this_affix = this_word.left(this_word.length()- stem_length);
                if (this_affix.length() == 0){ this_affix = "NULL"; }
                pPrefix = m_Prefixes->find_or_fail(this_affix);
                if (! pPrefix){
                    add_paraprefix(this_affix, this_word);
                    continue;
                }
                working_prefix_string_list.append(this_affix);
            }
        }
        // go thru signatures, find the highest robustness sig in WorkingSig, and then successively larger sigs, always containing the previous best one found.
        int i;
        best_intersection_count = 0;
        pBestSig = NULL;
        if (get_suffix_flag()){
            if (working_suffix_string_list.length() < 2) continue;
            working_suffix_string_list.sort();
            for (i=0; i < signatures->get_count(); i++){
                this_sig = signatures->get_at_sorted(i);
                QStringList this_sig_affix_string_list = this_sig->get_affix_string_list();
                if (this_sig_affix_string_list.length() > working_suffix_string_list.length()){
                    continue;
                }
                if (this_sig->get_robustness() < 5){
                    break;
                }
                if (contains(working_suffix_string_list, this_sig_affix_string_list)){
                    pBestSig = this_sig;
                    best_sig_affix_list = this_sig_affix_string_list;
                    best_intersection_count = intersection_count(working_suffix_string_list, best_sig_affix_list);
                    break;
                }
            // there is no signature that appears inside the working affix list, so we give up on this stem.
            }
        }else{  //  TODO
            if (working_prefix_string_list.length() == 0) continue;
            working_prefix_string_list.sort();
            for (i=0; i < signatures->get_count(); i++){
                this_sig = signatures->get_at_sorted(i);
                QStringList this_sig_affix_string_list = this_sig->get_affix_string_list();
                if (this_sig->get_robustness() < 5){
                    break;
                }
                if (contains(this_sig_affix_string_list, working_prefix_string_list)){
                    pBestSig = this_sig;
                    best_sig_affix_list = this_sig_affix_string_list;
                    best_intersection_count = intersection_count(working_prefix_string_list, best_sig_affix_list);

                    break;
                }
            // there is no signature that appears inside the working affix list, so we give up on this stem.
            }
        }
        if (! pBestSig){
            continue; // to next protostem
        }
        for (; i < signatures->get_count(); i++){
             pSig = signatures->get_at_sorted(i);
             QStringList sig_affix_list = pSig->get_affix_string_list();
             // this can be done fast, so do it first:
             if (sig_affix_list.length() <= best_intersection_count) {continue;}
             if ( contains(sig_affix_list, best_sig_affix_list) &&
                  contains(working_suffix_string_list, sig_affix_list) &&
                  intersection_count(sig_affix_list, working_suffix_string_list) > best_intersection_count){
                    pBestSig = pSig;
                    best_intersection_count = intersection_count(sig_affix_list, working_suffix_string_list);
                    best_sig_affix_list = sig_affix_list;
             }
        }
        foreach (QString this_affix, best_sig_affix_list){
            //if (this_affix == "NULL") this_affix = "";
            add_parse(new CParse(this_stem, this_affix, m_SuffixesFlag));

        }
        // this is the right place to identify parasuffixes -- the extensions of *real* stems, not protostems (as is currently done).

    }// end of protostem loop

    step3_from_parses_to_stem_to_sig_maps("Good sigs inside bad");
    step4_create_signatures("Good sigs inside bad", MS_ignore_minimum_stem_count);
    replace_parse_pairs_from_current_signature_structure();

}

