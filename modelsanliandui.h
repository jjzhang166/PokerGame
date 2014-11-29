#ifndef MODELSANLIANDUI_H
#define MODELSANLIANDUI_H


#include "pokercarditem.h"
#include <QList>

class ModelSanLianDui
{
    typedef QList<PokerCardItem*> CardList;
public:
    ModelSanLianDui();
    static bool isModel(CardList &list);
    static bool compareModel(const CardList &v1, const CardList &v2);
private:
    static bool lessThan(const PokerCardItem *p1, const PokerCardItem *p2);

};

#endif // MODELSANLIANDUI_H
