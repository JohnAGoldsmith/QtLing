#include "hypothesis.h"
#include "Lexicon.h"

void CLexicon::generate_hypotheses()
{
    sig_tree_edge * p_edge;
    lxa_sig_tree_edge_map_iter edge_iter (m_SigTreeEdgeMap);
    while (edge_iter.hasNext()){
        p_edge = edge_iter.next().value();
        QString this_morph = p_edge->get_morph();
        CSignature* pSig1 = p_edge->get_sig_1();
        CSignature* pSig2 = p_edge->get_sig_2();
        if (this_morph.length() < 2){
            continue;
        }
        if (pSig1->get_stem_entropy() < m_entropy_threshold_for_stems){
            continue;
        }
        double minimal_savings = 0;
        double maximal_savings = 0;

    }
}
