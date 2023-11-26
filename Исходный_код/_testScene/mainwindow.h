/* Класс главного окна программы

*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QtWidgets>
//#include <QtGui>
#include "header.h"
#include "myscene.h"
#include "item.h"
#include "view.h"
#include "bfs.h"
#include "dfs.h"
#include "windowstate.h"
class MainWindow : public QMainWindow
{
    Q_OBJECT
    QVector<QVector< QPointer<Item> > > cells;//двумерный массив клеток лабиринта
    MyScene* scene;
    MyView* view;
    QVector<QPoint> bord;//массив клеток с препятствиями
    QVector<QPoint> result;//массив клеток пути, результат вычисления
    QPoint start, end;//начальная и конечная клетки пути
    State state;//состояние: 0-нет построения пути, 1-есть построение пути
    QThread threadBfs;//поток для вычисления пути в ширину
    QThread threadDfs;//поток для вычисления пути в глубину
    Bfs bfs;//вычисляющий объект -алгоритм для пути в ширину
    Dfs dfs;//вычисляющий объект -алгоритм для пути в глубину
    QRadioButton* btBfs;//кнопка для задания алгоритма в ширину
    QRadioButton* btDfs;//кнопка для задания алгоритма в глубину
    QPushButton* btGen;//кнопка генерации лабиринта
    QLineEdit* inputW, *inputH;//поля ввода ширины и высоты лабиринта
    WindowState sw;//объект для сохранения и восстановления расположения и размера окна приложения
    size_t SIZE_W, SIZE_H;//текущие ширина и высота лабиринта
    bool isValid(QPoint p);//функция проверки принадлежности p лабиринту
    void startThread();//функция запуска вычисления пути
    void drawPath();//функция рисования пути
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void hoveItem(int i, int j);//выполняется при перемещении мыши с нажатой левой кнопкой над клеткой, задает клетку финиша
    void pressItem(int i, int j);//выполняется при щелчке с нажатой левой кнопкой по клетке, задает клетку старта
    void dblPress(int i, int j);//выполняется при щелчке с нажатой правой кнопкой по клетке, устанавливает/снимает препятствие
    void releaseIt();//выполняется при отпускании кнопки мыши
    void erasePath();//очистить ранее нарисованный путь
    void erasePoint();//очистить клетки начала и конца
    void size_change();//построение лабиринта
    void draw(QVector<QPoint> res);//рисование пути между заданными клетками по вычисленному результату.
    void about();//отображение окна "о программе"
    void radioBt();//перестроение пути по выбранному алгоритму, выполняется при нажатии bt_bfs,bt_dfs
    void generation();//генерация лабиринта, выполняется при нажатии кнопки bt_gen
};

#endif // MAINWINDOW_H
