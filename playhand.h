#ifndef PLAYHAND_H
#define PLAYHAND_H

#include  "card.h"
#include "cards.h"
class PlayHand
{
public:
    enum HandType{
        //unuse extra
        Hand_Unknown,
        Hand_Pass,

        Hand_Single,
        Hand_Pair,

        Hand_Triple,
        Hand_Triple_Single,
        Hand_Triple_Pair,

        Hand_Quadruple_Two_Single,

        //use extra
        Hand_Plane,
        Hand_Plane_X_Single,
        Hand_Plane_X_Pair,

        Hand_Seq_Pair,
        Hand_Seq_Single,

        //bomb
        Hand_Bomb,
        Hand_Bomb_Jokers
    };

public:
    PlayHand();
    explicit PlayHand(Cards& cards);
    explicit PlayHand(HandType type,Card::CardPoint pt,int extra);

public:
    HandType getType() const {return m_type;}
    Card::CardPoint getPoint() const {return m_point;}
    int getExtra() const  {return m_extra;}

    bool canBeat(const PlayHand& other);

private:
    void classify(Cards& cards);
    void judgeCardType();
    void handlePass();
    void handleSingle();
    void handlePair();
    void handleTriple();
    void handleTripleSingle();
    void handleTriplePair();
    void handlePlane();
    void handlePlaneXSingle();
    void handlePlaneXPair();
    void handleSeqPair();
    void handleSeqSingle();
    void handleBomb();
    void handleQuadrupleTwoSingle();
    void handleBombJokers();


private:
    HandType m_type;
    Card::CardPoint m_point;//用于判断同牌型大小
    int m_extra;//用于判断是否同牌型的额外条件

    QVector<Card::CardPoint> m_singleCard;
    QVector<Card::CardPoint> m_twoSameCard;
    QVector<Card::CardPoint> m_threeSameCard;
    QVector<Card::CardPoint> m_fourSameCard;



};

#endif // PLAYHAND_H
