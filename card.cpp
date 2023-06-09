#include "card.h"



Card &Card::operator=(const Card &c)
{
    if (this==&c){
        return *this;
    }
    this->m_point=c.point();
    this->m_suit=c.suit();
    return *this;
}



bool cardLess(const Card &c1, const Card &c2)
{
    if(c1.point() == c2.point())
        return c1.suit()<c2.suit();
    return c1.point()<c2.point();
}

bool cardEqual(const Card &c1, const Card &c2)
{
    return c1.point()==c2.point()&&c1.suit()==c2.suit();
}

bool cardGreater(const Card &c1,const Card &c2){
    return cardLess(c2,c1);
}

bool operator<(const Card& c,const Card& c2){
    return cardLess(c,c2);
}

bool operator==(const Card& c,const Card& c2){
    return cardEqual(c,c2);
}

bool operator>(const Card& c,const Card& c2){
    return cardGreater(c,c2);
}

unsigned int qHash(const Card &card)
{
       return card.point() * 100+card.suit();
}
