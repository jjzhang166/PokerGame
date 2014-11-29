#include "modeldan.h"

ModelDan::ModelDan()
{
}

bool ModelDan::isModel(ModelDan::CardList &list)
{
    if(list.length() == 1)
        return true;
    else
        return false;
}

bool ModelDan::compareModel(const ModelDan::CardList &v1, const ModelDan::CardList &v2)
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
