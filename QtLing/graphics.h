#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <QDebug>
#include<QGraphicsItem>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QGraphicsScene>
#include <QPoint>
#include <QColor>
#include "supersignature.h"
#include <QGraphicsItem>
class CSignature;
class CSignatureCollection;
class MainWindow;
class lxa_graphics_scene;
class CSupersignature;



/////////////////////////////////////////////////////////////////////////////
//          Graphic signature
//          the base class for specific shapes
/////////////////////////////////////////////////////////////////////////////
class graphic_signature2 : public QGraphicsItem
{
    lxa_graphics_scene * m_graphics_scene;
    CSignature *         m_signature;
protected:
    bool                 m_focus_flag;
    Qt::GlobalColor      m_color;

    public:
    graphic_signature2  ();
    graphic_signature2  (QString );
    graphic_signature2   (CSignature*,   QColor, bool focus_flag = false);
    ~graphic_signature2 ();
    QRectF              boundingRect() const
    {
        return QRectF(00, 0,60,150);
    }
    void mark_color(Qt::GlobalColor);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget)
    {
    }
    CSignature*         get_signature() {return m_signature;}
};

//--------------------->       <-----------------------------//
class triangle2 : public graphic_signature2
{
public:
    triangle2(QString);
    void paint ();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    void set_text(sigstring_t this_sigstring);

};
//--------------------->       <-----------------------------//
class square2 : public graphic_signature2
{
public:
    //square2();
    square2(QString);
    ~square2();
    void paint ();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget,QColor);
    void set_text(sigstring_t this_sigstring);
};
//--------------------->       <-----------------------------//
class pentagon2 : public graphic_signature2
{
public:
    pentagon2(QString);
    void paint ();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    void set_text(sigstring_t this_sigstring);
};
//--------------------->       <-----------------------------//
class hexagon2 : public graphic_signature2
{
public:
    hexagon2(QString);
    void paint ();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    void set_text(sigstring_t this_sigstring);
};
//--------------------->       <-----------------------------//
class septagon2 : public graphic_signature2
{
public:
    septagon2(QString);
    void paint ();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    void set_text(sigstring_t this_sigstring);
};
//--------------------->       <-----------------------------//
class octagon2 : public graphic_signature2
{
public:
    octagon2(QString);
    void paint ();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    void set_text(sigstring_t this_sigstring);
};
//--------------------->       <-----------------------------//
class nonagon2 : public graphic_signature2
{
public:
    nonagon2(QString);
    void paint ();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    void set_text(sigstring_t this_sigstring);
};
//--------------------->       <-----------------------------//

// not currently used.
class graphic_super_signature : public QRect
{
    lxa_graphics_scene *    m_graphics_scene;
    CSupersignature *       m_super_signature;
    Qt::GlobalColor         m_color;
    public:
    graphic_super_signature(int x, int y, CSupersignature*, lxa_graphics_scene* scene);
    void                    mousePressEvent (QGraphicsSceneMouseEvent*);
    void                    set_color(Qt::GlobalColor this_color) { m_color = this_color;}
    CSupersignature*        get_super_signature() {return m_super_signature;}
};


/////////////////////////////////////////////////////////////////////////////
//
//          lxa graphics view
//
/////////////////////////////////////////////////////////////////////////////

class lxa_graphics_view : public QGraphicsView
{   friend:: lxa_graphics_scene;

    QList<QList<CSignature*>*>      m_signature_lattice;
    MainWindow *                    m_main_window;
    lxa_graphics_scene*             m_graphics_scene;
    double                          m_scale;
    double                             m_x_scale;
    double                             m_y_scale;
    void                            mousePressEvent(QMouseEvent*);

public:
                                    lxa_graphics_view( MainWindow * );
    void                            expand() {scale(1.25,1.25);}
    void                            contract() {scale(0.8,0.8);}
    void                            move_up() ;
    void                            move_down() {translate(0,-50);}
    void                            move_left() {translate(-50,0);}
    void                            move_right() {translate(50,0);}
    lxa_graphics_scene*             get_graphics_scene() {return m_graphics_scene;}
    void                            set_graphics_scene( lxa_graphics_scene* pScene ) {m_graphics_scene = pScene;}
    void                            zoom_up();
    void                            zoom_down();


};

/////////////////////////////////////////////////////////////////////////////
//
//          lxa graphics scene
//
/////////////////////////////////////////////////////////////////////////////

class lxa_graphics_scene : public QGraphicsScene
{  // friend          graphic_signature;

    MainWindow*                             m_main_window;
    eDisplayType                            m_display_type;
    lxa_graphics_view*                      m_graphics_view;
    QList<QList<CSignature*>*>              m_signature_lattice;
    QList<QPair<CSignature*,CSignature*>*>  m_signature_containment_edges;
    QMap<CSignature*, int>                  m_map_from_sig_to_column_no; // deprecated, not used.
    QMap<CSignature*,QPair<int, int > >     m_map_from_sig_to_row_and_column;
    QMap<CSignature*,graphic_signature2*>    m_map_from_sig_to_pgraphsig;
    graphic_signature2 *                     m_top_graphic_signature;
    CSignature*                             m_focus_signature_1;
    CSignature*                             m_focus_signature_2;
    CSignatureCollection *                  m_signature_collection;
    Qt::GlobalColor                         m_normal_color;
    Qt::GlobalColor                         m_focus_color;
    eGraphicsStatus                         m_graphics_status;
    int                                     m_row_delta;
    int                                     m_column_delta;
    int                                     m_location_of_bottom_row;
    int                                     m_signature_radius;
    int                                     m_maximum_y;
    int                                     m_bottom_left_x;
    int                                     m_bottom_left_y;
private:
    void                                    mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent);
public:
                    ~lxa_graphics_scene();
                    lxa_graphics_scene(MainWindow *);
    void            add_signature_containment_edge (QPair<CSignature*, CSignature*>* pPair)
                                           {m_signature_containment_edges.append (pPair); }
    void            assign_scene_positions_to_signatures(CSignatureCollection*, eDisplayType );
    eGraphicsStatus change_graphics_status();
    void            clear();
    void            clear_all();
    void            display_focus_signature();
    graphic_signature2* get_focus_signature_1();
    void            move_rows_apart();
    void            move_rows_closer();
    void            mouseMoveEvent(QGraphicsSceneMouseEvent * event);
    void            narrow_columns();
    void            place_containment_edges();
    void            place_signatures();
    void            set_focus_signature();
    void            set_focus_signature_1(CSignature* pSig)       {m_focus_signature_1 = pSig;}
    void            set_focus_signature_2(CSignature* pSig)       {m_focus_signature_2 = pSig;}
    void            set_graphics_view (lxa_graphics_view* );
    void            update_signature_focus();
    void            widen_columns();

};



#endif // GRAPHICS_H
