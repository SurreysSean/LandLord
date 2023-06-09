#include "gamecontrol.h"
#include "playhand.h"

#include <QRandomGenerator>
#include <QTimer>
#include <QDebug>

GameControl::GameControl(QObject *parent ):QObject(parent)
{

}

void  GameControl::playerInit(){
    m_robotLeft = new Robot("Robot A");
    m_robotRight = new Robot("Robot B");
    m_user = new UserPlayer("Me");

    m_robotLeft->setDirection(Player::Left);
    m_robotRight->setDirection(Player::Right);
    m_user->setDirection(Player::Right);

    Player::Sex sex;
    sex = (Player::Sex)QRandomGenerator::global()->bounded(2);
    m_robotLeft->setSex(sex);
    sex = (Player::Sex)QRandomGenerator::global()->bounded(2);
    m_robotRight->setSex(sex);
    sex = (Player::Sex)QRandomGenerator::global()->bounded(2);
    m_user->setSex(sex);

    m_user->setPrevPlayer(m_robotLeft);
    m_user->setNextPlayer(m_robotRight);

    m_robotLeft->setPrevPlayer(m_robotRight);
    m_robotLeft->setNextPlayer(m_user);

    m_robotRight->setPrevPlayer(m_user);
    m_robotRight->setNextPlayer(m_robotLeft);

    m_currPlayer = m_user;

    connect(m_user, &UserPlayer::notifyGrabLordBet, this, &GameControl::onGrabBet);
    connect(m_robotLeft, &Robot::notifyGrabLordBet, this, &GameControl::onGrabBet);
    connect(m_robotRight, &Robot::notifyGrabLordBet, this, &GameControl::onGrabBet);

    connect(this, &GameControl::pendingInfo, m_robotLeft, &Robot::storePendingInfo);
    connect(this, &GameControl::pendingInfo, m_robotRight, &Robot::storePendingInfo);
    connect(this, &GameControl::pendingInfo, m_user, &UserPlayer::storePendingInfo);

    connect(m_robotLeft, &Robot::notifyPlayHand, this, &GameControl::onPlayHand);
    connect(m_robotRight, &Robot::notifyPlayHand, this, &GameControl::onPlayHand);
    connect(m_user, &UserPlayer::notifyPlayHand, this, &GameControl::onPlayHand);
}

void GameControl::initAllCards(){
    m_allCards.clear();
    for(int p = Card::Card_Begin+1; p<Card::Card_SJ; ++p)
    {
        for(int s = Card::Suit_Begin+1; s<Card::Suit_End-1; ++s)
        {
            Card c((Card::CardPoint)p, (Card::CardSuit)s);
            m_allCards.add(c);
        }
    }
    m_allCards.add(Card(Card::Card_SJ, Card::Joker));
    m_allCards.add(Card(Card::Card_BJ, Card::Joker));
}

Card GameControl::takeOneCard(){
    return m_allCards.takeRandomCard();
}

void GameControl::resetCardData(){
    initAllCards();
    m_robotLeft->clearCards();
    m_robotRight->clearCards();
    m_user->clearCards();
    m_pendPlayer = nullptr;
    m_pendCards.clear();
}

void GameControl::startLordCard(){
    m_currPlayer->prepareCallLord();
    emit playerStatusChanged(m_currPlayer,ThinkingForCallLord);
}

void GameControl::becomeLord(Player *player, int bet){
    m_curBet = bet;
    player->setRole(Player::Lord);
    player->getPrevPlayer()->setRole(Player::Farmer);
    player->getNextPlayer()->setRole(Player::Farmer);

    m_currPlayer = player;
    player->storeDispatchCard(m_allCards);

    QTimer::singleShot(1000,this,[=](){
        emit gameStatusChanged(PlayingHand);
        emit playerStatusChanged(player,ThinkingForPlayHand);
        m_currPlayer->preparePlayHand();
    });


}

void GameControl::clearPlayerScore(){
    m_robotLeft->setScore(0);
    m_robotRight->setScore(0);
    m_user->setScore(0);
}

int GameControl::getPlayerMaxBet(){
    return m_betRecord.bet;
}

void GameControl::onGrabBet(Player* player, int bet){
    if(bet == 0 || m_betRecord.bet >= bet)
    {
        emit notifyGrabLordBet(player, 0, false);
    }
    else if(bet > 0 && m_betRecord.bet == 0)
    {
        emit notifyGrabLordBet(player, bet, true);
    }
    else
    {
        emit notifyGrabLordBet(player, bet, false);
    }

    if (bet==3){
        becomeLord(player,bet);
        m_betRecord.reset();
        return ;
    }
    if (m_betRecord.bet<bet){
        m_betRecord.bet = bet;
        m_betRecord.player = player;
    }
    m_betRecord.times ++;

    if (m_betRecord.times == 3){
        if (m_betRecord.bet==0){
            emit gameStatusChanged(DispatchCard);
        }
        else{
            becomeLord(m_betRecord.player,m_betRecord.bet);
        }
        m_betRecord.reset();
        return ;
    }
    m_currPlayer = player->getNextPlayer();
    emit playerStatusChanged(m_currPlayer, ThinkingForCallLord);
    m_currPlayer->prepareCallLord();

}

void GameControl::onPlayHand(Player *player, const Cards &card){
    emit notifyPlayHand(player,card);
    if (!card.isEmpty()){
        m_pendCards = card;
        m_pendPlayer = player;
        emit pendingInfo(player,card);
    }

    Cards myCards = card;
    PlayHand::HandType type = PlayHand(myCards).getType();
    if (type == PlayHand::Hand_Bomb || type == PlayHand::Hand_Bomb_Jokers){
        m_curBet <<=1;
    }

    if (player->getCards().isEmpty()){
        Player *prev = player->getPrevPlayer();
        Player *next = player->getNextPlayer();
        if (player->getRole() == Player::Lord){
            player->setScore(player->getScore() + 2 * m_curBet);
            prev->setScore(prev->getScore() - m_curBet);
            next->setScore(next->getScore() - m_curBet);
            player->setWin(true);
            prev->setWin(false);
            next->setWin(false);
        }
        else {
            player->setWin(true);
            player->setScore(player->getScore() + m_curBet);
            if(prev->getRole() == Player::Lord)
            {
                prev->setScore(prev->getScore() - 2 * m_curBet);
                next->setScore(next->getScore() + m_curBet);
                prev->setWin(false);
                next->setWin(true);
            }
            else
            {
                next->setScore(next->getScore() - 2 * m_curBet);
                prev->setScore(prev->getScore() + m_curBet);
                next->setWin(false);
                prev->setWin(true);
            }
        }
        emit playerStatusChanged(player, GameControl::Winning);
        return;
    }
    m_currPlayer = player->getNextPlayer();
    m_currPlayer->preparePlayHand();
    emit playerStatusChanged(m_currPlayer, GameControl::ThinkingForPlayHand);
}
