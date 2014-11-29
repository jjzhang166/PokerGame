#include "pokercarditem.h"



PokerCardItem::PokerCardItem(QGraphicsSvgItem *parent) :
    QGraphicsSvgItem(parent)
{
    renderer_ = NULL;
}

PokerCardItem::~PokerCardItem()
{
    if(cardID_ < 54 && renderer_)
    {
        delete renderer_;
        renderer_ = NULL;
    }
}
int PokerCardItem::cardID() const
{
    return cardID_;
}

void PokerCardItem::setCardID(int cardNum)
{
    if(cardNum == -1)
    {
        setSuit(NotKnownSuit);
        setRank(NotKnownRank);
    }
    if(cardNum == 52)
    {
        setSuit(BlackJoker);
        setRank(NotKnownRank);
    }
    else if(cardNum == 53)
    {
        setSuit(RedJoker);
        setRank(NotKnownRank);
    }
    else if(cardNum == 54)
    {
        setSuit(Back);
        setRank(NotKnownRank);
    }
    else
    {
        setRank(Rank(cardNum % 13 + 1));
        setSuit(Suit(cardNum / 13 + 1));
    }
    cardID_ = cardNum;
    renderer_ = NULL;

    if(cardNum >= 0 && cardNum <= 54)
        setValid(true);
    else
        setValid(false);


}
bool PokerCardItem::getValid() const
{
    return valid;
}

void PokerCardItem::setValid(bool value)
{
    valid = value;
}
Rank PokerCardItem::rank() const
{
    return rank_;
}

void PokerCardItem::setRank(const Rank &rank)
{
    rank_ = rank;
}
Suit PokerCardItem::suit() const
{
    return suit_;
}

void PokerCardItem::setSuit(const Suit &suit)
{
    suit_ = suit;
}
QSvgRenderer *PokerCardItem::renderer() const
{
    return renderer_;
}

void PokerCardItem::loadImage()
{
    if(!getValid())
        return;

    QString strName, strSuit;

    if(suit_ == Back)
    {
        strName = QString(":/cards/back.svg");
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


    renderer_ = new QSvgRenderer(strName, this);
    this->setSharedRenderer(renderer_);



}

void PokerCardItem::loadImageByRenderer(QSvgRenderer *r)
{
    if(renderer_)
        delete renderer_;
    renderer_ = r;
    this->setSharedRenderer(r);

}

void PokerCardItem::setRenderer(QSvgRenderer *renderer)
{
    renderer_ = renderer;
}
bool PokerCardItem::isClicked() const
{
    return clicked;
}

void PokerCardItem::setClicked(bool value)
{
    clicked = value;
}

bool PokerCardItem::clickable() const
{
    return clickable_;
}

void PokerCardItem::setClickable(bool clickable)
{
    clickable_ = clickable;
}

bool PokerCardItem::compareCard(const PokerCardItem *c1, const PokerCardItem *c2)
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






