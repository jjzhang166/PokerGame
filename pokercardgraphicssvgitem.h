#ifndef POKERCARDGRAPHICSSVGITEM_H
#define POKERCARDGRAPHICSSVGITEM_H

#include <QGraphicsItem>
#include <QGraphicsSvgItem>

class PokerCardGraphicsSvgItem : public QGraphicsSvgItem
{

public:
    explicit PokerCardGraphicsSvgItem(QGraphicsItem *parent = 0);

    enum {Type = UserType + 5};

    int type() const
    {
        return Type;
    }

    bool isClicked() const;
    void setClicked(bool val);

    bool isBackShow() const;
    void setBackShow(bool val);

private:
    bool isClicked_;
    bool isBackShow_;

signals:

public slots:

};

#endif // POKERCARDGRAPHICSSVGITEM_H
