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
#include "CompareFunc.h"

/////////////////////////////////////////////////////////////////////////////
//          Graphic signaturev
//          the base class for specific shapes
/////////////////////////////////////////////////////////////////////////////


graphic_signature2::graphic_signature2  () {
    m_focus_flag = false;
    m_color = Qt::green;
    m_font_size = 12;
};

graphic_signature2::graphic_signature2   (CSignature* this_signature,   QColor, bool )
{
    m_focus_flag =false;
    m_color = Qt::green;
    m_signature = this_signature;
        m_font_size = 12;
};
graphic_signature2::graphic_signature2   (CSignature* this_signature )
{
    m_signature = this_signature;
    m_color = Qt::green;
    m_focus_flag = false;
        m_font_size = 12;
};

void graphic_signature2::set_color(Qt::GlobalColor this_color){
    m_color = this_color;

};


/////////////////////////////////////////////////////////////////////////////
//
//          lxa graphics view
//
/////////////////////////////////////////////////////////////////////////////


lxa_graphics_view::lxa_graphics_view(lxa_graphics_scene* this_scene, MainWindow* this_window):
    QGraphicsView( static_cast<QGraphicsScene*> ( this_scene) )
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
    QGraphicsView::mousePressEvent(event);
};

/*
void lxa_graphics_view::set_graphics_scene(lxa_graphics_scene*  pScene)
{
    m_graphics_scene = pScene;
}
*/

/////////////////////////////////////////////////////////////////////////////
//
//          lxa graphics scene
//
/////////////////////////////////////////////////////////////////////////////



//--------------------------------------------------------------------------//
lxa_graphics_scene::lxa_graphics_scene(MainWindow * window, CLexicon * lexicon, bool display_circles_flag ){
    m_main_window               = window;
    m_lexicon                   = lexicon;
    m_location_of_bottom_row    = 0;
    m_row_delta                 = 250;
    m_column_delta              = 250;
    m_border                    = 0;
    m_normal_color              = Qt::green;
    m_focus_color               = Qt::red;
    m_out_of_focus_color        = Qt::gray;
    m_focus_signature           = NULL;
    m_focus_signature_2         = NULL;
    m_focus_graphic_signature   = NULL;
    m_suffix_flag               = true;
    m_signature_slide_flag      = false;
    m_display_circles_for_signatures= display_circles_flag ;
    m_graphics_view = NULL;

    // click on a hypothesis, watch its effects on the signature lattice.
    connect(m_main_window->get_upper_right_tableview(),SIGNAL(clicked(const QModelIndex & )),
            this,SLOT(implement_hypothesis(const QModelIndex &  )));
};
//--------------------------------------------------------------------------//

int lxa_graphics_scene::m_calculate_row_in_scene_pos_coord(int row_no)
{
   return  m_location_of_bottom_row - (row_no - 2) * m_row_delta;
}

//void lxa_graphics_scene::add_arrow(CSignature* start_sig, CSignature* end_sig)
//{
    //lxa_arrow* new_arrow = new lxa_arrow(start_sig,end_sig);
//}



lxa_arrow::lxa_arrow(CSignature* start_sig, CSignature* end_sig){
    m_start_sig = start_sig;
    m_end_sig = end_sig;
    m_color = Qt::blue;
}

void lxa_graphics_scene::place_arrow(lxa_arrow* this_arrow)
{
    QPointF start = m_map_from_sig_to_pgraphsig[this_arrow->m_start_sig]->get_center();
    QPointF end   = m_map_from_sig_to_pgraphsig[this_arrow->m_end_sig]->get_center();
    double slope = (end.ry() - start.ry()) / (end.rx() - start.rx());
    double delta_x = end.rx() - start.rx();
    double margin = 0;
    double real_start_x = start.rx() + margin;
    double real_start_y = start.ry() + margin * slope;
    double real_end_x = start.rx() +  delta_x - margin;
    double real_end_y = start.ry() +  (delta_x - margin) * slope ;

    QPointF start_point (real_start_x, real_start_y);
    QPointF end_point (real_end_x, real_end_y);

    QLineF line_1 (start_point, end_point);

    QPen pen;
    pen.setColor(this_arrow->m_color);
    pen.setWidth(5);
    addLine(line_1, pen);
};

