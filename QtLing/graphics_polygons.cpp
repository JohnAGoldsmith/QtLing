#include <QPolygon>
#include <QFont>
#include <QGraphicsScene>
#include "graphics.h"
#include <QPainter>



class CSignature;

triangle2::triangle2(CSignature* pSig): graphic_signature2(  pSig){
         set_text(pSig->get_key());
};

//-----------------------------------------------------------//
square2::~square2()
{

}
square2::square2(CSignature* pSig): graphic_signature2(pSig){
         set_text(pSig->get_key());
};
//-----------------------------------------------------------//
pentagon2::pentagon2(CSignature* pSig): graphic_signature2(pSig){
         set_text(pSig->get_key());
};
//-----------------------------------------------------------//
hexagon2::hexagon2(CSignature* pSig): graphic_signature2(pSig){
         set_text(pSig->get_key());
};
//-----------------------------------------------------------//
septagon2::septagon2(CSignature* pSig): graphic_signature2(pSig){
         set_text(pSig->get_key());
};
//-----------------------------------------------------------//
octagon2::octagon2(CSignature* pSig): graphic_signature2(pSig){
         set_text(pSig->get_key());
};
//-----------------------------------------------------------//
nonagon2::nonagon2(CSignature* pSig): graphic_signature2(){
         set_text(pSig->get_key());
};
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

// --------------------------------->      <--------------------------------------------//

void square2::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                      QWidget *widget) {
    qDebug() << 400;

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
    text_item_list[0]->setPos( 10 - br_list[0]->width()  ,  0  - br_list[0]->height()/2 );
    text_item_list[1]->setPos(  70                       ,  0  - br_list[1]->height()/2  );
    text_item_list[2]->setPos(  70                       ,  50 - br_list[2]->height()/2 );
    text_item_list[3]->setPos( 10 - br_list[3]->width()  ,  50 - br_list[3]->height()/2       );

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

void nonagon2::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                      QWidget *widget){

   QBrush brush(m_color, Qt::SolidPattern);
    QPen pen (Qt::darkBlue, 3);
    painter->setBrush(brush);
    painter->setPen(pen);
    int x = 50;
    int y = 0;
    static const QPointF points[3] = {
        QPointF(x + 0 , y + 30),
        QPointF(x + 40, y + 30),
        QPointF(x + 20, y - 10)
    };
    painter->drawPolygon(points,3);
    x = 0;
    y = 50;
    static const QPointF points_2[3] = {
        QPointF(x + 0 , y + 30),
        QPointF(x + 40, y + 30),
        QPointF(x + 20, y - 10)
    };
    painter->drawPolygon(points_2,3);
    x = 50;
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
    int x = 0;
    int y = 0;
    text_item_list[0]->setPos(x -7 - br_list[0]->width()  , y + 30 -  br_list[1]->height()/2 );
    text_item_list[1]->setPos(x + 25 - br_list[1]->width(), y - 30 -  br_list[1]->height()/2 );
    text_item_list[2]->setPos(x +  47                     , y + 30 -  br_list[1]->height()/2 );

}

