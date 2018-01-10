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
    qDebug() << "implement hypothesis" << 25;
    if (index.isValid()) {
        row = index.row();
        column = index.column();
    }
    sigstring_t sigstring_1   = index.sibling(row,0).data().toString();
qDebug() << sigstring_1;


}
