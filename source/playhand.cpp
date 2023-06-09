#include "playhand.h"

#include <QVector>
PlayHand::PlayHand()
{

}
PlayHand::PlayHand(Cards& cards){
    classify(cards);
    judgeCardType();
}
PlayHand::PlayHand(HandType type,Card::CardPoint pt,int extra){
    m_type = type;
    m_point = pt;
    m_extra = extra;
}


bool PlayHand::canBeat(const PlayHand& other){
    if (m_type == Hand_Unknown)
    {
        return false;
    }
    if (other.getType() == Hand_Pass){
        return true;
    }
    if (m_type == Hand_Bomb_Jokers){
        return true;
    }
    if (m_type == Hand_Bomb && other.getType() < Hand_Bomb){
        return true;
    }
    if (m_type == other.getType() && m_extra == other.getExtra()){
        return m_point > other.getPoint();
    }
    return false;
}


void PlayHand::classify(Cards& cards){
    CardList list = cards.toCardList();
    QVector<int> pointCount(Card::Card_End);

    for(const Card& c:list){
        pointCount[c.point()]++;
    }


    m_singleCard.clear();
    m_twoSameCard.clear();
    m_threeSameCard.clear();
    m_fourSameCard.clear();

    for (int i=Card::Card_Begin;i<Card::Card_End;++i){
        switch (pointCount[i]) {

        case 1:
            m_singleCard.push_back(static_cast<Card::CardPoint>(i));
            break;
        case 2:
            m_twoSameCard.push_back(static_cast<Card::CardPoint>(i));
            break;
        case 3:
            m_threeSameCard.push_back(static_cast<Card::CardPoint>(i));
            break;
        case 4:
            m_fourSameCard.push_back(static_cast<Card::CardPoint>(i));
        }
    }



}

void PlayHand::judgeCardType(){
    m_type = Hand_Unknown;
    m_point = Card::Card_Begin;
    m_extra =0;

    handlePass();
    handleSingle();
    handlePair();
    handleTriple();
    handleTripleSingle();
    handleTriplePair();
    handlePlane();
    handlePlaneXSingle();
    handlePlaneXPair();
    handleSeqPair();
    handleSeqSingle();
    handleBomb();
    handleQuadrupleTwoSingle();
    handleBombJokers();
}


void PlayHand::handlePass(){
    if (m_singleCard.empty()&&m_twoSameCard.empty()&& m_threeSameCard.empty() && m_fourSameCard.empty())
        m_type = Hand_Pass;
}

void PlayHand::handleSingle(){
    bool isOtherEmpty = m_twoSameCard.empty()&& m_threeSameCard.empty() && m_fourSameCard.empty();
    if( m_singleCard.size()==1 && isOtherEmpty){
        m_point = m_singleCard[0];
        m_type = Hand_Single;
    }
}

void PlayHand::handlePair(){

    bool isOtherEmpty = m_singleCard.empty()&& m_threeSameCard.empty() && m_fourSameCard.empty();
    if(m_twoSameCard.size()==1 && isOtherEmpty){
        m_point = m_twoSameCard[0];
        m_type = Hand_Pair;
    }
}

void PlayHand::handleTriple(){
    bool isOtherEmpty = m_singleCard.empty()&& m_twoSameCard.empty() && m_fourSameCard.empty();
    if(m_threeSameCard.size()==1 && isOtherEmpty){
        m_point = m_threeSameCard[0];
        m_type = Hand_Triple;
    }
}

void PlayHand::handleTripleSingle(){
    bool isOtherEmpty = m_twoSameCard.empty() && m_fourSameCard.empty();
    if(m_threeSameCard.size()==1&&m_singleCard.size()==1 && isOtherEmpty){
        m_point = m_threeSameCard[0];
        m_type = Hand_Triple_Single;
    }
}

void PlayHand::handleTriplePair(){
    bool isOtherEmpty = m_singleCard.empty() && m_fourSameCard.empty();
    if(m_threeSameCard.size()==1&&m_twoSameCard.size()==1 && isOtherEmpty){
        m_point = m_threeSameCard[0];
        m_type = Hand_Triple_Pair;
    }
}

void PlayHand::handlePlane(){
    bool isOtherEmpty = m_singleCard.empty()&& m_twoSameCard.empty() && m_fourSameCard.empty();
    bool isValid= false;
    const int threeCardSize = m_threeSameCard.size();
    if(threeCardSize>=2 && isOtherEmpty){
        std::sort(m_threeSameCard.begin(),m_threeSameCard.end());
        isValid = (m_threeSameCard[threeCardSize-1] < Card::Card_2);
        for (int i=1;i<threeCardSize && isValid;++i){
            if (m_threeSameCard[i]-m_threeSameCard[i-1]!=1)
            {
                isValid = false;
            }
        }
    }
    if (isValid){
        m_extra = threeCardSize;
        m_point = m_threeSameCard[0];
        m_type = Hand_Plane;
    }
}

