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
//#include "spine.h"


extern QString QStringList2signature(QStringList);

bool contains(QStringList& list1, QStringList & list2) {
    int i(0), j(0);
    if (list1.length()< list2.length()) {
        return false;}
    while (i < list1.length() && j < list2.length()){
        if (list1[i] == list2[j]){
            i++;
            j++;
            continue;
        } else {
            if (list1[i] < list2[j]){
                i++;
                continue;
            } else{
                return false;
            }
        }
    }
    if (j == list2.length()){
        return true;
    }
    return false;
}


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

   // find_parasuffixes();

    if (m_suffix_flag) {
        m_Signatures->produce_latex ();
    }

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
//----------------------------   para-affixes:  continuations on a stem that aren't integrated into signature ----------------------//
void CLexicon::find_parasuffixes(){
    foreach (CStem* stem, *m_suffixal_stems->get_stem_list()){
        CStem* protostem = m_suffix_protostems->find_or_fail(stem->get_stem());
        for (int n = protostem->get_start_word(); n <= protostem->get_end_word(); n++){
            QString word = m_Words->get_word(n)->get_key();
            if (m_word2suffix_sigs.contains(word)) {continue;}
            //qDebug() << 129 << stem->get_stem() << m_Words->get_word(n)->get_word();
        }

    }
}

void CLexicon::find_parasuffixes(int wordno, int stem_length, QStringList working_affix_string_list){
    CSuffix *pSuffix;
    QString this_word = m_Words->get_string_from_sorted_list(wordno);
    QString this_affix = this_word.mid( stem_length );
    if (this_affix.length() == 0){this_affix = "NULL";}
    pSuffix =  m_Suffixes->find_or_fail(this_affix);
    if (! pSuffix ){
        add_parasuffix(this_affix, this_word);
        qDebug() << 131 << this_affix << this_word;
        return;
    }
    working_affix_string_list.append(this_affix);
}

// not checked yet or verified:
void CLexicon::find_paraprefixes(int wordno, int stem_length, QStringList working_affix_string_list){
    CPrefix *pPrefix;
    QString this_word = m_Words->get_string_from_sorted_list(wordno);
    QString this_affix = this_word.mid( stem_length );
    if (this_affix.length() == 0){this_affix = "NULL";}
    pPrefix =  m_Prefixes->find_or_fail(this_affix);
    if (! pPrefix ){
        add_paraprefix(this_affix, this_word);
        return;
    }
    working_affix_string_list.append(this_affix);
}
void  CLexicon::add_parasuffix(QString parasuffix, QString word){
    m_ParaSuffixes->find_or_add(parasuffix);
/*
    if (! m_ParaSuffixes->contains(parasuffix)){
        m_ParaSuffixes->insert(parasuffix, new QStringList());
    }
    m_ParaSuffixes->value(parasuffix)->append(word);
*/
}
void  CLexicon::add_paraprefix(QString paraprefix, QString word){
    if (! m_ParaPrefixes->contains(paraprefix)){
        m_ParaPrefixes->insert(paraprefix, new QStringList());
    }
    m_ParaPrefixes->value(paraprefix)->append(word);
}

