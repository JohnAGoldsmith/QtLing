#include <QDebug>
#include <QToolTip>
#include <QGraphicsScene>
#include <QScrollBar>
#include <QMouseEvent>
#include <QGraphicsSceneEvent>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QRect>
#include <QtMath>
#include "graphics.h"
#include "generaldefinitions.h"
#include "SignatureCollection.h"
#include "mainwindow.h"


/////////////////////////////////////////////////////////////////////////////
//          Graphic signature
//          the base class for specific shapes
/////////////////////////////////////////////////////////////////////////////

graphic_signature2::graphic_signature2  () {
    m_focus_flag = false;
    m_color = Qt::green;
};

graphic_signature2::graphic_signature2   (CSignature* this_signature,   QColor, bool focus_flag )
{
    m_focus_flag =false;
    m_color = Qt::green;
    m_signature = this_signature;
};
graphic_signature2::graphic_signature2   (CSignature* this_signature )
{
    m_signature = this_signature;
    m_color = Qt::green;
    m_focus_flag = false;
};

void graphic_signature2::set_color(Qt::GlobalColor this_color){
    m_color = this_color;
//    update();
};

graphic_signature2::~graphic_signature2(){

}

// not currently used:
graphic_super_signature::graphic_super_signature(int x, int y, CSupersignature* pSig, lxa_graphics_scene * scene)
{
    m_graphics_scene = scene;
    m_super_signature = pSig;
    m_color = Qt::red;
    int row_delta = 40;

    int my_x = 100;
    int my_y = 100;
    int my_width= 20;
    int my_height = 30;
    scene->addRect(my_x,my_y,my_width, my_height ,QPen(),QBrush(m_color));

    QGraphicsTextItem * p_text_item = new QGraphicsTextItem;
    int text_width = p_text_item->textWidth();
    p_text_item->setPos (x - 0.5 * text_width,y + 0.3* row_delta);

    QGraphicsTextItem * q_text_item = new QGraphicsTextItem;
    q_text_item->setPos (x - 0.5 * text_width,y + 0.3* row_delta + 20);

    scene->addItem(p_text_item);
    scene->addItem(q_text_item);
};
/////////////////////////////////////////////////////////////////////////////
//
//          lxa graphics view
//
/////////////////////////////////////////////////////////////////////////////


lxa_graphics_view::lxa_graphics_view(MainWindow* this_window)
{
    m_main_window = this_window;
    m_scale =1;
    m_x_scale =1.0;
    m_y_scale = 1.0;

    setDragMode(QGraphicsView::ScrollHandDrag);
    setMouseTracking(true);

};
//--------------------------------------------------------------------------//
void lxa_graphics_view::mousePressEvent(QMouseEvent* event)
{
    // ADD STUFF HERE.


    QGraphicsView::mousePressEvent(event);
};


/////////////////////////////////////////////////////////////////////////////
//
//          lxa graphics scene
//
/////////////////////////////////////////////////////////////////////////////



//--------------------------------------------------------------------------//
lxa_graphics_scene::lxa_graphics_scene(MainWindow * window, CLexicon * lexicon){
    m_main_window               = window;
    m_lexicon                   = lexicon;
    m_location_of_bottom_row    = 0;
    m_row_delta                 = 225;
    m_column_delta              = 200;
    m_normal_color              = Qt::green;
    m_focus_color               = Qt::red;
    m_out_of_focus_color        = Qt::gray;
    m_focus_signature_1         = NULL;
    m_focus_signature_2         = NULL;
    m_suffix_flag               = true;

    // click on a hypothesis, watch its effects on the signature lattice.
    connect(m_main_window->get_upper_right_tableview(),SIGNAL(clicked(const QModelIndex & )),
            this,SLOT(implement_hypothesis(const QModelIndex &  )));



};

