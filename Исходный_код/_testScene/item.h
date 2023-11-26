/*
*Это класс клетки лабиринта.


*/
#ifndef ITEM_H
#define ITEM_H
#include <QtWidgets>
//#include <QtGui>
#include "header.h"
class Item : public QGraphicsObject
{
    Q_OBJECT
    public:
    Item(int i_=0, int j_=0);      // конструктор клетки
    QRectF boundingRect() const;   //область клетки
    void paint(QPainter* painter,  //отрисовка клетки
               const QStyleOptionGraphicsItem* option,
               QWidget* );
    void setColor(QColor color_);//установить цвет клетки
    void setBord(bool b);        //установить препятствие в клетке
    void setStart(bool b);
    void setEnd(bool b);
    bool isBord();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent*);

private:
    int i, j;//координаты клетки
    bool itemBord;//если истина, то препятствие
    bool itemStart;//если истина, то старт
    bool itemEnd;  //если истина, то финиш
    QColor color;//цвет клетки

signals:
    void press(int i, int j);
    void pressRight(int i, int j);
    void hover(int i, int j);

};

#endif // ITEM_H