//--------------------------------------------------------------------------//
lxa_graphics_scene::~lxa_graphics_scene ()
{    for (int itemno = 0; itemno < m_graphic_signature_lattice.size(); itemno ++ ){
            delete m_graphic_signature_lattice[itemno];

    }
    m_graphic_signature_lattice.clear();
};
//--------------------------------------------------------------------------//
void lxa_graphics_scene::clear_all()
{
    if (m_graphic_signature_lattice.size() > 0){
        for (int i = 0; i < m_graphic_signature_lattice.size(); i++){
            for (int j = 0; j < m_graphic_signature_lattice.at(i)->size(); j++){
                delete m_graphic_signature_lattice.at(i)->at(j);
            }
         delete m_graphic_signature_lattice.at(i);
        }
    }
    if (m_signature_lattice.size() > 0){
        for (int i = 0; i < m_signature_lattice.size(); i++){
            m_signature_lattice.at(i)->clear();
        }
    }
   m_graphic_signature_lattice.clear();
   m_suffixes                   = NULL;
   m_prefixes                   = NULL;

   m_map_from_sig_to_pgraphsig.clear();

   QGraphicsScene::clear();


    // There is more that needs to be cleared here. //
}
//--------------------------------------------------------------------------//
void lxa_graphics_scene::clear_scene()
{
    QGraphicsScene::clear();
}
//--------------------------------------------------------------------------//
/*
void lxa_graphics_scene::set_graphics_view(lxa_graphics_view * this_graphics_view)
{
    m_graphics_view  = this_graphics_view;
}
*/
//--------------------------------------------------------------------------//
bool compare_stem_count_2(const CSignature* pSig1, const CSignature* pSig2){
    return pSig1->get_number_of_stems() > pSig2->get_number_of_stems();
}
//--------------------------------------------------------------------------//
 bool compare_robustness(const CSignature* pSig1, const CSignature* pSig2)
{
 return  pSig1->get_robustness() > pSig2->get_robustness();
}
//--------------------------------------------------------------------------//
void lxa_graphics_scene::assign_lattice_positions_to_signatures(CSignatureCollection* signatures, eDataType this_display_type){



    m_signature_collection          = signatures;
    m_data_type                     = this_display_type;
    CLexicon*   lexicon             = m_main_window->get_lexicon();
    double      entropy_threshold   = lexicon->get_entropy_threshold_for_positive_signatures();
    int         max_size            = 0;
    int         sig_size;
    //int         MAXIMUM_NUMBER_OF_CONTAINMENT_EDGES = 2;
    int         MINIMUM_NUMBER_OF_STEMS = 2;
    CSignature * pSig;//, *qSig;

    clear_all();
    if (m_graphics_view){
        m_graphics_view->m_scale =1;
        m_graphics_view->m_x_scale =1.0;
        m_graphics_view->m_y_scale = 1.0;
    }
    //  -->  Find out what the largest number of affixes is in the signatures  <-- //

    QListIterator<CSignature*> sig_iter_1 (* signatures->get_signature_list());
    while(sig_iter_1.hasNext()){
        pSig = sig_iter_1.next();
        if ( (pSig->get_number_of_stems() < MINIMUM_NUMBER_OF_STEMS) ||
           (  m_data_type == e_data_epositive_suffixal_signatures && pSig->get_stem_entropy() <entropy_threshold )   ){
                continue;
           }
        int this_size = pSig->get_number_of_affixes();
        if (this_size > max_size){ max_size = this_size;}
    }
    if (max_size > 9){
        m_column_delta += 300;
    }
    //  --> Initialize a list of signatures for each "row" (of equal number of affixes)   <-- //
    m_insertion_point_in_signature_lattice.resize(max_size + 1);
    m_insertion_point_in_signature_lattice.fill(0);

    for (int size = 0; size <= max_size; size++){

        QList<CSignature*> * signature_list = new QList<CSignature*>;
        m_signature_lattice.append(signature_list);

        QList<graphic_signature2*> * graphic_signature_list = new QList<graphic_signature2*>;
        m_graphic_signature_lattice.append(graphic_signature_list);
    }

    //  --> Put each signature in the right row, based on its number of affixes    <-- //

    QMapIterator<QString,CSignature*> sig_iter_2 (* signatures->get_signature_map());
    while(sig_iter_2.hasNext()){
        pSig = sig_iter_2.next().value();
        if (pSig->get_number_of_stems() < MINIMUM_NUMBER_OF_STEMS) {continue;}
        if ( (m_data_type == e_data_epositive_prefixal_signatures ||
              m_data_type == e_data_epositive_suffixal_signatures) &&
                pSig->get_stem_entropy() < entropy_threshold){
            continue;
        }
        sig_size = pSig->get_number_of_affixes();
        //qDebug() << 283 << "graphics"<< pSig->get_key() << sig_size;
        m_signature_lattice[sig_size]->append(pSig);
    }


    // -->  Sort each row of the m_signature_lattice by stem frequency
    for (int rowno = 0; rowno < m_signature_lattice.size(); rowno ++){
        std::sort(m_signature_lattice[rowno]->begin(), m_signature_lattice[rowno]->end(),  compare_stem_count_2);
        for (int colno = 0; colno < m_signature_lattice[rowno]->size(); colno++){
            pSig = m_signature_lattice[rowno]->at(colno);
        }
     }

    //  --> Now the signatures are nicely organized in a matrix of sorts.   <-- //

    update();

    //lexicon->dump_signatures_to_debug();


 }

