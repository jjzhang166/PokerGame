#ifndef MODELLONG_H
#define MODELLONG_H

#include "pokercarditem.h"
#include <QList>
#include <QtAlgorithms>

class ModelLong
{
    typedef QList<PokerCardItem*> CardList;
public:
    ModelLong();
    static bool isModel(CardList &list);
    static bool compareModel(const CardList &v1, const CardList &v2);
private:
    static bool lessThan(const PokerCardItem* p1, const PokerCardItem*);
};

#endif // MODELLONG_H
