#include <QSettings>
#include <QDir>
#include <QDebug>
#include "windowstate.h"

WindowState::WindowState()
{
}
void WindowState::Save(QPoint pos, QSize size){
    QSettings settings(QDir::currentPath()+"/setting.ini", QSettings::IniFormat);
    settings.beginGroup("Application");
    settings.setValue("pos", pos);
    settings.setValue("size", size);
    settings.endGroup();
}
void WindowState::Restore(QPoint &pos, QSize &size){
    QSettings settings(QDir::currentPath()+"/setting.ini", QSettings::IniFormat);
    settings.beginGroup("Application");
    QPoint oldP=settings.value("pos", QPoint(-1,-1)).toPoint();
    if(oldP!=QPoint(0,0))
        pos=oldP;
    QSize oldS=settings.value("size", QSize(-1,-1)).toSize();
    if(oldS!=QSize(0,0))
        size=oldS;
    settings.endGroup();
}
