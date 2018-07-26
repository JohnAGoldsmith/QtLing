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

         // Test code for checking valid doomed signatures that are found
        const QString& str_affected_signature = pSig2_shorter_stem->display();
        const QString& str_affected_signature1 = pSig1_longer_stem->display();
        qDebug() << "Affected signatures:" << str_affected_signature << str_affected_signature1
                 << "| Doomed affixes:" << doomed_affixes.join(",")
                 << "| Num of affected stems:" << pSig2_shorter_stem->get_number_of_stems();
        if (affected_signatures.contains(str_affected_signature))
            qDebug() << "  This signature is not unique!";
        else
            affected_signatures.append(str_affected_signature);

        doomed_signature_info_map[original_sig2_affixes_shorter_stem]
                = DoomedSignatureInfo(p_edge, doomed_affixes);
    }


    /* --- 2) Go through list of doomed signatures,
     * call remove_signature, and replace them with new ones. --- */

    DoomedSignatureInfoMap::iterator infomap_iter;
    for (infomap_iter = doomed_signature_info_map.begin();
         infomap_iter != doomed_signature_info_map.end();
         infomap_iter++) {


        DoomedSignatureInfo& info = infomap_iter.value();

        // Get string representation of new signature
        // removed doomed affixes from old signature
        const QString str_old_sig = info.m_edge_ptr->get_sig2_string();
        QStringList affixes = str_old_sig.split('=');
        QStringList::iterator affixes_iter;
        for (affixes_iter = affixes.begin(); affixes_iter != affixes.end(); ) {
            if (info.m_doomed_affixes.contains(*affixes_iter))
                affixes.erase(affixes_iter);
            else
                affixes_iter++;
        }
        const QString new_affix =
                QString("%1[%2]")
                .arg(info.m_edge_ptr->get_morph())
                .arg(info.m_edge_ptr->get_sig1_string().replace('=','~'));
        affixes.append(new_affix);
        std::sort(affixes.begin(), affixes.end());

        // create new signature and add it to the signature collection
        const QString str_new_sig = affixes.join('=');
        CSignature* p_new_sig;
        p_new_sig = *p_signatures << str_new_sig;
        if (!m_SuffixesFlag)
            p_new_sig->set_suffix_flag(false);

        foreach(QString affix, affixes) {
            step4a_link_signature_and_affix(p_new_sig, affix);
        }

        CSignature* p_old_sig = info.m_edge_ptr->get_sig_2();
        foreach(CStem* p_stem, *(p_old_sig->get_stems())) {
            const QString str_stem = p_stem->get_key();
            step4b_link_signature_and_stem_and_word(str_stem, p_new_sig, str_new_sig, "Hypothesis");
        }

        remove_signature(p_old_sig, "Hypothesis");
        info.m_edge_ptr->m_sig_2 = p_new_sig;
        update_pointer_in_edge_map(str_old_sig, p_new_sig);

        info.m_str_revised_sig = str_new_sig;

        /* --- 3) Create CHypothesis object --- */
        eHypothesisType this_hypothesis_type = HT_affix_goes_to_signature;
        // CHypothesis (this_hypothesis_type, info.m_edge_ptr);
        CHypothesis * this_hypothesis = new CHypothesis( this_hypothesis_type, info.m_edge_ptr->get_morph(),
                                                         info.m_edge_ptr->get_sig1_string(),
                                                         str_old_sig,
                                                         p_new_sig->display(),
                                                         info.m_doomed_affixes,
                                                         info.m_edge_ptr->get_number_of_words());
        m_Hypotheses->append(this_hypothesis);
        m_Hypothesis_map->insert (this_hypothesis->express_as_string(),  this_hypothesis);
    }
    p_signatures->sort_each_signatures_stems_alphabetically();
}

/*!
 * \brief Goes through the map of sig_graph_edges and updates the pointer
 * `m_sig_1` or `m_sig_2` to p_new_sig if it is found that `m_sig_string_1`
 * or `m_sig_string_2` is identical to str_old_sig.
 * \param String representation of signature whose pointer needs to be updated
 * \param New pointer of that signature.
 */
void CLexicon::update_pointer_in_edge_map(const QString& str_old_sig, CSignature* p_new_sig)
{
    QMap<QString,sig_graph_edge*>::iterator edge_map_iter;
    for (edge_map_iter = m_SigGraphEdgeMap.begin();
         edge_map_iter != m_SigGraphEdgeMap.end();
         edge_map_iter++) {
        sig_graph_edge* p_edge = edge_map_iter.value();
        if (p_edge->m_sig_string_1 == str_old_sig)
            p_edge->m_sig_1 = p_new_sig;
        if (p_edge->m_sig_string_2 == str_old_sig)
            p_edge->m_sig_2 = p_new_sig;
    }
}

