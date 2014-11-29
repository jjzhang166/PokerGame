#include "modelzha.h"

ModelZha::ModelZha()
{
}

bool ModelZha::isModel(ModelZha::CardList &list)
{
    if(list.length() == 4)
    {
        Rank firstRank = list.at(0)->rank();
        for(int i = 1; i < 4; i++)
        {
            if(list.at(i)->rank() != firstRank)
                return false;
        }
        return true;
    }
    return false;
}

bool ModelZha::compareModel(const ModelZha::CardList &v1, const ModelZha::CardList &v2)
{
    PokerCardItem* c1 = v1.at(0);
    PokerCardItem* c2 = v2.at(0);

    int t1 = c1->rank();
    int t2 = c2->rank();
    if(t1 == 1 || t1 == 2)
        t1 += 13;
    if(t2 == 1 || t2 == 2)
        t2 += 13;
    return t1 > t2;

}
