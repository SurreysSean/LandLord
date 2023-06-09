#ifndef STRATEGY_H
#define STRATEGY_H

#include "player.h"
#include "playhand.h"

class Strategy
{
public:
    Strategy(Player* player,const Cards& cards);

    Cards makeStrategy();
    Cards firstPlay();
    Cards getGreaterCards(PlayHand type);
    bool whetherToBeat(Cards& cs);

    Cards findSpecificPointCards(Card::CardPoint point, int count);//在m_cards中获取指定point的牌count张
    QVector<Cards> findCardsByCount(int count);//按点数遍历m_cards中选取数量为count的牌。ex:若count=1，点数4仅有红桃，则选取红桃4；点数5有黑桃和红桃，则不会被选取
    Cards getRangeCards(Card::CardPoint begin, Card::CardPoint end);
    QVector<Cards> findCardType(PlayHand hand, bool beat);

    void pickSeqSingles(QVector<QVector<Cards>> &allSeqRecord, const QVector<Cards> &seqSingle, const Cards& cards);//获取使用顺子拆解m_cards的所有可能方法
    QVector<Cards> pickOptimalSeqSingles(); //选择顺子拆解方法中剩余单牌数量少并且剩余单排点数较大的方法为最优方法

private:
    using function = Cards (Strategy::*)(Card::CardPoint point);
    QVector<Cards> getCards(Card::CardPoint point, int number);
    QVector<Cards> getTripleSingleOrPair(Card::CardPoint begin, PlayHand::HandType type);
    QVector<Cards> getQuadruple2Single(Card::CardPoint begin);
    QVector<Cards> getPlane(Card::CardPoint begin,int extra);
    QVector<Cards> getPlaneXSingleOrXPair(Card::CardPoint begin, PlayHand::HandType type,int extra);
    QVector<Cards> getSepPairOrSeqSingle(Card::CardPoint begin, PlayHand::HandType type,int extra);


private:
    Player* m_player;
    Cards m_cards;
};

#endif // STRATEGY_H
