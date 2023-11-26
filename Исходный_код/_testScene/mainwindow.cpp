#include "mainwindow.h"
#include "header.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //восстановление настроек приложения
    QPoint pos;
    QSize sz;
    sw.Restore(pos,sz);
    if(pos!=QPoint(-1,-1))
       this->move(pos);
    if(sz!=QSize(-1,-1))
       this->resize(sz);
    //рагистрация типа для передачи сигнал-слот
    qRegisterMetaType<QVector<QPoint> >("QVector<QPoint>");

    //инициализация
    scene=NULL;
    SIZE_W=SIZE_WIDTH_DEFAULT;
    SIZE_H=SIZE_HEIGHT_DEFAULT;

    //создание представления
    view=new MyView(NULL);
    connect(view,SIGNAL(release()),this,SLOT(releaseIt()));

    //связывание потоков с вычисляющими объектами
    connect(&threadBfs, SIGNAL(started()), &bfs, SLOT(run()));
    connect(&bfs, SIGNAL(finished()), &threadBfs, SLOT(terminate()));
    connect(&bfs, SIGNAL(ready(QVector<QPoint>)), this, SLOT(draw(QVector<QPoint>)));
    bfs.moveToThread(&threadBfs);

    connect(&threadDfs, SIGNAL(started()), &dfs, SLOT(run()));
    connect(&dfs, SIGNAL(finished()), &threadDfs, SLOT(terminate()));
    connect(&dfs, SIGNAL(ready(QVector<QPoint>)), this, SLOT(draw(QVector<QPoint>)));
    dfs.moveToThread(&threadDfs);

    //создание виджетов главного окна
    menuBar()->addAction("&О программе",this,SLOT(about()));
    btBfs=new QRadioButton("Поиск в ширину");
    btDfs=new QRadioButton("Поиск в глубину");
    QVBoxLayout *v_group_lay=new QVBoxLayout;
    v_group_lay->addWidget(btBfs);
    v_group_lay->addWidget(btDfs);
    btBfs->setChecked(true);
    QGroupBox* group=new QGroupBox("Алгоритм поиска пути",this);
    group->setLayout(v_group_lay);
    QLabel* lb=new QLabel("Ширина поля",this);
    inputW=new QLineEdit(this);
    inputW->setText(QString::number(SIZE_W));
    lb->setBuddy(inputW);
    inputH= new QLineEdit(this);
    QLabel* lb1=new QLabel("Высота поля",this);
    lb1->setBuddy(inputH);
    inputH->setText(QString::number(SIZE_H));
    btGen=new QPushButton("Генерировать",this);
    connect(btGen,SIGNAL(clicked(bool)),this, SLOT(generation()));
    QVBoxLayout *lay_w = new QVBoxLayout;
    QVBoxLayout *lay_left = new QVBoxLayout;
    lay_left->setSpacing(15);
    lay_w->addWidget(lb);
    lay_w->addWidget(inputW);
    lay_w->addWidget(lb1);
    lay_w->addWidget(inputH);
    lay_w->addWidget(btGen);
    lay_w->setSpacing(4);
    lay_left->addLayout(lay_w);        ;
    lay_left->addWidget(group);
    lay_left->addStretch();
    QWidget* w=new QWidget;
    w->setLayout(lay_left);
    w->setMaximumWidth(170);
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(view);
    layout->addWidget(w);
    QWidget *widget = new QWidget;
    widget->setLayout(layout);
    setCentralWidget(widget);
    connect(btBfs,SIGNAL(clicked()), this, SLOT(radioBt()));
    connect(btDfs,SIGNAL(clicked()), this, SLOT(radioBt()));

    //построение лабиринта по дефолтным данным
    size_change();
}

void MainWindow::generation(){
    bool ok;
    size_t w=inputW->text().toUInt(&ok);
    if(!ok){
        QMessageBox::critical(0,"Ошибка","Значение ширины введено неверно!",QMessageBox::Cancel);
        inputW->setText(QString::number(SIZE_W));
        return;
    }
    size_t h=inputH->text().toUInt(&ok);
    if(!ok){
        QMessageBox::critical(0,"Ошибка","Значение высоты введено неверно!",QMessageBox::Cancel);
        inputH->setText(QString::number(SIZE_H));
        return;

    }
    //очистка сцены
    if(scene) {
        for(int i=0;i<(int)SIZE_H;++i)
        for(int j=0;j<(int)SIZE_W;++j)
           cells[i][j]->deleteLater();
        scene->deleteLater();
    }
    SIZE_W=w;
    SIZE_H=h;

    //построение лабиринта по пользовательским данным
    size_change();
}

void MainWindow::size_change(){
    result.clear();
    state=STATE_FREE;
    start=QPoint(-1,-1);
    end=QPoint(-1,-1);

    //создание сцены
    scene=new MyScene(this);
    cells.clear();
    cells.resize(SIZE_H);
    for(int i=0;i<(int)SIZE_H;++i){
        cells[i].resize(SIZE_W);
        for(int j=0;j<(int)SIZE_W;++j){
            cells[i][j]=new Item(i, j);
            cells[i][j]->setPos(20*j,20*i);
            scene->addItem(static_cast<QGraphicsItem*>(cells[i][j]));
            connect(cells[i][j],SIGNAL(hover(int, int)),this,SLOT(hoveItem(int, int)));
            connect(cells[i][j],SIGNAL(press(int, int)),this,SLOT(pressItem(int, int)));
            connect(cells[i][j],SIGNAL(pressRight(int,int)),this,SLOT(dblPress(int, int)));
        }
    }
   //рандомное заполнение массива препятствий
   bord.clear();
   while(bord.size()<(int)(SIZE_W+SIZE_H)*3){
        QPoint p=QPoint(qrand()%SIZE_H,qrand()%SIZE_W);
        if(!bord.contains(p)){
            bord.append(p);
        }
    }
   //установка препятствий на сцену
    for(int i=0;i<bord.size();i++){
       cells[bord.at(i).x()][bord.at(i).y()]->setBord(true);
   }
   //установка сцены в представление
   view->setScene(scene);
   if((SIZE_W!=SIZE_WIDTH_DEFAULT)||(SIZE_H!=SIZE_HEIGHT_DEFAULT)){
       view->setSceneRect(scene->sceneRect());
   }
}

