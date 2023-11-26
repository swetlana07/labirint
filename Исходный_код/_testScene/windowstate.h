#ifndef WINDOWSTATE_H
#define WINDOWSTATE_H
#include <QPoint>
#include<QSize>
class WindowState
{
public:
    WindowState();
    void Save(QPoint pos, QSize size);
    void Restore(QPoint &pos, QSize &size);
};

#endif // WINDOWSTATE_H
