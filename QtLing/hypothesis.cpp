#include <QDebug>
#include "hypothesis.h"
#include "Lexicon.h"

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
void CLexicon::step9_generate_hypotheses()
{
    sig_graph_edge * p_edge;
    lxa_sig_graph_edge_map_iter edge_iter (m_SigGraphEdgeMap);
    QString affix_1, doomed_affix;
    QStringList affixes1, affixes2, doomed_affixes;
    // Map from string representation of doomed affix (e.g. ation)
    //     to   sig_graph_edge object
    QMap<QString, sig_graph_edge*> doomed_affix_map;
    int MINIMUM_AFFIX_OVERLAP = 10;
    int MINIMUM_NUMBER_OF_WORDS = M_MINIMUM_HYPOTHESIS_WORD_COUNT;
    CSignatureCollection * signatures;
    m_SuffixesFlag ?
        signatures = m_Signatures:
        signatures = m_PrefixSignatures;

    while (edge_iter.hasNext()){
        p_edge = edge_iter.next().value();
        QString this_morph = p_edge->get_morph();
        sigstring_t original_sig1_affixes_longer_stem = p_edge->m_sig_string_1;
        sigstring_t original_sig2_affixes_shorter_stem = p_edge->m_sig_string_2;

        CSignature* pSig1_longer_stem  = signatures->find_or_fail(original_sig1_affixes_longer_stem);
        CSignature* pSig2_shorter_stem = signatures->find_or_fail(original_sig2_affixes_shorter_stem);
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
        new_pSig2.clear();
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
           // qDebug() << 72 << this_morph << original_sig1_affixes_longer_stem;

        // from now on the doomed affixes in the list are all valid ones.
        /*
        foreach (doomed_affix, doomed_affixes) {
            if (!doomed_affix_map.contains(doomed_affix)) {
                doomed_affix_map.insert
            }
        }
        */
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
            pSig2_shorter_stem->add_affix_string(this_morph);
            // !! this signature needs to send itself to the Dict in Signatures to be updated.

            // Then pSig2 loses all of the stems that created the shared words here.
            // But that has not yet been implemented.
        */
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

        // --- End of code that needs to be fixed --- //

    }
}

void CLexicon::step9a_update_parse_pairs_for_hypotheses()
{
    clear_parses();

    QList<CSignature*>* p_old_signature_list;
    m_SuffixesFlag?
            p_old_signature_list = m_Signatures->get_signature_list():
            p_old_signature_list = m_PrefixSignatures->get_signature_list();
    CSignature* pSig;
    foreach (pSig, *p_old_signature_list){
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
