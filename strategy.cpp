#include "strategy.h"

#include <QMap>



Strategy::Strategy(Player* player,const Cards& cards){
    m_player = player;
    m_cards = cards;
}

Cards Strategy::makeStrategy(){
    Player *pendPlayer = m_player->getPendPlayer();
    Cards pendCards = m_player->getPendCards();

    if (pendPlayer == m_player || pendPlayer == nullptr){
        return firstPlay();
    }
    else{
        PlayHand target(pendCards);
        Cards beatCards = getGreaterCards(target);
        bool shouldBeat = whetherToBeat(beatCards);

        if (shouldBeat){
            return beatCards;
        }
    }
    return Cards();
}

Cards Strategy::firstPlay(){
    PlayHand hand(m_cards);
    if (hand.getType() != PlayHand::Hand_Unknown)
    {
        return m_cards;
    }

    QVector<Cards> optimalSeq = pickOptimalSeqSingles();
    if (!optimalSeq.isEmpty()){
        int baseNum = findCardsByCount(1).size();
        Cards save = m_cards;
        save.remove(optimalSeq);
        int lastNum = Strategy(m_player,save).findCardsByCount(1).size();
        if (baseNum > lastNum){
            return optimalSeq[0];
        }
    }

    bool hasPlane,hasTriple,hasPair;
    hasPair = hasTriple = hasPlane = false;
    Cards backup = m_cards;

    QVector<Cards> bombArray = findCardType(PlayHand(PlayHand::Hand_Bomb,Card::Card_Begin,0),false);
    backup.remove(bombArray);

    QVector<Cards> planeArray = Strategy(m_player,backup).findCardType(PlayHand(PlayHand::Hand_Plane,Card::Card_Begin,0),false);
    if (!planeArray.isEmpty()){
        hasPlane = true;
        backup.remove(planeArray);
    }

    QVector<Cards> seqTripleArray = Strategy(m_player,backup).findCardType(PlayHand(PlayHand::Hand_Triple,Card::Card_Begin,0),false);
    if (!seqTripleArray.isEmpty()){
        hasTriple = true;
        backup.remove(seqTripleArray);
    }

    QVector<Cards> seqPairArray = Strategy(m_player,backup).findCardType(PlayHand(PlayHand::Hand_Pair,Card::Card_Begin,0),false);
    if (!seqPairArray.isEmpty()){
        hasPair = true;
        backup.remove(seqPairArray);
    }

    if (hasPair){
        Cards mimPair=seqPairArray[0];
        for (int i=0;i<seqPairArray.size();++i){
            if (seqPairArray[i].minPoint()<mimPair.minPoint())
                mimPair = seqPairArray[i];
        }
        return mimPair;
    }

    if (hasPlane){
        QVector<Cards> pairArray;
        Cards tmp = planeArray[0];
        int tmpExtra = PlayHand(tmp).getExtra();
        for(Card::CardPoint point = Card::Card_3; point <= Card::Card_BJ; point = Card::CardPoint(point + 1))
        {
            Cards pair = Strategy(m_player, backup).findSpecificPointCards(point, 2);
            if(!pair.isEmpty())
            {
                pairArray.push_back(pair);
                if(pairArray.size() == tmpExtra) {
                    tmp.add(pairArray);
                    return tmp;
                }
            }
        }
        bool twoSingleFound = false;
        QVector<Cards> singleArray;
        for(Card::CardPoint point = Card::Card_3; point <= Card::Card_BJ; point = Card::CardPoint(point + 1))
        {
            if(backup.pointCount(point) == 1)
            {
                Cards single = Strategy(m_player, backup).findSpecificPointCards(point, 1);
                if(!single.isEmpty())
                {
                    singleArray.push_back(single);
                    if(singleArray.size() == 2)
                    {
                        twoSingleFound = true;
                        break;
                    }
                }
            }
        }
        if(twoSingleFound)
        {
            Cards tmp = planeArray[0];
            tmp.add(singleArray);
            return tmp;
        }
        else
        {
            return planeArray[0];
        }
    }

    if (hasTriple){
        if (PlayHand(seqTripleArray[0]).getPoint()<Card::Card_SJ){
            for(Card::CardPoint point = Card::Card_3;point<=Card::Card_SJ;point= static_cast<Card::CardPoint>(point+1)){
                int pointCount = backup.pointCount(point);
                if (pointCount == 1)
                {
                    Cards single = Strategy(m_player, backup).findSpecificPointCards(point, 1);
                    Cards tmp = seqTripleArray[0];
                    tmp.add(single);
                    return tmp;
                }
                else if(pointCount == 2)
                {
                    Cards pair = Strategy(m_player, backup).findSpecificPointCards(point, 2);
                    Cards tmp = seqTripleArray[0];
                    tmp.add(pair);
                    return tmp;
                }
            }
        }
        return seqTripleArray[0];
    }

    Player* nextPlayer = m_player->getNextPlayer();
    if (nextPlayer->getCards().cardCount() == 1 && m_player->getRole() != nextPlayer->getRole())
    {
        for(Card::CardPoint point = Card::CardPoint(Card::Card_End-1);
             point >= Card::Card_3; point = Card::CardPoint(point-1))
        {
            int pointCount = backup.pointCount(point);
            if(pointCount == 1)
            {
                Cards single = Strategy(m_player, backup).findSpecificPointCards(point, 1);
                return single;
            }
            else if(pointCount == 2)
            {
                Cards pair = Strategy(m_player, backup).findSpecificPointCards(point, 2);
                return pair;
            }
        }
    }
    else
    {
        for(Card::CardPoint point =  Card::Card_3;
             point < Card::Card_End; point = Card::CardPoint(point+1))
        {
            int pointCount = backup.pointCount(point);
            if(pointCount == 1)
            {
                Cards single = Strategy(m_player, backup).findSpecificPointCards(point, 1);
                return single;
            }
            else if(pointCount == 2)
            {
                Cards pair = Strategy(m_player, backup).findSpecificPointCards(point, 2);
                return pair;
            }
        }
    }
    return Cards();

}

