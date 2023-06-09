#include "robotgrablord.h"

RobotGrabLord::RobotGrabLord(Player *player,QObject *parent):QThread(parent)
{
    m_player = player;
}

void RobotGrabLord::run(){
    msleep(2000);
    m_player->thinkCallLord();
}
