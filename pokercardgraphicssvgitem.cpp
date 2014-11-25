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
bool PokerCardGraphicsSvgItem::isBackShow() const
{
    return isBackShow_;
}

void PokerCardGraphicsSvgItem::setBackShow(bool val)
{
    isBackShow_ = val;
}