Cards Strategy::getGreaterCards(PlayHand type){
    Player* pendPlayer = m_player->getPendPlayer();
    if (pendPlayer&&pendPlayer->getRole()!=m_player->getRole() && pendPlayer->getCards().cardCount()<=3){
        QVector<Cards> bombs = findCardsByCount(4);
        for (int i=0;i<bombs.size();++i){
            if (PlayHand(bombs[i]).canBeat(type)){
                return bombs[i];
            }
        }
        Cards sj = findSpecificPointCards(Card::Card_SJ,1);
        Cards bj = findSpecificPointCards(Card::Card_BJ,1);
        if (!sj.isEmpty()&&bj.isEmpty()){
            Cards jokers;
            jokers<<sj<<bj;
            return jokers;
        }

    }
    Player* nextPlayer = m_player->getNextPlayer();
    Cards remain = m_cards;
    remain.remove(Strategy(m_player,remain).pickOptimalSeqSingles());

    auto beatCard = std::bind([=](const Cards & cards){
        QVector<Cards> beatCardsArray = Strategy(m_player, cards).findCardType(type, true);
        if(!beatCardsArray.isEmpty())
        {
            if(m_player->getRole() != nextPlayer->getRole() && nextPlayer->getCards().cardCount() <= 2)
            {
                return beatCardsArray.back();
            }
            else
            {
                return beatCardsArray.front();
            }
        }
        return Cards();
    }, std::placeholders::_1);

    Cards cs ;
    if(!(cs = beatCard(remain)).isEmpty())
    {
        return cs;
    }
    else
    {
        if(!(cs = beatCard(m_cards)).isEmpty()) return cs;
    }
    return Cards();
}

bool Strategy::whetherToBeat(Cards& cs){
    if (cs.isEmpty()){
        return false;
    }

    Player* pendPlayer = m_player->getPendPlayer();
    if (m_player->getRole() == pendPlayer->getRole()){
        Cards left = m_cards;
        left.remove(cs);
        if (PlayHand(left).getType()!=PlayHand::Hand_Unknown){
            return true;
        }
        Card::CardPoint basePoint = PlayHand(cs).getPoint();
        if (basePoint == Card::Card_2 || basePoint == Card::Card_SJ || basePoint == Card::Card_BJ)
        {
            return false;
        }
    }
    else{
        PlayHand myHand(cs);
        if ((myHand.getType()>=PlayHand::Hand_Triple||myHand.getType()<=PlayHand::Hand_Triple_Pair)&& myHand.getPoint()==Card::Card_2){
            return false;
        }
        if(myHand.getType() == PlayHand::Hand_Pair && myHand.getPoint() == Card::Card_2
            && pendPlayer->getCards().cardCount() >= 10 && m_player->getCards().cardCount() >= 5)
        {
            return false;
        }

    }
    return true;

}

