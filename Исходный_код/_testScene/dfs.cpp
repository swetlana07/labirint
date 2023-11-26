#include "dfs.h"
Dfs::Dfs(QObject* parent):QObject(parent){
    isStop=false;
    start=QPoint(-1,-1);
    end=QPoint(-1,-1);
}
void Dfs::setData(size_t sizeH, size_t sizeW, QVector<QPoint> bord_,
                  QPoint start_, QPoint end_){
    bord=bord_;
    start=start_;
    end=end_;
    isStop=false;
    SIZE_H=sizeH;
    SIZE_W=sizeW;
}
void Dfs::run()
{
    result.clear();
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
    if (func_recurs(isStop, start, end, 2)!=1){
        result.clear();
    }else{
        for(int j=0;j<result.count()-1; j++)
            qDebug()<<result.at(j).x()<<" "<<result.at(j).y();
        int i=0;
        eraseMas();
        while(i<result.count()){
            QVector<QPoint> V=sosedy(FREE,result.at(i).x(),result.at(i).y());
            for(int j=result.count()-1; j>i; j--){
                foreach(QPoint v,V){
                    if(v==result.at(j)){
                        result.remove(i+2,j-i-1);
                        result[i+1]=v;
                        j=i;
                        break;
                    }
                }
            }
           ++i;
        }
    }
    emit ready(result);
    emit finished();
}
void Dfs::stop(){
    isStop=true;
}
int Dfs::func_recurs(const bool& is_stop, QPoint cur, QPoint end, int level){
    if(is_stop){
      emit finished();
      return -1;
    }
    result.append(cur);
    mas[cur.x()][cur.y()]=level;
    if(cur==end){

        return 1;
    }
    QVector<QPoint> V=sosedy(FREE,cur.x(),cur.y());
    if(!V.count()){
        result.pop_back();
        mas[cur.x()][cur.y()]=-1;
        return -1;
    }
    foreach(QPoint v,V){
        if(v==end){
          mas[v.x()][v.y()]=level+1;
          result.append(v);
          return 1;
        }
    }
    foreach(QPoint v,V){
        if(is_stop){
          emit finished();
          return -1;
        }
        if(func_recurs(is_stop, v, end, level+1)==1){
           return 1;
        }
    }
    result.pop_back();
    mas[cur.x()][cur.y()]=-1;
    return -1;
}


QVector<QPoint> Dfs::sosedy(int state, int i, int j){
   QVector<QPoint> result;
   if(!mas[i][j]) return result;
   if(mas[i-1][j]==state)result.append(QPoint(i-1,j));
   if(mas[i][j-1]==state)result.append(QPoint(i,j-1));
   if(mas[i+1][j]==state)result.append(QPoint(i+1,j));
   if(mas[i][j+1]==state)result.append(QPoint(i,j+1));
   return result;
}

void Dfs::eraseMas(){
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


