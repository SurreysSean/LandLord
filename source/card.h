#ifndef CARD_H
#define CARD_H

#include <cstdint>

class Card
{
public:


    enum  CardSuit{
        Suit_Begin=0,
        Diamond,
        Club,
        Heart,
        Spade,
        Joker,
        Suit_End
    };

    enum  CardPoint{
        Card_Begin=0,
        Card_3,
        Card_4,
        Card_5,
        Card_6,
        Card_7,
        Card_8,
        Card_9,
        Card_10,
        Card_J,
        Card_Q,
        Card_K,
        Card_A,
        Card_2,
        Card_SJ,
        Card_BJ,
        Card_End
    };


public:

    Card(CardPoint p=Card_End,CardSuit s=Suit_End):m_point(p),m_suit(s){}
    Card(const Card& c):m_point(c.point()),m_suit(c.suit()){}
    Card& operator=(const Card&c);


public:
    void setPoint(CardPoint p) {m_point=p;}
    void setSuit(CardSuit s){m_suit=s;}
    CardPoint point() const{return m_point;}
    CardSuit suit() const {return m_suit;}



private:
    CardPoint m_point;
    CardSuit m_suit;

};

bool cardLess(const Card& c1,const Card &c2);
bool cardEqual(const Card& c1,const Card &c2);
bool cardGreater(const Card &c1,const Card &c2)	;
bool operator<(const Card& c,const Card& c2);
bool operator==(const Card& c,const Card& c2);
bool operator>(const Card& c,const Card& c2);
unsigned int qHash(const Card& card);

#endif // CARD_H
