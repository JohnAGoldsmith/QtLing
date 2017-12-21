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
        if (pSig1->get_stem_entropy() < m_entropy_threshold_for_stems ||
            pSig2->get_stem_entropy() < m_entropy_threshold_for_stems  ){
            continue;
        }
        double minimal_savings = 0;
        double maximal_savings = 0;
        QString affix;
        QStringList affixes1, newaffixes1;
        pSig1->get_string_list(affixes1);
        for (int affixno = 0; affixno < affixes1.count(); affixno++){
            affix = affixes1[affixno];
            if (affix == "NULL"){
                newaffixes1.append(this_morph);
            } else{
                newaffixes1.append(this_morph + affix);
            }
        }
        // we remove all of the newaffixes from pSig2, and replace them with this_morph, and
        // this_morph points directly to pSig1.
        // Then pSig2 loses all of the stems that created the shared words here.



        word_stem_struct this_word_stems;
        {
             //this_word_stems = new word_stem_struct;

        }

    }
}