//--------------------------------------------------------------------------//
void lxa_graphics_scene::ingest( CLexicon* lexicon, CSignatureCollection* signatures, bool suffix_flag)
{
    m_lexicon                   = lexicon;
    m_signature_collection      = signatures;
    m_suffix_flag               = suffix_flag;

};
//--------------------------------------------------------------------------//
lxa_graphics_scene::~lxa_graphics_scene ()
{    for (int itemno = 0; itemno < m_signature_lattice.size(); itemno ++ ){
            delete m_signature_lattice[itemno];
    }
    m_signature_lattice.clear();
};
//--------------------------------------------------------------------------//
void lxa_graphics_scene::clear_all()
{  m_signature_lattice.clear();
   m_map_from_sig_to_column_no.clear();
   m_suffixes = NULL;
   m_prefixes = NULL;
   m_signature_lattice.clear();
   m_map_from_sig_to_row_and_column.clear();
   m_map_from_sig_to_pgraphsig.clear();

   QGraphicsScene::clear();


    // There is more that needs to be cleared here. //
}
//--------------------------------------------------------------------------//
void lxa_graphics_scene::clear()
{
    QGraphicsScene::clear();
}
//--------------------------------------------------------------------------//
void lxa_graphics_scene::set_graphics_view(lxa_graphics_view * this_graphics_view)
{
    m_graphics_view  = this_graphics_view;
}
//--------------------------------------------------------------------------//
const bool compare_stem_count_2(const CSignature* pSig1, const CSignature* pSig2){
    return pSig1->get_number_of_stems() > pSig2->get_number_of_stems();
}
//--------------------------------------------------------------------------//
const  bool compare_robustness(const CSignature* pSig1, const CSignature* pSig2)
{
 return  pSig1->get_robustness() > pSig2->get_robustness();
}
//--------------------------------------------------------------------------//
void lxa_graphics_scene::assign_scene_positions_to_signatures(CSignatureCollection* signatures, eDataType this_display_type){

    m_signature_collection          = signatures;
    m_data_type                     = this_display_type;
    CLexicon*   lexicon             = m_main_window->get_lexicon();
    double      entropy_threshold   = lexicon->get_entropy_threshold_for_positive_signatures();
    int         max_size            = 0;
    int         sig_size;
    int         MAXIMUM_NUMBER_OF_CONTAINMENT_EDGES = 2;
    int         MINIMUM_NUMBER_OF_STEMS = 2;
    CSignature * pSig, *qSig;

    //  -->  Find out what the largest number of affixes is in the signatures  <-- //
    map_sigstring_to_sig_ptr_iter sig_iter(*signatures->get_map());
    while(sig_iter.hasNext()){
        pSig = sig_iter.next().value();
        if ( (pSig->get_number_of_stems() < MINIMUM_NUMBER_OF_STEMS) ||
           (  m_data_type == e_data_epositive_suffixal_signatures && pSig->get_stem_entropy() <entropy_threshold )   ){
                continue;
           }
        int this_size = pSig->get_number_of_affixes();
        if (this_size > max_size){ max_size = this_size;}
    }

    //  --> Initialize a list of signatures for each "row" (of equal number of affixes)   <-- //
    for (int size = 0; size <= max_size; size++){
        QList<CSignature*> * signature_list = new QList<CSignature*>;
        m_signature_lattice.append(signature_list);
    }

    //  --> Put each signature in the right row, based on its number of affixes    <-- //
    sig_iter.toFront();
    while(sig_iter.hasNext()){
        pSig = sig_iter.next().value();
        if (pSig->get_number_of_stems() < MINIMUM_NUMBER_OF_STEMS) {continue;}
        if ( (m_data_type == e_data_epositive_prefixal_signatures ||
              m_data_type == e_data_epositive_suffixal_signatures) &&
                pSig->get_stem_entropy() < entropy_threshold){
            continue;
        }
        sig_size = pSig->get_number_of_affixes();
        m_signature_lattice[sig_size]->append(pSig);
    }

    // -->  Sort each row of the m_signature_lattice by stem frequency
    for (int rowno = 0; rowno < m_signature_lattice.size(); rowno ++){
        std::sort(m_signature_lattice[rowno]->begin(), m_signature_lattice[rowno]->end(),  compare_stem_count_2);
        for (int colno = 0; colno < m_signature_lattice[rowno]->size(); colno++){
            pSig = m_signature_lattice[rowno]->at(colno);
            //m_map_from_sig_to_column_no[pSig] = colno;
            m_map_from_sig_to_row_and_column[pSig] = QPair<int,int>(rowno, colno);
        }
     }
    //  --> Now the signatures are nicely organized in a matrix of sorts.   <-- //

    // -->    Containment relations between signatures   <-- //
    QMapIterator<CSignature*,QList<CSignature*>*> sig_iter_2 (*signatures->get_containment_map());
    while(sig_iter_2.hasNext()){
        pSig                     = sig_iter_2.next().key();
        int pSig_row             = pSig->get_number_of_affixes();
        SignatureList * pSigList = sig_iter_2.value();
        if (pSig->get_number_of_stems() < MINIMUM_NUMBER_OF_STEMS) {continue;}
        int count =0;
        for (int signo = 0; signo < pSigList->size(); signo++ ){
            qSig = pSigList->at(signo);
            if (qSig->get_number_of_stems() < MINIMUM_NUMBER_OF_STEMS){continue;}
            int qSig_row = qSig->get_number_of_affixes();
            QPair<CSignature*,CSignature*>* pPair = new QPair<CSignature*,CSignature*> (pSig,qSig);
            add_signature_containment_edge(pPair);
            count++;
            if (count > MAXIMUM_NUMBER_OF_CONTAINMENT_EDGES){
                break;
            }
        }
    }
    update();

 }

