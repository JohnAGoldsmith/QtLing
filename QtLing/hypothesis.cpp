#include <QDebug>
#include "hypothesis.h"
#include "Lexicon.h"
#include "StemCollection.h"
#include "WordCollection.h"
#include "SuffixCollection.h"

CHypothesis* CLexicon::get_hypothesis(QString hypothesis)
{
    return m_Hypothesis_map->value( hypothesis );
}

/* e.g.
 * transform transforms transformed transformation transformations
 * consult   consults   consulted   consultation   consultations
 * sig1_longer_stem is "NULL=s", with the stems "transformation" and "consultation"
 * sig2_shorter_stem is "NULL=s=ed=ation=ations", with the stems "transform" and "consult"
 * the edge would be:
 * "NULL=s" ---[ation]--- "NULL=s=ed=ation=ations"
 * shared_word_stems:
 * transformation=transformation=transform
 * transformations=transformation=transform
 * consultation=consultation=consult
 * consultations=consultation=consult
 */

/*!
 * \brief 1) Given a map of sig_graph_edges, find which of those are valid;
 * 2) for valid (or "doomed") sig_graph_edges, remove `sig_2_shorter_stem` from
 * the signature collection in the lexicon, replace it with a new one reflecting
 * the hypothesis; 3) create `CHypotheses` objects for valid sig_graph_edges.
 *
 * Modifications to other parts of the program to support hypotheses generation:
 * 1.   Revised `step4b_link_signature_and_stem_and_word` do deal with cases like
 *      "ation[NULL~s]" when linking words and signatures - treat this as two
 *      separate affixes: "ation" and "ations"
 * 2.   Made similar revision in compound discovery.
 */