Cards Strategy::findSpecificPointCards(Card::CardPoint point, int count){
    if (count <1 || count >4){
        return Cards();
    }

    if (point ==Card::Card_SJ || point ==Card::Card_BJ){
        if (count > 1){
            return Cards();
        }

        Card card;
        card.setSuit(Card::Joker);
        card.setPoint(point);
        if (m_cards.contains(card)){
            Cards cards;
            cards.add(card);
            return cards;
        }
        return Cards();
    }

    int findCount = 0;
    Cards foundCards;
    for (int suit = Card::Suit_Begin+1;suit<Card::Joker;++suit){
        Card card;
        card.setPoint(point);
        card.setSuit(static_cast<Card::CardSuit>(suit));
        if (m_cards.contains(card)){
            ++findCount;
            foundCards.add(card);
            if (findCount == count){
                return foundCards;
            }
        }
    }
    return Cards();
}

QVector<Cards> Strategy::findCardsByCount(int count){
    if (count <1 || count >4){
        return QVector<Cards>();
    }

    QVector<Cards> cardsArray;
    for (Card::CardPoint point = Card::Card_3;point <Card::Card_End; point=static_cast<Card::CardPoint>(point +1)){
        if (m_cards.pointCount(point) == count ){
            Cards cs;
            cs << findSpecificPointCards(point,count);
            cardsArray<<cs;
        }
    }
    return cardsArray;
}

Cards Strategy::getRangeCards(Card::CardPoint begin, Card::CardPoint end){
    Cards rangeCards;
    for(Card::CardPoint point = begin;point<end;point =static_cast<Card::CardPoint>(point+1)){
        int count = m_cards.pointCount(point);
        Cards cs = findSpecificPointCards(point,count);
        rangeCards << cs;
    }
    return rangeCards;
}

QVector<Cards> Strategy::findCardType(PlayHand hand, bool beat){
    PlayHand::HandType type = hand.getType();
    Card::CardPoint pt = hand.getPoint();
    int extra = hand.getExtra();

    Card::CardPoint beginPoint = beat?Card::CardPoint(pt+1):Card::Card_3;
    switch (type){
    case PlayHand::Hand_Single:
        return getCards(beginPoint,1);
    case PlayHand::Hand_Pair:
        return getCards(beginPoint,2);
    case PlayHand::Hand_Triple:
        return getCards(beginPoint,3);
    case PlayHand::Hand_Triple_Single:
        return getTripleSingleOrPair(beginPoint,PlayHand::Hand_Single);
    case PlayHand::Hand_Triple_Pair:
        return getTripleSingleOrPair(beginPoint,PlayHand::Hand_Pair);
    case PlayHand::Hand_Quadruple_Two_Single:

    case PlayHand::Hand_Plane:
        return getPlane(beginPoint,extra);
    case PlayHand::Hand_Plane_X_Single:
        return getPlaneXSingleOrXPair(beginPoint,PlayHand::Hand_Single,extra);
    case PlayHand::Hand_Plane_X_Pair:
        return getPlaneXSingleOrXPair(beginPoint,PlayHand::Hand_Pair,extra);
    case PlayHand::Hand_Seq_Pair:
        return getSepPairOrSeqSingle(beginPoint,PlayHand::Hand_Seq_Pair,extra);
    case PlayHand::Hand_Seq_Single:
        return getSepPairOrSeqSingle(beginPoint,PlayHand::Hand_Seq_Single,extra);
    case PlayHand::Hand_Bomb:
        return getCards(beginPoint,4);
    default:
        return QVector<Cards> ();
    }
}