//--------------------------------------------------------------------------//
void lxa_graphics_scene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() == Qt::LeftButton)
    {
        QGraphicsItem * item = this->itemAt(  mouseEvent->scenePos() , QTransform() );
        graphic_signature2 * this_graph_sig = dynamic_cast<graphic_signature2*> (item);
        if (item && this_graph_sig) {
            {  // qDebug() << this_graph_sig->get_key()<<288;
                set_focus_signature_and_move(this_graph_sig);
            }
        }
    }
}

//--------------------------------------------------------------------------//


/*
 * As it is done now, we need to keep the lattices for both the
 * CSignature's and the graphic-signatures. That is bad design,
 * and should be changed. And its "pos" needs to know where it is, too, in the graphics-scene when we move it.
 */

void   lxa_graphics_scene::move_graphic_signature_to_the_left(graphic_signature2* graphic_sig)
{
    if (m_signature_slide_flag == false) {
        return;
    }
    int col_no(0);
    int row_no = graphic_sig->get_signature()->get_number_of_affixes();
    int row_pos = m_calculate_row_in_scene_pos_coord(row_no);



    col_no = m_graphic_signature_lattice[row_no]->indexOf(graphic_sig);

    // test that the row is the correct value?
    if (col_no == m_insertion_point_in_signature_lattice[row_no]){
        m_insertion_point_in_signature_lattice[row_no] ++ ;
    }
    if (col_no < m_insertion_point_in_signature_lattice[row_no])
    {
        return;
    }
    int new_column = m_insertion_point_in_signature_lattice[row_no];
    m_graphic_signature_lattice[row_no]->move(col_no, new_column);

    // now reassign each graphic_sig in this row its proper "pos"...
    graphic_signature2 * graphic_sig_2;
    QList<graphic_signature2*> * row_of_graphic_sigs;
    row_of_graphic_sigs = m_graphic_signature_lattice.at(row_no);
    for (int col_no_2 = new_column; col_no_2 <= col_no; col_no_2++){
        graphic_sig_2 = row_of_graphic_sigs->at(col_no_2);
        graphic_sig_2->setPos(m_calculate_column_in_scene_pos_coord(col_no_2), row_pos );
    }

    m_insertion_point_in_signature_lattice[row_no] ++ ;
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
void    lxa_graphics_scene::re_place_signatures()
{
    int number_of_rows  = m_graphic_signature_lattice.size();
    for (int row = 2; row < number_of_rows; row++){
        for (int col = 0; col < m_graphic_signature_lattice[row]->size(); col++  ){
            graphic_signature2* this_graphic_signature = m_graphic_signature_lattice.at(row)->at(col);
            int x = m_calculate_column_in_scene_pos_coord(col);
            int y = m_location_of_bottom_row - (row-2) * m_row_delta;
            this_graphic_signature -> setPos(x,y);
         }
    }
    place_arrows();
}
//--------------------------------------------------------------------------//
void lxa_graphics_scene::place_arrows()
{
    for (int arrow_no = 0; arrow_no < m_arrows.count(); arrow_no++){
        lxa_arrow * this_arrow = m_arrows.at(arrow_no);
        place_arrow(this_arrow);
    }
}
//--------------------------------------------------------------------------//

void lxa_graphics_scene::create_and_place_signatures()
{
    m_signature_radius  = 30;
    //int radius;
    int number_of_rows = m_graphic_signature_lattice.size();

    if (m_graphic_signature_lattice.size() > 0){
        for (int i = 0; i < m_graphic_signature_lattice.size(); i++){
            for (int j = 0; j < m_graphic_signature_lattice.at(i)->size(); j++){
                delete m_graphic_signature_lattice.at(i)->at(j);
            }
         delete m_graphic_signature_lattice.at(i);
        }
    }
    m_graphic_signature_lattice.clear();



    for (int size = 0; size <= number_of_rows; size++){
        QList<graphic_signature2*> * graphic_signature_list = new QList<graphic_signature2*>;
        m_graphic_signature_lattice.append(graphic_signature_list);
    }

    m_location_of_bottom_row = m_row_delta * number_of_rows;
    m_maximum_y = m_row_delta * number_of_rows;

    m_bottom_left_x = 0;
    m_bottom_left_y = m_location_of_bottom_row ;

    /*
     * We build up a list of lists of graphic-sigs, and a map, from sig to graphic-sig.
     */
    //  -->    Iterate through the rows of signatures    <--//



    if (m_display_circles_for_signatures){
        for (int row = 2; row < number_of_rows; row++){
            CSignature_ptr_list_iterator sig_iter(*m_signature_lattice[row]);
            int col = 0;
            while (sig_iter.hasNext()){
                CSignature* pSig = sig_iter.next();
                int x = m_calculate_column_in_scene_pos_coord(col);
                int y = m_calculate_row_in_scene_pos_coord(row);
                sig_circle * this_sig_circle = new sig_circle(this, pSig);
                addItem(this_sig_circle);
                this_sig_circle->setPos(x,y+ this_sig_circle->get_radius());
                qDebug() << 437 << x << y;
                m_map_from_sig_to_pgraphsig[pSig]=this_sig_circle;
                m_graphic_signature_lattice[row]->append(this_sig_circle);
                col++;
            }
        }
        update();
        return;
    }





    for (int row = 2; row < number_of_rows; row++){
        CSignature_ptr_list_iterator sig_iter(*m_signature_lattice[row]);
        int col = 0;
        while (sig_iter.hasNext()){
            CSignature* pSig = sig_iter.next();
            int x = m_calculate_column_in_scene_pos_coord(col);
            int y = m_calculate_row_in_scene_pos_coord(row);

            switch (row){
            case 2:{
                bar * this_bar  = new bar (pSig);
                addItem(this_bar);
                this_bar->setPos(x,y);
                m_map_from_sig_to_pgraphsig[pSig]=this_bar;
                m_graphic_signature_lattice[row]->append(this_bar);
                break;
                }
            case 3:{
                triangle2 *  this_triangle_2  = new triangle2 (pSig);
                addItem(this_triangle_2);                this_triangle_2->setPos(x,y);
                m_map_from_sig_to_pgraphsig[pSig]=this_triangle_2;
                m_graphic_signature_lattice[row]->append(this_triangle_2);
                break;
            }
            case 4:{
                square2 * this_square_2  = new square2 (pSig);
                addItem(this_square_2);
                this_square_2->setPos(x,y);
                m_map_from_sig_to_pgraphsig[pSig]=this_square_2;
                m_graphic_signature_lattice[row]->append(this_square_2);
                break;
            }
            case 5:{
                pentagon2 * this_pentagon_2  = new pentagon2 (pSig);
                //qDebug() << 431 << pSig->get_key(); // here there is doubling the second time around.
                addItem(this_pentagon_2);
                this_pentagon_2->setPos(x,y);
                m_map_from_sig_to_pgraphsig[pSig]=this_pentagon_2;
                m_graphic_signature_lattice[row]->append(this_pentagon_2);
                break;
            }
            case 6:{
                hexagon2 * this_hexagon_2  = new hexagon2 (pSig);
                addItem(this_hexagon_2);
                this_hexagon_2->setPos(x,y);
                m_map_from_sig_to_pgraphsig[pSig]=this_hexagon_2;
                m_graphic_signature_lattice[row]->append(this_hexagon_2);
                break;
            }
            case 7:{
                septagon2 * this_septagon_2  = new septagon2 (pSig);
                addItem(this_septagon_2);
                this_septagon_2->setPos(x,y);
                m_map_from_sig_to_pgraphsig[pSig]=this_septagon_2;
                m_graphic_signature_lattice[row]->append(this_septagon_2);
                break;
            }
            case 8:{
                octagon2 * this_octagon_2  = new octagon2 (pSig);
                addItem(this_octagon_2);
                this_octagon_2->setPos(x,y);
                m_map_from_sig_to_pgraphsig[pSig]=this_octagon_2;
                m_graphic_signature_lattice[row]->append(this_octagon_2);
                break;
            }
            case 9:{
                nonagon2 * this_nonagon_2  = new nonagon2 (pSig);
                addItem(this_nonagon_2);
                this_nonagon_2->setPos(x,y);
                m_map_from_sig_to_pgraphsig[pSig]=this_nonagon_2;
                m_graphic_signature_lattice[row]->append(this_nonagon_2);
                break;
            }
            case 10:{
                decagon2 * this_decagon_2  = new decagon2 (pSig);
                addItem(this_decagon_2);
                this_decagon_2->setPos(x,y);
                m_map_from_sig_to_pgraphsig[pSig]=this_decagon_2;
                m_graphic_signature_lattice[row]->append(this_decagon_2);

                break;
           }
            case 11:{
                elevenagon2 * this_elevenagon_2  = new elevenagon2 (pSig);
                addItem(this_elevenagon_2);
                this_elevenagon_2->setPos(x,y);
                m_map_from_sig_to_pgraphsig[pSig]=this_elevenagon_2;
                m_graphic_signature_lattice[row]->append(this_elevenagon_2);

                break;
           }
            case 12:{
                twelvagon2 * this_twelvagon_2  = new twelvagon2 (pSig);
                addItem(this_twelvagon_2);
                this_twelvagon_2->setPos(x,y);
                m_map_from_sig_to_pgraphsig[pSig]=this_twelvagon_2;
                m_graphic_signature_lattice[row]->append(this_twelvagon_2);

                break;
           }
            case 13:{
                thirteenagon * this_thirteenagon  = new thirteenagon (pSig);
                addItem(this_thirteenagon);
                this_thirteenagon->setPos(x,y);
                m_map_from_sig_to_pgraphsig[pSig]=this_thirteenagon;
                m_graphic_signature_lattice[row]->append(this_thirteenagon);

                break;
           }
            case 14:{
                fourteenagon * this_fourteenagon  = new fourteenagon (pSig);
                addItem(this_fourteenagon);
                this_fourteenagon->setPos(x,y);
                m_map_from_sig_to_pgraphsig[pSig]=this_fourteenagon;
                m_graphic_signature_lattice[row]->append(this_fourteenagon);

                break;
           }
            case 15:{
                fifteenagon * this_fifteenagon  = new fifteenagon (pSig);
                addItem(this_fifteenagon);
                this_fifteenagon->setPos(x,y);
                m_map_from_sig_to_pgraphsig[pSig]=this_fifteenagon;
                m_graphic_signature_lattice[row]->append(this_fifteenagon);

                break;
           }
            default:{
              //graphic   p_graph_sig = new graphic_signature (x,y, pSig,  m_row_delta, m_normal_color);
              //  m_map_from_sig_to_pgraphsig[pSig] = p_graph_sig;
              //  addItem(p_graph_sig);
                break;
            }
        }
            col++;
        }
    }
    place_arrows();
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
    default:{

    }
    }
}

