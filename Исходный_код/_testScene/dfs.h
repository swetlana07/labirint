/*
*Это класс для построения пути между двумя клетками лабиринта в ширину.


*/
#ifndef DFS_H
#define DFS_H
#include <QtWidgets>
//#include <QtGui>
#include "header.h"

class Dfs : public QObject
{
        Q_OBJECT
    QVector<QPoint> sosedy(int state, int i, int j);
    int func_recurs(const bool& is_stop, QPoint cur, QPoint end, int level);
    void eraseMas();// void print_mas();
    QVector<QVector<int> >mas;//int mas[SIZE+2][SIZE+2];
    QVector<QPoint> bord;
    QVector<QPoint> result;
    QPoint start, end;
    bool isStop;
    size_t SIZE_H, SIZE_W;
    public:
        Dfs(QObject* parent = 0);
        void setData(size_t sizeH, size_t sizeW, QVector<QPoint> bord_,
                    QPoint start_, QPoint end_);

    signals:
        void finished();
        void ready(QVector<QPoint> result);
    public slots:
        void run();
        void stop();
};

#endif // DFS_H
