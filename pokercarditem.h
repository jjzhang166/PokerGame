#ifndef POKERCARDITEM_H
#define POKERCARDITEM_H

#include <QGraphicsSvgItem>
#include <QSvgRenderer>

enum Suit {NotKnownSuit = -3, Back, RedJoker, BlackJoker, Spade, Heart, Diamond, Club};
enum Status {PreGame, InHands, OutHands};
enum Rank {NotKnownRank = 0, Ace = 1, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King};


class PokerCardItem : public QGraphicsSvgItem
{



public:
    enum { Type = UserType + 1 };
    int type() const
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

    explicit PokerCardItem(QGraphicsSvgItem *parent = 0);
    ~PokerCardItem();
    int cardID() const;
    void setCardID(int cardID);

    bool getValid() const;


    Rank rank() const;
    Suit suit() const;

    QSvgRenderer *renderer() const;


    void loadImage();
    void loadImageByRenderer(QSvgRenderer *);

    bool clickable() const;
    void setClickable(bool clickable);

    static bool compareCard(const PokerCardItem* c1, const PokerCardItem* c2);

    bool isClicked() const;
    void setClicked(bool value);

private:

    void setValid(bool value);
    void setRank(const Rank &rank);
    void setSuit(const Suit &suit);
    void setRenderer(QSvgRenderer *renderer);

private:
    int cardID_;
    bool valid;
    Rank rank_;
    int owner_;   //0 if it has no owner
    Suit suit_;
    Status status_;
    QSvgRenderer *renderer_;
    bool clickable_;
    bool clicked;
};

#endif // POKERCARDITEM_H