void CLexicon::step9_from_sig_graph_edges_map_to_hypotheses()
{
    m_StatusBar->showMessage("9: Generating Hypotheses");

    sig_graph_edge * p_edge;
    lxa_sig_graph_edge_map_iter edge_iter (m_SigGraphEdgeMap);
    QString affix_1, doomed_affix;
    QStringList affixes1, affixes2, doomed_affixes;
    // Map from string representation of signature containing doomed affixes
    //     to pointer to a signature graph edge
    //     and to a list of doomed affixes
    DoomedSignatureInfoMap doomed_signature_info_map;
    int MINIMUM_AFFIX_OVERLAP = 10;
    int MINIMUM_NUMBER_OF_WORDS = M_MINIMUM_HYPOTHESIS_WORD_COUNT;
    CSignatureCollection * p_signatures;
    m_SuffixesFlag ?
        p_signatures = m_Signatures:
        p_signatures = m_PrefixSignatures;

    QStringList affected_signatures;

    while (edge_iter.hasNext()){
        p_edge = edge_iter.next().value();
        QString this_morph = p_edge->get_morph();
        sigstring_t original_sig1_affixes_longer_stem = p_edge->m_sig_string_1;
        sigstring_t original_sig2_affixes_shorter_stem = p_edge->m_sig_string_2;

        CSignature* pSig1_longer_stem  = p_signatures->find_or_fail(original_sig1_affixes_longer_stem);
        CSignature* pSig2_shorter_stem = p_signatures->find_or_fail(original_sig2_affixes_shorter_stem);
        if (this_morph.length() < 2){
            continue;
        }

        if (pSig1_longer_stem ->get_stem_entropy() < m_entropy_threshold_for_stems ||
            pSig2_shorter_stem->get_stem_entropy() < m_entropy_threshold_for_stems  ){
            // probably we should kill these rather than keep them around;
            continue;
        }

        affixes1.clear();
        affixes2.clear();
        doomed_affixes.clear();
        pSig1_longer_stem->get_string_list(affixes1);
        pSig2_shorter_stem->get_string_list(affixes2);
        //qDebug() << 44 << original_sig1_affixes_longer_stem << original_sig2_affixes_shorter_stem;
        bool success_flag = true;
        if (p_edge->get_number_of_words() < MINIMUM_NUMBER_OF_WORDS ){continue;}
        //--> doomed_affixes is the set of affixes that sig1 proposes to sig2  for retirement
        int matching_affixes_count = 0;
        for (int affixno = 0; affixno < affixes1.count(); affixno++){
            affix_1 = affixes1[affixno];
            if (affix_1 == "NULL"){
                doomed_affix = this_morph;
            } else{
                if (m_SuffixesFlag){
                    doomed_affix = this_morph + affix_1;
                } else {
                    doomed_affix = affix_1 + this_morph;
                }
            }
            doomed_affixes.append(doomed_affix);
            if (affixes2.contains(doomed_affix)){
                //qDebug() << 58 << doomed_affix << affixes2;
                matching_affixes_count++;
                //qDebug() << "success" << this_morph << doomed_affix << this_morph <<  pSig1_longer_stem->display() << pSig2_shorter_stem->display();
            }else{
                // not all of the first sig is continued in the second sig.
                success_flag = false;
               // qDebug() << 59 << " failure " << this_morph << doomed_affix << pSig1_longer_stem->display() << pSig2_shorter_stem->display() << "matches so far " << matching_affixes_count;
                break;
            }
        }
        // We only accept cases where the all of the "doomed" affixes were found in affixes2.
        if (success_flag == false) continue;
        // from now on the doomed affixes in the list are all valid ones.

         // -- Test code for checking valid doomed signatures that are found
        const sigstring_t& str_affected_signature = pSig2_shorter_stem->display();
        const sigstring_t& str_affected_signature1 = pSig1_longer_stem->display();
        qDebug() << "Affected signatures:" << str_affected_signature << str_affected_signature1
                 << "| Doomed affixes:" << doomed_affixes.join(",")
                 << "| Num of affected stems:" << pSig2_shorter_stem->get_number_of_stems();
        if (affected_signatures.contains(str_affected_signature))
            qDebug() << "  This signature is not unique!";
        else
            affected_signatures.append(str_affected_signature);
        // -- End of test code, added by Hanson, 7.30

        // -- created new data structure to store relevant info of valid hypotheses
        doomed_signature_info_map[original_sig2_affixes_shorter_stem]
                = DoomedSignatureInfo(p_edge, doomed_affixes);
        // -- added by Hanson 7.30

        // --- Beginning of code that needs to be fixed ---
        /*
            // we remove all of the newaffixes from pSig2, and replace them with this_morph, and
            // this_morph poiznts directly to pSig1.
            foreach (QString this_affix, affixes2){
                if (doomed_affixes.contains(this_affix)){
                    m_SuffixesFlag?
                        pSig2_shorter_stem->remove_suffix(this_affix):
                        pSig2_shorter_stem->remove_prefix(this_affix);
                }
            }
            pSig2_shorter_stem->add_affix_string(this_morph); */
            // !! this signature needs to send itself to the Dict in Signatures to be updated.

            // Then pSig2 loses all of the stems that created the shared words here.
            // But that has not yet been implemented.
            /*
            eHypothesisType this_hypothesis_type = HT_affix_goes_to_signature;
            CHypothesis (this_hypothesis_type, p_edge);
            CHypothesis * this_hypothesis = new CHypothesis( this_hypothesis_type, this_morph,
                                                             original_sig1_affixes_longer_stem,
                                                             original_sig2_affixes_shorter_stem,
                                                             pSig2_shorter_stem->display(),
                                                             doomed_affixes,
                                                             p_edge->get_number_of_words());
            m_Hypotheses->append(this_hypothesis);
            m_Hypothesis_map->insert (this_hypothesis->express_as_string(),  this_hypothesis);
            */
        // --- End of code that needs to be fixed -- commented out by Hanson, 7.30
    }
}

/*!
 * \brief Create new parses using the current collection of signatures; also
 * takes the map containing info of signatures to be modified after hypothesis
 * generation.
 * \param ref_doomed_info_map
 *
 * -- added by Hanson 7.30
 */
