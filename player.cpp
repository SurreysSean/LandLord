#include "player.h"

Player::Player(QObject *parent) : QObject(parent)
{
}

Player::Player(QString name,QObject *parent):QObject(parent),m_name(name){

}
void Player::grabLordBet(int point){
    emit notifyGrabLordBet(this,point);
}

void Player::storeDispatchCard(const Card& card){
    m_cards.add(card);
    Cards cs;
    cs.add(card);
    emit notifyPickCards(this,cs);
}

void Player::storeDispatchCard(const Cards& cards){
    m_cards.add(cards);
    emit notifyPickCards(this,cards);
}
void Player::playHand(const Cards& cards){
    m_cards.remove(cards);
    emit notifyPlayHand(this,cards);
}

void Player::storePendingInfo(Player* player, const Cards& cards){
    m_pendPlayer = player;
    m_pendCards = cards;
}
