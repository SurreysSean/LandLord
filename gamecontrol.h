#ifndef GAMECONTROL_H
#define GAMECONTROL_H

#include <QObject>

#include "robot.h"
#include "userplayer.h"
#include "cards.h"

struct BetRecord
{
    BetRecord()
    {
        reset();
    }
    void reset()
    {
        player = nullptr;
        bet = 0;
        times = 0;
    }
    Player* player;
    int bet;
    int times;
};
class GameControl :public QObject
{
    Q_OBJECT
public:
    enum GameStatus
    {
        DispatchCard,
        CallingLord,
        PlayingHand
    };
    enum PlayerStatus
    {
        ThinkingForCallLord,
        ThinkingForPlayHand,
        Winning //Concluding
    };
public:
    explicit GameControl(QObject *parent = nullptr);

public:
    void playerInit();
    Robot* getLeftRobot() const {return m_robotLeft;}
    Robot* getRightRobot()const {return m_robotRight;}
    UserPlayer* getUserPlayer()const {return m_user;}

    void setCurrentPlayer(Player* player){m_currPlayer = player;}
    Player* getCurrentPlayer()const {return m_currPlayer;}

    Player* getPendPlayer()const {return m_pendPlayer;}
    Cards getPendCards()const {return m_pendCards;}

    void initAllCards();
    Card takeOneCard();
    Cards getSurplusCards()const {return m_allCards;}
    void resetCardData();

    void startLordCard();
    void becomeLord(Player *player, int bet);
    void clearPlayerScore();
    int getPlayerMaxBet();

    void onGrabBet(Player* player, int bet);

    void onPlayHand(Player *player, const Cards &card);
signals:
    void playerStatusChanged(Player* player, PlayerStatus status);
    void notifyGrabLordBet(Player* player, int bet, bool flag);
    void gameStatusChanged(GameStatus status);
    void notifyPlayHand(Player* player, const Cards& card);
    void pendingInfo(Player* player, const Cards& card);

private:
    Robot* m_robotLeft = nullptr;
    Robot* m_robotRight = nullptr;
    UserPlayer* m_user = nullptr;
    Player* m_currPlayer = nullptr;
    Player* m_pendPlayer = nullptr;
    Cards m_pendCards;
    Cards m_allCards;
    BetRecord m_betRecord;
    int m_curBet = 0;
};

#endif // GAMECONTROL_H