int convert_count_to_radius(int count)
{
    if (count<5){
        return 1;
    } else if (count< 105){
        return 5 + (count - 5) * .5;
    } else if (count < 205){
        return  55  + (count -105) *.2;
    }   else {
        return  75  + 10.0 * log(count -205);
    }

}

//--------------------------------------------------------------------------//
void lxa_graphics_scene::place_signatures()
{
    m_signature_radius  = 30;
    int border          = 100;
    int radius;
    int number_of_rows = m_signature_lattice.size();

    m_location_of_bottom_row = m_row_delta * number_of_rows;
    m_maximum_y = m_row_delta * number_of_rows;

    m_bottom_left_x = 0;
    m_bottom_left_y = m_location_of_bottom_row ;

    //  -->    Iterate through the rows of signatures, and calculate a reasonable radius to show how many stems each has. <--//
    for (int row = 2; row < number_of_rows; row++){
        CSignature_ptr_list_iterator sig_iter(*m_signature_lattice[row]);
        int col = 0;
        while (sig_iter.hasNext()){
            CSignature* pSig = sig_iter.next();
            int stem_count = pSig->get_number_of_stems();
            radius = convert_count_to_radius(stem_count);
            int x = border + col * m_column_delta;
            int y = m_location_of_bottom_row - (row-2) * m_row_delta;

            switch (row){
            case 2:{
                bar * this_bar  = new bar (pSig);
                addItem(this_bar);
                this_bar->setPos(x,y);
                m_map_from_sig_to_pgraphsig[pSig]=this_bar;
                break;
            }

            case 3:{
                if (col == 0) { m_focus_signature_1 = pSig;}
                triangle2 *  this_triangle_2  = new triangle2 (pSig);
                addItem(this_triangle_2);
                this_triangle_2->setPos(x,y);
                m_map_from_sig_to_pgraphsig[pSig]=this_triangle_2;
                break;
            }
            case 4:{
                square2 * this_square_2  = new square2 (pSig);
                addItem(this_square_2);
                this_square_2->setPos(x,y);
                m_map_from_sig_to_pgraphsig[pSig]=this_square_2;
                break;
            }
            case 5:{
                pentagon2 * this_pentagon_2  = new pentagon2 (pSig);
                addItem(this_pentagon_2);
                this_pentagon_2->setPos(x,y);
                m_map_from_sig_to_pgraphsig[pSig]=this_pentagon_2;
                break;
            }
            case 6:{
                hexagon2 * this_hexagon_2  = new hexagon2 (pSig);
                addItem(this_hexagon_2);
                this_hexagon_2->setPos(x,y);
                m_map_from_sig_to_pgraphsig[pSig]=this_hexagon_2;
                break;
            }
            case 7:{
                septagon2 * this_septagon_2  = new septagon2 (pSig);
                addItem(this_septagon_2);
                this_septagon_2->setPos(x,y);
                m_map_from_sig_to_pgraphsig[pSig]=this_septagon_2;
                break;
            }
            case 8:{
                octagon2 * this_octagon_2  = new octagon2 (pSig);
                addItem(this_octagon_2);
                this_octagon_2->setPos(x,y);
                m_map_from_sig_to_pgraphsig[pSig]=this_octagon_2;
                break;
            }
            case 9:{
                nonagon2 * this_nonagon_2  = new nonagon2 (pSig);
                addItem(this_nonagon_2);
                this_nonagon_2->setPos(x,y);
                m_map_from_sig_to_pgraphsig[pSig]=this_nonagon_2;
                break;
            }
//            case 10:{

                //                decagon2 * this_decagon_2  = new decagon2 (pSig);
//                addItem(this_decagon_2);
//                m_map_from_sig_to_pgraphsig[pSig]=this_decagon_2;
//                this_decagon_2->setPos(x,y);

//                break;
//           }

            default:{
              //graphic   p_graph_sig = new graphic_signature (x,y, pSig,  m_row_delta, m_normal_color);
              //  m_map_from_sig_to_pgraphsig[pSig] = p_graph_sig;
              //  addItem(p_graph_sig);
              //  break;
            }
        }
            col++;
        }
    }
   // if (m_focus_signature_1){
        //qDebug() << 389 << m_focus_signature_1->get_key();
        //m_map_from_sig_to_pgraphsig[m_focus_signature_1]->set_color(m_focus_color);
        //update();
        // CAUSED  PROBLEMS
 //   }

//-->  the m_top_graphic_signature is the first item on the top row, and will be the first signature chosen for graphics.

//    CSignature* top_sig = m_signature_lattice[number_of_rows-1]->first();
//   delete m_map_from_sig_to_pgraphsig[top_sig];
//   qDebug() << 333 <<  "Setting focus " << p_graph_sig->get_signature()->get_key();



    //m_graphics_view->centerOn(m_bottom_left_x, m_bottom_left_y);
    update();
}
//--------------------------------------------------------------------------//
void lxa_graphics_scene::update_signature_focus()
{
    switch (m_graphics_status){
        case GS_no_focus:{

            }
        case GS_one_focus_signature:{

        }
        case GS_one_focus_signature_all_others_grayed:{

        }
        case GS_one_focus_signature_some_satellites_others_grayed:{

        }
    }
}

