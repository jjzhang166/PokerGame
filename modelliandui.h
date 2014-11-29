#ifndef MODELLIANDUI_H
#define MODELLIANDUI_H


#include "pokercarditem.h"
#include <QList>

class ModelLianDui
{
    typedef QList<PokerCardItem*> CardList;
public:
    ModelLianDui();
    static bool isModel(CardList &list);
    static bool compareModel(const CardList &v1, const CardList &v2);
private:
    static bool lessThan(const PokerCardItem *p1, const PokerCardItem *p2);
};
#endif // MODELLIANDUI_H
