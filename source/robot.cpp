#include <QDebug>


#include "robot.h"
#include "robotgrablord.h"
#include "robotplayhand.h"
#include "strategy.h"

Robot::Robot(QObject *parent):Player (parent){
    m_type = Player::Robot;
}

Robot::Robot(QString name, QObject *parent):Player(name,parent)
{

}

void Robot::prepareCallLord() {
    RobotGrabLord* subThd=new RobotGrabLord(this);
    connect(subThd,&RobotGrabLord::finished,this,[=]{
        qDebug()<<"RobotGrabLord 子线程对象析构"<<", Robot Name: "<<this->getName();
                                            subThd->deleteLater();
    });
    subThd->start();
}

void Robot::preparePlayHand() {
    RobotPlayHand* subThd=new RobotPlayHand(this);
    connect(subThd,&RobotPlayHand::finished,this,[=]{
        qDebug()<<"RobotPlayHand 子线程对象析构"<<", Robot Name: "<<this->getName();
                                            subThd->deleteLater();
    });
    subThd->start();
}
void Robot::thinkCallLord(){
    int weight=0;
    Strategy st(this,m_cards);
    weight += st.getRangeCards(Card::Card_SJ,Card::Card_BJ).cardCount()*6;

    QVector<Cards> optSeq = st.pickOptimalSeqSingles();
    weight += optSeq.size() *5;

    QVector<Cards> bombs = st.findCardsByCount(4);
    weight += bombs.size() *5;

    weight += m_cards.pointCount(Card::Card_2)*3;

    Cards tmp = m_cards;
    tmp.remove(optSeq);
    tmp.remove(bombs);

    Cards Card_2 = st.getRangeCards(Card::Card_2,Card::Card_2);
    tmp.remove(Card_2);

    QVector<Cards> triples = Strategy(this,tmp).findCardsByCount(3);
    weight += triples.size() *4;
    tmp.remove(triples);

    QVector<Cards> pairs = Strategy(this,tmp).findCardsByCount(2);
    weight+=pairs.size()*1;

    if (weight>=22){
        grabLordBet(3);
    }
    else if(weight <22 && weight >=18){
        grabLordBet(2);
    }
    else if (weight<18&&weight >=10){
        grabLordBet(1);
    }
    else{
        grabLordBet(0);
    }


}
void Robot::thinkPlayHand(){
    Strategy st(this,m_cards);
    Cards cs = st.makeStrategy();
    playHand(cs);
}
