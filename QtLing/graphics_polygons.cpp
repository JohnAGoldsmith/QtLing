#include <QPolygon>
#include <QFont>
#include <QGraphicsScene>
#include "graphics.h"
#include <QPainter>



class CSignature;


bar::bar(CSignature* pSig): graphic_signature2(  pSig){
         set_text(pSig->get_key());
};
bar::~bar()
{

}

//-----------------------------------------------------------//

triangle2::triangle2(CSignature* pSig): graphic_signature2(  pSig){
         set_text(pSig->get_key());
};
triangle2::~triangle2()
{

}
//-----------------------------------------------------------//

square2::square2(CSignature* pSig): graphic_signature2(pSig){
         set_text(pSig->get_key());
};
square2::~square2()
{

}
//-----------------------------------------------------------//
pentagon2::pentagon2(CSignature* pSig): graphic_signature2(pSig){
         set_text(pSig->get_key());
};
pentagon2::~pentagon2()
{

}
//-----------------------------------------------------------//
hexagon2::hexagon2(CSignature* pSig): graphic_signature2(pSig){
         set_text(pSig->get_key());
};
hexagon2::~hexagon2()
{

}
//-----------------------------------------------------------//
septagon2::septagon2(CSignature* pSig): graphic_signature2(pSig){
         set_text(pSig->get_key());
};
septagon2::~septagon2()
{

}
//-----------------------------------------------------------//
octagon2::octagon2(CSignature* pSig): graphic_signature2(pSig){
         set_text(pSig->get_key());
};
octagon2::~octagon2()
{

}
//-----------------------------------------------------------//
nonagon2::nonagon2(CSignature* pSig): graphic_signature2(pSig){
         set_text(pSig->get_key());
};
nonagon2::~nonagon2()
{

}
//-----------------------------------------------------------//
decagon2::decagon2(CSignature* pSig): graphic_signature2(pSig){
         set_text(pSig->get_key());
};
decagon2::~decagon2()
{

}
//-----------------------------------------------------------//
elevenagon2::elevenagon2(CSignature* pSig): graphic_signature2(pSig){
         set_text(pSig->get_key());
};
elevenagon2::~elevenagon2()
{

}
//-----------------------------------------------------------//
twelvagon2::twelvagon2(CSignature* pSig): graphic_signature2(pSig){
         set_text(pSig->get_key());
};
twelvagon2::~twelvagon2()
{

}

// --------------------------------->      <--------------------------------------------//
void bar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                      QWidget *widget){

    QBrush brush(m_color, Qt::SolidPattern);
    QPen pen (Qt::darkBlue, 3);
    painter->setBrush(brush);
    painter->setPen(pen);
    static const QPointF points[4] = {
        QPointF(20,0),
        QPointF(60,0),
        QPointF(60,20),
        QPointF(20,20)
    };
    painter->drawPolygon(points,4);
}
void bar::set_text(sigstring_t this_sigstring){
    QList<QRectF*> br_list;
    QList<QGraphicsSimpleTextItem*> text_item_list;
    QStringList affixes = this_sigstring.split("=");
    QFont serifFont ("Times", 16, QFont::Bold);

    for (int i=0; i< 2; i++){
        text_item_list.append( new QGraphicsSimpleTextItem(this));
        text_item_list[i]->setText( affixes[i] );
        text_item_list[i]->setFont( serifFont );
        br_list.append(new QRectF(text_item_list[i]->sceneBoundingRect()));
    }
    text_item_list[0]->setPos( 15 - br_list[0]->width()   , 10 - br_list[0]->height()/2   );
    text_item_list[1]->setPos( 65                         , 10 - br_list[1]->height()/2  );
}
QPointF bar::get_center(){
    QPointF this_point = pos();
    QPointF diff (40,10);
    return this_point + diff;
}
// --------------------------------->      <--------------------------------------------//
void triangle2::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                      QWidget *widget){

    QBrush brush(m_color, Qt::SolidPattern);
    QPen pen (Qt::darkBlue, 3);
    painter->setBrush(brush);
    painter->setPen(pen);
    static const QPointF points[3] = {
        QPointF(10,60),
        QPointF(50,60),
        QPointF(30,20)
    };
    painter->drawPolygon(points,3);
}
void triangle2::set_text(sigstring_t this_sigstring){
    QList<QRectF*> br_list;
    QList<QGraphicsSimpleTextItem*> text_item_list;
    QStringList affixes = this_sigstring.split("=");
    QFont serifFont ("Times", 16, QFont::Bold);

    for (int i=0; i< 3; i++){
        text_item_list.append( new QGraphicsSimpleTextItem(this));
        text_item_list[i]->setText( affixes[i] );
        text_item_list[i]->setFont( serifFont );
        br_list.append(new QRectF(text_item_list[i]->sceneBoundingRect()));
    }
    text_item_list[2]->setPos( 3 - br_list[2]->width()   ,  60 -  br_list[2]->height()/2      );
    text_item_list[0]->setPos( 35 - br_list[0]->width()/2   , 0 -  br_list[0]->height()/2 );
    text_item_list[1]->setPos( 57                         ,  60 -  br_list[1]->height()/2  );
}
QPointF triangle2::get_center(){
    QPointF this_point = pos();
    QPointF diff (30,40);
    return this_point + diff;
}
// --------------------------------->      <--------------------------------------------//