void PlayHand::handlePlaneXSingle(){
    bool isOtherEmpty =  m_twoSameCard.empty() && m_fourSameCard.empty();
    bool isValid= false;
    const int threeCardSize = m_threeSameCard.size();
    if(threeCardSize>=2 && m_singleCard.size()==threeCardSize &&isOtherEmpty){
        std::sort(m_threeSameCard.begin(),m_threeSameCard.end());
        std::sort(m_singleCard.begin(),m_singleCard.end());
        isValid = (m_threeSameCard[threeCardSize-1] < Card::Card_2 && m_singleCard[m_singleCard.size()-1]<Card::Card_SJ);
        for (int i=1;i<threeCardSize && isValid;++i){
            if (m_threeSameCard[i]-m_threeSameCard[i-1]!=1)
            {
                isValid = false;
            }
        }
    }
    if (isValid){
        m_extra = threeCardSize;
        m_point = m_threeSameCard[0];
        m_type = Hand_Plane_X_Single;
    }
}

void PlayHand::handlePlaneXPair(){
    bool isOtherEmpty =  m_singleCard.empty() && m_fourSameCard.empty();
    bool isValid= false;
    const int threeCardSize = m_threeSameCard.size();
    if(threeCardSize>=2 && m_twoSameCard.size()==threeCardSize &&isOtherEmpty){
        std::sort(m_threeSameCard.begin(),m_threeSameCard.end());
        isValid = (m_threeSameCard[threeCardSize-1] < Card::Card_2 );
        for (int i=1;i<threeCardSize && isValid;++i){
            if (m_threeSameCard[i]-m_threeSameCard[i-1]!=1)
            {
                isValid = false;
            }
        }
    }
    if (isValid){
        m_extra = threeCardSize;
        m_point = m_threeSameCard[0];
        m_type = Hand_Plane_X_Pair;
    }
}

void PlayHand::handleSeqPair(){
    bool isOtherEmpty =  m_singleCard.empty()&&m_threeSameCard.empty() && m_fourSameCard.empty();
    bool isValid= false;
    const int twoCardSize = m_twoSameCard.size();
    if(twoCardSize>=3  && isOtherEmpty){
        std::sort(m_twoSameCard.begin(),m_twoSameCard.end());
        isValid = (m_twoSameCard[twoCardSize-1] < Card::Card_2 );
        for (int i=1;i<twoCardSize && isValid;++i){
            if (m_twoSameCard[i]-m_twoSameCard[i-1]!=1)
            {
                isValid = false;
            }
        }
    }
    if (isValid){
        m_extra = twoCardSize;
        m_point = m_twoSameCard[0];
        m_type = Hand_Seq_Pair;
    }
}

void PlayHand::handleSeqSingle(){
    bool isOtherEmpty =  m_twoSameCard.empty()&&m_threeSameCard.empty() && m_fourSameCard.empty();
    bool isValid= false;
    const int oneCardSize = m_singleCard.size();
    if(oneCardSize>=5  && isOtherEmpty){
        std::sort(m_singleCard.begin(),m_singleCard.end());
        isValid = (m_singleCard[oneCardSize-1] < Card::Card_2 );
        for (int i=1;i<oneCardSize && isValid;++i){
            if (m_singleCard[i]-m_singleCard[i-1]!=1)
            {
                isValid = false;
            }
        }
    }
    if (isValid){
        m_extra = oneCardSize;
        m_point = m_singleCard[0];
        m_type =Hand_Seq_Single;
    }
}

void PlayHand::handleBomb(){
    bool isOtherEmpty =  m_singleCard.empty()&&m_twoSameCard.empty()&&m_threeSameCard.empty();
    if (m_fourSameCard.size()==1&&isOtherEmpty){
        m_point = m_fourSameCard.first();
        m_type =Hand_Bomb;
    }
}

void PlayHand::handleQuadrupleTwoSingle(){
    bool isOtherEmpty = m_threeSameCard.empty();
    bool isValid = (m_singleCard.size()==2&&m_twoSameCard.empty())||(m_singleCard.empty()&&m_twoSameCard.size()==1);
    if (m_fourSameCard.size()==1&&isOtherEmpty&&isValid){
        m_point = m_fourSameCard.first();
        m_type =Hand_Quadruple_Two_Single;
    }
}

void PlayHand::handleBombJokers(){
    bool isOtherEmpty =  m_twoSameCard.empty()&&m_fourSameCard.empty()&&m_threeSameCard.empty();
    if (m_singleCard.size()==2&&isOtherEmpty){
        std::sort(m_singleCard.begin(),m_singleCard.end());
        if (m_singleCard.constFirst()==Card::Card_SJ&&m_singleCard.constLast()==Card::Card_BJ)
            m_type =Hand_Bomb_Jokers;
    }
}

