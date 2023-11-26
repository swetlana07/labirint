#include "item.h"

Item::Item(int i_, int j_):i(i_), j(j_)//const int& state_click):state_click(state_click)
{
    this->setAcceptHoverEvents(true);  //setAcceptsHoverEvents(true);
    itemBord=false;
    itemStart=false;
    itemEnd=false;
    color=COLOR_FREE;

}
QRectF Item::boundingRect() const {
     return QRectF(0, 0, 18, 18);
}
void Item::paint(QPainter* painter,
           const QStyleOptionGraphicsItem* option,
           QWidget* ){
    if(itemBord){
        painter->setBrush(COLOR_BORD);
        painter->setPen(COLOR_BORD);}
    else{
        if(option->state & QStyle::State_MouseOver){// &&(option->state & QStyle::State_MouseOver)){
            if(itemStart){
                painter->setBrush(COLOR_START);
                painter->setPen(COLOR_START);
            }else if(itemEnd){
                painter->setBrush(COLOR_END);
                painter->setPen(COLOR_END);
            }else{
                painter->setBrush(COLOR_BEG);
                painter->setPen(COLOR_BEG);
            }
        }else{
            if(itemStart){
                painter->setBrush(COLOR_START);
                painter->setPen(COLOR_START);
            }else if(itemEnd){
                painter->setBrush(COLOR_END);
                painter->setPen(COLOR_END);
            }else{
                painter->setBrush(color);
                painter->setPen(color);
            }
        }
    }
    painter->drawRect(boundingRect());
}

void Item::setColor(QColor color_){
    color=color_;
    update();
}

void Item::setBord(bool bl){
    itemBord=bl;
    if(!itemBord)setColor(COLOR_FREE);
    else setColor(COLOR_BORD);
    update();
}
void Item::setStart(bool bl){
    itemStart=bl;
    if(!itemStart)setColor(COLOR_FREE);
    else setColor(COLOR_START);
    update();
}
void Item::setEnd(bool bl){
    itemEnd=bl;
    if(!itemEnd)setColor(COLOR_FREE);
    else setColor(COLOR_END);
    update();
}
bool Item::isBord(){
    return itemBord;
}

void Item::mousePressEvent(QGraphicsSceneMouseEvent* event){
    if(event->buttons() & Qt::LeftButton){
        if(!itemBord)
           emit press(i,j);
    }else
        emit pressRight(i,j);
}

void Item::hoverEnterEvent(QGraphicsSceneHoverEvent*){
    if(itemBord)return;
    emit hover(i,j);
    update();
}


