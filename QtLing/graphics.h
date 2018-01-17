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
#include "Lexicon.h"

class CSignature;
class CSignatureCollection;
class MainWindow;
class lxa_graphics_scene;
class CSupersignature;
class CHypothesis;


/////////////////////////////////////////////////////////////////////////////
//          Graphic signature
//          the base class for specific shapes
/////////////////////////////////////////////////////////////////////////////
class graphic_signature2 : public QGraphicsItem
{
    lxa_graphics_scene * m_graphics_scene;

protected:
    CSignature *         m_signature;
protected:
    bool                 m_focus_flag;
    Qt::GlobalColor      m_color;
//    Qt::GlobalColor     m_out_of_focus_color;

    public:
    graphic_signature2  ();
    graphic_signature2  (CSignature* );
    graphic_signature2   (CSignature*,   QColor, bool focus_flag = false);
    virtual QPointF             get_center() {};
    ~graphic_signature2 () {};
    QRectF              boundingRect() const
    {
        return QRectF(0, 0,130,150);
    }
    CSignature*         get_signature() {return m_signature;}
    void                set_color(Qt::GlobalColor);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget)
    {
    }
};
//--------------------->       <-----------------------------//
class bar: public graphic_signature2
{
public:
    bar(CSignature*);
    ~bar();
    void paint ();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    void set_text(sigstring_t this_sigstring);
    QPointF get_center();

};

//--------------------->       <-----------------------------//
class triangle2 : public graphic_signature2
{
public:
    triangle2(CSignature*);
    ~triangle2();
    void paint ();
    QPointF             get_center();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    void set_text(sigstring_t this_sigstring);

};
//--------------------->       <-----------------------------//
class square2 : public graphic_signature2
{
public:
    //square2();
    square2(CSignature*);
    ~square2();
    void paint ();
    QPointF             get_center();
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
    pentagon2(CSignature*);
    ~pentagon2();
    void paint ();
    QPointF             get_center();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    void set_text(sigstring_t this_sigstring);
};
//--------------------->       <-----------------------------//
class hexagon2 : public graphic_signature2
{
public:
    hexagon2(CSignature*);
    ~hexagon2();
    void paint ();
    QPointF             get_center();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    void set_text(sigstring_t this_sigstring);
};
//--------------------->       <-----------------------------//
class septagon2 : public graphic_signature2
{
public:
    septagon2(CSignature*);
    ~septagon2();
    void paint ();
    QPointF             get_center();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    void set_text(sigstring_t this_sigstring);
};
//--------------------->       <-----------------------------//
class octagon2 : public graphic_signature2
{
public:
    octagon2(CSignature*);
    ~octagon2();
    void paint ();
    QPointF             get_center();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    void set_text(sigstring_t this_sigstring);
};
//--------------------->       <-----------------------------//
class nonagon2 : public graphic_signature2
{
public:
    nonagon2(CSignature*);
    ~nonagon2();
    void paint ();
    QPointF             get_center();
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
    void                            reset_scale();


};

/////////////////////////////////////////////////////////////////////////////
//
//          lxa graphics scene
//
/////////////////////////////////////////////////////////////////////////////

class lxa_graphics_scene : public QGraphicsScene
{

    Q_OBJECT


    friend                                  lxa_graphics_view;

    MainWindow*                             m_main_window;
    CLexicon*                               m_lexicon;
    bool                                    m_suffix_flag;
    CSuffixCollection *                     m_suffixes;
    CPrefixCollection *                     m_prefixes;
    eDataType                               m_data_type;
    lxa_graphics_view*                      m_graphics_view;

    // the signature-lattice is a set of lists, one for each set of signatures with the same number of affixes.
    // There is a natural order of them, provided by the sorting function.
    // However, sometimes we want to shift some signatures to the left-end, so that they are visible.
    // For that reason, we keep a pointer to the right-end of the set of signatures which have
    // been permuted to the beginning: this is a 0-based index of that sig in the list.
    QList<QList<CSignature*>*>              m_signature_lattice;
    QVector<int>                            m_insertion_point_in_signature_lattice;

    QList<QPair<CSignature*,CSignature*>*>  m_signature_containment_edges;
//    QMap<CSignature*, int>                  m_map_from_sig_to_column_no; // deprecated, not used.
    QMap<CSignature*,QPair<int, int > >     m_map_from_sig_to_row_and_column;
    QMap<CSignature*,graphic_signature2*>    m_map_from_sig_to_pgraphsig;
    graphic_signature2 *                     m_top_graphic_signature;
    CSignature*                             m_focus_signature_1;
    CSignature*                             m_focus_signature_2;
    CSignatureCollection *                  m_signature_collection;
    Qt::GlobalColor                         m_normal_color;
    Qt::GlobalColor                         m_focus_color;
    Qt::GlobalColor                         m_out_of_focus_color;
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


public slots:
    void                implement_hypothesis(const QModelIndex & );


public:
                        ~lxa_graphics_scene();
                        lxa_graphics_scene(MainWindow *, CLexicon* );
    void                ingest( CLexicon*, CSignatureCollection*, bool suffix_flag);
    void                add_signature_containment_edge (QPair<CSignature*, CSignature*>* pPair)
                                           {m_signature_containment_edges.append (pPair); }
    void                assign_scene_positions_to_signatures(CSignatureCollection*, eDataType );
    eGraphicsStatus     change_graphics_status();
    void                clear();
    void                clear_all();
    void                display_focus_signature();
    graphic_signature2* get_focus_signature_1();
    CLexicon*           get_lexicon()           {return m_lexicon;}
    void                move_rows_apart();
    void                move_rows_closer();
    void                move_signature_to_the_left(CSignature*);
    void                mouseMoveEvent(QGraphicsSceneMouseEvent * event);
    void                narrow_columns();
    void                place_containment_edges();
    void                place_signatures();
    void                place_arrow(QPointF, QPointF);
    void                set_focus_signature(CSignature* );
    void                set_focus_signature_and_move(CSignature* );
    void                set_focus_signature_1(CSignature* pSig)       {m_focus_signature_1 = pSig;}
    void                set_focus_signature_2(CSignature* pSig)       {m_focus_signature_2 = pSig;}
    void                set_graphics_view (lxa_graphics_view* );
    void                show_hypothesis_1(CHypothesis*);
    void                set_column_delta (double s)                 {m_column_delta = s;}
    void                set_row_delta(double s)                     {m_row_delta = s;}
    void                show_subsignatures() ;
    void                show_subsignatures_and_move_them() ;
    void                update_signature_focus();
    void                widen_columns();

};



#endif // GRAPHICS_H
