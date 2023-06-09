#include "cards.h"

#include <QRandomGenerator>
#include <QDebug>

Cards::Cards()
{

}

void Cards::add(const Card& card){
    m_cards.insert(card);
}

void Cards::add(const Cards& cs){
    m_cards.unite(cs.cardSet());
}

void Cards::add(const QVector<Cards> &cs){
    foreach(const Cards &c,cs){
        add(c);
    }
}

Cards & Cards::operator <<(const Card& card){
    add(card);
    return *this;
}
Cards & Cards::operator <<(const Cards& cards){
    add(cards);
    return *this;
}

void Cards::remove(const Card& card){
    m_cards.remove(card);
}

void Cards::remove(const Cards& cards){
    m_cards.subtract(cards.cardSet());
}

void Cards::remove(const QVector<Cards>& cards){
    foreach(const Cards &c,cards){
        remove(c);
    }
}

int Cards::cardCount() const {
    return m_cards.size();
}

bool Cards::isEmpty() const{
    return m_cards.isEmpty();
}

void Cards::clear(){
    m_cards.clear();
}

Card::CardPoint Cards::maxPoint(){
    Card::CardPoint maxPoint = Card::Card_Begin;
    if (!isEmpty()){
        foreach(const Card &c,m_cards){
            maxPoint=std::max(maxPoint,c.point());
        }
    }
    return maxPoint;
}
Card::CardPoint Cards::minPoint(){
    Card::CardPoint minPoint = Card::Card_End;
    if (!isEmpty()){
        foreach(const Card &c,m_cards){
            minPoint=std::min(minPoint,c.point());
        }
    }
    return minPoint;
}

int Cards::pointCount(Card::CardPoint point){
    int count =0;
    for (QSet<Card>::iterator iter = m_cards.begin();iter!=m_cards.end();++iter){
        if (iter->point()==point)
            ++count;
    }
    return count;
}


bool Cards::contains(const Card& card){
    return m_cards.contains(card);
}
bool Cards::contains(const Cards& cards){
    return m_cards.contains(cards.m_cards);
}

Card Cards::takeRandomCard(){
    int num = QRandomGenerator::global()->bounded(m_cards.size());
    auto iter = m_cards.begin();
    for (;num>0;--num,++iter);
    Card c = *iter;
    m_cards.erase(iter);
    return c;
}

CardList Cards::toCardList(SortType type){
    CardList list;
    foreach (const Card &c,m_cards){
        list.push_back(c);
    }

    if (type == SortType::Asc){
        std::sort(list.begin(),list.end(),cardLess);
    }

    if (type ==SortType::Desc){
        std::sort(list.begin(),list.end(),cardGreater);
    }
    return list;
}

void Cards::printAllCardInfo(){
    QString text;
    QVector<QString> suitMsg{"方片","梅花","红桃","黑桃"};
    QString pointMsg="JQKA2";
    for(auto it = m_cards.begin(); it != m_cards.end(); ++it)
    {
        QString msg;
        Card::CardPoint pt = it->point();
        Card::CardSuit suit = it->suit();

        if (suit==Card::Joker){
            if (pt==Card::Card_SJ)
                msg= "Small Joker";
            else
                msg= "Big Joker";
        }
        else{
            msg = suitMsg[suit-Card::Diamond];
            if (pt>=Card::Card_3&&pt<=Card::Card_10){
                msg += QString::asprintf("%d",pt-Card::Card_3+3);
            }
            else{
                msg += pointMsg[pt-Card::Card_J];
            }
        }
        msg += "  ";
        text += msg;
    }
    qDebug() << text;
}

