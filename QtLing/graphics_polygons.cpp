#include <QPolygon>
#include "graphics.h"

class CSignature;

void triangle(CSignature* pSig, int x, int y, int row_delta, lxa_graphics_scene * scene, int count, QColor color)
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
    p_text_item->setPos (x + 5 ,y-8 + 0.3* row_delta );
    scene->addItem(p_text_item);

}
void square(CSignature* pSig, int x, int y, int row_delta, lxa_graphics_scene * scene, int count, QColor color)
{
    QPolygon square;
    square.append(QPoint(x,y+30));
    square.append(QPoint(x+40,y+30));
    square.append(QPoint(x+40,y-10));
    square.append(QPoint(x,y-10));
    square.append(QPoint(x,y+30));
    QGraphicsPolygonItem * pSquareItem = scene->addPolygon(square,QPen(), QBrush(Qt::red));
    QGraphicsSimpleTextItem * p_text_item1 = new QGraphicsSimpleTextItem;
    QGraphicsSimpleTextItem * p_text_item2 = new QGraphicsSimpleTextItem;
    QGraphicsSimpleTextItem * p_text_item3 = new QGraphicsSimpleTextItem;
    QGraphicsSimpleTextItem * p_text_item4 = new QGraphicsSimpleTextItem;
    QStringList affixes = pSig->get_key().split("=");
    p_text_item1->setText(affixes[0]);
    p_text_item2->setText(affixes[1]);
    p_text_item3->setText(affixes[2]);
    p_text_item4->setText(affixes[3]);
    QRectF bR1 = p_text_item1->sceneBoundingRect();
    QRectF bR2 = p_text_item2->sceneBoundingRect();
    QRectF bR3 = p_text_item3->sceneBoundingRect();
    QRectF bR4 = p_text_item4->sceneBoundingRect();
    p_text_item1->setPos( x-10 - bR1.width()  , y+30 - bR1.height()/2 );
    p_text_item2->setPos( x-10 - bR2.width()  , y-20 - bR2.height()/2 );
    p_text_item3->setPos( x+50                , y-20 - bR3.height()/2 );
    p_text_item4->setPos( x+50                , y+30 - bR3.height()/2 );
    scene->addItem(p_text_item1);
    scene->addItem(p_text_item2);
    scene->addItem(p_text_item3);
    scene->addItem(p_text_item4);
    QGraphicsSimpleTextItem * p_text_item = new QGraphicsSimpleTextItem;
    p_text_item->setText(QString::number(pSig->get_number_of_stems()));
    p_text_item->setPos (x + 5 ,y-15 + 0.3* row_delta );
    scene->addItem(p_text_item);
}
void pentagon(CSignature* pSig, int x, int y, int row_delta, lxa_graphics_scene * scene, int count, QColor color)
{
    QPolygon pentagon;
    pentagon.append(QPoint(x,y-30));
    pentagon.append(QPoint(x-29,y-12));
    pentagon.append(QPoint(x-18,y+24));
    pentagon.append(QPoint(x+18,y+24));
    pentagon.append(QPoint(x+29,y-9));
    pentagon.append(QPoint(x,y-30));
    QGraphicsPolygonItem * pPentagonItem = scene->addPolygon(pentagon,QPen(), QBrush(Qt::darkMagenta));
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
    QGraphicsSimpleTextItem * p_text_item = new QGraphicsSimpleTextItem;
    p_text_item->setText(QString::number(pSig->get_number_of_stems()));
    p_text_item->setPos (x-5, y-15 + 0.3* row_delta );
    scene->addItem(p_text_item);
}
void hexagon(CSignature* pSig, int x, int y, int row_delta, lxa_graphics_scene * scene, int count, QColor color)
{

    x  += 20;
    QPolygon triangle1, triangle2;
    triangle1.append(QPoint(x,y-15));
    triangle1.append(QPoint(x+40,y-15 ));
    triangle1.append(QPoint(x+20,y-35));
    triangle1.append(QPoint(x,y-15));
    QGraphicsPolygonItem * pTriangleItem1 = scene->addPolygon(triangle1,QPen(), QBrush(Qt::darkBlue));
    triangle2.append(QPoint(x,y-5));
    triangle2.append(QPoint(x+40,y-5));
    triangle2.append(QPoint(x+20,y+15));
    triangle2.append(QPoint(x,y-5));
    QGraphicsPolygonItem * pTriangleItem2 = scene->addPolygon(triangle2,QPen(), QBrush(Qt::darkBlue));
   x  -= 20;

    QGraphicsSimpleTextItem * p_text_item1 = new QGraphicsSimpleTextItem;
    QGraphicsSimpleTextItem * p_text_item2 = new QGraphicsSimpleTextItem;
    QGraphicsSimpleTextItem * p_text_item3 = new QGraphicsSimpleTextItem;
    QGraphicsSimpleTextItem * p_text_item4 = new QGraphicsSimpleTextItem;
    QGraphicsSimpleTextItem * p_text_item5 = new QGraphicsSimpleTextItem;
    QGraphicsSimpleTextItem * p_text_item6 = new QGraphicsSimpleTextItem;


    QStringList affixes = pSig->get_key().split("=");
    p_text_item1->setText(affixes[0]);
    p_text_item2->setText(affixes[1]);
    p_text_item3->setText(affixes[2]);
    p_text_item4->setText(affixes[3]);
    p_text_item5->setText(affixes[4]);
    p_text_item6->setText(affixes[5]);

    QRectF bR1 = p_text_item1->sceneBoundingRect();
    QRectF bR2 = p_text_item2->sceneBoundingRect();
    QRectF bR3 = p_text_item3->sceneBoundingRect();
    QRectF bR4 = p_text_item4->sceneBoundingRect();
    QRectF bR5 = p_text_item5->sceneBoundingRect();
    QRectF bR6 = p_text_item6->sceneBoundingRect();

    p_text_item1->setPos( x+35 - bR1.width()/2, y-50  );
    p_text_item2->setPos( x+80                , y-25  );
    p_text_item3->setPos( x    - bR3.width()  , y-25  );
    p_text_item4->setPos( x+45 - bR4.width()/2, y+18  );
    p_text_item5->setPos( x+80                , y - 5 );
    p_text_item6->setPos( x     - bR6.width() , y - 5 );
    scene->addItem(p_text_item1);
    scene->addItem(p_text_item2);
    scene->addItem(p_text_item3);
    scene->addItem(p_text_item4);
    scene->addItem(p_text_item5);
    scene->addItem(p_text_item6);
    QGraphicsSimpleTextItem * p_text_item = new QGraphicsSimpleTextItem;
    p_text_item->setText(QString::number(pSig->get_number_of_stems()));
    QRectF bR = p_text_item->sceneBoundingRect();
    p_text_item->setPos (x+38 - bR.width()/2, y-10 + 0.3* row_delta );
    scene->addItem(p_text_item);
}

