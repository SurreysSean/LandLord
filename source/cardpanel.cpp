#include "cardpanel.h"

#include <QMouseEvent>
#include <QPainter>


CardPanel::CardPanel(QWidget *parent):QWidget(parent){}

void CardPanel::clicked(){
    emit cardSelected(Qt::LeftButton);
}

void CardPanel::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter p(this);
    if (m_isfront){
        p.drawPixmap(rect(),m_front);
    }
    else{
        p.drawPixmap(rect(),m_back);
    }
}

void CardPanel::mousePressEvent(QMouseEvent *event){
    emit cardSelected(event->button());
}