eGraphicsStatus lxa_graphics_scene::change_graphics_status(){
    switch (m_graphics_status) {
        case GS_no_focus:{
           m_graphics_status =  GS_one_focus_signature;
           break;
        }
        case GS_one_focus_signature:{
            m_graphics_status =GS_one_focus_signature_all_others_grayed;
            break;
        }
        case GS_one_focus_signature_all_others_grayed:{
            m_graphics_status =GS_one_focus_signature_some_satellites_others_grayed;
            break;
        }
        case GS_one_focus_signature_some_satellites_others_grayed:{
            m_graphics_status =GS_no_focus;
            break;
        }
        default:{
            m_graphics_status = GS_default;
            break;
        }
    }
    return m_graphics_status;
}

//--------------------------------------------------------------------------//
// not currently used;
void lxa_graphics_scene::place_containment_edges(){
    //QPair<CSignature*, CSignature*> * pPair;
    //QPair<int,int>* row_and_col_1, * row_and_col_2;
    //CSignature* sig1, *sig2;
    for (int i= 0; i < m_signature_containment_edges.size(); i++){
        //pPair = m_signature_containment_edges[i];
        //sig1 = pPair->first;
        //ig2 = pPair->second;
        //int row1 = sig1->get_number_of_affixes();
        //int row2 = sig2->get_number_of_affixes();
        //row_and_col_1 = m_map_from_sig_to_row_and_column[sig1];
        //int col1 = row_and_col_1->second;
        //row_and_col_2 = m_map_from_sig_to_row_and_column[sig2];
        //int col2 = row_and_col_2->second;
        //int x1 = col1 * m_column_delta + m_signature_radius/2 ;
        //int x2 = col2 * m_column_delta + m_signature_radius/2;
        //int y1 = m_location_of_bottom_row - (row1-2) * m_row_delta + m_signature_radius/2;
        //int y2 = m_location_of_bottom_row - (row2-2) * m_row_delta + m_signature_radius/2;
        //addLine(x1,y1,x2,y2,QPen());
    }
}
//--------------------------------------------------------------------------//
void lxa_graphics_scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
   //QToolTip::showText(event->screenPos().toPoint(), "x");
   //QGraphicsScene::mouseMoveEvent();
    (void) event; // placeholder
 }