void MainWindow::draw(QVector<QPoint> res){
    //рисование результата
     result=res;
     drawPath();
}

MainWindow::~MainWindow()
{
    //сохранение положения и размера окна в файл настроек
    sw.Save(this->pos(),this->size());
}

void MainWindow::pressItem(int i, int j){
    //при щелчке мыши на клетке, при условии, что это не препятствие,
    //клетка становится стартом
    if(state==STATE_FREE){
        state=STATE_WORK;//1;
        if(isValid(start)){
           cells[start.x()][start.y()]->setStart(false);
       }
       if(isValid(end)){
           cells[end.x()][end.y()]->setEnd(false);
       }
       if(!bord.contains(QPoint(i,j))){
        start=QPoint(i,j);
        cells[start.x()][start.y()]->setStart(true);
       }
    }else {
    }
}
void MainWindow::hoveItem(int i, int j){
//если мышь перемещается над клеткой, то она становится финишем
    if(state==STATE_WORK){
       if(isValid(end)){
         cells[end.x()][end.y()]->setEnd(false);
       }
       end=QPoint(i,j);
       cells[end.x()][end.y()]->setEnd(true);
//при этом запускается поток вычисления пути
       startThread();
    }
}

void MainWindow::releaseIt(){
    //при отпускании кнопки мыши переходим прекращаем вычисления пути
    state=STATE_FREE;
}

void MainWindow::drawPath(){
    //рисуем путь по координатам в массиве result
    for(int i=0;i<(int)SIZE_H;++i){
       for(int j=0;j<(int)SIZE_W;++j){
              if(cells[i][j]->isBord())continue;
              if(result.contains(QPoint(i+1,j+1))){
                  cells[i][j]->setColor(COLOR_DRAW);
              }else cells[i][j]->setColor(COLOR_FREE);

       }
    }
    scene->update();
    return;

}

void MainWindow::radioBt(){
    //переключение алгоритма построения пути
    erasePath();
    startThread();

}

void MainWindow::erasePoint(){
    //очистка старта и финиша
    if(isValid(start))
       cells[start.x()][start.y()]->setStart(false);
    if(isValid(end))
       cells[end.x()][end.y()]->setEnd(false);
    start=QPoint(-1,-1);
    end=QPoint(-1,-1);
}

void MainWindow::erasePath(){
    //очистка лабиринта от ранее построенного пути
     result.clear();
     drawPath();
}

void MainWindow::dblPress(int i, int j){
    //установка либо снятие препятствия по щелчку мыши
    // с нажатой правой кнопкой
     QPoint cur=QPoint(i,j);
     if(cells[cur.x()][cur.y()]->isBord()){
         cells[cur.x()][cur.y()]->setBord(false);
         int i=-1;
         if((i=bord.indexOf(cur))!=-1)
             bord.remove(i,1);
         if(isValid(start) && isValid(end)){
            erasePath();
            startThread();
         }
     }else{
         if((cur==start)||(cur==end)){
            erasePoint();
            erasePath();
        }else{
            if(!bord.contains(cur))
               bord.append(cur);
            cells[cur.x()][cur.y()]->setBord(true);
            erasePath();
            startThread();
        }
     }
}

void MainWindow::startThread(){
    //запуск вычисления пути в зависимости от выбранного алгоритма
    if(btBfs->isChecked()){
        if(threadBfs.isRunning()){
            bfs.stop();
        }
        bfs.setData(SIZE_H,SIZE_W,bord,start,end);
        threadBfs.start();
    }else{
        if(threadDfs.isRunning())
            dfs.stop();
        dfs.setData(SIZE_H,SIZE_W,bord,start,end);
        threadDfs.start();
    }
}

bool MainWindow::isValid(QPoint p){
    //проверка принадлежности точки лабиринту
    if((p.x()>=0) && (p.x()<(int)SIZE_H) && (p.y()>=0) && (p.y()<(int)SIZE_W))
        return true;
    return false;
}

void MainWindow::about(){
    //построение окна "о программе"
    QMessageBox* box=new QMessageBox(this);
    box->setTextFormat(Qt::RichText);
    box->setWindowTitle("О программе");
    box->setText("<p>Эта программа- результат выполнения тестового задания поиска пути в лабиринте.</p>"
      "<p>Лабиринт заполняется случайным образом. Пользователь щелчком мыши с нажатой правой кнопкой"
      " может установить или снять препятствие в выбранной клетке.</p> Лабиринт масштабируется колесиком мыши.</p>"
      "<p>Для поиска пути выполните следующие действия:<br>"
      "1 Щелкните мышью с нажатой левой кнопкой на любой свободной от препятствия клетке.<br>"
      "2 Не отпуская левую кнопку  переместите мышь на лубую другую свободную клетку и отпустите кнопку мыши."
      "Программа построит путь между двумя клетками.</p>");
    box->exec();
    box->deleteLater();
}