eGraphicsStatus lxa_graphics_scene::change_graphics_status(){
    switch (m_graphics_status) {
        case GS_no_focus:{
           m_graphics_status =  GS_one_focus_signature;
            }
        case GS_one_focus_signature:{
            m_graphics_status =GS_one_focus_signature_all_others_grayed;
        }
        case GS_one_focus_signature_all_others_grayed:{
            m_graphics_status =GS_one_focus_signature_some_satellites_others_grayed;
        }
        case GS_one_focus_signature_some_satellites_others_grayed:{
            m_graphics_status =GS_no_focus;
        }
    }
}

//--------------------------------------------------------------------------//
void lxa_graphics_scene::place_containment_edges(){
    QPair<CSignature*, CSignature*> * pPair;
    CSignature* sig1, *sig2;
    for (int i= 0; i < m_signature_containment_edges.size(); i++){
        pPair = m_signature_containment_edges[i];
        sig1 = pPair->first;
        sig2 = pPair->second;
        int row1 = sig1->get_number_of_affixes();
        int row2 = sig2->get_number_of_affixes();
        int col1 = m_map_from_sig_to_column_no[sig1];
        int col2 = m_map_from_sig_to_column_no[sig2];
        int x1 = col1 * m_column_delta + m_signature_radius/2 ;
        int x2 = col2 * m_column_delta + m_signature_radius/2;
        int y1 = m_location_of_bottom_row - (row1-2) * m_row_delta + m_signature_radius/2;
        int y2 = m_location_of_bottom_row - (row2-2) * m_row_delta + m_signature_radius/2;
        addLine(x1,y1,x2,y2,QPen());
    }
}
//--------------------------------------------------------------------------//
void lxa_graphics_scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
   //QToolTip::showText(event->screenPos().toPoint(), "x");
   //QGraphicsScene::mouseMoveEvent();
}
//--------------------------------------------------------------------------//
void lxa_graphics_scene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() == Qt::LeftButton)
    {
        QGraphicsItem * item = this->itemAt(  mouseEvent->scenePos() , QTransform() );
        graphic_signature2 * this_sig = dynamic_cast<graphic_signature2*> (item);
        if (item && this_sig) {
            {
                CSignature* local_sig = this_sig->get_signature();
                if (local_sig){
                    qDebug() << local_sig->get_key();
                    set_focus_signature(local_sig);
                }
            }
        }
    }
}
//--------------------------------------------------------------------------//
void lxa_graphics_scene::widen_columns()
{
    m_column_delta *= 1.25;
    clear();
    place_signatures();
    update();
//    m_graphics_view->centerOn(m_bottom_left_x, m_bottom_left_y);
}
void lxa_graphics_scene::narrow_columns()
{
    m_column_delta *= 0.8;
    clear();
    place_signatures();
//    m_graphics_view->centerOn(m_bottom_left_x, m_bottom_left_y);
}
void  lxa_graphics_scene::move_rows_apart()
{    m_row_delta *= 1.2;
    clear();
    place_signatures();
//     m_graphics_view->centerOn(m_bottom_left_x, m_bottom_left_y);
}

