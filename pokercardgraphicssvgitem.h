#ifndef POKERCARDGRAPHICSSVGITEM_H
#define POKERCARDGRAPHICSSVGITEM_H

#include <QGraphicsItem>
#include <QGraphicsSvgItem>

class PokerCardGraphicsSvgItem : public QGraphicsSvgItem
{
    Q_OBJECT
public:
    explicit PokerCardGraphicsSvgItem(QGraphicsItem *parent = 0);

    enum {Type = UserType + 5};



    bool isClicked() const;
    void setClicked(bool val);

    bool isBack() const;
    void setBack(bool val);

private:
    bool isClicked_;
    bool isBack_;

signals:

public slots:

};

#endif // POKERCARDGRAPHICSSVGITEM_H