void CLexicon::step9a_from_doomed_info_map_to_parses(DoomedSignatureInfoMap& ref_doomed_info_map)
{
    // QMap<QString, QPair<sig_graph_edge*, QStringList>> = DoomedInfoMap
    clear_parses();

    QList<CSignature*>* p_old_signature_list;
    m_SuffixesFlag?
            p_old_signature_list = m_Signatures->get_signature_list():
            p_old_signature_list = m_PrefixSignatures->get_signature_list();
    CSignature* pSig;
    QList<affix_t> affixes;

    // Iterate through each signature in the current collection of signatures
    foreach (pSig, *p_old_signature_list){
        const sigstring_t& str_old_signature = pSig->display();
        DoomedSignatureInfoMap::iterator iter
                = ref_doomed_info_map.find(str_old_signature);
        affixes = str_old_signature.split('=');

        // see if the current signature is affected by hypothesis generation
        if (iter != ref_doomed_info_map.end()) {
            DoomedSignatureInfo& ref_info = iter.value();
            sig_graph_edge* p_edge = ref_info.m_edge_ptr;
            const QList<affix_t>& ref_doomed_affixes = ref_info.m_doomed_affixes;
            // remove doomed affixes from this signature
            // e.g. NULL=s=ed=ation=ations --> NULL=s=ed
            QList<affix_t>::iterator aff_iter;
            for (aff_iter = affixes.begin(); aff_iter != affixes.end(); ) {
                const affix_t& curr_affix = *aff_iter;
                if (ref_doomed_affixes.contains(curr_affix))
                    affixes.erase(aff_iter);
                else
                    aff_iter++;
            }
            // add into new affix representation
            // e.g. NULL=s=ed --> NULL=s=ed=ation[NULL~s]
            sigstring_t secondary_sig = p_edge->get_sig1_string();
            secondary_sig.replace('=','~');
            const affix_t new_affix = p_edge->get_morph() + "[" + secondary_sig + "]";
            affixes.append(new_affix);
            std::sort(affixes.begin(), affixes.end());
            ref_info.m_str_revised_sig = affixes.join('=');
        }

        // Creating parse objects given list of stems and affixes.
        QList<CStem*>* stem_list = pSig->get_stems();
        CStem* pStem;
        foreach (pStem, *stem_list){
            const stem_t& this_stem = pStem->display();
            /*
            if (this_stem == "call") {
                qDebug() << "Found affixes for [call]:" << affixes.join(",");
                if (sig_is_doomed)
                    qDebug() << "Generated from a doomed signature";
                else
                    qDebug() << "Not generated from a doomed signature";
            }*/
            foreach (affix_t this_affix, affixes){
                CParse* this_parse;
                m_SuffixesFlag?
                        this_parse = new CParse(this_stem, this_affix, true):
                        this_parse = new CParse(this_affix, this_stem, false);
                m_Parses->append(this_parse);
            }
        }
    }
}

/*!
 * \brief This function looks at the string representation of signatures in the
 * sig_graph_edge_map, and updates the pointers associated with them by
 * searching through the newly generated signature collection.
 * \param ref_doomed_info_map
 *
 * -- added by Hanson 7.30
 */
void CLexicon::step9b_redirect_ptrs_in_sig_graph_edges_map(const DoomedSignatureInfoMap &ref_doomed_info_map)
{
    CSignatureCollection* p_signatures = m_SuffixesFlag?
                m_Signatures : m_PrefixSignatures;
    QMap<QString,sig_graph_edge*>::iterator edge_map_iter;
    for (edge_map_iter = m_SigGraphEdgeMap.begin();
         edge_map_iter != m_SigGraphEdgeMap.end();
         edge_map_iter++) {
        sig_graph_edge* p_edge = edge_map_iter.value();
        const sigstring_t& str_sig_1 = p_edge->get_sig1_string();
        const sigstring_t& str_sig_2 = p_edge->get_sig2_string();
        // QString message("Checking edge: " + p_edge->label());
        CSignature *p_new_sig_1, *p_new_sig_2;
        //bool not_found_flag = false;
        p_new_sig_1 = p_signatures->find_or_fail(str_sig_1);
        if (p_new_sig_1 == NULL) {
            QMap<sigstring_t, DoomedSignatureInfo>::ConstIterator info_map_iter
                    = ref_doomed_info_map.find(str_sig_1);
            if (info_map_iter != ref_doomed_info_map.constEnd())
                p_new_sig_1 = p_signatures->find_or_fail(info_map_iter.value().m_str_revised_sig);
        }
        p_edge->m_sig_1 = p_new_sig_1;

        p_new_sig_2 = p_signatures->find_or_fail(str_sig_2);
        if (p_new_sig_2 == NULL) {
            QMap<sigstring_t, DoomedSignatureInfo>::ConstIterator info_map_iter
                    = ref_doomed_info_map.find(str_sig_2);
            if (info_map_iter != ref_doomed_info_map.constEnd())
                p_new_sig_2 = p_signatures->find_or_fail(info_map_iter.value().m_str_revised_sig);
        }
        p_edge->m_sig_2 = p_new_sig_2;

        if (p_new_sig_1 == NULL)
            qDebug() << "Did not find signature1" << str_sig_1;
        if (p_new_sig_2 == NULL)
            qDebug() << "Did not find signature2" << str_sig_2;
        /*if (not_found_flag)
            qDebug() << message;*/
    }
}


/*!
 * \brief Test function not actually used in code; was intended for checking
 * whether in the previous method (where parses are regenerated and the whole
 * lexicon is reconstructed) signatures discovered in step7 were rejected after
 * calling step3 and step4 as a part of step9.
 */
