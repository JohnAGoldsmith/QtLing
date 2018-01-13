#include "Lexicon.h"
#include "graphics.h"
#include "hypothesis.h"
#include "mainwindow.h"

void lxa_graphics_scene::show_hypothesis_1(CHypothesis* hypothesis)
{

    sigstring_t  sig1 = hypothesis->m_signature_1;
    sigstring_t  sig2 = hypothesis->m_signature_2;

   CSignature* pSig1 = m_lexicon->get_signatures()->get_signature(sig1);
   CSignature* pSig2 = m_lexicon->get_signatures()->get_signature(sig2);

   graphic_signature2 *  p_graph_sig1 = m_map_from_sig_to_pgraphsig[pSig1];
   graphic_signature2 *  p_graph_sig2 = m_map_from_sig_to_pgraphsig[pSig2];

    p_graph_sig1->set_color(Qt::green);
    p_graph_sig2->set_color(Qt::green);
}


void  lxa_graphics_scene::implement_hypothesis(const QModelIndex &  index )
{   int row, column;

    // this function should fire only if the hypotheses are being shown in the right tables,
    // and the main windows is in graphics mode, showing the lattice of signatures.

    if (m_main_window->get_graphic_display_flag() == true &&
        m_main_window->get_upper_right_tableview()->get_data_type() == e_data_hypotheses)
    {
        qDebug() << "implement hypothesis" << 25;
        if (index.isValid()) {
            row = index.row();
            column = index.column();
        }
        affix_t morph   = index.sibling(row,0).data().toString();
        sigstring_t  sigstring_2 = index.sibling(row,1).data().toString();
        sigstring_t  sigstring_1 = index.sibling(row,2).data().toString();

        CSignature* pSig1 = m_lexicon->get_signatures()->get_signature(sigstring_1);
        graphic_signature2 *  p_graph_sig = m_map_from_sig_to_pgraphsig[pSig1];

        qDebug() << morph << sigstring_1 << sigstring_2 << index;

        p_graph_sig->set_color(Qt::green);

    }


}
