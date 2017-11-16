#ifndef GRAPHICS_H
#define GRAPHICS_H
#include<QGraphicsItem>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QGraphicsScene>
#include <QPoint>
#include <QColor>

class CSignature;
class CSignatureCollection;
class MainWindow;
class lxa_graphics_scene;

class graphic_signature : public QGraphicsEllipseItem
{
    lxa_graphics_scene * m_graphics_scene;
    CSignature *         m_signature;
    Qt::GlobalColor            m_color;
    public:
    graphic_signature(int x, int y, CSignature*, lxa_graphics_scene* scene, int radius, int row_delta);
    void mousePressEvent (QGraphicsSceneMouseEvent*);
    void                set_color(QColor) { m_color = Qt::white;}
};

class lxa_graphics_view : public QGraphicsView
{ friend:: lxa_graphics_scene;
    QList<QList<CSignature*>*>      m_signature_lattice;
    MainWindow *                    m_main_window;
    lxa_graphics_scene*             m_graphics_scene;
    double                          m_scale;

    void                            mousePressEvent(QMouseEvent*);
public:
    lxa_graphics_view( MainWindow * );
    void                            draw_signatures();
    void                            expand() {scale(1.25,1.25);}
    void                            contract() {scale(0.8,0.8);}
    void                            move_up() ;
    void                            move_down() {translate(0,-50);}
    void                            move_left() {translate(-50,0);}
    void                            move_right() {translate(50,0);}
    lxa_graphics_scene*             get_graphics_scene() {return m_graphics_scene;}
    void                            set_graphics_scene( lxa_graphics_scene* pScene ) {m_graphics_scene = pScene;}


};

class lxa_graphics_scene : public QGraphicsScene
{
    MainWindow*                             m_main_window;
    lxa_graphics_view*                      m_graphics_view;
    QList<QList<CSignature*>*>              m_signature_lattice;
    QList<QPair<CSignature*,CSignature*>*>  m_signature_containment_edges;
    QMap<CSignature*, int>                  m_map_from_sig_to_column_no;
    CSignature*                             m_focus_signature;
    int                                     m_row_delta;
    int                                     m_column_delta;
    int                                     m_location_of_bottom_row;
    int                                     m_signature_radius;

private:
    void                                    mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent);
public:
                    lxa_graphics_scene( MainWindow * , CSignatureCollection* );
                    ~lxa_graphics_scene();
    void            set_graphics_view (lxa_graphics_view* );
    void            ingest_signatures(CSignatureCollection*);
    void            add_signature_containment_edge (QPair<CSignature*, CSignature*>* pPair)
                            {m_signature_containment_edges.append (pPair); }
    void            place_signatures();
    void            place_containment_edges();
    void            widen_columns();
    void            narrow_columns();
    void            mouseMoveEvent(QGraphicsSceneMouseEvent * event);
    void            set_focus_signature(CSignature* pSig)       {m_focus_signature = pSig;}
    void            display_focus_signature();
};

class signature_node : public QGraphicsItem
{
public:
    QRectF boundingRect() const;
    void paint (QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget){
        painter->drawEllipse(100,100,10,10);
    }
};

#endif // GRAPHICS_H
