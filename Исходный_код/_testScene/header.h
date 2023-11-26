/*
*Этот класс содержит определения констант


*/
#ifndef HEADER_H
#define HEADER_H
#include <QtWidgets>
//#include <QtGui>
const QColor COLOR_FREE=QColor(255,255,200);//цвет свободной клетки
const QColor COLOR_BORD=Qt::gray;//цвет клетки - препятствия
const QColor COLOR_BEG=Qt::blue;//цвет клетки при наведении мыши
const QColor COLOR_DRAW=Qt::green;//цвет путимежду клеткой-старта и финиша
const QColor COLOR_START=Qt::darkRed;//цвет клетки-старта
const QColor COLOR_END=QColor(255,128,0);//цвет клетки финиша

const size_t SIZE_WIDTH_DEFAULT=20;//ширина лабиринта по умолчанию
const size_t SIZE_HEIGHT_DEFAULT=16;//высота лабиринта по умолчанию

const int FREE=1;//свободное состояние клетки

enum State{//состояния лабиринта
    STATE_FREE=0,//нет построения пути
    STATE_WORK=1,//есть построение пути
};
#endif // HEADER_H
