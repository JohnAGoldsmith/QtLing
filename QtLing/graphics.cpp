#include <QDebug>
#include <QToolTip>
#include <QGraphicsScene>
#include <QScrollBar>
#include <QMouseEvent>
#include <QGraphicsSceneEvent>
#include <QGraphicsItem>
#include <QRect>
#include <QtMath>
#include "graphics.h"
#include "generaldefinitions.h"
#include "SignatureCollection.h"
#include "mainwindow.h"


graphic_signature::graphic_signature(int x, int y, CSignature* pSig, lxa_graphics_scene * scene, int radius, int row_delta)
{
    m_graphics_scene = scene;
    m_signature = pSig;
    m_color = Qt::red;
    //scene->setForegroundBrush(m_color);
    QGraphicsItem::setAcceptHoverEvents(true);
    QGraphicsItem::ItemIsSelectable;
    QGraphicsItem::ItemIsMovable;
    switch(pSig->get_number_of_affixes()){
    case 3:{
        QPolygon triangle;
        triangle.append(QPoint(x,y+30));
        triangle.append(QPoint(x+40,y+30));
        triangle.append(QPoint(x+20,y-10));
        triangle.append(QPoint(x,y+28));
        QGraphicsPolygonItem * pTriangleItem = scene->addPolygon(triangle,QPen(), QBrush(m_color));
        break;}
    case 4:{
        QPolygon square;
        square.append(QPoint(x,y+30));
        square.append(QPoint(x+40,y+30));
        square.append(QPoint(x+40,y-10));
        square.append(QPoint(x,y-10));
        square.append(QPoint(x,y+30));
        QGraphicsPolygonItem * p_square_item = scene->addPolygon(square,QPen(), QBrush(m_color));
        break;}
    case 5:{
        QPolygon pent;
        double xprime  = x+ 30;
        pent.append(QPoint(xprime,y-30 ));
        pent.append(QPoint(xprime-29,y-12));
        pent.append(QPoint(xprime-18,y+24));
        pent.append(QPoint(xprime+18,y+24));
        pent.append(QPoint(xprime+18,y+24));
        pent.append(QPoint(xprime+29,y-9));
        pent.append(QPoint(xprime,y-30 ));
        QGraphicsPolygonItem * p_square_item = scene->addPolygon(pent,QPen(), QBrush(m_color));
        break;}
    default:
        scene->addEllipse(x,y,radius ,radius,QPen(),QBrush(m_color));
    }

    QGraphicsTextItem * p_text_item = new QGraphicsTextItem;
    p_text_item->setPlainText(pSig->get_key());
    int text_width = p_text_item->textWidth();
    p_text_item->setPos (x - 0.5 * text_width,y + 0.3* row_delta);

    QGraphicsTextItem * q_text_item = new QGraphicsTextItem;
    q_text_item->setPlainText(QString::number(pSig->get_number_of_stems()));
    q_text_item->setPos (x - 0.5 * text_width,y + 0.3* row_delta + 20);

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

    //QGraphicsItem::setAcceptHoverEvents(true);
    //QGraphicsItem::ItemIsSelectable;
    //QGraphicsItem::ItemIsMovable;
    int my_x = 100;
    int my_y = 100;
    int my_width= 20;
    int my_height = 30;
    scene->addRect(my_x,my_y,my_width, my_height ,QPen(),QBrush(m_color));

    QGraphicsTextItem * p_text_item = new QGraphicsTextItem;
    //p_text_item->setPlainText(pSig->get_key());
    int text_width = p_text_item->textWidth();
    p_text_item->setPos (x - 0.5 * text_width,y + 0.3* row_delta);

    QGraphicsTextItem * q_text_item = new QGraphicsTextItem;
    //q_text_item->setPlainText(QString::number(pSig->get_number_of_stems()));
    q_text_item->setPos (x - 0.5 * text_width,y + 0.3* row_delta + 20);

    scene->addItem(p_text_item);
    scene->addItem(q_text_item);
};


lxa_graphics_view::lxa_graphics_view(MainWindow* this_window)
{
    m_main_window = this_window;
    m_scale =1;
    setDragMode(QGraphicsView::ScrollHandDrag);
    setMouseTracking(true);

};

void lxa_graphics_view::mousePressEvent(QMouseEvent* event)
{
    // ADD STUFF HERE.


    QGraphicsView::mousePressEvent(event);
};
lxa_graphics_scene::lxa_graphics_scene(MainWindow * window){
    m_main_window = window;
    m_location_of_bottom_row = 0;
    m_row_delta = 100;
    m_column_delta = 200;

};

void lxa_graphics_scene::set_parameters(CSignatureCollection* p_signatures, eDisplayType this_display_type)
{
    m_signature_collection  = p_signatures;
    m_display_type          = this_display_type;
}