//--------------------------------------------------------------------------//
void lxa_graphics_scene::widen_columns()
{
    m_column_delta *= 1.25;
    re_place_signatures();
    update();
}
void lxa_graphics_scene::narrow_columns()
{
    m_column_delta *= 0.8;
    re_place_signatures();
}
void  lxa_graphics_scene::move_rows_apart()
{    m_row_delta *= 1.2;
     re_place_signatures();
}

void lxa_graphics_scene::move_rows_closer()
{
    m_row_delta *= 0.8;
    re_place_signatures();

}
void lxa_graphics_view::zoom_up()
{
    m_x_scale *= 0.8;
    m_y_scale *= 0.8;
    lxa_graphics_scene * p_scene =static_cast<lxa_graphics_scene*>(scene());
    scale(0.8,0.8);
    if ( p_scene->m_focus_signature ){
        graphic_signature2 * p_sig = p_scene->m_focus_graphic_signature;
        QGraphicsView::ensureVisible(p_sig->boundingRect());
    } else{
        QGraphicsView::ensureVisible(0,0,10,10);

    }
    return;
}

void lxa_graphics_view::zoom_down()
{
        m_x_scale *= 1.2;
        m_y_scale *= 1.2;
        scale(1.2,1.2);
        QGraphicsView::ensureVisible(0,0,10,10);

}
void lxa_graphics_view::reset_scale()
{
    double new_scale = 1.0 / m_x_scale;
    scale(new_scale, new_scale);

    m_x_scale = 1.0;
    m_y_scale = 1.0;

    static_cast<lxa_graphics_scene*>(scene())->set_row_delta (225);
    static_cast<lxa_graphics_scene*>(scene())->set_column_delta ( 200 ) ;


}



