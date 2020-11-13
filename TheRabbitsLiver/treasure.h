#pragma once
#include "actor.h"
class Treasure :
    public Actor
{
public:
    enum TreasureType
    {
        kPotion
    };

    Treasure(Gfw* gfw, TreasureType type, Gfw::Layer layer = Gfw::Layer::kTreasure);

private:
    class BoxComponent* mBox;
    
    TreasureType mType;
};

