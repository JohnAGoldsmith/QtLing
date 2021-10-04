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

extern bool contains(QList<QString> * list2, QList<QString> * list1);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*!
* This is lke AssignSuffixesToStems, but crucially: with Suffixes known ahead of time.
* This creates signatures and stems: signatures that only have approved affixes.
* I need to refactorize this function.
*/
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

/*!
 *  This function looks at all the non-signatures that were rejected
 *  because they were associated with only one stem. For each one, it
 *  looks for the broadest signature that occurs inside it, and assigns
 *  its stem to that signature.
 *
 * 2nd function of Crab 2.
 */
void   CLexicon::step7_FindGoodSignaturesInsideParaSignatures()  // step 3
{   stem_t                      this_stem;
    word_t                      this_word;
    affix_t                     this_affix;
    affix_list                  affixes_of_residual_sig;
    CSignature*                 pBestSig, *this_sig, *pSig;
    CSignatureCollection*       signatures;
    suffix_t                    Null_string ("NULL");
    CStemCollection*            stems;
    int                         best_intersection_count = 0;
    QStringList                 best_affix_list;
    QList<CPrefix*>             working_prefix_list;
    QList<CSuffix*>             working_suffix_list;
    CSuffix*                    pSuffix;
    CPrefix*                    pPrefix;



    //m_StatusBar->showMessage("Crab 3: Find good signatures inside bad.");
    qApp->processEvents();

    QMap<QString, protostem*> * these_protostems;
    if (m_SuffixesFlag) {
         signatures = m_Signatures;
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
            m_StatusBar->showMessage("Crab 3: Find good signatures inside bad: " + this_stem);
            qApp->processEvents();
        } //---------------------------------------------------------------------------------------//
        affixes_of_residual_sig.clear();
        working_suffix_list.clear();
        working_prefix_list.clear();
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
                working_suffix_list.append(pSuffix);
            } else{
                this_word = m_Words->get_reverse_sort_list()->at(wordno);
                this_affix = this_word.left(this_word.length()- stem_length);
                if (this_affix.length() == 0){
                    this_affix = "NULL";
                }
                pPrefix = m_Prefixes->find_or_fail(this_affix);
                if (! pPrefix){
                    continue;
                }
                working_prefix_list.append(pPrefix);
            }
        }
        // go thru signatures, find the highest robustness sig in WorkingSig, and then successively larger sigs, always containing the previous best one found.
        int i;
        best_intersection_count = 0;
        pBestSig = NULL;
        if (get_suffix_flag()){
            for (i=0; i < signatures->get_count(); i++){
                this_sig = signatures->get_at_sorted(i);
                if (this_sig->get_robustness() < 50){
                    break;
                }
                if (Contains( &working_suffix_list, this_sig->get_suffix_list())){
                    pBestSig = this_sig;
                    best_intersection_count = IntersectionCount(&working_suffix_list,pBestSig->get_suffix_list());
                    break;
                }
            // there is no signature that appears inside the working affix list, so we give up on this stem.
            }
        }else{
            for (i=0; i < signatures->get_count(); i++){
                this_sig = signatures->get_at_sorted(i);
                if (Contains( &working_prefix_list, this_sig->get_prefix_list())){
                    pBestSig = this_sig;
                    best_intersection_count = IntersectionCount(&working_prefix_list,pBestSig->get_prefix_list());
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
             if ( pSig->contains(pBestSig) &&
                  Contains(&working_suffix_list, pSig->get_suffix_list()) &&
                  pSig->get_size_of_intersection(&working_suffix_list) > best_intersection_count ){
                    pBestSig = pSig;
                    best_intersection_count = pBestSig->get_size_of_intersection(pBestSig);
             }
        }
        QListIterator<QString> affix_iter_2(pBestSig->get_affix_string_list(best_affix_list));
        while(affix_iter_2.hasNext()){
            this_affix = affix_iter_2.next();
            add_parse(new CParse(this_stem, this_affix, m_SuffixesFlag));

        }
        // this is the right place to identify parasuffixes -- the extensions of *real* stems, not protostems (as is currently done).

    }// end of protostem loop

    step3_from_parses_to_stem_to_sig_maps("Good sigs inside bad");
    step4_create_signatures("Good sigs inside bad");
    replace_parse_pairs_from_current_signature_structure();

}