void nonagon(CSignature* pSig, int x, int y, lxa_graphics_scene * scene, QColor color)
{

    x -= 30;
    y += 15;
    QPolygon triangle;
    triangle.append(QPoint(x,y+30));
    triangle.append(QPoint(x+40,y+30));
    triangle.append(QPoint(x+20,y-10));
    triangle.append(QPoint(x,y+28));
    QGraphicsPolygonItem * pTriangleItem = scene->addPolygon(triangle,QPen(), QBrush(Qt::darkRed));
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
    p_text_item2->setPos( x+25 - bR2.width()/2, y-30   );
    p_text_item3->setPos( x+47                , y+30 - bR3.height()/2 );

    scene->addItem(p_text_item1);
    scene->addItem(p_text_item2);
    scene->addItem(p_text_item3);
////////////
    x += 110;
    QPolygon triangle2;
    triangle2.append(QPoint(x,y+30));
    triangle2.append(QPoint(x+40,y+30));
    triangle2.append(QPoint(x+20,y-10));
    triangle2.append(QPoint(x,y+28));
    QGraphicsPolygonItem * pTriangleItem2 = scene->addPolygon(triangle2,QPen(), QBrush(Qt::darkRed));
    QGraphicsSimpleTextItem * p_text_item4 = new QGraphicsSimpleTextItem;
    QGraphicsSimpleTextItem * p_text_item5 = new QGraphicsSimpleTextItem;
    QGraphicsSimpleTextItem * p_text_item6 = new QGraphicsSimpleTextItem;
    p_text_item4->setText(affixes[3]);
    p_text_item5->setText(affixes[4]);
    p_text_item6->setText(affixes[5]);
    QRectF bR4 = p_text_item4->sceneBoundingRect();
    QRectF bR5 = p_text_item5->sceneBoundingRect();
    QRectF bR6 = p_text_item6->sceneBoundingRect();
    p_text_item4->setPos( x-7 - bR4.width()   , y+30 - bR4.height()/2 );
    p_text_item5->setPos( x+25 - bR5.width()/2, y-30   );
    p_text_item6->setPos( x+47                , y+30 - bR6.height()/2 );

    scene->addItem(p_text_item4);
    scene->addItem(p_text_item5);
    scene->addItem(p_text_item6);
    ////////////
        x -= 60;
        y -= 75;
        QPolygon triangle3;
        triangle3.append(QPoint(x,y+30));
        triangle3.append(QPoint(x+40,y+30));
        triangle3.append(QPoint(x+20,y-10));
        triangle3.append(QPoint(x,y+28));
        QGraphicsPolygonItem * pTriangleItem3 = scene->addPolygon(triangle3,QPen(), QBrush(Qt::darkRed));
        QGraphicsSimpleTextItem * p_text_item7 = new QGraphicsSimpleTextItem;
        QGraphicsSimpleTextItem * p_text_item8 = new QGraphicsSimpleTextItem;
        QGraphicsSimpleTextItem * p_text_item9 = new QGraphicsSimpleTextItem;

        p_text_item7->setText(affixes[6]);
        p_text_item8->setText(affixes[7]);
        p_text_item9->setText(affixes[8]);
        QRectF bR7 = p_text_item1->sceneBoundingRect();
        QRectF bR8 = p_text_item2->sceneBoundingRect();
        QRectF bR9 = p_text_item3->sceneBoundingRect();
        p_text_item7->setPos( x-7 - bR7.width()   , y+30 - bR7.height()/2 );
        p_text_item8->setPos( x+25 - bR8.width()/2, y-30   );
        p_text_item9->setPos( x+47                , y+30 - bR9.height()/2 );

        scene->addItem(p_text_item7);
        scene->addItem(p_text_item8);
        scene->addItem(p_text_item9);



    QGraphicsSimpleTextItem * p_text_item = new QGraphicsSimpleTextItem;
    p_text_item->setText(QString::number(pSig->get_number_of_stems()));
    QRectF bR = p_text_item->sceneBoundingRect();
    p_text_item->setPos (x+38 - bR.width()/2, y-10 + 65 );
    scene->addItem(p_text_item);
}
void decagon(CSignature* pSig, int x, int y,  lxa_graphics_scene * scene, QColor color)
{
    QPolygon pentagon1;
    pentagon1.append(QPoint(x,y-30));
    pentagon1.append(QPoint(x-29,y-12));
    pentagon1.append(QPoint(x-18,y+24));
    pentagon1.append(QPoint(x+18,y+24));
    pentagon1.append(QPoint(x+29,y-9));
    pentagon1.append(QPoint(x,y-30));
    QGraphicsPolygonItem * pPentagonItem1 = scene->addPolygon(pentagon1,QPen(), QBrush(Qt::darkMagenta));
    QGraphicsSimpleTextItem * p_text_item1 = new QGraphicsSimpleTextItem;
    QGraphicsSimpleTextItem * p_text_item2 = new QGraphicsSimpleTextItem;
    QGraphicsSimpleTextItem * p_text_item3 = new QGraphicsSimpleTextItem;
    QGraphicsSimpleTextItem * p_text_item4 = new QGraphicsSimpleTextItem;
    QGraphicsSimpleTextItem * p_text_item5 = new QGraphicsSimpleTextItem;

    QStringList affixes = pSig->get_key().split("=");
    p_text_item1->setText(affixes[0]);
    p_text_item2->setText(affixes[1]);
    p_text_item3->setText(affixes[2]);
    p_text_item4->setText(affixes[3]);
    p_text_item5->setText(affixes[4]);

    QRectF bR1 = p_text_item1->sceneBoundingRect();
    QRectF bR2 = p_text_item2->sceneBoundingRect();
    QRectF bR3 = p_text_item3->sceneBoundingRect();
    QRectF bR4 = p_text_item4->sceneBoundingRect();
    QRectF bR5 = p_text_item5->sceneBoundingRect();
    p_text_item1->setPos( x+3  - bR1.width()/2, y-35 - bR1.height()   );
    p_text_item2->setPos( x-35 - bR2.width()  , y-12 - bR2.height()/2 );
    p_text_item3->setPos( x-22 - bR3.width()  , y+24 - bR3.height()/2 );
    p_text_item4->setPos( x+22                , y+24 - bR3.height()/2 );
    p_text_item5->setPos( x+32                , y-9  - bR3.height()/2 );
    scene->addItem(p_text_item1);
    scene->addItem(p_text_item2);
    scene->addItem(p_text_item3);
    scene->addItem(p_text_item4);
    scene->addItem(p_text_item5);

    x+= 130;
    QPolygon pentagon2;
    pentagon2.append(QPoint(x,y-30));
    pentagon2.append(QPoint(x-29,y-12));
    pentagon2.append(QPoint(x-18,y+24));
    pentagon2.append(QPoint(x+18,y+24));
    pentagon2.append(QPoint(x+29,y-9));
    pentagon2.append(QPoint(x,y-30));
    QGraphicsPolygonItem * pPentagonItem2 = scene->addPolygon(pentagon2,QPen(), QBrush(Qt::darkMagenta));
    QGraphicsSimpleTextItem * p_text_item6 = new QGraphicsSimpleTextItem;
    QGraphicsSimpleTextItem * p_text_item7 = new QGraphicsSimpleTextItem;
    QGraphicsSimpleTextItem * p_text_item8 = new QGraphicsSimpleTextItem;
    QGraphicsSimpleTextItem * p_text_item9 = new QGraphicsSimpleTextItem;
    QGraphicsSimpleTextItem * p_text_item10 = new QGraphicsSimpleTextItem;

    p_text_item6->setText(affixes[5]);
    p_text_item7->setText(affixes[6]);
    p_text_item8->setText(affixes[7]);
    p_text_item9->setText(affixes[8]);
    p_text_item10->setText(affixes[9]);

    QRectF bR6 = p_text_item6->sceneBoundingRect();
    QRectF bR7 = p_text_item7->sceneBoundingRect();
    QRectF bR8 = p_text_item8->sceneBoundingRect();
    QRectF bR9 = p_text_item9->sceneBoundingRect();
    QRectF bR10 = p_text_item10->sceneBoundingRect();
    p_text_item6->setPos( x+3  - bR6.width()/2, y-35 - bR6.height()   );
    p_text_item7->setPos( x-25 - bR7.width()  , y-12 - bR7.height()/2 );
    p_text_item8->setPos( x-22 - bR8.width()  , y+24 - bR8.height()/2 );
    p_text_item9->setPos( x+22                , y+24 - bR9.height()/2 );
    p_text_item10->setPos( x+32                , y-9  - bR10.height()/2 );
    scene->addItem(p_text_item6);
    scene->addItem(p_text_item7);
    scene->addItem(p_text_item8);
    scene->addItem(p_text_item9);
    scene->addItem(p_text_item10);


    QGraphicsSimpleTextItem * p_text_item = new QGraphicsSimpleTextItem;
    p_text_item->setText(QString::number(pSig->get_number_of_stems()));
    p_text_item->setPos (x-5, y-15 + 65 );
    scene->addItem(p_text_item);
}
    void elevenagon(CSignature* pSig, int x, int y,  lxa_graphics_scene * scene,  QColor color)
{

    x  += 20;
    QPolygon square1, square2;

    square1.append(QPoint(x,y-20));
    square1.append(QPoint(x+40,y-20));
    square1.append(QPoint(x+40,y-60));
    square1.append(QPoint(x,y-60));
    square1.append(QPoint(x,y-20));

    QGraphicsPolygonItem * pSquareItem1 = scene->addPolygon(square1,QPen(), QBrush(Qt::darkBlue));

    square2.append(QPoint(x,y+30));
    square2.append(QPoint(x+40,y+30));
    square2.append(QPoint(x+40,y-10));
    square2.append(QPoint(x,y-10));
    square2.append(QPoint(x,y+30));

    QGraphicsPolygonItem * pSquareItem2 = scene->addPolygon(square2,QPen(), QBrush(Qt::darkBlue));

    x  -= 20;

    QGraphicsSimpleTextItem * p_text_item1 = new QGraphicsSimpleTextItem;
    QGraphicsSimpleTextItem * p_text_item2 = new QGraphicsSimpleTextItem;
    QGraphicsSimpleTextItem * p_text_item3 = new QGraphicsSimpleTextItem;
    QGraphicsSimpleTextItem * p_text_item4 = new QGraphicsSimpleTextItem;
    QGraphicsSimpleTextItem * p_text_item5 = new QGraphicsSimpleTextItem;
    QGraphicsSimpleTextItem * p_text_item6 = new QGraphicsSimpleTextItem;
    QGraphicsSimpleTextItem * p_text_item7 = new QGraphicsSimpleTextItem;
    QGraphicsSimpleTextItem * p_text_item8 = new QGraphicsSimpleTextItem;

    QStringList affixes = pSig->get_key().split("=");
    p_text_item1->setText(affixes[0]);
    p_text_item2->setText(affixes[1]);
    p_text_item3->setText(affixes[2]);
    p_text_item4->setText(affixes[3]);
    p_text_item5->setText(affixes[4]);
    p_text_item6->setText(affixes[5]);
    p_text_item7->setText(affixes[6]);
    p_text_item8->setText(affixes[7]);
    \
    QRectF bR1 = p_text_item1->sceneBoundingRect();
    QRectF bR2 = p_text_item2->sceneBoundingRect();
    QRectF bR3 = p_text_item3->sceneBoundingRect();
    QRectF bR4 = p_text_item4->sceneBoundingRect();
    QRectF bR5 = p_text_item5->sceneBoundingRect();
    QRectF bR6 = p_text_item6->sceneBoundingRect();
    QRectF bR7 = p_text_item7->sceneBoundingRect();
    QRectF bR8 = p_text_item8->sceneBoundingRect();

    p_text_item1->setPos( x+15 - bR1.width()   , y-70  );
    p_text_item2->setPos( x + 70                , y-70  );
    p_text_item3->setPos( x + 70    , y-30  );
    p_text_item4->setPos( x+15 - bR4.width()    , y-30  );
    p_text_item5->setPos( x+15 - bR5.width()                , y-10 );
    p_text_item6->setPos( x + 70                 , y -10 );
    p_text_item7->setPos( x + 70    , y +15 );
    p_text_item8->setPos( x+15 - bR8.width()       , y +15 );

    scene->addItem(p_text_item1);
    scene->addItem(p_text_item2);
    scene->addItem(p_text_item3);
    scene->addItem(p_text_item4);
    scene->addItem(p_text_item5);
    scene->addItem(p_text_item6);
    scene->addItem(p_text_item7);
    scene->addItem(p_text_item8);


    ///////////////////////////
    x += 150;
    y -= 10;
    QPolygon triangle;
    triangle.append(QPoint(x,y+30));
    triangle.append(QPoint(x+40,y+30));
    triangle.append(QPoint(x+20,y-10));
    triangle.append(QPoint(x,y+28));
    QGraphicsPolygonItem * pTriangleItem = scene->addPolygon(triangle,QPen(), QBrush(Qt::darkBlue));
    QGraphicsSimpleTextItem * p_text_item9 = new QGraphicsSimpleTextItem;
    QGraphicsSimpleTextItem * p_text_item10 = new QGraphicsSimpleTextItem;
    QGraphicsSimpleTextItem * p_text_item11 = new QGraphicsSimpleTextItem;
    p_text_item9->setText(affixes[8]);
    p_text_item10->setText(affixes[9]);
    p_text_item11->setText(affixes[10]);
    QRectF bR9 = p_text_item9->sceneBoundingRect();
    QRectF bR10 = p_text_item10->sceneBoundingRect();
    QRectF bR11 = p_text_item11->sceneBoundingRect();
    p_text_item9->setPos( x-7 - bR9.width()   , y+30 - bR9.height()/2 );
    p_text_item10->setPos( x+25 - bR10.width()/2, y-30 - bR10.height()/2 );
    p_text_item11->setPos( x+47                , y+30 - bR11.height()/2 );
    scene->addItem(p_text_item9);
    scene->addItem(p_text_item10);
    scene->addItem(p_text_item11);

    QGraphicsSimpleTextItem *p_text_item = new QGraphicsSimpleTextItem;
    p_text_item->setText(QString::number(pSig->get_number_of_stems()));
    QRectF bR = p_text_item->sceneBoundingRect();
    p_text_item->setPos (x+38 - bR.width()/2, y-10 + 65 );
    scene->addItem(p_text_item);
}
void twelvagon(CSignature* pSig, int x, int y,  lxa_graphics_scene * scene,  QColor color)
{

    x  += 20;
    QPolygon square1, square2, square3;

    square1.append(QPoint(x,y-20));
    square1.append(QPoint(x+40,y-20));
    square1.append(QPoint(x+40,y-60));
    square1.append(QPoint(x,y-60));
    square1.append(QPoint(x,y-20));
    QGraphicsPolygonItem * pSquareItem1 = scene->addPolygon(square1,QPen(), QBrush(Qt::darkBlue));
    square2.append(QPoint(x,y+30));
    square2.append(QPoint(x+40,y+30));
    square2.append(QPoint(x+40,y-10));
    square2.append(QPoint(x,y-10));
    square2.append(QPoint(x,y+30));
    QGraphicsPolygonItem * pSquareItem2 = scene->addPolygon(square2,QPen(), QBrush(Qt::darkBlue));

    x+= 120;
    square3.append(QPoint(x,y+30));
    square3.append(QPoint(x+40,y+30));
    square3.append(QPoint(x+40,y-10));
    square3.append(QPoint(x,y-10));
    square3.append(QPoint(x,y+30));
    QGraphicsPolygonItem * pSquareItem = scene->addPolygon(square3,QPen(), QBrush(Qt::darkBlue));

    x  -= 140;

    QGraphicsSimpleTextItem * p_text_item1 = new QGraphicsSimpleTextItem;
    QGraphicsSimpleTextItem * p_text_item2 = new QGraphicsSimpleTextItem;
    QGraphicsSimpleTextItem * p_text_item3 = new QGraphicsSimpleTextItem;
    QGraphicsSimpleTextItem * p_text_item4 = new QGraphicsSimpleTextItem;
    QGraphicsSimpleTextItem * p_text_item5 = new QGraphicsSimpleTextItem;
    QGraphicsSimpleTextItem * p_text_item6 = new QGraphicsSimpleTextItem;
    QGraphicsSimpleTextItem * p_text_item7 = new QGraphicsSimpleTextItem;
    QGraphicsSimpleTextItem * p_text_item8 = new QGraphicsSimpleTextItem;
    QGraphicsSimpleTextItem * p_text_item9 = new QGraphicsSimpleTextItem;
    QGraphicsSimpleTextItem * p_text_item10 = new QGraphicsSimpleTextItem;
    QGraphicsSimpleTextItem * p_text_item11= new QGraphicsSimpleTextItem;
    QGraphicsSimpleTextItem * p_text_item12 = new QGraphicsSimpleTextItem;



    QStringList affixes = pSig->get_key().split("=");
    p_text_item1->setText(affixes[0]);
    p_text_item2->setText(affixes[1]);
    p_text_item3->setText(affixes[2]);
    p_text_item4->setText(affixes[3]);
    p_text_item5->setText(affixes[4]);
    p_text_item6->setText(affixes[5]);
    p_text_item7->setText(affixes[6]);
    p_text_item8->setText(affixes[7]);
    p_text_item9->setText(affixes[8]);
    p_text_item10->setText(affixes[9]);
    p_text_item11->setText(affixes[10]);
    p_text_item12->setText(affixes[11]);


    QRectF bR1 = p_text_item1->sceneBoundingRect();
    QRectF bR2 = p_text_item2->sceneBoundingRect();
    QRectF bR3 = p_text_item3->sceneBoundingRect();
    QRectF bR4 = p_text_item4->sceneBoundingRect();
    QRectF bR5 = p_text_item5->sceneBoundingRect();
    QRectF bR6 = p_text_item6->sceneBoundingRect();
    QRectF bR7 = p_text_item7->sceneBoundingRect();
    QRectF bR8 = p_text_item8->sceneBoundingRect();
    QRectF bR9 = p_text_item9->sceneBoundingRect();
    QRectF bR10 = p_text_item10->sceneBoundingRect();
    QRectF bR11 = p_text_item11->sceneBoundingRect();
    QRectF bR12 = p_text_item12->sceneBoundingRect();

    p_text_item1->setPos( x+15 - bR1.width()   , y-70  );
    p_text_item2->setPos( x + 70                , y-70  );
    p_text_item3->setPos( x + 70    , y-30  );
    p_text_item4->setPos( x+15 - bR4.width()    , y-30  );
    p_text_item5->setPos( x+15 - bR5.width()                , y-10 );
    p_text_item6->setPos( x + 70                 , y -10 );
    p_text_item7->setPos( x + 70    , y +15 );
    p_text_item8->setPos( x+15 - bR8.width()       , y +15 );
    x+=120;

    p_text_item9->setPos( x+15 - bR9.width()                , y-10 );
    p_text_item10->setPos( x + 70                 , y -10 );
    p_text_item11->setPos( x + 70    , y +15 );
    p_text_item12->setPos( x+15 - bR12.width()       , y +15 );


    x-=120;


    scene->addItem(p_text_item1);
    scene->addItem(p_text_item2);
    scene->addItem(p_text_item3);
    scene->addItem(p_text_item4);
    scene->addItem(p_text_item5);
    scene->addItem(p_text_item6);
    scene->addItem(p_text_item7);
    scene->addItem(p_text_item8);
    scene->addItem(p_text_item9);
    scene->addItem(p_text_item10);
    scene->addItem(p_text_item11);
    scene->addItem(p_text_item12);


    ///////////////////////////

    QGraphicsSimpleTextItem *p_text_item = new QGraphicsSimpleTextItem;
    p_text_item->setText(QString::number(pSig->get_number_of_stems()));
    QRectF bR = p_text_item->sceneBoundingRect();
    p_text_item->setPos (x+38 - bR.width()/2, y-10 + 65 );
    scene->addItem(p_text_item);

    ///////////////////////////


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