void square2::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                      QWidget *widget) {


    QBrush brush(m_color, Qt::SolidPattern);
    QPen pen (Qt::blue, 3);
    painter->setBrush(brush);
    painter->setPen(pen);
    QFont serifFont ("Times", 16, QFont::Bold);

    static const QPointF points[4] = {
        QPointF(20,50),
        QPointF(60,50),
        QPointF(60,10),
        QPointF(20, 10)
    };
    painter->drawPolygon(points,4);

}
void square2::set_text(sigstring_t this_sigstring){
    QList<QRectF*> br_list;
    QList<QGraphicsSimpleTextItem*> text_item_list;
    QStringList affixes = this_sigstring.split("=");
    QFont serifFont ("Times", 16, QFont::Bold);

    for (int i=0; i< 4; i++){
        text_item_list.append( new QGraphicsSimpleTextItem(this));
        text_item_list[i]->setFont( serifFont );
        text_item_list[i]->setText( affixes[i] );
        br_list.append(new QRectF(text_item_list[i]->sceneBoundingRect()));
    }
    text_item_list[0]->setPos( 10 - br_list[0]->width(), 0  - br_list[0]->height()/2 );
    text_item_list[1]->setPos( 70                      , 0  - br_list[1]->height()/2 );
    text_item_list[2]->setPos( 70                      , 50 - br_list[2]->height()/2 );
    text_item_list[3]->setPos( 10 - br_list[3]->width(), 50 - br_list[3]->height()/2 );

}
QPointF square2::get_center(){
    QPointF this_point = pos();
    QPointF diff (40,30);
    return this_point + diff;
}

// --------------------------------->      <--------------------------------------------//


void pentagon2::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                      QWidget *widget){
    QBrush brush(m_color, Qt::SolidPattern);
    QPen pen (Qt::darkBlue, 3);
    painter->setBrush(brush);
    painter->setPen(pen);
    static const QPointF points[5] = {
        QPointF(40,10),
        QPointF(11,30),
        QPointF(22,64),
        QPointF(58,64),
        QPointF(69,30)
    };
    painter->drawPolygon(points,5);
}

void pentagon2::set_text(sigstring_t this_sigstring){
    QList<QRectF*> br_list;
    QList<QGraphicsSimpleTextItem*> text_item_list;
    QStringList affixes = this_sigstring.split("=");
    QFont serifFont ("Times", 16, QFont::Bold);

        for (int i=0; i< 5; i++){
        text_item_list.append( new QGraphicsSimpleTextItem(this));
        text_item_list[i]->setText( affixes[i] );
        text_item_list[i]->setFont( serifFont );
        br_list.append(new QRectF(text_item_list[i]->sceneBoundingRect()));
    }
    text_item_list[0]->setPos( 43 - br_list[0]->width()/2  , 10 - br_list[0]->height()       );
    text_item_list[4]->setPos( 5 - br_list[4]->width()  , 28 - br_list[1]->height()/2 );
    text_item_list[3]->setPos( 10 - br_list[3]->width()  , 64 - br_list[3]->height()/2  );
    text_item_list[2]->setPos(  62                        , 64 - br_list[2]->height()/2 );
    text_item_list[1]->setPos(  72                        ,  31 - br_list[1]->height()/2 );
}
QPointF pentagon2::get_center(){
    QPointF this_point = pos();
    QPointF diff (50,37);
    return this_point + diff;
}

