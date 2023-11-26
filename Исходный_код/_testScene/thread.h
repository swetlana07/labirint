#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QtGui>
#include "header.h"
class Thread : public QThread
{
    Q_OBJECT
    QPoint* start_cell;
    QPoint* end_cell;
    const QVector<QPoint>& bord;
    QVector<QPoint>& result;
    int id_func;
    void func_bfs(QPoint start, QPoint end);
    int func_recurs(QPoint start, QPoint end, int level);

    void func_dfs(QPoint start, QPoint end);
    QVector<QPoint> sosedy(int state, int i, int j);
    QVector<QPoint> path(QPoint start, QPoint end);
    void print_mas();
    int mas[SIZE+2][SIZE+2];
    void erase_mas();
public:

    Thread(int func_,const size_t size, const QVector<QPoint>& bord_,
           QPoint* start, QPoint* end, QVector<QPoint>& result, QObject* parent = 0);

    void run();

};

#endif // THREAD_H
