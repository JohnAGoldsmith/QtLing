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

    Qt::GlobalColor color_target_1, color_target_2, color_target_3, color_target_old_1, color_target_old_2, color_target_old_3;
    color_target_1 = Qt::red;
    color_target_old_1 = Qt::yellow;
    color_target_2 = Qt::blue;
    color_target_3 = Qt::green;


    graphic_signature2 * original_graphic_sig, * lower_graphic_sig, * shortened_graphic_sig, * this_graphic_signature;
    bool    shortened_graphic_signature_already_existed (false);

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


        for (auto sig_iter : m_map_from_sig_to_pgraphsig.values()){
            this_graphic_signature = sig_iter;
            sigstring_t this_sig_string = this_graphic_signature->get_signature()->get_key();

            if ( this_sig_string == original_sigstring ){
                original_graphic_sig = this_graphic_signature;
                original_graphic_sig ->set_color(color_target_1);
                move_graphic_signature_to_the_left(this_graphic_signature);
                original_point = original_graphic_sig->get_center() ;
                if (m_target_1_list.count() > 0 ) {
                 //   m_target_1_list.last()->set_color() = color_target_old_1;
                }
                m_target_1_list.append(original_graphic_sig);
            } else if (this_sig_string == lower_sigstring )
            {
                lower_graphic_sig = this_graphic_signature;
                lower_graphic_sig->set_color(color_target_2);
                move_graphic_signature_to_the_left(this_graphic_signature);
                lower_point = lower_graphic_sig->get_center() ;

            } else if (this_sig_string == shortened_sigstring)
            {
                shortened_graphic_sig =  this_graphic_signature;
                if (shortened_graphic_sig){
                    move_graphic_signature_to_the_left(this_graphic_signature);
                    shortened_graphic_sig -> set_color(m_focus_color);
                    shortened_point = shortened_graphic_sig->get_center() ;
                    shortened_graphic_signature_already_existed = true;
                } else{
                    shortened_graphic_signature_already_existed = false;
                }
            } else
            {
                    this_graphic_signature->set_color(m_out_of_focus_color);
            }
        }

        place_arrow(original_point, lower_point);
        if (shortened_graphic_signature_already_existed){
            place_arrow(shortened_point, lower_point);
            place_arrow(original_point, shortened_point);
        }

        re_place_signatures();
   }

   update();




}
