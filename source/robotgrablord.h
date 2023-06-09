#ifndef ROBOTGRABLORD_H
#define ROBOTGRABLORD_H

#include <QThread>
#include "player.h"

class RobotGrabLord:public QThread
{
public:
    explicit RobotGrabLord(Player* player,QObject* parent=nullptr);
protected:
    virtual void run() override;
private:
    Player* m_player;
};

#endif // ROBOTGRABLORD_H
