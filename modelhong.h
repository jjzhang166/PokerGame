#ifndef MODELHONG_H
#define MODELHONG_H

#include "pokercarditem.h"
#include <QList>

class ModelHong
{
    typedef QList<PokerCardItem*> CardList;
public:
    ModelHong();
    static bool isModel(CardList &list);
    static bool compareModel(const CardList &v1, const CardList &v2);
};

#endif // MODELHONG_H
