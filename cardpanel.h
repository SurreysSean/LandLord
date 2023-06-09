#ifndef CARDPANEL_H
#define CARDPANEL_H

#include <QWidget>

#include "card.h"
#include "player.h"

class CardPanel:public QWidget
{
    Q_OBJECT
public:
    explicit CardPanel(QWidget *parent =nullptr);

    void setImage(const QPixmap &front, const QPixmap &back){
        m_front = front;
        m_back = back;

        setFixedSize(m_front.size());

        update();
    }
    QPixmap getImage() const {return m_front;}

    void setFrontSide(bool flag){m_isfront = flag;}
    bool isFrontSide()const {return m_isfront;}

    void setSeclected(bool flag){m_isSelect = flag;}
    bool isSelected()const {return m_isSelect;}

    void setCard(const Card& card){m_card = card;}
    Card getCard()const{ return m_card;}

    void setOwner(Player* player){m_owner = player;}
    Player* getOwner()const {return m_owner;}

    void clicked();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

signals:
    void cardSelected(Qt::MouseButton button);

private:
    QPixmap m_front;
    QPixmap m_back;
    bool m_isfront = true;
    bool m_isSelect = false;
    Card m_card;
    Player* m_owner = nullptr;
};

#endif // CARDPANEL_H
