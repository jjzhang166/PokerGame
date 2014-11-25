#include "pokerscene.h"
#include "pokercardgraphicssvgitem.h"
#include <QGraphicsItem>
PokerScene::PokerScene(QObject *parent) :
    QGraphicsScene(parent)
{
}

void PokerScene::mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    //qDebug() << mouseEvent->scenePos();
    QGraphicsItem *item = this->itemAt(mouseEvent->scenePos(), QTransform());
    PokerCardGraphicsSvgItem *svgitem;
    svgitem = qgraphicsitem_cast<PokerCardGraphicsSvgItem *>(item);
    if(svgitem && svgitem->type() > PokerCardGraphicsSvgItem::UserType && svgitem->isBackShow() == false)
    {

        QPointF pos = svgitem->pos();
        if(svgitem->isClicked())
        {
            svgitem->setPos(pos.x(), pos.y() + 20);
            svgitem->setClicked(false);
        }
        else
        {
            svgitem->setPos(pos.x(), pos.y() - 20);
            svgitem->setClicked(true);
        }
    }

}
