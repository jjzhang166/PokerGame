#ifndef MODELDAN_H
#define MODELDAN_H
#include "pokercarditem.h"
#include <QList>



class ModelDan
{
    typedef QList<PokerCardItem*> CardList;
public:
    ModelDan();
    static bool isModel(CardList &list);
    static bool compareModel(const CardList &v1, const CardList &v2);
};

#endif // MODELDAN_H
