#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>

#include <cards.h>

class Player :public QObject
{
    Q_OBJECT
public:
    enum Role{Lord, Farmer};    // 角色
    enum Sex{Man, Woman};       // 性别
    enum Direction{Left, Right};    // 头像的显示方位
    enum Type{Robot, User, UnKnow}; // 玩家的类型
public:
    explicit Player(QObject *parent = nullptr);
    explicit Player(QString name,QObject *parent = nullptr);

public:

    void setName(QString name) {m_name=name;}
    QString getName()const {return m_name;}

    void setRole(Role role){m_role = role;}
    Role getRole() const {return m_role;}

    void setSex(Sex sex){m_sex=sex;}
    Sex getSex() const {return m_sex;}

    void setDirection(Direction direction){m_direction = direction;}
    Direction getDirection() const {return m_direction;}

    void setType(Type type){m_type = type;}
    Type getType()const {return m_type;}

    void setScore(int score) {m_score = score;}
    int getScore() const {return m_score;}

    void setWin(bool flag) {m_isWin = flag;}
    bool isWin() const {return m_isWin;}

    void setPrevPlayer(Player* player) {m_prev = player;}
    void setNextPlayer(Player* player) {m_next = player;}
    Player* getPrevPlayer() const {return m_prev;}
    Player* getNextPlayer() const  {return m_next;}

    void grabLordBet(int point);

    void storeDispatchCard(const Card& card);
    void storeDispatchCard(const Cards& cards);

    Cards getCards(){return m_cards;}
    void clearCards() {m_cards.clear();}
    void playHand(const Cards& cards);

    Player* getPendPlayer() const {return m_pendPlayer;}
    Cards getPendCards() const  {return m_pendCards;}

    void storePendingInfo(Player* player, const Cards& cards);

    virtual void prepareCallLord(){}
    virtual void preparePlayHand(){}
    virtual void thinkCallLord(){}
    virtual void thinkPlayHand(){}

signals:
    void notifyGrabLordBet(Player* player,int bet);
    void notifyPlayHand(Player* player,const Cards& cards);
    void notifyPickCards(Player* player,const Cards& cards);

protected:
    int m_score = 0;
    QString m_name;
    Role m_role;
    Sex m_sex;
    Direction m_direction;
    Type m_type;
    bool m_isWin = false;
    Player* m_prev = nullptr;
    Player* m_next = nullptr;
    Cards m_cards;
    Cards m_pendCards; // 轮到该玩家时，当前牌桌上的最大牌
    Player* m_pendPlayer = nullptr; //轮到该玩家时，打出当前最大牌的玩家
};

#endif // PLAYER_H
