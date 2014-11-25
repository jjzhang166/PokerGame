#include "pokercardgraphicssvgitem.h"


PokerCardGraphicsSvgItem::PokerCardGraphicsSvgItem(QGraphicsItem *parent): QGraphicsSvgItem(parent)
{

}

bool PokerCardGraphicsSvgItem::isClicked() const
{
    return isClicked_;
}

void PokerCardGraphicsSvgItem::setClicked(bool val)
{
    isClicked_ = val;
}
bool PokerCardGraphicsSvgItem::isBack() const
{
    return isBack_;
}

void PokerCardGraphicsSvgItem::setBack(bool val)
{
    isBack_ = val;
}