// --------------------------------->      <--------------------------------------------//
void hexagon2::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                      QWidget *widget){
    QBrush brush(m_color, Qt::SolidPattern);
    QPen pen (Qt::darkBlue, 3);
    painter->setBrush(brush);
    painter->setPen(pen);
    static const QPointF points[3] = {
        QPointF(10,55),
        QPointF(50,55),
        QPointF(30,35)
    };
    painter->drawPolygon(points,3);
    static const QPointF points_2[3] = {
        QPointF(10,65),
        QPointF(50,65),
        QPointF(30,85)
    };
    painter->drawPolygon(points_2,3);
}

void hexagon2::set_text(sigstring_t this_sigstring){
    QList<QRectF*> br_list;
    QList<QGraphicsSimpleTextItem*> text_item_list;
    QStringList affixes = this_sigstring.split("=");
    QFont serifFont ("Times", 16, QFont::Bold);

    for (int i=0; i< 6; i++){
        text_item_list.append( new QGraphicsSimpleTextItem(this));
        text_item_list[i]->setFont( serifFont );
        text_item_list[i]->setText( affixes[i] );
        br_list.append(new QRectF(text_item_list[i]->sceneBoundingRect()));
    }
    text_item_list[0]->setPos(  32 - br_list[0]->width()/2   , 0      );
    text_item_list[1]->setPos(  60                           , 40  );
    text_item_list[2]->setPos(  60                           , 60   );
    text_item_list[3]->setPos(  35 - br_list[3]->width()/2   , 85   );
    text_item_list[4]->setPos(  5  - br_list[4]->width()     , 60   );
    text_item_list[5]->setPos(  5  - br_list[5]->width()    , 40  );

}
QPointF hexagon2::get_center(){
    QPointF this_point = pos();
    QPointF diff (30,60);
    return this_point + diff;
}


// --------------------------------->      <--------------------------------------------//
void septagon2::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                      QWidget *widget){
    QBrush brush(m_color, Qt::SolidPattern);
    QPen pen (Qt::darkBlue, 3);
    painter->setBrush(brush);
    painter->setPen(pen);
    static const QPointF points[4] = {
        QPointF(0,100),
        QPointF(40,100),
        QPointF(40,60),
        QPointF(0,60)
    };
    painter->drawPolygon(points,4);
    static const QPointF points_2[3] = {
        QPointF(0,50),
        QPointF(40,50),
        QPointF(20,25)
    };
    painter->drawPolygon(points_2,3);

}


void septagon2::set_text(sigstring_t this_sigstring){
    QList<QRectF*> br_list;
    QList<QGraphicsSimpleTextItem*> text_item_list;
    QStringList affixes = this_sigstring.split("=");
    QFont serifFont ("Times", 16, QFont::Bold);

    for (int i=0; i< 7; i++){
        text_item_list.append( new QGraphicsSimpleTextItem(this));
        text_item_list[i]->setFont( serifFont );
        text_item_list[i]->setText( affixes[i] );
        br_list.append(new QRectF(text_item_list[i]->sceneBoundingRect()));
    }

    text_item_list[0]->setPos( 20 - br_list[0]->width()/2 ,  10 - br_list[0]->height()/2 );
    text_item_list[1]->setPos( 50                         ,  45 - br_list[1]->height()/2  );

    text_item_list[2]->setPos( 50                         , 70 -  br_list[2]->height()/2 );
    text_item_list[3]->setPos( 50                         ,  90 -  br_list[3]->height()/2 );
    text_item_list[4]->setPos( -5 - br_list[4]->width()    ,  100 -  br_list[4]->height()/2 );
    text_item_list[5]->setPos( -5 - br_list[5]->width()    , 70 -  br_list[5]->height()/2       );

    text_item_list[6]->setPos( -5 - br_list[6]->width()    ,  45 -  br_list[6]->height()/2  );


}
QPointF septagon2::get_center(){
    QPointF this_point = pos();
    QPointF diff (30,50);
    return this_point + diff;
}
// --------------------------------->      <--------------------------------------------//
// ==================================      =============================================//

