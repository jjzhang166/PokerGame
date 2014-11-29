#ifndef MODELDUI_H
#define MODELDUI_H
#include "pokercarditem.h"
#include <QList>

class ModelDui
{
    typedef QList<PokerCardItem*> CardList;
public:
    ModelDui();
    static bool isModel(CardList &list);
    static bool compareModel(const CardList &v1, const CardList &v2);
};

#endif // MODELDUI_H
