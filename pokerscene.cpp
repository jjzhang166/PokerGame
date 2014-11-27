#include "pokerscene.h"
#include "pokercardgraphicssvgitem.h"
#include <QGraphicsItem>
#include <QDebug>
PokerScene::PokerScene(QObject *parent) :
    QGraphicsScene(parent)
{
}

void PokerScene::mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    //qDebug() << mouseEvent->scenePos();
    QGraphicsItem *item = this->itemAt(mouseEvent->scenePos(), QTransform());
    qDebug() << mouseEvent->scenePos();
    if(!item) return;
    if(item->type() > PokerCardGraphicsSvgItem::UserType)
    {
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
    if(item->type() == QGraphicsSvgItem::Type)
    {
        qDebug() << "SVGITEM";
        emit playCards();
    }
}

