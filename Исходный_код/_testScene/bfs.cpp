#include "bfs.h"
Bfs::Bfs(QObject* parent):QObject(parent){
    isStop=false;
    start=QPoint(-1,-1);
    end=QPoint(-1,-1);
}
void Bfs::setData(size_t sizeH,size_t sizeW, QVector<QPoint> bord_,
                  QPoint start_, QPoint end_){
    bord=bord_;
    start=start_;
    end=end_;
    isStop=false;
    SIZE_H=sizeH;
    SIZE_W=sizeW;
}
void Bfs::run()
{
    QVector<QPoint> result;
    if ((start==QPoint(-1,-1))||(end==QPoint(-1,-1))){
        emit ready(result);
        emit finished();
        return;
    }
    start.setX(start.x()+1);
    start.setY(start.y()+1);
    end.setX(end.x()+1);
    end.setY(end.y()+1);
    eraseMas();
    if(mas[start.x()][start.y()]==0){
        emit ready(result);
        emit finished();
        return;
    }
    if((start.x()==end.x()) && (start.y()==end.y())){
       result.append(start);
       emit ready(result);
       emit finished();
       return;
    }
    QQueue<QPoint> qu;
    qu.enqueue(start);
    mas[start.x()][start.y()]=2;
    QPoint p;
    while (!qu.isEmpty()){
         p=qu.dequeue();
         if (isStop){
             emit finished();
             return;
         }
         QVector<QPoint> V=sosedy(FREE,p.x(),p.y());
         foreach(QPoint v,V){
             if (isStop){
                 emit finished();
                 return;
             }
             mas[v.x()][v.y()]=mas[p.x()][p.y()]+1;
             if(v==end){
                mas[v.x()][v.y()]=-mas[v.x()][v.y()];
                mas[p.x()][p.y()]=-mas[p.x()][p.y()];
                result=path(start, end);
                emit ready(result);
                emit finished();
                return;
             }
             qu.enqueue(v);
         }
         mas[p.x()][p.y()]=-mas[p.x()][p.y()];
     }
    emit ready(result);
    emit finished();
}
void Bfs::stop(){
  isStop=true;
}

QVector<QPoint> Bfs::sosedy(int state, int i, int j){
   QVector<QPoint> result;
   if(!mas[i][j]) return result;
   if(mas[i-1][j]==state)result.append(QPoint(i-1,j));
   if(mas[i][j-1]==state)result.append(QPoint(i,j-1));
   if(mas[i+1][j]==state)result.append(QPoint(i+1,j));
   if(mas[i][j+1]==state)result.append(QPoint(i,j+1));
   return result;
}
QVector<QPoint> Bfs::path(QPoint start, QPoint end){
    QVector<QPoint> res;
    res.append(end);
    while(res.at(res.count()-1)!=start){
        int value=mas[res.at(res.count()-1).x()][res.at(res.count()-1).y()]+1;
        QVector<QPoint> V=sosedy(value,res.at(res.count()-1).x(),res.at(res.count()-1).y());
        if(!V.count()){
            res.clear();
            return res;
        }
        res.append(V.at(0));
    }
    foreach(QPoint p,res)
        mas[p.x()][p.y()]=7;
    return res;
}
void Bfs::eraseMas(){
    mas.resize(SIZE_H+2);
    for(int i=0;i<(int)SIZE_H+2;i++){
        mas[i].resize(SIZE_W+2);
        for(int j=0;j<(int)SIZE_W+2;j++){
            if((i==0)||(j==0)||(i==((int)SIZE_H+1))||(j==((int)SIZE_W+1)))
                mas[i][j]=0;
            else if(bord.contains(QPoint(i-1,j-1))) mas[i][j]=0;
            else mas[i][j]=1;
        }
    }
}

