#ifndef MODELZHA_H
#define MODELZHA_H
#include "pokercarditem.h"
#include <QList>

class ModelZha
{
    typedef QList<PokerCardItem*> CardList;
public:
    ModelZha();
    static bool isModel(CardList &list);
    static bool compareModel(const CardList &v1, const CardList &v2);
};

#endif // MODELZHA_H
