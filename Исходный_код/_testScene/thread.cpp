#include "thread.h"
#include <QQueue>
Thread::Thread(int func_,const size_t size, const QVector<QPoint>& bord_,
               QPoint* start, QPoint* end, QVector<QPoint>& result, QObject* parent)
    :id_func(func_), bord(bord_), start_cell(start), end_cell(end), result(result), QThread(parent){
     //
}
void Thread::run()
{
    QPoint start=*start_cell;
    start.setX(start.x()+1);
    start.setY(start.y()+1);
    QPoint end=*end_cell;
    end.setX(end.x()+1);
    end.setY(end.y()+1);
    erase_mas();
    result.clear();
    if(mas[start.x()][start.y()]==0){
        qDebug()<<"return mas[start.x()][start.y()]==0";
        return;
    }
    if((start.x()==end.x()) && (start.y()==end.y())){
       result.append(start);
       qDebug()<<"return (start.x()==end.x()) && (start.y()==end.y()";
       return;
    }
    if(1)
        func_dfs(start, end);
    else{
        if (func_recurs(start, end, 2)!=1){
            qDebug()<<"path no found";
            result.clear();
        }else{
            qDebug()<<"path yes=";
            for(int j=0;j<result.count()-1; j++)
                qDebug()<<result.at(j).x()<<" "<<result.at(j).y();
            int i=0;
            erase_mas();
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
           // qDebug()<<"optimize"; for(int j=0;j<result.count()-1; j++) qDebug()<<result.at(j).x()<<" "<<result.at(j).y();
        }
    }
    return;
}
//алгоритм в глубину
void Thread::func_bfs(QPoint start, QPoint end){
  // func_recurs(start,end,2);

}

int Thread::func_recurs(QPoint cur, QPoint end, int level){
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
    foreach(QPoint v,V){//оптимизация агоритма-просмотр соседей
        if(v==end){
          mas[v.x()][v.y()]=level+1;
          result.append(v);
          return 1;
        }
    }
    foreach(QPoint v,V){
        qDebug()<<"v level="<<level<<" coord="<<v.x()<<" "<<v.y();
        if(func_recurs(v, end, level+1)==1){
           return 1;
        }
    }
    result.pop_back();
    mas[cur.x()][cur.y()]=-1;
    return -1;
}

//алгоритм в ширину
void Thread::func_dfs(QPoint start,QPoint end){
   QQueue<QPoint> qu;
   qu.enqueue(start);
   mas[start.x()][start.y()]=2;
   QPoint p;
   while (!qu.isEmpty()){
        p=qu.dequeue();
        QVector<QPoint> V=sosedy(FREE,p.x(),p.y());
        foreach(QPoint v,V){
            mas[v.x()][v.y()]=mas[p.x()][p.y()]+1;
            if(v==end){
               mas[v.x()][v.y()]=-mas[v.x()][v.y()];
               mas[p.x()][p.y()]=-mas[p.x()][p.y()];
               print_mas();
               result=path(start, end);// qDebug()<<"finish path";
               return;
            }
            qu.enqueue(v);
        }
        mas[p.x()][p.y()]=-mas[p.x()][p.y()];
    }//qDebug()<<"finish path no found";
}
QVector<QPoint> Thread::sosedy(int state, int i, int j){
   QVector<QPoint> result;
   if(!mas[i][j]) return result;
   if(mas[i-1][j]==state)result.append(QPoint(i-1,j));
   if(mas[i][j-1]==state)result.append(QPoint(i,j-1));
   if(mas[i+1][j]==state)result.append(QPoint(i+1,j));
   if(mas[i][j+1]==state)result.append(QPoint(i,j+1));
}
QVector<QPoint> Thread::path(QPoint start, QPoint end){
    QVector<QPoint> res;
    res.append(end);
    qDebug()<<"thread-start";
    while(res.at(res.count()-1)!=start){
        int value=mas[res.at(res.count()-1).x()][res.at(res.count()-1).y()]+1;
        QVector<QPoint> V=sosedy(value,res.at(res.count()-1).x(),res.at(res.count()-1).y());
        if(!V.count()){
            res.clear();
            return res;
        }
        res.append(V.at(0));
    }
    qDebug()<<"thread-result=";
    foreach(QPoint p,res)
        mas[p.x()][p.y()]=7;
    for(int i=1;i<SIZE;++i){
       QString s;
       for(int j=1;j<SIZE;++j)
           s+=QString::number(mas[i][j])+" ";
       qDebug()<<s;
    }

    return res;
}
void Thread::erase_mas(){
    for(int i=0;i<SIZE+2;i++){
        for(int j=0;j<SIZE+2;j++){
            if((i==0)||(j==0)||(i==(SIZE+1))||(j==(SIZE+1)))
                mas[i][j]=0;
            else if(bord.contains(QPoint(i-1,j-1))) mas[i][j]=0;
            else mas[i][j]=1;

        }

    }
    qDebug()<<"mas erase";

}
void Thread::print_mas(){
    for(int i=0;i<SIZE+2;i++){
        QString s;
        for(int j=0;j<SIZE+2;j++){
           s+=QString::number((int)mas[i][j])+" ";
        }
         qDebug()<<s;
    }
    qDebug()<<"-------print_mas";

}
