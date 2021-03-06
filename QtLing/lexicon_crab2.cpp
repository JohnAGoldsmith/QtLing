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

void CLexicon::Crab_2()
{

    step6_ReSignaturizeWithKnownAffixes();
    step5a_replace_parse_pairs_from_current_signature_structure();


    m_SuffixesFlag?
                m_Signatures->compute_containment_list():
                m_PrefixSignatures->compute_containment_list();
    m_SuffixesFlag?
                m_Signatures->calculate_sig_robustness():
                m_PrefixSignatures->calculate_sig_robustness();
    //step10_find_compounds();
}


/**
 * @brief CLexicon::Crab_4
 *
 */
void CLexicon::Crab_4()
{
    step5a_replace_parse_pairs_from_current_signature_structure();
    step5b_find_full_signatures();

    collect_parasuffixes();

    step7_FindGoodSignaturesInsideParaSignatures();

    m_SuffixesFlag ?
        m_Signatures->calculate_stem_entropy():
        m_PrefixSignatures->calculate_stem_entropy();

    // Temporarily stop these, June 2021
    //step8a_compute_sig_graph_edges();
    //step8b_compute_sig_graph_edge_map();
    //step9_from_sig_graph_edges_map_to_hypotheses();

    // step10_find_compounds();

    m_SuffixesFlag?
                m_Signatures->calculate_sig_robustness():
                m_PrefixSignatures->calculate_sig_robustness();

    test_json_functionality();

    qDebug() << "finished crab 2.";

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*!
* This is lke AssignSuffixesToStems, but crucially: with Suffixes known ahead of time.
* This creates signatures and stems: signatures that only have approved affixes.
* First function of Crab_4.
* I need to refactorize this function.
*/
void CLexicon::step6_ReSignaturizeWithKnownAffixes()
{
     m_StatusBar->showMessage("6: resignaturize with known affixes");
    m_ProgressBar->reset();
    m_ProgressBar->setMinimum(0);
    m_ProgressBar->setMaximum(m_Parses->size());
    time_stamp("Resignaturize with known affixes");

    CStemCollection*            stems;
    m_SuffixesFlag ?
               stems = m_suffixal_stems:
               stems = m_prefixal_stems;

   //--> We establish a temporary map from stems to sets of affixes as we iterate through parses. <--//
   //--> THIS is where the continuations that are not affixes are eliminated -- well, they are not
   //    eliminated, but they are not copied into ref_stems_to_affix_set. Changing that to a Protosigs.
    m_intermediate_sig_to_stem_map.clear();
    m_intermediate_stem_to_sig_map.clear();


    step6a_create_new_parse_set();
    step3_from_parses_to_stem_to_sig_maps("Resignaturize with known affixes");
    step4_create_signatures("Resignaturize with known affixes");

 }
/**
 * helper function for preceding function.
 *
 */
void CLexicon::step6a_create_new_parse_set()
{

    m_StatusBar->showMessage("6 Resignaturize: temporary map from stem to affix set.");
    qApp->processEvents();
    CParse*                     this_parse;
    int                         count = 0;
    QString                     this_stem_t, this_affix_t;
    CStemCollection *           stems;
    m_SuffixesFlag ?
                stems = m_suffixal_stems:
                stems = m_prefixal_stems;

    m_Raw_parses = m_Parses;
    qDebug() << 128 << " Size of parses" << m_Raw_parses->size();
    m_Parses = new QList<CParse*>;
    m_ParseMap.clear();

    // iterate through parselist, and assign to stem and affix collections;
    //m_Parses->clear();
    //m_intermediate_stem_to_sig_map.clear();
    for (int parseno = 0; parseno < m_Raw_parses->size(); parseno++){
        this_parse = m_Raw_parses->at(parseno);
        qDebug() << 136 << this_parse->display_with_gap();
        count++;
        if (count == 10000){
            count = 0;
            m_ProgressBar->setValue(parseno);
            qApp->processEvents();
        }
        this_stem_t = this_parse->get_stem();
        this_affix_t = this_parse->get_affix();
        if (m_SuffixesFlag){
            if (! m_Suffixes->contains(this_affix_t)){
                   qDebug() << this_stem_t << this_affix_t << 121;
                    continue;
            }
        } else{
            if (! m_Prefixes->contains(this_affix_t)){
                continue;
            }
        }
        /*
        if (! m_intermediate_stem_to_sig_map.contains(this_stem_t)){
            m_intermediate_stem_to_sig_map[this_stem_t] = QSet<affix_t>();
        }
        m_intermediate_stem_to_sig_map[this_stem_t].insert(this_affix_t);
        qDebug() << this_stem_t << this_affix_t << 131;
        */
        add_parse(this_parse);
        qDebug() << this_parse->display()<<170 << m_ParseMap.size();
    }
}

/*!
 *  This function looks at all the non-signatures that were rejected
 *  because they were associated with only one stem. For each one, it
 *  looks for the broadest signature that occurs inside it, and assigns
 *  its stem to that signature.
 *
 * 2nd function of Crab 2.
 */


void   CLexicon::step7_FindGoodSignaturesInsideParaSignatures()
{   stem_t                      this_stem;
    word_t                      this_word;
    affix_t                     this_affix;
    affix_list                  affixes_of_residual_sig;
    CSuffix_ptr_list            this_residual_sig_suffix_pointer_list;
    CSignatureCollection*       signatures;
    suffix_t                    Null_string ("NULL");
    CStemCollection*            stems;
    m_SuffixesFlag ?
                stems = m_suffixal_stems:
                stems = m_prefixal_stems;

    affix_list                  best_affix_list;


    m_StatusBar->showMessage("7: Find good signatures inside bad.");
    qApp->processEvents();
    time_stamp("Find good signatures inside bad");

    //---->   We iterate through the list of Residual Signatures <-------//

    if (m_SuffixesFlag) {
         signatures = m_Signatures;
    } else{
         signatures = m_PrefixSignatures;
    }
    signatures->sort(SIG_BY_AFFIX_COUNT);

    signatures->find_minimal_cover();
    // the signature collection finds a "cover", which is a set of signatures
    // which are a superset of all the other signatures.

    QMap<QString, protostem*> * these_protostems;
    m_SuffixesFlag?
                these_protostems = & m_suffix_protostems:
                these_protostems = & m_prefix_protostems;

    //--------------------------------------------------------------------------------//
    m_ProgressBar->reset();
    m_ProgressBar->setMinimum(0);
    m_ProgressBar->setMaximum(these_protostems->count());
    //--------------------------------------------------------------------------------//

    int protostem_count = 0;
    int temp_i = 0;
    foreach (auto this_protostem, * these_protostems)
    {   affixes_of_residual_sig.clear();
        stem_t this_stem = this_protostem->get_stem();
        int stem_length = this_stem.length();

        if (this_stem == "bat"){
            qDebug() << 196 << "protostem is hop";
        }

        //-----------------------------------------------------------------------------------------//
        m_ProgressBar->setValue(protostem_count++);
        qApp->processEvents();
        temp_i++;
        if (temp_i == 1000){
            temp_i = 0;
            m_StatusBar->showMessage("7: Find good signatures inside bad: " + this_stem);
            qApp->processEvents();
        } //---------------------------------------------------------------------------------------//


        for (int wordno= this_protostem->get_start_word(); wordno <= this_protostem->get_end_word(); wordno++){
            QString this_word, affix;
            if (m_SuffixesFlag){
                this_word = m_Words->get_string_from_sorted_list(wordno);
                affix = this_word.mid( stem_length );
            } else{
                this_word = m_Words->get_reverse_sort_list()->at(wordno);
                affix = this_word.left(this_word.length()- stem_length);
            }
            if (!affix.isEmpty())
                affixes_of_residual_sig.append( affix );
        }
        if (m_Words->contains(this_stem)) {
                affixes_of_residual_sig.append("NULL");
        }

        // 1. Find best subsignature.
        best_affix_list.clear();
        affix_list affix_intersection;
        for (int i = 0; i <signatures->get_minimal_cover()->size(); i++ ){
            CSignature* pSig = signatures->get_minimal_cover()->at(i);
            pSig->intersection_with(affixes_of_residual_sig, affix_intersection);
            if ( affix_intersection.length() > 1 &&
                 affix_intersection.size() > best_affix_list.size()){
                     best_affix_list = affix_intersection;
                    // if (this_stem == "bat"){
                         qDebug() << this_stem   << affixes_of_residual_sig <<  best_affix_list << 235;
                    //´ }
            }
            /*
            if ( affix_intersection.length() > 1) {
              if (this_stem == "bur"){
                qDebug() << "bur" << affix_intersection;
              }
            } */
        }
        if (best_affix_list.size() == 0) {
            continue;
        }

        // 2. add the signature string to the signature collection:
        CSignature * pSig;
        QString best_affix_list_string = best_affix_list.join("=");

        // . This shouldn't happen: there are no affixes.
        QMessageBox msgBox;
        if (best_affix_list.length() == 0){
            //qDebug() << 359 << "affix list with no entries; this should not happen."<<this_stem << best_affix_list;
            msgBox.setText("Affix list with no entries. " + this_stem);
            msgBox.exec();
            continue;
        }

        pSig = *signatures << best_affix_list_string;
        if (!m_SuffixesFlag){ pSig->set_suffix_flag(false);}

        QListIterator<QString> affix_iter_2(best_affix_list);
        while(affix_iter_2.hasNext()){
            this_affix = affix_iter_2.next();
            step4a_link_signature_and_affix(pSig,this_affix);
        }
        step4b_link_signature_and_stem_and_word(this_stem, pSig, "Good sigs inside bad");
   } // end of protostem loop
   signatures->sort_each_signatures_stems_alphabetically();
}