//  deprecated:
lxa_graphics_scene::lxa_graphics_scene (MainWindow * window, CSignatureCollection* p_signatures, eDisplayType this_display_type, CSignature* pSig1, CSignature * pSig2)
{   qDebug() << "scene constructor" << 128 << this_display_type;

    m_main_window = window;
    m_location_of_bottom_row = 0;
    m_row_delta = 100;
    m_column_delta = 200;
    m_display_type = this_display_type;
    ingest_signatures(p_signatures, this_display_type);
    set_focus_signature_1 (pSig1);
    set_focus_signature_2 (pSig2);
    place_signatures();
    //place_containment_edges();
};
lxa_graphics_scene::~lxa_graphics_scene ()
{

    for (int itemno = 0; itemno < m_signature_lattice.size(); itemno ++ ){
        delete m_signature_lattice[itemno];
    }
    m_signature_lattice.clear();

};
void lxa_graphics_scene::set_graphics_view(lxa_graphics_view * this_graphics_view)
{
    m_graphics_view  = this_graphics_view;
}
const bool compare_stem_count_2(const CSignature* pSig1, const CSignature* pSig2){
    return pSig1->get_number_of_stems() > pSig2->get_number_of_stems();
}
const  bool compare_robustness(const CSignature* pSig1, const CSignature* pSig2)
{
 return  pSig1->get_robustness() > pSig2->get_robustness();
}
void lxa_graphics_scene::ingest_signatures(CSignatureCollection* signatures, eDisplayType this_display_type){
    qDebug() << "Ingesting signatures.";
    m_display_type = this_display_type;
    CLexicon* lexicon = m_main_window->get_lexicon();
    double entropy_threshold = lexicon->get_entropy_threshold_for_positive_signatures();
    int max_size = 0;
    int sig_size;
    int MAXIMUM_NUMBER_OF_CONTAINMENT_EDGES = 2;
    int MINIMUM_NUMBER_OF_STEMS = 2;
    CSignature * pSig, *qSig;
    map_sigstring_to_sig_ptr_iter sig_iter(*signatures->get_map());
    while(sig_iter.hasNext()){
        pSig = sig_iter.next().value();
        if (pSig->get_number_of_stems() < MINIMUM_NUMBER_OF_STEMS) {
            continue;
        }
        if (m_display_type == DT_Positive_Suffix_Signatures &&
                pSig->get_stem_entropy() <entropy_threshold ){
            continue;
        }
        int this_size = pSig->get_number_of_affixes();
        if (this_size > max_size){ max_size = this_size;}
    }
    for (int size = 0; size <= max_size; size++){
        QList<CSignature*> * signature_list = new QList<CSignature*>;
        m_signature_lattice.append(signature_list);
    }
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
            m_map_from_sig_to_column_no[pSig] = colno;
        }
       }





    // -->    containment relations between signatures   <-- //
    QMapIterator<CSignature*,QList<CSignature*>*> sig_iter_2 (*signatures->get_containment_map());
    while(sig_iter_2.hasNext()){
        pSig = sig_iter_2.next().key();
        int pSig_row = pSig->get_number_of_affixes();
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


    // --->   and we place them. <---   //
    place_signatures();
}
void lxa_graphics_scene::place_signatures()
{
    m_signature_radius = 30;
    int border = 100;
    int radius;
    int number_of_rows = m_signature_lattice.size();
    m_location_of_bottom_row = m_row_delta * number_of_rows;
    for (int row = 2; row <m_signature_lattice.size(); row++){
        CSignature_ptr_list_iterator sig_iter(*m_signature_lattice[row]);
        int col = 0;
        while (sig_iter.hasNext()){
            CSignature* pSig = sig_iter.next();
            int stem_count = pSig->get_number_of_stems();
            if (stem_count<5)
            {
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
            graphic_signature * p_graph_sig = new graphic_signature (x,y, pSig, this, radius, m_row_delta);
            addItem(p_graph_sig);
            col++;
        }
    }
}
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

void lxa_graphics_scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
//   QToolTip::showText(event->screenPos().toPoint(), "x");
//   QGraphicsScene::mouseMoveEvent();
//    qDebug() << "mouse over";
}

void lxa_graphics_scene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() == Qt::LeftButton)
    {
        qDebug() << "mouse move"<<178;
    }
}

void lxa_graphics_scene::widen_columns()
{
    m_column_delta *= 1.25;
    clear();
    place_signatures();
    //place_containment_edges();
}
void lxa_graphics_scene::narrow_columns()
{
    m_column_delta *= 0.8;
    clear();
    place_signatures();
    //place_containment_edges();
}
void  lxa_graphics_scene::move_rows_apart()
{
    m_row_delta *= 1.2;
    clear();
    place_signatures();
}

void lxa_graphics_scene::move_rows_closer()
{
    m_row_delta *= 0.8;
    clear();
    place_signatures();
}

void lxa_graphics_view::move_up()
{//   int min  = horizontalScrollBar()->minimum;
 //   horizontalScrollBar()->setvalue(0);
 //   update;
 //  qDebug() << "translate";
}

void lxa_graphics_scene::display_focus_signature(){
    QList<QGraphicsItem*> item_list = QGraphicsScene::items();
    for (int i = 0; i < item_list.size(); i++){
        QGraphicsItem* p_item = item_list[i];
        graphic_signature * p_graphic_sig = dynamic_cast<graphic_signature *>( p_item );
        if (p_graphic_sig){
            p_graphic_sig->set_color(Qt::white);
            //qDebug() << p_graphic_sig->get_signature()->get_key();
        }
    }
    update();
    qDebug() << "236";

}
