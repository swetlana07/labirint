/*
*Это класс для построения пути между двумя клетками лабиринта в глубину.


*/

#ifndef BFS_H
#define BFS_H
#include <QtWidgets>
//#include <QtGui>
#include "header.h"

class Bfs : public QObject
{
    Q_OBJECT
QVector<QPoint> sosedy(int state, int i, int j);
QVector<QPoint> path(QPoint start, QPoint end);
void eraseMas();
QVector<QVector<int> >mas;
QVector<QPoint> bord;
QPoint start, end;
bool isStop;
size_t SIZE_H, SIZE_W;
public:
   Bfs(QObject* parent = 0);
   void setData(size_t sizeH, size_t sizeW, QVector<QPoint> bord_,
                QPoint start_, QPoint end_);

signals:
    void finished();
    void ready(QVector<QPoint> result);
public slots:
    void run();
    void stop();
};

#endif // BFS_H