void octagon2::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                      QWidget *widget){
   QBrush brush(m_color, Qt::SolidPattern);
    QPen pen (Qt::darkBlue, 3);
    painter->setBrush(brush);
    painter->setPen(pen);
    static const QPointF points[4] = {
        QPointF(20,70),
        QPointF(60,70),
        QPointF(60,30),
        QPointF(20,30)
    };
    painter->drawPolygon(points,4);
    static const QPointF points_2[4] = {
        QPointF( 20, 120),
        QPointF(60, 120),
        QPointF(60, 80),
        QPointF( 20, 80)
    };
    painter->drawPolygon(points_2,4);

}
void octagon2::set_text(sigstring_t this_sigstring){
    QList<QRectF*> br_list;
    QList<QGraphicsSimpleTextItem*> text_item_list;
    QStringList affixes = this_sigstring.split("=");
    QFont serifFont ("Times", 16, QFont::Bold);

    for (int i=0; i< 8; i++){
        text_item_list.append( new QGraphicsSimpleTextItem(this));
        text_item_list[i]->setFont( serifFont );
        text_item_list[i]->setText( affixes[i] );
        br_list.append(new QRectF(text_item_list[i]->sceneBoundingRect()));
    }
    text_item_list[0]->setPos( 10 - br_list[0]->width()  , 20 - br_list[0]->height()/2 );
    text_item_list[1]->setPos(  70                        , 20 - br_list[1]->height()/2  );
    text_item_list[2]->setPos(  70                        ,  60 - br_list[2]->height()/2 );
    text_item_list[3]->setPos(  70                        ,  80 - br_list[3]->height()/2  );
    text_item_list[4]->setPos(  70                        ,  120 - br_list[4]->height()/2 );
    text_item_list[5]->setPos( 10 - br_list[5]->width()  ,  120 - br_list[5]->height()/2 );
    text_item_list[6]->setPos( 10 - br_list[6]->width()  ,  80 - br_list[6]->height()/2 );
    text_item_list[7]->setPos( 10 - br_list[7]->width()  ,  60 - br_list[7]->height()/2 );



}
QPointF octagon2::get_center(){
    QPointF this_point = pos();
    QPointF diff (55,35);
    return this_point + diff;
}
// --------------------------------->      <--------------------------------------------//
// ==================================      =============================================//

void nonagon2::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                      QWidget *widget){

   QBrush brush(m_color, Qt::SolidPattern);
    QPen pen (Qt::darkBlue, 3);
    painter->setBrush(brush);
    painter->setPen(pen);
    int x = 60;
    int y = 0;
    static const QPointF points[3] = {
        QPointF(x + 0 , y + 30),
        QPointF(x + 40, y + 30),
        QPointF(x + 20, y - 10)
    };
    painter->drawPolygon(points,3);
    x = 0;
    y = 90;
    static const QPointF points_2[3] = {
        QPointF(x + 0 , y + 30),
        QPointF(x + 40, y + 30),
        QPointF(x + 20, y - 10)
    };
    painter->drawPolygon(points_2,3);
    x = 120;
    static const QPointF points_3[3] = {
        QPointF(x + 0 , y + 30),
        QPointF(x + 40, y + 30),
        QPointF(x + 20, y - 10)
    };
    painter->drawPolygon(points_3,3);

}
void nonagon2::set_text(sigstring_t this_sigstring){

    QList<QRectF*> br_list;
    QList<QGraphicsSimpleTextItem*> text_item_list;
    QStringList affixes = this_sigstring.split("=");
    QFont serifFont ("Times", 16, QFont::Bold);

    for (int i=0; i< 9; i++){
        text_item_list.append( new QGraphicsSimpleTextItem(this));
        text_item_list[i]->setFont( serifFont );
        text_item_list[i]->setText( affixes[i] );
        br_list.append(new QRectF(text_item_list[i]->sceneBoundingRect()));
    }
    int x = 60;
    int y = 0;
    text_item_list[0]->setPos(x + 20 - br_list[0]->width()/2, y - 30 -  br_list[0]->height()/2 );
    text_item_list[1]->setPos(x + 47                      , y + 30 -  br_list[1]->height()/2 );
    text_item_list[2]->setPos(x -  7 - br_list[2]->width(), y + 30 -  br_list[2]->height()/2 );

    x = 0;
    y = 90;
    text_item_list[3]->setPos(x + 20 - br_list[3]->width()/2, y - 30 -  br_list[3]->height()/2 );
    text_item_list[4]->setPos(x + 47                        , y + 30 -  br_list[4]->height()/2 );
    text_item_list[5]->setPos(x -  7 - br_list[5]->width()  , y + 30 -  br_list[5]->height()/2 );

    x = 120;
    y = 90;
    text_item_list[6]->setPos(x + 20 - br_list[6]->width()/2, y - 30 -  br_list[6]->height()/2 );
    text_item_list[7]->setPos(x + 47                      , y + 30 -  br_list[7]->height()/2 );
    text_item_list[8]->setPos(x -  7 - br_list[8]->width(), y + 30 -  br_list[8]->height()/2 );

}
QPointF nonagon2::get_center(){
    QPointF this_point = pos();
    QPointF diff (55,35);
    return this_point + diff;
}
// --------------------------------->      <--------------------------------------------//
// ==================================      =============================================//
void decagon2::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                      QWidget *widget){
    QBrush brush(m_color, Qt::SolidPattern);
    QPen pen (Qt::darkBlue, 3);
    painter->setBrush(brush);
    painter->setPen(pen);
    static const QPointF points[5] = {
        QPointF(40,10),
        QPointF(11,30),
        QPointF(22,64),
        QPointF(58,64),
        QPointF(69,30)
    };
    int delta = 80;
    static const QPointF points2[5] = {
        QPointF(40,10+delta),
        QPointF(11,30+delta),
        QPointF(22,64+delta),
        QPointF(58,64+delta),
        QPointF(69,30+delta)
    };
    painter->drawPolygon(points,5);
    painter->drawPolygon(points2,5);
}

