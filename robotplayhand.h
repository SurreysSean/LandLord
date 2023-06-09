#ifndef ROBOTPLAYHAND_H
#define ROBOTPLAYHAND_H

#include <QThread>

#include "player.h"

class RobotPlayHand:public QThread
{
public:
    explicit RobotPlayHand(Player* player,QThread* parent=nullptr);
protected:
    virtual void run() override;

private:
    Player* m_player;

};

#endif // ROBOTPLAYHAND_H
