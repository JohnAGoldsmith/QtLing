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
    QList<QList<CSignature*>*>  m_signature_lattice;
    MainWindow *                m_main_window;
    lxa_graphics_scene*         m_graphics_scene;
public:
    lxa_graphics_view( MainWindow * , lxa_graphics_scene * my_scene);
    void draw_signatures();
    lxa_graphics_scene*             get_graphics_scene() {return m_graphics_scene;}


};

class lxa_graphics_scene : public QGraphicsScene
{
    MainWindow*                     m_main_window;
    lxa_graphics_view*              m_graphics_view;
    QList<QList<CSignature*>*>      m_signature_lattice;

public:
                    lxa_graphics_scene( MainWindow * );
                    ~lxa_graphics_scene();
    void            set_graphics_view (lxa_graphics_view* );
    void            ingest_signatures(CSignatureCollection*);
    void            place_signatures();
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