void Strategy::pickSeqSingles(QVector<QVector<Cards>> &allSeqRecord, const QVector<Cards> &seqSingle, const Cards& cards){
    QVector<Cards> allSeq = Strategy(m_player,cards).findCardType(PlayHand(PlayHand::Hand_Seq_Single,Card::Card_Begin,0),false);
    if (allSeq.isEmpty()){
        allSeqRecord<<seqSingle;
    }
    else{
        Cards saveCards = cards;
        for (int i=0;i<allSeq.size();++i){
            Cards aScheme = allSeq[i];
            Cards temp = saveCards;
            temp.remove(aScheme);

            QVector<Cards> seqArray = seqSingle;
            seqArray <<aScheme;

            pickSeqSingles(allSeqRecord,seqArray,temp);
        }
    }
}

QVector<Cards> Strategy::pickOptimalSeqSingles(){
    QVector<QVector<Cards>> seqRecord;
    QVector<Cards> seqSingles;
    Cards save = m_cards;
    save.remove(findCardsByCount(4));
    save.remove(findCardsByCount(3));

    pickSeqSingles(seqRecord,seqSingles,save);
    if (seqRecord.isEmpty()){
        return QVector<Cards>();
    }

    QMap<int,int> seqMarks;
    for (int i=0;i<seqRecord.size();++i){
        Cards backupCards = m_cards;
        QVector<Cards> seqArray = seqRecord[i];
        backupCards.remove(seqArray);

        QVector<Cards>singleArray = Strategy(m_player,backupCards).findCardsByCount(1);

        CardList cardList;
        for (int j=0;j<cardList.size();++j){
            cardList<<singleArray[j].toCardList();
        }
        int mark =0;
        for (int j=0;j<cardList.size();++j){
            mark += cardList[j].point() +15;
        }
        seqMarks.insert(i,mark);
    }

    int value = 0;
    int comMark = 1000;
    auto it = seqMarks.constBegin();
    for (;it!=seqMarks.constEnd();++it){
        if (it.value() <comMark){
            comMark = it.value();
            value = it.key();
        }
    }

    return seqRecord[value];

}

QVector<Cards> Strategy::getCards(Card::CardPoint point, int number){
    QVector<Cards> retCardsArray;
    for (Card::CardPoint pt = point ;pt<Card::Card_End;pt = static_cast<Card::CardPoint>(pt+1)){
        if (m_cards.pointCount(pt) == number){//>= ?
            Cards cs = findSpecificPointCards(pt,number);
            retCardsArray<<cs;
        }
    }
    return retCardsArray;
}

QVector<Cards> Strategy::getTripleSingleOrPair(Card::CardPoint begin, PlayHand::HandType type){
    QVector<Cards> retCardsArray = getCards(begin,3);
    if (!retCardsArray.empty()){
        Cards remainCards = m_cards;
        remainCards.remove(retCardsArray);
        Strategy st(m_player,remainCards);
        QVector<Cards> cardsArray = st.findCardType(PlayHand(type,Card::Card_Begin,0),false);
        if (cardsArray.empty()){
            retCardsArray.clear();
        }
        else
        {
            for (int i=0;i<retCardsArray.size();++i){
                retCardsArray[i].add(cardsArray.at(i));
            }
        }
    }
    return retCardsArray;

}

QVector<Cards> Strategy::getQuadruple2Single(Card::CardPoint begin)
{
    QVector<Cards> retCardsArray=getCards(begin,4);
    if (!retCardsArray.isEmpty()){

        Cards remainCards = m_cards;
        remainCards.remove(retCardsArray);
        QVector<Cards> singleArray = Strategy(m_player,remainCards).findCardType(PlayHand(PlayHand::Hand_Single,Card::Card_Begin,0),false);
        remainCards.remove(singleArray);
        QVector<Cards> pairArray = Strategy(m_player,remainCards).findCardType(PlayHand(PlayHand::Hand_Pair,Card::Card_Begin,0),false);

        if (singleArray.size()>=2){
            for (int i=0;i<retCardsArray.size();++i){
                Cards tmp;
                tmp<<singleArray[0]<<singleArray[1];
                retCardsArray[i]<<tmp;
            }
        }
        else if (pairArray.size()>=1){
            for (int i=0;i<retCardsArray.size();++i){
                Cards tmp;
                tmp<<pairArray[0];
                retCardsArray[i]<<tmp;
            }
        }
        else{
            retCardsArray.clear();
        }
    }
    return retCardsArray;

}