void decagon2::set_text(sigstring_t this_sigstring){
    QList<QRectF*> br_list;
    QList<QGraphicsSimpleTextItem*> text_item_list;
    QStringList affixes = this_sigstring.split("=");
    QFont serifFont ("Times", 16, QFont::Bold);
    int delta = 80;

    for (int i=0; i< 10; i++){
        text_item_list.append( new QGraphicsSimpleTextItem(this));
        text_item_list[i]->setText( affixes[i] );
        text_item_list[i]->setFont( serifFont );
        br_list.append(new QRectF(text_item_list[i]->sceneBoundingRect()));
    }
    text_item_list[0]->setPos( 43 - br_list[0]->width()/2, 10 - br_list[0]->height()   );
    text_item_list[4]->setPos( 5 - br_list[4]->width()   , 28 - br_list[1]->height()/2 );
    text_item_list[3]->setPos( 10 - br_list[3]->width()  , 64 - br_list[3]->height()/2 );
    text_item_list[2]->setPos( 62                        , 64 - br_list[2]->height()/2 );
    text_item_list[1]->setPos( 72                        , 31 - br_list[1]->height()/2 );

    text_item_list[5]->setPos( 43 - br_list[5]->width()/2, 10 + delta - br_list[5]->height()   );
    text_item_list[9]->setPos( 5 - br_list[9]->width()   , 28 + delta - br_list[9]->height()/2 );
    text_item_list[8]->setPos( 10 - br_list[8]->width()  , 64 + delta - br_list[8]->height()/2 );
    text_item_list[7]->setPos( 62                        , 64 + delta - br_list[7]->height()/2 );
    text_item_list[6]->setPos( 72                        , 31 + delta - br_list[6]->height()/2 );


}
QPointF decagon2::get_center(){
    QPointF this_point = pos();
    QPointF diff (50,37);
    return this_point + diff;
}
// --------------------------------->      <--------------------------------------------//
// ==================================      =============================================//

void elevenagon2::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                      QWidget *widget){
    QBrush brush(m_color, Qt::SolidPattern);
     QPen pen (Qt::darkBlue, 3);
     painter->setBrush(brush);
     painter->setPen(pen);

     int delta_4 = 40;
     static const QPointF points[4] = {
         QPointF(20,70 + delta_4),
         QPointF(60,70 + delta_4),
         QPointF(60,30 + delta_4),
         QPointF(20,30 + delta_4)
     };
     int delta = 0;
     painter->drawPolygon(points,4);
     static const QPointF points_2[4] = {
         QPointF(20 + delta, 120 + delta_4),
         QPointF(60 + delta, 120 + delta_4),
         QPointF(60 + delta,  80 + delta_4),
         QPointF(20 + delta,  80 + delta_4)
     };
     painter->drawPolygon(points_2,4);

     int delta_2 = -50;
     int delta_3 = 0;
     static const QPointF points_3[3] = {
         QPointF(20 + delta_3,60 + delta_2 + delta_4),
         QPointF(60 + delta_3,60 + delta_2 + delta_4),
         QPointF(40 + delta_3,20 + delta_2 + delta_4)
     };
     painter->drawPolygon(points_3,3);



};

