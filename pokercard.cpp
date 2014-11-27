#include "pokercard.h"
#include <QtDebug>



PokerCard::PokerCard(int cardNum, QObject *parent):
    QObject(parent)
{
    if(cardNum == -1)
    {
        setSuit(NotKnownSuit);
        setRank(NotKnownRank);
    }
    if(cardNum == 52)
    {
        setSuit(BlackJoker);
    }
    else if(cardNum == 53)
    {
        setSuit(RedJoker);
    }
    else
    {
        setRank(Rank(cardNum % 13 + 1));
        setSuit(Suit(cardNum / 13 + 1));
    }
    cardID_ = cardNum;
    item_ = NULL;
    renderer_ = NULL;

}


Rank PokerCard::rank() const
{
    return rank_;
}


char PokerCard::owner() const
{
    return owner_;
}
void PokerCard::setRank(const Rank &rank)
{
    rank_ = rank;
}

void PokerCard::setSuit(const Suit &suit)
{
    suit_ = suit;
}

void PokerCard::setOwner(char owner)
{
    owner_ = owner;
}
Suit PokerCard::suit() const
{
    return suit_;
}



PokerCardGraphicsSvgItem *PokerCard::item() const
{
    return item_;
}

void PokerCard::setItem(PokerCardGraphicsSvgItem *item)
{
    item_ = item;
}
Status PokerCard::status() const
{
    return status_;
}

void PokerCard::setStatus(const Status &status)
{
    status_ = status;
}

void PokerCard::loadImage()
{
    QString strName, strSuit;

    if(suit_ == NotKnownSuit)
    {
        strName = QString(":/cards/p_back.svg");
    }
    else if(suit_ == BlueBack)
    {
        strName = QString(":/cards/p_back.svg");
    }
    else if(suit_ == RedJoker)
    {
        strName = QString(":/cards/red_joker.svg");
    }
    else if(suit_ == BlackJoker)
    {
        strName = QString(":/cards/black_joker.svg");
    }
    else
    {
        switch (suit_) {
        case Spade:
            strSuit = "spades";
            break;
        case Heart:
            strSuit = "hearts";
            break;
        case Diamond:
            strSuit = "diamonds";
            break;
        case Club:
            strSuit = "clubs";
            break;
        default:
            break;
        }
        strName = QString(":/cards/%1_of_%2.svg").arg(rank_).arg(strSuit);
    }

    item_ = new PokerCardGraphicsSvgItem();
    item_->setParent(this);
    renderer_ = new QSvgRenderer(strName, this);
    item_->setSharedRenderer(renderer_);
    isUniqueRenderer_ = true;
    item_->setBackShow(false);


}

void PokerCard::loadImageByRenderer(QSvgRenderer *renderer)
{
    if(item_)
        delete item_;
    item_ = new PokerCardGraphicsSvgItem;
    item_->setSharedRenderer(renderer);
    isUniqueRenderer_ = false;
    item_->setBackShow(true);

}

bool PokerCard::compareCard(const PokerCard* c1, const PokerCard* c2)
{

    if(c1->cardID() > 51 || c2->cardID() > 51)
        return c1->cardID() > c2->cardID();
    else
    {
        int t1 = c1->rank();
        int t2 = c2->rank();
        if(t1 == 1 || t1 == 2)
            t1 += 13;
        if(t2 == 1 || t2 == 2)
            t2 += 13;
        if(t1 == t2)
        {
            return c1->suit() > c2->suit();
        }
        else
            return t1 > t2;
    }



}
int PokerCard::cardID() const
{
    return cardID_;
}