//--------------------------------------------------------------------------//
//  These functions work together.
//--------------------------------------------------------------------------//

void lxa_graphics_scene::set_focus_graphic_signature_and_show_subsignatures (graphic_signature2 * graphic_sig){

    if (m_focus_graphic_signature){
         m_focus_graphic_signature->set_color(m_normal_color);
    }
    graphic_sig -> set_color(m_focus_color);
    show_subsignatures();
    re_place_signatures();

};

/* This function finds the signatures contained in the focus_graphic_signature
 */

void lxa_graphics_scene::show_subsignatures(){

    if (! m_focus_graphic_signature){
        return;
    }
    sigstring_t focus_signature = m_focus_graphic_signature->get_key();
    int affix_count = m_focus_graphic_signature->get_signature()->get_number_of_affixes();

    for (auto sig_iter : m_map_from_sig_to_pgraphsig.values()){
        if ( sig_iter->get_signature()->get_number_of_affixes() >= affix_count){
            continue;
        }
        if (signature_1_contains_signature_2 (focus_signature, sig_iter->get_key()))
        {
            sig_iter->set_color(m_focus_color);
        } else
        {
            sig_iter->set_color(m_out_of_focus_color);
        }
    }

};

// this is not yet working...we are using it.
void lxa_graphics_scene::set_focus_signature_and_move(graphic_signature2* graph_sig){

    if (m_focus_graphic_signature){
        m_focus_graphic_signature->set_color(m_normal_color);
    }
    m_focus_graphic_signature = graph_sig;
    show_subsignatures_and_move_them();
    re_place_signatures();

    update();
    re_place_signatures();


    };