void elevenagon2::set_text(sigstring_t this_sigstring)
{
    QList<QRectF*> br_list;
    QList<QGraphicsSimpleTextItem*> text_item_list;
    QStringList affixes = this_sigstring.split("=");
    QFont serifFont ("Times", 16, QFont::Bold);

    for (int i=0; i< 11; i++){
        text_item_list.append( new QGraphicsSimpleTextItem(this));
        text_item_list[i]->setFont( serifFont );
        text_item_list[i]->setText( affixes[i] );
        br_list.append(new QRectF(text_item_list[i]->sceneBoundingRect()));
    }
    int delta = 40;
    text_item_list[0]->setPos( 40 - br_list[0]->width()/2,-40 - br_list[0]->height()/2 + delta );
    text_item_list[1]->setPos( 70                      ,   0 - br_list[1]->height()/2 + delta );
    text_item_list[2]->setPos( 70                      ,  30 - br_list[2]->height()/2 + delta  );
    text_item_list[3]->setPos( 70                      ,  60 - br_list[3]->height()/2 + delta  );
    text_item_list[4]->setPos( 70                      ,  80 - br_list[4]->height()/2 + delta  );
    text_item_list[5]->setPos( 70                      , 110 - br_list[5]->height()/2 + delta );
    text_item_list[6]->setPos( 10 - br_list[6]->width(), 110 - br_list[6]->height()/2 + delta );
    text_item_list[7]->setPos( 10 - br_list[7]->width(),  80 - br_list[7]->height()/2 + delta );
    text_item_list[8]->setPos( 10 - br_list[8]->width(),  60 - br_list[8]->height()/2 + delta );
    text_item_list[9]->setPos( 10 - br_list[9]->width(),  30 - br_list[9]->height()/2 + delta );
    text_item_list[10]->setPos(10 - br_list[10]->width(),  0 - br_list[10]->height()/2 + delta );



    ///////////////////////////

//    QGraphicsSimpleTextItem *p_text_item = new QGraphicsSimpleTextItem;
//    p_text_item     ->  setText(QString::number(pSig->get_number_of_stems()));/
//    QRectF bR       =   p_text_item->sceneBoundingRect();
//    p_text_item     ->  setPos (x+38 - bR.width()/2, y-10 + 65 );
//    scene->addItem(p_text_item);
}

// --------------------------------->      <--------------------------------------------//
// ==================================      =============================================//

void twelvagon2::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                      QWidget *widget){
    QBrush brush(m_color, Qt::SolidPattern);
     QPen pen (Qt::darkBlue, 3);
     painter->setBrush(brush);
     painter->setPen(pen);

     int delta_4 = 40;
     static const QPointF points[4] = {
         QPointF(20,70 + delta_4),
         QPointF(60,70 + delta_4),
         QPointF(60,30 + delta_4),
         QPointF(20,30 + delta_4)
     };
     int delta = 0;
     painter->drawPolygon(points,4);
     static const QPointF points_2[4] = {
         QPointF(20 + delta, 120 + delta_4),
         QPointF(60 + delta, 120 + delta_4),
         QPointF(60 + delta,  80 + delta_4),
         QPointF(20 + delta,  80 + delta_4)
     };
     painter->drawPolygon(points_2,4);

     delta_4 = -60;
     static const QPointF points_3[4] = {
         QPointF(20 + delta, 120 + delta_4),
         QPointF(60 + delta, 120 + delta_4),
         QPointF(60 + delta,  80 + delta_4),
         QPointF(20 + delta,  80 + delta_4)
     };

      painter->drawPolygon(points_3,4);



};