//not currently used:
void CLexicon::find_new_affixes(protostem* this_protostem, CSignatureCollection * signatures,
                                CStemCollection* stems, QStringList&  working_affix_string_list) {
    Q_UNUSED(signatures);
    Affix_list affixes_of_residual_sig;
    affixes_of_residual_sig.clear();

    //CSuffix* pSuffix;
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



QString CLexicon::process_a_word_into_stem_and_affix(int wordno, int stem_length, bool suffix_flag){
    QString word, affix;
    if (suffix_flag){
        CSuffix * pSuffix;
        word = m_Words->get_string_from_sorted_list(wordno);
        affix = word.mid( stem_length );
        if (affix.length() == 0){affix = "NULL";}
        pSuffix =  m_Suffixes->find_or_fail(affix);
        if (! pSuffix ){
            //add_parasuffix(affix, word);
            return QString();
        } 
        return affix;
    } else{
        CPrefix* pPrefix;
        word = m_Words->get_end_sorted_list()->at(wordno);
        affix = word.left(word.length() - stem_length);
        if (affix.length() == 0){ affix = "NULL"; }
        pPrefix = m_Prefixes->find_or_fail(affix);
        if (! pPrefix){
            //add_paraprefix(affix, word);
            return QString();
        }
        return affix;
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
    if (! test_if_analysis_has_been_done()){
        return;
    }
    replace_parse_pairs_from_current_signature_structure();
    generate_virtual_signatures();
    CStemCollection * these_protostems;
    if (m_suffix_flag) {
         signatures = m_VirtualSignatures;
         stems = m_suffixal_stems;
         these_protostems =  m_suffix_protostems;
    } else{
         signatures = m_VirtualSignatures;
         stems = m_prefixal_stems;
         these_protostems =  m_prefix_protostems;
    }
    signatures->sort(SIG_BY_REVERSE_ROBUSTNESS);
    initialize_progress(these_protostems->get_count());
    int protostem_count = 0;
    foreach (CStem* this_protostem, * these_protostems->get_stem_list())
    {   //-----------------------------------------------------------------------------------------//
        mark_progress(protostem_count++);
        if (protostem_count % 1000 == 0){
            m_StatusBar->showMessage("Crab 2: Find good signatures inside bad: " + this_stem);
        } //---------------------------------------------------------------------------------------//
        affixes_of_residual_sig.   clear();
        working_suffix_string_list.clear();
        working_prefix_string_list.clear();
        stem_t this_stem = this_protostem->get_stem();
        if (this_stem == "ing"){
            qDebug() << 288 << "ing is a stem??";
        }
        int stem_length = this_stem.length();
        if (stems->find_or_fail( this_stem )){
            continue;
        }
        for (int wordno= this_protostem->get_start_word(); wordno <= this_protostem->get_end_word(); wordno++){
            if (m_suffix_flag){
                this_affix = process_a_word_into_stem_and_affix(wordno, stem_length, m_suffix_flag);
                if (this_affix.length() == 0) { continue; }
                working_suffix_string_list.append(this_affix);
            } else{
                this_affix = process_a_word_into_stem_and_affix(wordno, stem_length, m_suffix_flag);
                if (this_affix.length() == 0) { continue; }
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
                    //pBestSig = pSig;
                    best_intersection_count = intersection_count(sig_affix_list, working_suffix_string_list);
                    best_sig_affix_list = sig_affix_list;
             }
        }
        foreach (QString this_affix, best_sig_affix_list){
            add_parse(new CParse(this_stem, this_affix, m_suffix_flag));
        }
        foreach (QString affix, working_suffix_string_list){
            if (best_sig_affix_list.contains(affix)){
                continue;
            }
            m_ParaSuffixes->find_or_add(affix);
        }
        // this is the right place to identify parasuffixes -- the extensions of *real* stems, not protostems (as is currently done).
    }// end of protostem loop
    
    step3_from_parses_to_stem2sig_maps("Good sigs inside bad");

    // We have to drop the condition now -- and in the future -- that all signatures have at least 2 stems...
    change_minimum_stem_count(1);

    step4_create_signatures("Good sigs inside bad");
    replace_parse_pairs_from_current_signature_structure();

}

void CLexicon::generate_virtual_signatures(){
    QStringList signature_check_list;
    if (m_suffix_flag){
        foreach (CSignature* pSig, *m_Signatures->get_signature_list()){
            signature_check_list.clear();
            m_VirtualSignatures->add_this_and_all_subsignatures(pSig->display(), pSig->get_robustness(), signature_check_list);
        }
    } else{
        foreach (CSignature* pSig, *m_PrefixSignatures->get_signature_list()){
            signature_check_list.clear();
            m_VirtualSignatures->add_this_and_all_subsignatures(pSig->display(), pSig->get_robustness(),signature_check_list);
        }
    }
}
