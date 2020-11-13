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

    void UpdateActor() override;
    void ActorInput(unsigned char key) override;

    // Getters
    TreasureType GetType() const { return mType; }

private:
    class Actor* mHeart;

    TreasureType mType;
   
    float mPercentage;
};

