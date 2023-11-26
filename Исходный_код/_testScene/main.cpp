/*
Эта программа- результат выполнения тестового задания поиска пути в лабиринте.

Лабиринт заполняется случайным образом. Пользователь щелчком мыши с нажатой правой кнопкой
может установить или снять препятствие в выбранной клетке.
Лабиринт масштабируется колесиком мыши.

Для поиска пути выполните следующие действия:
1 Щелкните мышью с нажатой левой кнопкой на любой свободной от препятствия клетке.
2 Не отпуская левую кнопку  переместите мышь на лубую другую свободную клетку и отпустите кнопку мыши.
  Программа построит путь между двумя клетками.
*/

#include <QtGui>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    //QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    //QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Программа поиска пути в лабиринте");
    w.show();

    return a.exec();
}
