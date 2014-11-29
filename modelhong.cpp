#include "modelhong.h"

ModelHong::ModelHong()
{
}

bool ModelHong::isModel(ModelHong::CardList &list)
{
    if(list.length() == 3)
    {
        if(list.at(0)->rank() == list.at(1)->rank() && list.at(0)->rank() == list.at(2)->rank())
            return true;

    }
    return false;
}

bool ModelHong::compareModel(const ModelHong::CardList &v1, const ModelHong::CardList &v2)
{
    PokerCardItem* c1 = v1.at(0);
    PokerCardItem* c2 = v2.at(0);

    int t1 = c1->rank();
    int t2 = c2->rank();

    if(t1 == 1 || t1 == 2)
        t1 += 13;
    if(t2 == 1 || t2 == 2)
        t2 += 13;

    return t1>t2;

}
