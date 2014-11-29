#include "modeldui.h"

ModelDui::ModelDui()
{
}

bool ModelDui::isModel(ModelDui::CardList &list)
{
    if(list.length() == 2)
    {
        if(list.at(0)->rank() == list.at(1)->rank())
            return true;
    }
    return false;
}

bool ModelDui::compareModel(const ModelDui::CardList &v1, const ModelDui::CardList &v2)
{
    PokerCardItem* c1 = v1.at(0);
    PokerCardItem* c2 = v2.at(0);


    if(c1->cardID() > 51 || c2->cardID() > 51)
        return c1->cardID() > c2->cardID();
    else
    {
        int t1 = c1->rank();
        int t2 = c2->rank();
        if(t1 == 1 || t1 == 2)
            t1 += 13;
        if(t2 == 1 || t2 == 2)
            t2 += 13;
        return t1 > t2;
    }
}
