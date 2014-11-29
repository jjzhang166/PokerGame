#include "pokercardmodel.h"

PokerCardModel::PokerCardModel()
{
}

CardModel PokerCardModel::checkModel(CardList &list)
{
    if(ModelDan::isModel(list))
        return Dan;
    if(ModelDui::isModel(list))
        return Dui;
    if(ModelLong::isModel(list))
        return Long;
    if(ModelHong::isModel(list))
        return Hong;
    if(ModelZha::isModel(list))
        return Zha;
    if(ModelLianDui::isModel(list))
        return LianDui;
    if(ModelSanLianDui::isModel(list))
        return SanLianDui;
    return Illegal;
}

bool PokerCardModel::greaterThan(PokerCardModel::CardList &v1, CardModel &model1, PokerCardModel::CardList &v2, CardModel &model2)
{
    switch(model1)
    {
    case Dan:
    {
        if(model2 == Dan)
            return ModelDan::compareModel(v1, v2);
        return false;
    }
    case Dui:
    {
        if(model2 == Dui)
            return ModelDui::compareModel(v1, v2);
        return false;
    }
    case Long:
    {
        if(model2 == Long)
            return ModelLong::compareModel(v1, v2);
        return false;
    }
    case Hong:
    {
        if(model2 == Hong)
            return ModelHong::compareModel(v1, v2);
        else if(int(model2) > int(Hong))
            return false;
        return true;
    }
    case Zha:
    {
        if(model2 == Zha)
            return ModelZha::compareModel(v1, v2);
        return true;
    }
    case LianDui:
    {
        if(model2 == LianDui)
            return ModelLianDui::compareModel(v1, v2);
        return false;
    }
    case SanLianDui:
    {
        if(model2 == SanLianDui)
            return ModelSanLianDui::compareModel(v1, v2);
        return false;
    }
    default:
        return false;
    }
    return true;

}
