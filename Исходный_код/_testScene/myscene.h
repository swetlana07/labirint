
#ifndef MYSCENE_H
#define MYSCENE_H

#include <QGraphicsScene>
//#include <QtWidgets>
class MyScene : public QGraphicsScene
{
public:
    MyScene(QObject* parent);
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);

};

#endif // MYSCENE_H
