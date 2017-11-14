#include <QDebug>
#include <QToolTip>
#include <QGraphicsScene>
#include <QScrollBar>
#include <QMouseEvent>
#include <QGraphicsSceneEvent>
#include <QGraphicsItem>
#include "graphics.h"
#include "generaldefinitions.h"
#include "SignatureCollection.h"
#include "mainwindow.h"


graphic_signature::graphic_signature()
{
    QGraphicsItem::setAcceptHoverEvents(true);
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
}

lxa_graphics_scene::lxa_graphics_scene (MainWindow * window, CSignatureCollection* p_signatures)
{   qDebug() << "scene constructor";

    m_main_window = window;
    m_location_of_bottom_row = 0;
    m_row_delta = 80;
    m_column_delta = 30;
    ingest_signatures(p_signatures);
    place_signatures();
    place_containment_edges();
};
lxa_graphics_scene::~lxa_graphics_scene ()
{

    for (int itemno = 0; itemno < m_signature_lattice.size(); itemno ++ ){
        delete m_signature_lattice[itemno];
        qDebug() << "graphics scene ";
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
void lxa_graphics_scene::ingest_signatures(CSignatureCollection* signatures){
    qDebug() << "Ingesting signatures.";
    int max_size = 0;
    int sig_size;
    int MAXIMUM_NUMBER_OF_CONTAINMENT_EDGES = 2;
    int MINIMUM_NUMBER_OF_STEMS = 5;
    CSignature * pSig, *qSig;
    map_sigstring_to_sig_ptr_iter sig_iter(*signatures->get_map());
    while(sig_iter.hasNext()){
        pSig = sig_iter.next().value();
        if (pSig->get_number_of_stems() < MINIMUM_NUMBER_OF_STEMS) {continue;}
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
        sig_size = pSig->get_number_of_affixes();
        m_signature_lattice[sig_size]->append(pSig);
        //qDebug() << pSig->get_key() << pSig->get_number_of_stems() ;
        //if (pSig->get_number_of_affixes() == 2) { qDebug() << pSig->get_key() << pSig->get_number_of_stems();}
    }
    // -->  Sort each row of the m_signature_lattice by stem frequency
    for (int rowno = 0; rowno < m_signature_lattice.size(); rowno ++){
        std::sort(m_signature_lattice[rowno]->begin(), m_signature_lattice[rowno]->end(),  compare_stem_count_2);
        for (int colno = 0; colno < m_signature_lattice[rowno]->size(); colno++){
            pSig = m_signature_lattice[rowno]->at(colno);
            m_map_from_sig_to_column_no[pSig] = colno;
            qDebug() << pSig->get_key() << 83;
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
            //qDebug() << pSig->get_key() << qSig->get_key() << 87;
            add_signature_containment_edge(pPair);
            count++;
            if (count > MAXIMUM_NUMBER_OF_CONTAINMENT_EDGES){
                break;
                //qDebug() << "break at " << count;
            }
            //qDebug() << "count "<< count;
        }
    }
}
void lxa_graphics_scene::place_signatures()
{

    m_signature_radius = 20;
    int number_of_rows = m_signature_lattice.size();
    m_location_of_bottom_row = m_row_delta * number_of_rows;


    for (int row = 2; row <m_signature_lattice.size(); row++){
        CSignature_ptr_list_iterator sig_iter(*m_signature_lattice[row]);
        int col = 0;
        while (sig_iter.hasNext()){
            CSignature* pSig = sig_iter.next();
            int x = col * m_column_delta;
            int y = m_location_of_bottom_row - (row-2) * m_row_delta;
            QGraphicsEllipseItem * pEllipse = addEllipse(x,y,m_signature_radius,m_signature_radius,QPen(),QBrush(Qt::red));
            pEllipse->acceptHoverEvents();
            pEllipse->grabMouse();
            QGraphicsTextItem * p_text_item = new QGraphicsTextItem;
            p_text_item->setPlainText(pSig->get_key());
            QGraphicsTextItem * q_text_item = new QGraphicsTextItem;
            q_text_item->setPlainText(QString::number(pSig->get_number_of_stems()));
            p_text_item->setPos (x - 0.5 * p_text_item->textWidth(),y + 0.3* m_row_delta);
            q_text_item->setPos (x - 0.5 * p_text_item->textWidth(),y + 0.3* m_row_delta + 10);
            addItem(p_text_item);
            addItem(q_text_item);
            col++;
        }
    }
}
void lxa_graphics_scene::place_containment_edges(){
    QPair<CSignature*, CSignature*> * pPair;
    //qDebug() << "Placing containment edges";
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
        //qDebug() << x1 << y1 << x2 << y2;
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
        //QGraphicsItem *item = QGraphicsScene::itemAt(mouseEvent->scenePos());
        qDebug() << "mouse move"<<178;
    }
}

void lxa_graphics_scene::widen_columns()
{
    m_column_delta *= 1.25;
    clear();
    place_signatures();
    place_containment_edges();
  // QGraphicsScene::update();

}
void lxa_graphics_scene::narrow_columns()
{
    m_column_delta *= 0.8;
    clear();
    place_signatures();
    place_containment_edges();
  // QGraphicsScene::update();

}
void lxa_graphics_view::move_up()
{//   int min  = horizontalScrollBar()->minimum;
 //   horizontalScrollBar()->setvalue(0);
 //   update;
 //  qDebug() << "translate";
}