void CLexicon::check_autobiography_consistency()
{
    QMap<QString, QStringList*>::ConstIterator iter;
    for (iter = m_word_autobiographies.constBegin();
         iter != m_word_autobiographies.constEnd();
         iter++) {
        const QString& str_stem = iter.key();
        QStringList* p_curr_str_list = iter.value();
        bool sig_found_in_crab2 = false;
        QString str_sig_found_in_crab2;
        foreach (QString curr_string, *p_curr_str_list) {
            if (curr_string.contains("[Crab2]")) {
                if (curr_string.contains("Found signature")) {
                    sig_found_in_crab2 = true;
                    str_sig_found_in_crab2 = curr_string.split("=")[3];
                    break;
                }
            }
        }
        if (!sig_found_in_crab2) continue;
        foreach (QString curr_string, *p_curr_str_list) {
            if (curr_string.contains("[Hypotheses]")) {
                if (!curr_string.contains("Found signature")) {
                    QString str_sig_found_in_hypotheses = curr_string.split('=')[3];
                    qDebug() << "For word" << str_stem << ", Sig" << str_sig_found_in_crab2
                             << "rejected in hypotheses:" << str_sig_found_in_hypotheses;
                }
            }
        }
    }
}



CHypothesis::CHypothesis(eHypothesisType HypothesisT,   sig_graph_edge*  p_edge)
{
    m_hypothesis_type  = HypothesisT;
    m_number_of_words_saved = 0;
    m_signature_1_longer_stem       = p_edge->m_sig_string_1;
    m_signature_2_shorter_stem       = p_edge->m_sig_string_2;
    m_morpheme          = p_edge->get_morph();

    affix_t             new_affix, affix;
    QStringList         affixes1 = m_signature_1_longer_stem.split("=");
    QStringList         affixes2 = m_signature_2_shorter_stem.split("=");
    QStringList         new_affixes;
    QStringList         new_sig_2;

    m_new_edge          = new QPair<affix_t, sigstring_t>(new_affix,m_signature_1_longer_stem);

    //--> new_affixes is the set of affixes that sig1 proposes to sig2  for retirement
    for (int affixno = 0; affixno < affixes1.count(); affixno++){
        affix = affixes1[affixno];
        if (affix == "NULL"){
            new_affix = m_morpheme;
        } else{
            new_affix = m_morpheme + affix;
        }
        new_affixes.append(new_affix);
    }
    for (int affixno = 0; affixno < affixes2.count(); affixno++){
        affix = affixes2[affixno];
        if (!new_affixes.contains(affix)){
            new_sig_2.append(affix);
        }
    }
    new_sig_2.append(m_morpheme);
    new_sig_2.sort();


}


CHypothesis::CHypothesis (eHypothesisType HypothesisT, morph_t this_morph,
                          sigstring_t sig1,
                          sigstring_t sig2,
                          sigstring_t new_sig,
                          QStringList new_affixes, int number_of_words_saved)
{
    if (HypothesisT == HT_affix_goes_to_signature){
        m_hypothesis_type   = HypothesisT;
        m_number_of_words_saved = 0;
        m_signature_1_longer_stem       = sig1;
        m_signature_2_shorter_stem       = sig2;
        m_new_signature_2   = new_sig;
        m_morpheme          = this_morph;
        m_new_edge          = new QPair<QString, sigstring_t>(m_morpheme, m_signature_1_longer_stem);
        m_number_of_words_saved = number_of_words_saved;
        //m_number_of_stems   =
    }
}

QStringList CHypothesis::express(){
    QStringList output;
    output <<m_morpheme << "--> "
           << m_signature_1_longer_stem
           << m_signature_2_shorter_stem
           << m_new_signature_2
           << QString::number( get_number_of_words_saved());
    return output;
}

QString CHypothesis::express_as_string(){
    return express().join("@");
}
bool compare_number_of_words(CHypothesis* hyp1, CHypothesis* hyp2){
    return hyp1->get_number_of_words_saved()< hyp2->get_number_of_words_saved();
}
void sort_hypotheses(QStringList hypotheses)
{
 //   std::sort(hypotheses.begin(), hypotheses.end(),compare_number_of_words);
}
int CHypothesis::get_number_of_words_saved()
{
    return m_number_of_words_saved;
}
QString CHypothesis::get_key(){
    return m_morpheme + "@" + m_signature_1_longer_stem;
}
