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

    qDebug() << 26;
    graphic_signature2 * original_sig, * lower_sig, * shortened_sig;
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
        sigstring_t  lower_sigstring        = index.sibling(row,1).data().toString();
        sigstring_t  original_sigstring     = index.sibling(row,2).data().toString();
        sigstring_t  shortened_sigstring    = index.sibling(row,3).data().toString();
        QPointF original_point, lower_point, shortened_point;

        for (auto sig_iter : m_map_from_sig_to_pgraphsig.keys()){
            QString this_sig_string = sig_iter->get_key();
            CSignature* pSig = sig_iter;
            if ( this_sig_string == original_sigstring ){
                original_sig = m_map_from_sig_to_pgraphsig[sig_iter];
                original_sig ->set_color(m_focus_color);
                //move_signature_to_the_left(pSig);
                original_point = original_sig->get_center() ;
                qDebug() << 47 << this_sig_string;
            } else if (this_sig_string == lower_sigstring )
            {
                lower_sig = m_map_from_sig_to_pgraphsig[sig_iter];
                lower_sig->set_color(m_focus_color);
                lower_point = lower_sig->get_center() ;

            } else if (this_sig_string == shortened_sigstring)
            {
                shortened_sig =  m_map_from_sig_to_pgraphsig[sig_iter];
                shortened_point = shortened_sig->get_center() ;
                shortened_sig -> set_color(m_focus_color);
            } else
            {
                    m_map_from_sig_to_pgraphsig[sig_iter]->set_color(m_out_of_focus_color);
            }
        }

        place_arrow(original_point, lower_point);
        place_arrow(original_point, shortened_point);
        place_arrow(shortened_point, lower_point);
   }

   update();




}
