/*
*Это класс сцены
* Метод mouseMoveEvent переопределен для получения в item события перемещения мыши
* с нажатой кнопкой над клетками
*/
#include "myscene.h"
#include <QtGui>
#include <QtWidgets>
MyScene::MyScene(QObject* parent=0):QGraphicsScene(parent)
{
}
void MyScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->type()==QEvent::GraphicsSceneMouseMove)event->setButtons(Qt::NoButton);
    QGraphicsScene::mouseMoveEvent(event);

}