void septagon(CSignature* pSig, int x, int y, int row_delta, lxa_graphics_scene * scene, int count, QColor color)
{

    x  += 20;
    QPolygon square, triangle2;
    square.append(QPoint(x,y+30));
    square.append(QPoint(x+40,y+30));
    square.append(QPoint(x+40,y-10));
    square.append(QPoint(x,y-10));
    square.append(QPoint(x,y+30));

    QGraphicsPolygonItem * pTriangleItem1 = scene->addPolygon(square,QPen(), QBrush(Qt::darkBlue));
    triangle2.append(QPoint(x,y-25));
    triangle2.append(QPoint(x+40,y-25));
    triangle2.append(QPoint(x+20,y-45));
    triangle2.append(QPoint(x,y-25));
    QGraphicsPolygonItem * pTriangleItem2 = scene->addPolygon(triangle2,QPen(), QBrush(Qt::darkBlue));
    x  -= 20;

    QGraphicsSimpleTextItem * p_text_item1 = new QGraphicsSimpleTextItem;
    QGraphicsSimpleTextItem * p_text_item2 = new QGraphicsSimpleTextItem;
    QGraphicsSimpleTextItem * p_text_item3 = new QGraphicsSimpleTextItem;
    QGraphicsSimpleTextItem * p_text_item4 = new QGraphicsSimpleTextItem;
    QGraphicsSimpleTextItem * p_text_item5 = new QGraphicsSimpleTextItem;
    QGraphicsSimpleTextItem * p_text_item6 = new QGraphicsSimpleTextItem;
    QGraphicsSimpleTextItem * p_text_item7 = new QGraphicsSimpleTextItem;

    QStringList affixes = pSig->get_key().split("=");
    p_text_item1->setText(affixes[0]);
    p_text_item2->setText(affixes[1]);
    p_text_item3->setText(affixes[2]);
    p_text_item4->setText(affixes[3]);
    p_text_item5->setText(affixes[4]);
    p_text_item6->setText(affixes[5]);
    p_text_item7->setText(affixes[6]);

    QRectF bR1 = p_text_item1->sceneBoundingRect();
    QRectF bR2 = p_text_item2->sceneBoundingRect();
    QRectF bR3 = p_text_item3->sceneBoundingRect();
    QRectF bR4 = p_text_item4->sceneBoundingRect();
    QRectF bR5 = p_text_item5->sceneBoundingRect();
    QRectF bR6 = p_text_item6->sceneBoundingRect();
    QRectF bR7 = p_text_item7->sceneBoundingRect();

    p_text_item1->setPos( x+30 - bR1.width()/2  , y-60  );
    p_text_item2->setPos( x+80                , y-35  );
    p_text_item3->setPos( x    - bR3.width()  , y-35  );
    p_text_item4->setPos( x    - bR4.width()  , y-10  );
    p_text_item5->setPos( x+70                , y-10 );
    p_text_item6->setPos( x+70                , y +15 );
    p_text_item7->setPos( x    - bR7.width()  , y +15 );

    scene->addItem(p_text_item1);
    scene->addItem(p_text_item2);
    scene->addItem(p_text_item3);
    scene->addItem(p_text_item4);
    scene->addItem(p_text_item5);
    scene->addItem(p_text_item6);
    scene->addItem(p_text_item7);

    QGraphicsSimpleTextItem * p_text_item = new QGraphicsSimpleTextItem;
    p_text_item->setText(QString::number(pSig->get_number_of_stems()));
    QRectF bR = p_text_item->sceneBoundingRect();
    p_text_item->setPos (x+38 - bR.width()/2, y-10 + 0.3* row_delta );
    scene->addItem(p_text_item);
}
void octagon(CSignature* pSig, int x, int y, int row_delta, lxa_graphics_scene * scene, int count, QColor color)
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

    QGraphicsSimpleTextItem * p_text_item = new QGraphicsSimpleTextItem;
    p_text_item->setText(QString::number(pSig->get_number_of_stems()));
    QRectF bR = p_text_item->sceneBoundingRect();
    p_text_item->setPos (x+38 - bR.width()/2, y-10 + 0.3* row_delta );
    scene->addItem(p_text_item);
}
void nonagon(CSignature* pSig, int x, int y, int row_delta, lxa_graphics_scene * scene, int count, QColor color)
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
    p_text_item->setPos (x+38 - bR.width()/2, y-10 + 0.3* row_delta );
    scene->addItem(p_text_item);
}
void decagon(CSignature* pSig, int x, int y, int row_delta, lxa_graphics_scene * scene, int count, QColor color)
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
    p_text_item->setPos (x-5, y-15 + 0.3* row_delta );
    scene->addItem(p_text_item);
}
    void elevenagon(CSignature* pSig, int x, int y, int row_delta, lxa_graphics_scene * scene, int count, QColor color)
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
    p_text_item->setPos (x+38 - bR.width()/2, y-10 + 0.3* row_delta );
    scene->addItem(p_text_item);
}
void twelvagon(CSignature* pSig, int x, int y, int row_delta, lxa_graphics_scene * scene, int count, QColor color)
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
    p_text_item->setPos (x+38 - bR.width()/2, y-10 + 0.3* row_delta );
    scene->addItem(p_text_item);

    ///////////////////////////


}

