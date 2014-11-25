#include "pokercard.h"

PokerCard::PokerCard()
{
}
Rank PokerCard::rank() const
{
    return rank_;
}

void PokerCard::setRank(const Rank &rank)
{
    rank_ = rank;
}
char PokerCard::owner() const
{
    return owner_;
}

void PokerCard::setOwner(char owner)
{
    owner_ = owner;
}
Suit PokerCard::suit() const
{
    return suit_;
}

void PokerCard::setSuit(const Suit &suit)
{
    suit_ = suit;
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

    item_ = new PokerCardGraphicsSvgItem;
    renderer_ = new QSvgRenderer(strName);
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








