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





graphic_signature::graphic_signature(int x, int y, CSignature* pSig, lxa_graphics_scene * scene,
                                   QColor this_color, bool focus_flag)
{   int radius =100;
    m_graphics_scene = scene;
    m_signature = pSig;

    m_focus_flag = false;
    m_color = scene->m_normal_color;

    int push_figure_to_right = 50;
    int xprime = x+ push_figure_to_right;

    switch(pSig->get_number_of_affixes()){
    case 10:{
        decagon(pSig,xprime,y, scene,  m_color);
        break;}
    case 11:{
        elevenagon(pSig,xprime,y, scene,  m_color);
        break;}
    case 12:{
        twelvagon(pSig,xprime,y, scene,  m_color);
        break;}

    default:{
        scene->addEllipse(x,y,radius ,radius,QPen(),QBrush(m_color));
         break;
        }
    }
    QGraphicsTextItem * p_text_item = new QGraphicsTextItem;
    int text_width;
    if (pSig->get_number_of_affixes() > 6 || pSig->get_number_of_affixes() == 2) {
        p_text_item->setPlainText(pSig->get_key());
        text_width = p_text_item->textWidth();
        p_text_item->setPos (x - 0.5 * text_width,y + 65);
    }
    QGraphicsTextItem * q_text_item = new QGraphicsTextItem;
    if  (pSig->get_number_of_affixes() > 6 || pSig->get_number_of_affixes() == 2) {
        q_text_item->setPlainText(QString::number(pSig->get_number_of_stems()));
        text_width = q_text_item->textWidth();
        q_text_item->setPos (x - 0.5 * text_width,y + 85);
    }

    if  (focus_flag ) {
        QGraphicsTextItem * r_text_item = new QGraphicsTextItem;
        r_text_item->setPlainText(QString("In focus"));
        text_width = r_text_item->textWidth();
        r_text_item->setPos (x - 0.5 * text_width,y + 85);
    }

    scene->addItem(p_text_item);
    scene->addItem(q_text_item);

};