void lxa_graphics_scene::move_rows_closer()
{
    m_row_delta *= 0.8;
    clear();
    place_signatures();

}
void lxa_graphics_view::zoom_up()
{
    m_x_scale *= 0.8;
    m_y_scale *= 0.8;
    scale(0.8,0.8);

    return;
}

void lxa_graphics_view::zoom_down()
{
        m_x_scale *= 1.2;
        m_y_scale *= 1.2;
        scale(1.2,1.2);

}
void lxa_graphics_view::reset_scale()
{
    double new_scale = 1.0 / m_x_scale;
    scale(new_scale, new_scale);

    m_x_scale = 1.0;
    m_y_scale = 1.0;

    m_graphics_scene->set_row_delta (225);
    m_graphics_scene->set_column_delta ( 200 ) ;


}



//--------------------------------------------------------------------------//

//--------------------------------------------------------------------------//
void lxa_graphics_scene::set_focus_signature(CSignature* pSig){

    m_map_from_sig_to_pgraphsig[m_focus_signature_1]->set_color(m_normal_color);
    set_focus_signature_1(pSig);
    m_map_from_sig_to_pgraphsig[pSig]->set_color(m_focus_color);
    show_subsignatures();
    update();

};

void lxa_graphics_scene::show_subsignatures(){

    if (! m_focus_signature_1){
        return;
    }
    for (auto sig_iter : m_map_from_sig_to_pgraphsig.keys()){
        if (m_focus_signature_1->contains(sig_iter)){
            m_map_from_sig_to_pgraphsig[sig_iter]->set_color(m_focus_color);
        } else{
            m_map_from_sig_to_pgraphsig[sig_iter]->set_color(m_out_of_focus_color);
        }
    }
    update();

};
