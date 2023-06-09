#ifndef ROBOT_H
#define ROBOT_H

#include <QObject>
#include "player.h"

class Robot:public Player
{
    Q_OBJECT;

public:
//    using Player::Player;
    explicit Robot(QObject *parent=nullptr);
    explicit Robot(QString name ,QObject *parent=nullptr);


    virtual void prepareCallLord() override;
    virtual void preparePlayHand() override;
    virtual void thinkCallLord()override;
    virtual void thinkPlayHand()override;
};

#endif // ROBOT_H
