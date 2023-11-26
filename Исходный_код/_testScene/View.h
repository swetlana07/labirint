/*
*Это класс представления сцены.
* Метод wheelEvent переопределен для  масштабирования при повороте колесика мыши.
* Метод mouseReleaseEvent переопределен для выдачи сигнала отжатия кнопки мыши.
*/
#ifndef VIEW_H
#define VIEW_H
#include<math.h>
//#include <QtGui>
#include <QtWidgets>
class MyView : public QGraphicsView
{
    Q_OBJECT

public:
    MyView(QGraphicsScene* sc):QGraphicsView(sc){}
    void zoomAt(const QPoint &centerPos, double factor)
    {
        //transformationAnchor() по умолчанию == AnchorViewCenter,при установке в NoAnchor трансформация будет работать эффективнее
        QPointF targetScenePos = mapToScene(centerPos);
        ViewportAnchor oldAnchor = this->transformationAnchor();
        setTransformationAnchor(QGraphicsView::NoAnchor);
        QTransform matrix = transform();
        matrix.translate(targetScenePos.x(), targetScenePos.y())
                .scale(factor, factor)
                .translate(-targetScenePos.x(), -targetScenePos.y());
        setTransform(matrix);
        setTransformationAnchor(oldAnchor);
    }
protected:
    void mouseReleaseEvent(QMouseEvent *mouseEvent){
        emit release();
        QGraphicsView::mouseReleaseEvent(mouseEvent);
    }
    void wheelEvent(QWheelEvent *event)
    {
            double angle = event->delta();
            double factor = qPow(1.0015, angle);
            zoomAt(event->pos(), factor);
            return;

    }

signals:
    void release();
    void press(QPoint p);
};

#endif // VIEW_H
