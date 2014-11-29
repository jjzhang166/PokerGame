#ifndef POKERCARDMODEL_H
#define POKERCARDMODEL_H
#include "pokercarditem.h"
#include "modeldan.h"
#include "modeldui.h"
#include "modelhong.h"
#include "modelzha.h"
#include "modellong.h"
#include "modelliandui.h"
#include "modelsanliandui.h"

enum CardModel {Illegal, Dan, Dui, Long, LianDui, SanLianDui, Hong, Zha};

class PokerCardModel
{
typedef QList<PokerCardItem*> CardList;

public:
    PokerCardModel();
    static CardModel checkModel(CardList &);

    static bool greaterThan(PokerCardModel::CardList &v1, CardModel &model1, PokerCardModel::CardList &v2, CardModel &model2);
};

#endif // POKERCARDMODEL_H