void graphic_signature::mousePressEvent(QGraphicsSceneMouseEvent * event){
    if (event->button()== Qt::LeftButton){
        qDebug() << "change color";
        setSelected(true);
    }
}

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
lxa_graphics_scene::lxa_graphics_scene(MainWindow * window){
    m_main_window               = window;
    m_location_of_bottom_row    = 0;
    m_row_delta                 = 225;
    m_column_delta              = 200;
    m_normal_color              = Qt::red;
    m_focus_color               = Qt::blue;

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
    QGraphicsScene::clear();
}
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
const  bool compare_robustness(const CSignature* pSig1, const CSignature* pSig2)
{
 return  pSig1->get_robustness() > pSig2->get_robustness();
}
void lxa_graphics_scene::assign_scene_positions_to_signatures(CSignatureCollection* signatures, eDisplayType this_display_type){

    m_signature_collection      = signatures;
    m_display_type              = this_display_type;
    CLexicon*   lexicon          = m_main_window->get_lexicon();
    double      entropy_threshold= lexicon->get_entropy_threshold_for_positive_signatures();
    int         max_size                = 0;
    int         sig_size;
    int         MAXIMUM_NUMBER_OF_CONTAINMENT_EDGES = 2;
    int         MINIMUM_NUMBER_OF_STEMS = 2;
    CSignature * pSig, *qSig;

//  -->  Find out what the largest number of affixes is in the signatures  <-- //
    map_sigstring_to_sig_ptr_iter sig_iter(*signatures->get_map());
    while(sig_iter.hasNext()){
        pSig = sig_iter.next().value();
        if ( (pSig->get_number_of_stems() < MINIMUM_NUMBER_OF_STEMS) ||
           (  m_display_type == DT_Positive_Suffix_Signatures && pSig->get_stem_entropy() <entropy_threshold )   ){
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
        if ( (m_display_type == DT_Positive_Prefix_Signatures ||
              m_display_type == DT_Positive_Suffix_Signatures) &&
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
//--------------------------------------------------------------------------//
void lxa_graphics_scene::place_signatures()
{
    m_signature_radius  = 30;
    int border          = 100;
    int radius;
    int number_of_rows = m_signature_lattice.size();
    graphic_signature * p_graph_sig;
    triangle2 * this_triangle_2;
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
            if (stem_count<5){
                radius = 1;
            } else if (stem_count< 105){
                radius = 5 + (stem_count - 5) * .5;
            } else if (stem_count < 205){
                radius = 55  + (stem_count -105) *.2;
            }   else {
                radius = 75  + 10.0 * log(stem_count -205);
            }
            int x = border + col * m_column_delta;
            int y = m_location_of_bottom_row - (row-2) * m_row_delta;



            switch (row){
            case 3:{
                //this_triangle_2->mark_as_focus();
                this_triangle_2  = new triangle2 (pSig->get_key());

                addItem(this_triangle_2);
                this_triangle_2->setPos(x,y);
                break;
            }
            case 4:{
                square2 * this_square_2  = new square2 (pSig->get_key());
                addItem(this_square_2);
                this_square_2->setPos(x,y);

                break;
            }
            case 5:{
                pentagon2 * this_pentagon_2  = new pentagon2 (pSig->get_key());
                addItem(this_pentagon_2);
                this_pentagon_2->setPos(x,y);

                break;
            }
            case 6:{
                hexagon2 * this_hexagon_2  = new hexagon2 (pSig->get_key());
                addItem(this_hexagon_2);
                this_hexagon_2->setPos(x,y);

                break;
            }
            case 7:{
                septagon2 * this_septagon_2  = new septagon2 (pSig->get_key());
                addItem(this_septagon_2);
                this_septagon_2->setPos(x,y);

                break;
            }
            case 8:{
                octagon2 * this_octagon_2  = new octagon2 (pSig->get_key());
                addItem(this_octagon_2);
                this_octagon_2->setPos(x,y);

                break;
            }
           // case 9:{
                //nonagon2 * this_nonagon_2  = new nonagon2 (pSig->get_key());
                //addItem(this_nonagon_2);
                //this_nonagon_2->setPos(x,y);

            //    break;
           // }
/*            case 9:{
                nonagon2 * this_nonagon_2  = new nonagon2 (pSig->get_key());
                addItem(this_nonagon_2);
                this_nonagon_2->setPos(x,y);

                break;
            }
            */
            default:{
                p_graph_sig = new graphic_signature (x,y, pSig, this,   m_row_delta, m_normal_color);
                m_map_from_sig_to_pgraphsig[pSig] = p_graph_sig;
                addItem(p_graph_sig);
                break;
            }
            }




            col++;
        }
    }
   //-->  the m_top_graphic_signature is the first item on the top row, and will be the first signature chosen for graphics.
   CSignature* top_sig = m_signature_lattice[number_of_rows-1]->first();
   delete m_map_from_sig_to_pgraphsig[top_sig];
   // m_map_from_sig_to_pgraphsig = new p_graph_sig( );
   qDebug() << 333 <<  "Setting focus " << p_graph_sig->get_signature()->get_key();



    m_graphics_view->centerOn(m_bottom_left_x, m_bottom_left_y);
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
//   QToolTip::showText(event->screenPos().toPoint(), "x");
//   QGraphicsScene::mouseMoveEvent();
//    qDebug() << "mouse over";
}
//--------------------------------------------------------------------------//
void lxa_graphics_scene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() == Qt::LeftButton)
    {
        qDebug() << "mouse move"<<178;
    }
}
//--------------------------------------------------------------------------//
void lxa_graphics_scene::widen_columns()
{
    m_column_delta *= 1.25;
    clear();
    place_signatures();
    update();
    m_graphics_view->centerOn(m_bottom_left_x, m_bottom_left_y);

    //place_containment_edges();
}
void lxa_graphics_scene::narrow_columns()
{
    m_column_delta *= 0.8;
    clear();
    place_signatures();
    m_graphics_view->centerOn(m_bottom_left_x, m_bottom_left_y);

}
void  lxa_graphics_scene::move_rows_apart()
{    m_row_delta *= 1.2;
    clear();
    place_signatures();
     m_graphics_view->centerOn(m_bottom_left_x, m_bottom_left_y);

}

void lxa_graphics_scene::move_rows_closer()
{
    m_row_delta *= 0.8;
    clear();
    place_signatures();
    m_graphics_view->centerOn(m_bottom_left_x, m_bottom_left_y);

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


void lxa_graphics_view::move_up()
{//   int min  = horizontalScrollBar()->minimum;
 //   horizontalScrollBar()->setvalue(0);
 //   update;
 //  qDebug() << "translate";
}
//--------------------------------------------------------------------------//
void lxa_graphics_scene::display_focus_signature(){
    QList<QGraphicsItem*> item_list = QGraphicsScene::items();
    for (int i = 0; i < item_list.size(); i++){
        QGraphicsItem* p_item = item_list[i];
        graphic_signature * p_graphic_sig = dynamic_cast<graphic_signature *>( p_item );
        if (p_graphic_sig){
            p_graphic_sig->set_color(Qt::white);
        }
    }
    update();

}
//--------------------------------------------------------------------------//
void lxa_graphics_scene::set_focus_signature(){
    qDebug() << "set focus signature"<<379;
   // m_top_graphic_signature->mark_as_focus();
    update();

}