/*!
 * \brief Removes a CSignature object from CSignatureCollection, at the same
 * time removing all pointers to that object; delete any stem or affix if the
 * deleted signature is the only signature associated with it.
 * \param Pointer to signature
 * \param Used for logging into autobiography of stems and words
 *
 * This function is basically a reverse implementation of how a signature
 * is added in step4. Some methods such as `CStem::remove_signature()`,
 * `CStemCollection::remove_stem()`, `CSuffixCollection::remove_suffix()`,
 * `CPrefixCollection::remove_prefix()`, and
 * `CSignatureCollection::remove_signature()` are added to support this
 * function.
 *
 */
void CLexicon::remove_signature(CSignature* p_sig, const QString& name_of_calling_function)
{
    CSignatureCollection* p_sigs = m_SuffixesFlag?
                m_Signatures : m_PrefixSignatures;
    CStemCollection* p_stems = m_SuffixesFlag?
                m_suffixal_stems : m_prefixal_stems;
    const QString str_sig = p_sig->get_key();

    // PART1 -- reverse implementation of 4b
    // iterate through the stem list - remove stem connections to signature,
    // generate words that are connected to this signature
    // - remove parse-triples in these words
    const QStringList str_affixes = str_sig.split('=');

    foreach (CStem* p_stem, *(p_sig->get_stems())) {
        const QString str_stem = p_stem->get_key();

        // iterate through all affixes in the list
        foreach (QString str_affix, str_affixes) {
            if (str_affix == "NULL")
                str_affix = "";
            const QString str_word = m_SuffixesFlag?
                        str_stem + str_affix:
                        str_affix + str_stem;
            CWord* p_word = m_Words->find_or_fail(str_word);
            if (p_word == NULL) {
                qDebug() << "CLexicon::remove_signature() - did not find word" << str_word;
                continue;
            }

            // delete parse_triple object from word if that word has this signature
            QMap<stem_t, Parse_triple*>::iterator ptm_iter;
            QMap<stem_t, Parse_triple*>* p_ptm = p_word->get_parse_triple_map();
            for (ptm_iter = p_ptm->begin(); ptm_iter != p_ptm->end(); ) {
                const QString& str_curr_stem = ptm_iter.key();
                Parse_triple* p_curr_parse_triple = ptm_iter.value();
                if (str_curr_stem == str_stem
                        && ptm_iter.value()->p_sig_string == str_sig) {
                    delete p_curr_parse_triple;
                    p_ptm->erase(ptm_iter);
                    add_to_word_autobiographies(str_word,
                                                QString("[%1]=Removed signature=Stem: %2=%3")
                                                .arg(name_of_calling_function)
                                                .arg(str_stem)
                                                .arg(str_sig));
                } else {
                    ptm_iter++;
                }
            }
        }

        // remove stem's association with signature, delete stem if current
        // signature is the only associated signature with the stem
        int sig_ptrs_left_in_stem =
                p_stem->remove_signature(p_sig);
        add_to_stem_autobiographies(str_stem,
                                    QString("[%1]=Removed signature=%2")
                                    .arg(name_of_calling_function)
                                    .arg(str_sig));
        if (sig_ptrs_left_in_stem == 0) { // reverse of find_or_add
            p_stems->remove_stem(p_stem);
            add_to_stem_autobiographies(str_stem,
                                        QString("[%1]=Removed stem")
                                        .arg(name_of_calling_function));
        }
    }

    // PART2 -- reverse implementation of 4a
    if (m_SuffixesFlag) {
        foreach (CSuffix* p_suffix, *(p_sig->get_suffix_list())) {
            int updated_sig_count = p_suffix->decrement_count();
            if (updated_sig_count == 0)
                m_Suffixes->remove_suffix(p_suffix);
        }
    } else {
        foreach (CPrefix* p_prefix, *(p_sig->get_prefix_list())) {
            int updated_sig_count = p_prefix->decrement_count();
            if (updated_sig_count == 0)
                m_Prefixes->remove_prefix(p_prefix);
        }
    }

    // PART3 -- reverse implementation of "*m_Signature << this_signature_string;
    p_sigs->remove_signature(p_sig);

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