void lxa_graphics_scene::show_subsignatures_and_move_them()
{
    if (! m_focus_graphic_signature){
        return;
    }
    for (auto sig_iter : m_map_from_sig_to_pgraphsig.values()){
        if (m_focus_graphic_signature->get_signature()==sig_iter->get_signature())
        {
            sig_iter->set_color(m_focus_color);

        } else if (m_focus_graphic_signature->get_signature()->contains(sig_iter->get_signature()))
        {
            move_graphic_signature_to_the_left(sig_iter);
            sig_iter->set_color(Qt::blue);
        } else if (sig_iter->get_signature() -> contains (m_focus_graphic_signature->get_signature()))
        {
            move_graphic_signature_to_the_left(sig_iter);
            sig_iter->set_color(Qt::magenta);
        }
        else
        {
            sig_iter->set_color(m_out_of_focus_color);
        }
    }
    update();

};


void lxa_graphics_scene::make_debug_report()
{
    qDebug() << "********  scene report ********************* ";

    qDebug() << "Report on graphics scene";
    qDebug() << "signature lattice size:" << m_signature_lattice.size();
    qDebug() << "graphic signature lattice size:" << m_graphic_signature_lattice.size();
    qDebug() << "map from sig to graphic sig:" << m_map_from_sig_to_pgraphsig.size();

    qDebug() << "lattice of signatures";
    for (int row = 0; row < m_signature_lattice.size(); row ++ ){
        qDebug() << "row number and size " << row << m_signature_lattice.at(row)->size();
    }
    qDebug() << "lattice of graphic signatures";
    for (int row = 0; row < m_graphic_signature_lattice.size(); row ++ ){
        qDebug() << "row number and size " << row << m_graphic_signature_lattice.at(row)->size();
    }
    qDebug() << "********  end of scene report ********************* ";

}

QColor random_color(){
QColor colors[10] = {QColor("cyan"), QColor("magenta"), QColor("red"),
                      QColor("darkRed"), QColor("darkCyan"), QColor("darkMagenta"),
                      QColor("green"), QColor("darkGreen"), QColor("yellow"),
                      QColor("blue")};
return colors[0];
}
/*
void lxa_graphics_scene::toggle_display_circles_for_signatures()
{
    if (m_display_circles_for_signatures)
        { clear_all();
          m_display_circles_for_signatures = false;}
    else {
        clear_all();
        m_display_circles_for_signatures = true;}
}
*/
