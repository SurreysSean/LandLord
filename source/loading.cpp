#include "loading.h"
#include "GamePanel.h"
#include <QTimer>
#include <QPainter>

Loading::Loading(QWidget *parent):QWidget(parent)
{
    m_bk.load(":/images/loading.png");
    setFixedSize(m_bk.size());

    setWindowFlags(Qt::FramelessWindowHint|Qt::WindowTransparentForInput|windowFlags());
    setAttribute(Qt::WA_TranslucentBackground);

    QPixmap pixmap(":/images/progress.png");

    QTimer *timer=new QTimer(this);
    connect(timer,&QTimer::timeout,this,[=](){
        m_progress = pixmap.copy(0,0,m_dist,pixmap.height());
        update();
        if (m_dist>=pixmap.width()){
            timer->stop();
            timer->deleteLater();
            GamePanel *panel=new GamePanel;
            panel->show();
            close();
        }
        m_dist+=5;
    });
        timer->start(15);

}

std::shared_ptr<Loading> Loading::getInstance(){
    if (instance==nullptr){
            struct make_shared_enable:public Loading{};
            instance=std::make_shared<make_shared_enable>();
    }
    return  instance;
}

void Loading::paintEvent(QPaintEvent* event){
    Q_UNUSED(event);
    QPainter p(this);
    p.drawPixmap(rect(),m_bk);
    p.drawPixmap(62,417,m_progress.width(),m_progress.height(),m_progress);
}

