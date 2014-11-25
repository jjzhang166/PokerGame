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









