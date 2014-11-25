#ifndef POKERCARD_H
#define POKERCARD_H
#include "pokercardgraphicssvgitem.h"
#include <QSvgRenderer>

enum Suit {NotKnownSuit = -3, BlueBack, RedJoker, BlackJoker, Spade, Heart, Diamond, Club};
enum Status {PreGame, InHands, OutHands};
enum Rank {NotKnownRank = 0, Ace = 1, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King};

class PokerCard
{
public:
    PokerCard();

    Rank rank() const;
    void setRank(const Rank &rank);

    char owner() const;
    void setOwner(char owner);

    Suit suit() const;
    void setSuit(const Suit &suit);


    Status status() const;
    void setStatus(const Status &status);

    QSvgRenderer *renderer() const;
    bool isUniqueRenderer() const;

    PokerCardGraphicsSvgItem *item() const;
    void setItem(PokerCardGraphicsSvgItem *item);

    void loadImage();
    void loadImageByRenderer(QSvgRenderer *);

private:
    bool isUniqueRenderer_;
    Rank rank_;
    char owner_;   //0 if it has no owner
    Suit suit_;
    Status status_;
    PokerCardGraphicsSvgItem *item_;
    QSvgRenderer *renderer_;

};

#endif // POKERCARD_H
