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
#include "cparse.h"
#include "compound.h"

extern bool contains(QList<QString> * list2, QList<QString> * list1);


/**
 * @brief CLexicon::Crab_2
 * Crab_2 is a bit slower than Crab_1.
 */
void CLexicon::Crab_2()
{
    step6_ReSignaturizeWithKnownAffixes();
    step7_FindGoodSignaturesInsideParaSignatures();

    find_compounds();

     m_SuffixesFlag ?
        m_Signatures->calculate_stem_entropy():
        m_PrefixSignatures->calculate_stem_entropy();

    compute_sig_graph_edges();
    compute_sig_graph_edge_map();
    generate_hypotheses();


    qDebug() << "finished crab 2.";

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*!
* This is lke AssignSuffixesToStems, but crucially: with Suffixes known ahead of time.
* This creates signatures and stems: signatures that only have approved affixes.
* First function of Crab_2.
* I need to refactorize this function.
*/
void CLexicon::step6_ReSignaturizeWithKnownAffixes()

{
    const int MINIMUM_NUMBER_OF_STEMS = 2;
    CSignature*                 pSig;
    QString                     this_stem_t, this_suffix_t, this_prefix, this_affix, this_signature_string, this_word;
    stem_list *                 p_this_stem_list;
    affix_set *                 this_ptr_to_affix_set;
    CStem*                      pStem;
    CWord *                     pWord;
    CStemCollection*            stems;
    m_SuffixesFlag ?
               stems = m_suffixal_stems:
               stems = m_prefixal_stems;
   Stem_to_sig_map                    these_stem_to_sig_maps;
   map_sigstring_to_stem_list    temp_signatures_to_stems;

   m_StatusBar->showMessage("6: resignaturize with known affixes");
   m_ProgressBar->reset();
   m_ProgressBar->setMinimum(0);
   m_ProgressBar->setMaximum(m_Parses->size());
   time_stamp("Resignaturize with known affixes");

    // clear signatures and parse_triple_map stored in each word
    map_string_to_word_ptr_iter word_iter (*m_Words->get_map());
    while(word_iter.hasNext()){
       pWord = word_iter.next().value();
       pWord->clear_signatures();
       pWord->clear_parse_triple_map();
   }
   //--> We establish a temporary map from stems to sets of affixes as we iterate through parses. <--//
   //--> THIS is where the continuations that are not affixes are eliminated -- well, they are not
   //    eliminated, but they are not copied into ref_stems_to_affix_set. Changing that to a Protosigs.

    step6a_create_temporary_map_from_stems_to_affix_sets( these_stem_to_sig_maps );                   // ref_stems_to_affix_set);


   step7_from_stem_to_sig_maps_to_xxx("re-signaturize to good affixes only", these_stem_to_sig_maps );      //ref_stems_to_affix_set);

}
/**
 * helper function for preceeding function.
 *
 */
void CLexicon::step6a_create_temporary_map_from_stems_to_affix_sets(Stem_to_sig_map  & these_stem_to_sig_maps//,
                                                             //map_sigstring_to_stem_list & ref_temp_signatures_to_stems
                                                             )
{
    m_StatusBar->showMessage("6 Resignaturize: temporary map from stems to affix sets.");
    qApp->processEvents();
    CParse*                     this_parse;
   // QPair<QString,QString>      this_pair;
    int                         count = 0;
    QString                     this_stem_t, this_affix_t;
    morph_set *                 pSet;
    CStemCollection *           stems;
    m_SuffixesFlag ?
                stems = m_suffixal_stems:
                stems = m_prefixal_stems;

    // iterate through parselist, and assign to stem and affix collections;
    for (int parseno = 0; parseno < m_Parses->size(); parseno++){
        // iterate through each parse stored in CLexicon::m_Parses
        this_parse = m_Parses->at(parseno);
        count++;
        if (count = 10000){
            count = 0;
            m_ProgressBar->setValue(parseno);
            qApp->processEvents();
        }

        if (m_SuffixesFlag){
            this_stem_t = this_parse->get_string1();
            this_affix_t = this_parse->get_string2();
            if (! m_Suffixes->contains(this_affix_t)){
                continue;
            }
        } else{
            this_stem_t = this_parse->get_string2();
            this_affix_t = this_parse->get_string1();
            if (! m_Prefixes->contains(this_affix_t)){
                continue;
            }
        }

        if (! these_stem_to_sig_maps.contains(this_stem_t)){
            if (m_SuffixesFlag){
                pSet = new suffix_set();
            } else{
                pSet = new prefix_set();
            }
            these_stem_to_sig_maps.insert(this_stem_t,pSet);
        }
        these_stem_to_sig_maps.value(this_stem_t)->insert(this_affix_t);
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
    //sig_string                  this_signature_string;
    CStem*                      pStem;
    CWord*                      pWord;
    //CSignature*                 p_proven_sig;
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

    m_ProgressBar->reset();
    m_ProgressBar->setMinimum(0);
    m_ProgressBar->setMaximum(these_protostems->count());

    int protostem_count = 0;
    int temp_i = 0;
    foreach (auto this_protostem, * these_protostems)
    {
        m_ProgressBar->setValue(protostem_count++);
        qApp->processEvents();

        affixes_of_residual_sig.clear();
        stem_t this_stem = this_protostem->get_stem();
        int stem_length = this_stem.length();

        temp_i++;
        if (temp_i == 1000){
            temp_i = 0;
            m_StatusBar->showMessage("7: Find good signatures inside bad: " + this_stem);
            qApp->processEvents();
        }

        for (int wordno= this_protostem->get_start_word(); wordno <= this_protostem->get_end_word(); wordno++){ //corrected error here july 2018, hanson.
            QString this_word, affix;
            if (m_SuffixesFlag){
                this_word = m_Words->get_word_string(wordno);
                affix = this_word.mid( stem_length );
            } else{
                this_word = m_Words->get_reverse_sort_list()->at(wordno);
                affix = this_word.left(this_word.length()- stem_length);
            }
            affixes_of_residual_sig.append( affix ); // what if affix is NULL?
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
            }
        }
        if (best_affix_list.size() == 0) {
            continue;
        }

        // 2. add the signature string to the signature collection:
        CSignature * pSig;
        QString best_affix_list_string = best_affix_list.join("=");

        pSig = *signatures << best_affix_list_string;
        if (!m_SuffixesFlag){ pSig->set_suffix_flag(false);}

        QListIterator<QString> affix_iter_2(best_affix_list);
        while(affix_iter_2.hasNext()){
            this_affix = affix_iter_2.next();
            step4a_link_signature_and_affix(pSig,this_affix);
        }
        step4b_link_signature_and_stem_and_word(this_stem, pSig, best_affix_list_string, "Crab2");

        // . This shouldn't happen: there are no affixes.
        if (best_affix_list.length() == 0){
            qDebug() << 359 << "affix list with no entries; this should not happen."<<this_stem << best_affix_list;
            continue;
        }
        // 6. Make sure each affix has a c_affix.

        foreach (this_affix,best_affix_list){
              if (m_SuffixesFlag){
                  CSuffix* pSuffix = m_Suffixes->find_or_add(this_affix);
                  pSuffix->increment_count();
                  pSig->add_affix_ptr(pSuffix);
              } else {
                  CPrefix* pPrefix = m_Prefixes->find_or_add(this_affix);
                  pPrefix->increment_count();
                  pSig  ->add_affix_ptr(pPrefix);
              }
        } // end of best-affix loop
   } // end of protostem loop
   signatures->sort_each_signatures_stems_alphabetically();
}


void CLexicon::find_compounds()
{
    const int MIN_STEM_LENGTH = 2;
    CStemCollection* p_stems = m_SuffixesFlag ? m_suffixal_stems : m_prefixal_stems;
    const QMap<QString, protostem*>& ref_protostem_map
            = m_SuffixesFlag ? m_suffix_protostems : m_prefix_protostems;

    m_ProgressBar->reset();
    m_ProgressBar->setMinimum(0);
    m_ProgressBar->setMaximum(p_stems->get_count());
    m_StatusBar->showMessage("5: Finding Compounds - part 1.");
    int itercount = 0;

    // iterate through stems
    qDebug() << "Finding compounds, iterating thru stems";
    QMap<QString, CStem*>* p_stem_map = p_stems->get_map();
    QMap<QString, CStem*>::ConstIterator stem_map_iter;
    for (stem_map_iter = p_stem_map->constBegin();
         stem_map_iter != p_stem_map->constEnd();
         stem_map_iter++) {
        m_ProgressBar->setValue(itercount++);
        // for each stem, find its corresponding protostem
        const QString& str_stem = stem_map_iter.key();

        CStem* p_stem = stem_map_iter.value();
        protostem* p_protostem = ref_protostem_map[str_stem];
        int stem_length = str_stem.length();

        if (stem_length < MIN_STEM_LENGTH) continue;

        // iterate through all words in the protostem
        int wordno;
        int end_word = p_protostem->get_end_word();
        for (wordno = p_protostem->get_start_word();
             wordno <= end_word;
             wordno++) {
            // find the continuation of a word, i.e. the part of the word
            // left over after stem part is removed
            const QString& str_word = m_Words->get_word_string(wordno);
            const QString str_continuation = m_SuffixesFlag ?
                        str_word.mid(stem_length) :
                        str_word.left(str_word.length()-stem_length);
            bool continuation_valid = true;
            // determine if the continuation is already in stem's signature,
            // skip if it is.
            CSignature* p_signature;
            foreach (p_signature, *(p_stem->GetSignatures())) {
                if (m_SuffixesFlag) {
                    QList<CSuffix*>* p_suffixes = p_signature->get_suffix_list();
                    CSuffix* p_suffix;
                    foreach (p_suffix, *p_suffixes) {
                        if (p_suffix->get_key() == str_continuation) {
                            continuation_valid = false;
                            break;
                        }
                    }
                } else {
                    QList<CPrefix*>* p_prefixes = p_signature->get_prefix_list();
                    CPrefix* p_prefix;
                    foreach (p_prefix, *p_prefixes) {
                        if (p_prefix->get_key() == str_continuation) {
                            continuation_valid = false;
                            break;
                        }
                    }
                }
            }
            if (!continuation_valid) continue;

            // determine if the continuation is a word or a stem,
            // skip if it is not.
            if (m_Words->get_word(str_continuation) == NULL) {
                continuation_valid = (p_stems->find_or_fail(str_continuation) != NULL);
            } else {
                continuation_valid = true;
            }
            if (!continuation_valid) continue;

            m_Compounds->add_compound_word(str_word, str_stem, str_continuation);
        }
    }
    // finished part 1

    // starting part 2

    QList<CompoundComponent*> list_to_remove;
    CompoundComponentCollection* p_components = m_Compounds->get_components();
    QMap<QString, CompoundComponent*>& ref_components_map = p_components->get_map();

    m_ProgressBar->reset();
    m_ProgressBar->setMinimum(0);
    m_ProgressBar->setMaximum(ref_components_map.size());
    m_StatusBar->showMessage("9: Finding Compounds - part 2: "
                             "removing invalid components.");
    itercount = 0;
    QMap<QString, CompoundComponent*>::iterator components_iter;
    for (components_iter = ref_components_map.begin();
         components_iter != ref_components_map.end();
         components_iter++) {
        m_ProgressBar->setValue(itercount++);
        CompoundComponent* p_component = components_iter.value();
        if (!p_component->check_valid()) {
            list_to_remove.append(p_component);
        }
    }
    CompoundComponent* p_component_to_remove;
    foreach (p_component_to_remove, list_to_remove) {
        p_components->remove_component(p_component_to_remove);
    }

}
