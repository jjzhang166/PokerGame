#include "pokerscene.h"
#include "pokercarditem.h"
#include <QGraphicsItem>
#include <QDebug>
#include <Qt>


PokerScene::PokerScene(QObject *parent) :
    QGraphicsScene(parent)
{
}

void PokerScene::mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    if(mouseEvent->button() == Qt::LeftButton)
    {
        QGraphicsItem *item = this->itemAt(mouseEvent->scenePos(), QTransform());
        if(!item) return;
        if(item->type() == QGraphicsItem::UserType + 1 )
        {
            PokerCardItem *svgitem;
            svgitem = qgraphicsitem_cast<PokerCardItem *>(item);
            if(svgitem->clickable() == true)
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
            if(item->data(0).toString() == "Play")
                emit playCards();
            if(item->data(0).toString() == "Pass")
                emit passRound();
        }
    }
    else if (mouseEvent->button() == Qt::RightButton)
    {
        emit playCards();
    }
}

