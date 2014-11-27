#ifndef POKERCARD_H
#define POKERCARD_H
#include "pokercardgraphicssvgitem.h"
#include <QSvgRenderer>
#include <QObject>

enum Suit {NotKnownSuit = -3, BlueBack, RedJoker, BlackJoker, Spade, Heart, Diamond, Club};
enum Status {PreGame, InHands, OutHands};
enum Rank {NotKnownRank = 0, Ace = 1, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King};

class PokerCard : public QObject
{
    Q_OBJECT
public:

    explicit PokerCard(int num = -1, QObject *parent = 0);


    Rank rank() const;
    //

    char owner() const;
    void setOwner(char owner);

    Suit suit() const;
    //


    Status status() const;
    void setStatus(const Status &status);

    QSvgRenderer *renderer() const;
    bool isUniqueRenderer() const;

    PokerCardGraphicsSvgItem *item() const;
    void setItem(PokerCardGraphicsSvgItem *item);

    void loadImage();
    void loadImageByRenderer(QSvgRenderer *);

    static bool compareCard(const PokerCard* c1, const PokerCard* c2);
    int cardID() const;

private:
    void setRank(const Rank &rank);
    void setSuit(const Suit &suit);
private:
    int cardID_;
    bool isUniqueRenderer_;
    Rank rank_;
    int owner_;   //0 if it has no owner
    Suit suit_;
    Status status_;
    PokerCardGraphicsSvgItem *item_;
    QSvgRenderer *renderer_;


};

#endif // POKERCARD_H
