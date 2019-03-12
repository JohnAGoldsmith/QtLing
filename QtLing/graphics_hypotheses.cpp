#include "Lexicon.h"
#include "graphics.h"
#include "hypothesis.h"
#include "mainwindow.h"

// this is not currently being used; should probably be eliminated.
void lxa_graphics_scene::show_hypothesis_1(CHypothesis* hypothesis)
{

    sigstring_t  sig1 = hypothesis->m_signature_1_longer_stem;
    sigstring_t  sig2 = hypothesis->m_signature_2_shorter_stem;

    CSignature* pSig1 = m_lexicon->get_signatures()->get_signature(sig1);
    CSignature* pSig2 = m_lexicon->get_signatures()->get_signature(sig2);

    graphic_signature2 *  p_graph_sig1 = m_map_from_sig_to_pgraphsig[pSig1];
    graphic_signature2 *  p_graph_sig2 = m_map_from_sig_to_pgraphsig[pSig2];

    p_graph_sig1->set_color(Qt::green);
    p_graph_sig2->set_color(Qt::green);
}


void  lxa_graphics_scene::implement_hypothesis(const QModelIndex &  index )
{   int row;
//    int column;

//    Qt::GlobalColor color_target_1,
//                    color_target_2,
//                    color_target_3,
//                    color_target_old_1,
//                    color_target_old_2,
//                    color_target_old_3;
//    color_target_1      = Qt::white;
//    color_target_old_1  = Qt::yellow;
//    color_target_2      = Qt::blue;
//    color_target_3      = Qt::green;


//    graphic_signature2  * original_graphic_sig,
//                        * lower_graphic_sig,
//                        * shortened_graphic_sig,
//                        * this_graphic_signature;
//    bool    shortened_graphic_signature_already_existed (false);


    // this function should fire only if the hypotheses are being shown in the right tables,
    // and the main windows is in graphics mode, showing the lattice of signatures.

    if (m_main_window->get_graphic_display_flag() == true &&
        m_main_window->get_upper_right_tableview()->get_data_type() == e_data_hypotheses)
    {
        if (index.isValid()) {
            row = index.row();
            //column = index.column();
        }
        affix_t     morph                  = index.sibling(row,0).data().toString();
        sigstring_t lower_sigstring        = index.sibling(row,2).data().toString();
        sigstring_t original_sigstring     = index.sibling(row,3).data().toString();
        sigstring_t shortened_sigstring    = index.sibling(row,4).data().toString();
        QPointF     original_point, lower_point, shortened_point;

        qDebug() << 62 << shortened_sigstring << lower_sigstring;
        CSignature*     start_sig = m_lexicon->get_signatures()->get_signature(shortened_sigstring);
        CSignature*     end_sig   = m_lexicon->get_signatures()->get_signature(lower_sigstring);
        lxa_arrow* this_arrow = new lxa_arrow(start_sig,end_sig);
        m_arrows.append(this_arrow);
        re_place_signatures();
   }

   update();




}