void twelvagon2::set_text(sigstring_t this_sigstring)
{
    QList<QRectF*> br_list;
    QList<QGraphicsSimpleTextItem*> text_item_list;
    QStringList affixes = this_sigstring.split("=");
    QFont serifFont ("Times", 16, QFont::Bold);

    for (int i=0; i< 12; i++){
        text_item_list.append( new QGraphicsSimpleTextItem(this));
        text_item_list[i]->setFont( serifFont );
        text_item_list[i]->setText( affixes[i] );
        br_list.append(new QRectF(text_item_list[i]->sceneBoundingRect()));
    }
    int delta = 40;
    text_item_list[0]->setPos( 70                      , -15 - br_list[0]->height()/2 + delta );
    text_item_list[1]->setPos( 70                      ,  10 - br_list[1]->height()/2 + delta );
    text_item_list[2]->setPos( 70                      ,  40 - br_list[2]->height()/2 + delta  );
    text_item_list[3]->setPos( 70                      ,  60 - br_list[3]->height()/2 + delta  );
    text_item_list[4]->setPos( 70                      ,  90 - br_list[4]->height()/2 + delta  );
    text_item_list[5]->setPos( 70                      , 110 - br_list[5]->height()/2 + delta );
    text_item_list[6]->setPos( 10 - br_list[6]->width(), 110 - br_list[6]->height()/2 + delta );
    text_item_list[7]->setPos( 10 - br_list[7]->width(),  90 - br_list[7]->height()/2 + delta );
    text_item_list[8]->setPos( 10 - br_list[8]->width(),  60 - br_list[8]->height()/2 + delta );
    text_item_list[9]->setPos( 10 - br_list[9]->width(),  40 - br_list[9]->height()/2 + delta );
    text_item_list[10]->setPos(10 - br_list[10]->width(), 10 - br_list[10]->height()/2 + delta );
    text_item_list[11]->setPos(10 - br_list[11]->width(),-15- br_list[11]->height()/2 + delta );



    ///////////////////////////

//    QGraphicsSimpleTextItem *p_text_item = new QGraphicsSimpleTextItem;
//    p_text_item     ->  setText(QString::number(pSig->get_number_of_stems()));/
//    QRectF bR       =   p_text_item->sceneBoundingRect();
//    p_text_item     ->  setPos (x+38 - bR.width()/2, y-10 + 65 );
//    scene->addItem(p_text_item);
}



QPointF elevenagon2::get_center(){
        QPointF this_point = pos();
        QPointF diff (50,37);
        return this_point + diff;
}


QPointF twelvagon2::get_center(){
        QPointF this_point = pos();
        QPointF diff (50,37);
        return this_point + diff;
}


/*
void triangle(CSignature* pSig, int x, int y, lxa_graphics_scene * scene,   QColor color)
{
    QPolygon triangle;
    triangle.append(QPoint(x,y+30));
    triangle.append(QPoint(x+40,y+30));
    triangle.append(QPoint(x+20,y-10));
    triangle.append(QPoint(x,y+28));
    QGraphicsPolygonItem * pTriangleItem = scene->addPolygon(triangle,QPen(), QBrush(color));
    QGraphicsSimpleTextItem * p_text_item1 = new QGraphicsSimpleTextItem;
    QGraphicsSimpleTextItem * p_text_item2 = new QGraphicsSimpleTextItem;
    QGraphicsSimpleTextItem * p_text_item3 = new QGraphicsSimpleTextItem;
    QStringList affixes = pSig->get_key().split("=");
    p_text_item1->setText(affixes[0]);
    p_text_item2->setText(affixes[1]);
    p_text_item3->setText(affixes[2]);
    QRectF bR1 = p_text_item1->sceneBoundingRect();
    QRectF bR2 = p_text_item2->sceneBoundingRect();
    QRectF bR3 = p_text_item3->sceneBoundingRect();
    p_text_item1->setPos( x-7 - bR1.width()   , y+30 - bR1.height()/2 );
    p_text_item2->setPos( x+25 - bR2.width()/2, y-30 - bR2.height()/2 );
    p_text_item3->setPos( x+47                , y+30 - bR3.height()/2 );
    scene->addItem(p_text_item1);
    scene->addItem(p_text_item2);
    scene->addItem(p_text_item3);
    QGraphicsSimpleTextItem * p_text_item = new QGraphicsSimpleTextItem;
    p_text_item->setText(QString::number(pSig->get_number_of_stems()));
    p_text_item->setPos (x + 5 ,y-8 + 65);
    scene->addItem(p_text_item);

}
*/