QVector<Cards> Strategy::getPlane(Card::CardPoint begin,int extra){
    int lowerLimit = 2;
    int upperLimit = Card::Card_2 - begin;
    QVector<Cards> retCardsArray;
    QVector<QVector<Cards>> cards_store(upperLimit,QVector<Cards>(upperLimit));
    QVector<QVector<int>> length_store(upperLimit,QVector<int>(upperLimit,0));

    for (int i=0;i<=upperLimit-lowerLimit;++i){
        for (int j=i;j<upperLimit;++j){
            Card::CardPoint pt = static_cast<Card::CardPoint>(j+begin);
            if (i==j){
                cards_store[i][i] = findSpecificPointCards(begin,3);
                if (!cards_store[i][i].isEmpty()) length_store[i][i]=1;
                continue;
            }
            cards_store[i][j] = findSpecificPointCards(pt,3);
            if (!cards_store[i][j].isEmpty()&&!cards_store[i][j-1].isEmpty()){
                cards_store[i][j]<<cards_store[i][j-1];
                length_store[i][j] = length_store[i][j-1]+1;
            }
        }
    }

    for (int i=0;i<=upperLimit-lowerLimit;++i){
        for (int j=i;j<upperLimit;++j){
            if (!extra&&length_store[i][j]>=lowerLimit){
                retCardsArray<<cards_store[i][j];
            }
            else if(length_store[i][j]==extra){
                retCardsArray<<cards_store[i][j];
            }
        }
    }
    return retCardsArray;
}

QVector<Cards> Strategy::getPlaneXSingleOrXPair(Card::CardPoint begin, PlayHand::HandType type,int extra){
    if (!extra) return QVector<Cards>();
    QVector<Cards> retCardsArray = getPlane(begin,extra);
    if (!retCardsArray.isEmpty()){
        Cards remainCards = m_cards;
        remainCards.remove(retCardsArray);
        Strategy st(m_player,remainCards);
        QVector<Cards> cardsArray = st.findCardType(PlayHand(type,Card::Card_Begin,extra),false);
        if (cardsArray.size()>=extra){
            Cards tmp;
            for (int i=0;i<extra;++i){
                tmp <<cardsArray[i];
            }
            for (int i=0;i<retCardsArray.size();++i){
                retCardsArray[i]<<tmp;
            }
        }
        else
        {
            retCardsArray.clear();
        }
    }
    return retCardsArray;
}

QVector<Cards> Strategy::getSepPairOrSeqSingle(Card::CardPoint begin, PlayHand::HandType type,int extra)
{
    int number = PlayHand::Hand_Seq_Pair-type+2;
    int lowerLimit = 3+2*(type-PlayHand::Hand_Seq_Pair);
    int upperLimit = Card::Card_2 - begin;
    if (upperLimit<lowerLimit) return QVector<Cards> ();

    QVector<Cards> retCardArray;
    QVector<QVector<Cards>> cards_store(upperLimit,QVector<Cards>(upperLimit));
    QVector<QVector<int>> length_store(upperLimit,QVector<int>(upperLimit,0));

    for (int i=0;i<=upperLimit-lowerLimit;++i){
        for (int j=i;j<upperLimit;++j){
            Card::CardPoint pt = static_cast<Card::CardPoint>(j+begin);
            if (i==j){
                cards_store[i][i] = findSpecificPointCards(begin,number);
                if (!cards_store[i][i].isEmpty()) length_store[i][i]=1;
                continue;
            }
            cards_store[i][j] = findSpecificPointCards(pt,number);
            if (!cards_store[i][j].isEmpty()&&!cards_store[i][j-1].isEmpty()){
                cards_store[i][j]<<cards_store[i][j-1];
                length_store[i][j] = length_store[i][j-1]+1;
            }
        }
    }

    for (int i=0;i<=upperLimit-lowerLimit;++i){
        for (int j=i;j<upperLimit;++j){
            if (!extra&&length_store[i][j]>=lowerLimit){
                retCardArray<<cards_store[i][j];
            }
            else if(length_store[i][j]==extra){
                retCardArray<<cards_store[i][j];
            }
        }
    }
    return retCardArray;
}

