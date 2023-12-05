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
#include "spine.h"

extern bool contains(QStringList & container, QStringList & contained);
extern QString QStringList2signature(QStringList);




/**
 * @brief CLexicon::Crab_2
 *
 */
void CLexicon::Crab_2()
{    m_StatusBar->showMessage("Crab 2: Find good signatures inside bad.");

    find_good_signatures_inside_bad();
    m_StatusBar->showMessage("Crab 2: Find good signatures inside bad, completed.");
    m_suffix_flag?
        m_Signatures->calculate_sig_robustness():
        m_PrefixSignatures->calculate_sig_robustness();
    m_suffix_flag ?
        m_Signatures->calculate_stem_entropy():
        m_PrefixSignatures->calculate_stem_entropy();

    replace_parse_pairs_from_current_signature_structure();


}

// not used...
void CLexicon::find_all_signature_spines(){  // currently not used
    CSignatureCollection * signatures;
    if (m_suffix_flag){
        signatures = m_Signatures;
    } else{
        signatures = m_PrefixSignatures;
    }
    /*
    foreach (CWord* word, m_Words->get_){
        if (m_SuffixesFlag){
            word->get_suffixal_spine();
        }
    }
    */
}

/*
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
*/
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

void CLexicon::find_parasuffixes(int wordno, int stem_length, QStringList working_affix_string_list){
    CSuffix *pSuffix;
    QString this_word = m_Words->get_string_from_sorted_list(wordno);
    QString this_affix = this_word.mid( stem_length );
    if (this_affix.length() == 0){this_affix = "NULL";}
    pSuffix =  m_Suffixes->find_or_fail(this_affix);
    if (! pSuffix ){
        add_parasuffix(this_affix, this_word);
        return;
    }
    working_affix_string_list.append(this_affix);
}
void CLexicon::find_paraprefixes(int wordno, int stem_length, QStringList working_affix_string_list){
    CSuffix *pSuffix;
    QString this_word = m_Words->get_string_from_sorted_list(wordno);
    QString this_affix = this_word.mid( stem_length );
    if (this_affix.length() == 0){this_affix = "NULL";}
    pSuffix =  m_Suffixes->find_or_fail(this_affix);
    if (! pSuffix ){
        add_parasuffix(this_affix, this_word);
        return;
    }
    working_affix_string_list.append(this_affix);
}
void CLexicon::find_new_affixes(protostem* this_protostem, CSignatureCollection * signatures,
                                CStemCollection* stems, QStringList&  working_affix_string_list) {
    Affix_list affixes_of_residual_sig;
    affixes_of_residual_sig.clear();

    CSuffix* pSuffix;
    CPrefix* pPrefix;
    word_t this_word;
    affix_t this_affix;
    stem_t this_stem = this_protostem->get_stem();
    int stem_length = this_stem.length();
    if (stems->find_or_fail( this_stem )){
        return;
    }

    for (int wordno= this_protostem->get_start_word(); wordno <= this_protostem->get_end_word(); wordno++){
        if (m_suffix_flag){
            find_parasuffixes(wordno, stem_length, working_affix_string_list);
            /*
            QString this_word = m_Words->get_string_from_sorted_list(wordno);
            QString this_affix = this_word.mid( stem_length );
            if (this_affix.length() == 0){this_affix = "NULL";}
            pSuffix =  m_Suffixes->find_or_fail(this_affix);
            if (! pSuffix ){
                add_parasuffix(this_affix, this_word);
                continue;
            }
            working_affix_string_list.append(this_affix);
            if (this_affix.length()==0) qDebug() << 122 << "empty continuation"<< this_stem;
            */
        } else{
            this_word = m_Words->get_end_sorted_list()->at(wordno);
            qDebug() << 180 << this_word;
            this_affix = this_word.left(this_word.length()- stem_length);
            if (this_affix.length() == 0){ this_affix = "NULL"; }
            pPrefix = m_Prefixes->find_or_fail(this_affix);
            if (! pPrefix){
                add_paraprefix(this_affix, this_word);
                continue;
            }
            working_affix_string_list.append(this_affix);
            //qDebug() << 134 << this_word << this_affix;
        }
    }
}


/*!
 *  This function looks at all the non-signatures that were rejected
 *  because they were associated with only one stem. For each one, it
 *  looks for the broadest signature that occurs inside it, and assigns
 *  its stem to that signature.
 */
void   CLexicon::find_good_signatures_inside_bad()  // step 2
{   stem_t                      this_stem;
    word_t                      this_word;
    affix_t                     this_affix;
    Affix_list                  affixes_of_residual_sig;
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
    if (m_suffix_flag) {
         //signatures = m_Signatures;  // TODO remove this;
         signatures = m_VirtualSignatures;
         stems = m_suffixal_stems;
         these_protostems = & m_suffix_protostems;

    } else{
         qDebug() << 133 << m_prefix_protostems.count();
         signatures = m_VirtualSignatures;
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
            if (m_suffix_flag){
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
                this_word = m_Words->get_end_sorted_list()->at(wordno);
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
        int ROBUSTNESS_THRESHOLD = 5;
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
                if (this_sig->get_robustness() < ROBUSTNESS_THRESHOLD){
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
        }else{  //  TODO: Prefix case
            if (working_prefix_string_list.length() < 2) continue;
            working_prefix_string_list.sort();
            for (i=0; i < signatures->get_count(); i++){
                this_sig = signatures->get_at_sorted(i);
                QStringList this_sig_affix_string_list = this_sig->get_affix_string_list();
                if (this_sig->get_robustness() < ROBUSTNESS_THRESHOLD){
                    break;
                }
                if (contains(working_prefix_string_list, this_sig_affix_string_list)){
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
            add_parse(new CParse(this_stem, this_affix, m_suffix_flag));
        }
        // this is the right place to identify parasuffixes -- the extensions of *real* stems, not protostems (as is currently done).
    }// end of protostem loop

    step3_from_parses_to_stem_to_sig_maps("Good sigs inside bad");

    // We have to drop the condition now -- and in the future -- that all signatures have at least 2 stems...
    change_minimum_stem_count(1);

    step4_create_signatures("Good sigs inside bad");
    replace_parse_pairs_from_current_signature_structure();

}

