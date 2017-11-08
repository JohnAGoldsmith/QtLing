#ifndef GRAPHICS_H
#define GRAPHICS_H
#include<QGraphicsItem>
#include <QGraphicsView>

class CSignature;
class CSignatureCollection;
class MainWindow;
class lxa_graphics_scene;

class lxa_graphics_view : public QGraphicsView
{
    QList<QList<CSignature*>*>      m_signature_lattice;
    MainWindow *                    m_main_window;
    lxa_graphics_scene*             m_graphics_scene;
public:
    lxa_graphics_view( MainWindow * );
    void                            draw_signatures();
    lxa_graphics_scene*             get_graphics_scene() {return m_graphics_scene;}
    void                            set_graphics_scene( lxa_graphics_scene* pScene ) {m_graphics_scene = pScene;}


};

class lxa_graphics_scene : public QGraphicsScene
{
    MainWindow*                             m_main_window;
    lxa_graphics_view*                      m_graphics_view;
    QList<QList<CSignature*>*>              m_signature_lattice;
    QList<QPair<CSignature*,CSignature*> >  m_signature_containment_edges;
    QMap<CSignature*, int>                  m_map_from_sig_to_column_no;
    int                                     m_row_delta;
    int                                     m_column_delta;
    int                                     m_location_of_bottom_row;
    int                                     m_signature_radius;
public:
                    lxa_graphics_scene( MainWindow * );
                    ~lxa_graphics_scene();
    void            set_graphics_view (lxa_graphics_view* );
    void            ingest_signatures(CSignatureCollection*);
    void            place_signatures();
    void            place_containment_edges(CSignatureCollection*);
};

class signature_node : public QGraphicsItem
{
public:
    QRectF boundingRect() const
    {

    }
    void paint (QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget){
        painter->drawEllipse(100,100,10,10);
    }
};

#endif // GRAPHICS_H
