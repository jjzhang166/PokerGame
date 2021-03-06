#include "modelliandui.h"

ModelLianDui::ModelLianDui()
{
}

bool ModelLianDui::isModel(ModelLianDui::CardList &list)
{
    if(list.length() < 6 || list.length() % 2 != 0)
        return false;

    qSort(list.begin(), list.end(), lessThan);
    int start = list.at(0)->rank();
    if(start < 2)
        return false;
    for(int i = 1; i < list.length(); i++)
    {
        int r = list.at(i)->rank();
        if(r == 1)
            r+=13;
        if( r - start != (i/2))
            return false;
    }
    return true;
}

bool ModelLianDui::compareModel(const ModelLianDui::CardList &v1, const ModelLianDui::CardList &v2)
{
    if(v1.length() != v2.length())
        return false;
    else
    {
        int t1 = v1.at(0)->rank();
        int t2 = v2.at(0)->rank();
        return t1>t2;
    }
}
bool ModelLianDui::lessThan(const PokerCardItem* p1, const PokerCardItem* p2)
{
    int t1 = p1->rank();
    int t2 = p2->rank();
    if(t1 == 1)
        t1 += 13;
    if(t2 == 1)
        t2 += 13;
    return t1<t2;
}
