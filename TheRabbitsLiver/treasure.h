#pragma once
#include "actor.h"
class Treasure :
    public Actor
{
public:
    Treasure(Gfw* gfw, Gfw::Layer layer = Gfw::Layer::kTreasure);

    void UpdateActor() override;
    void ActorInput(unsigned char key) override;

    void GenerateItem();

private:
    class Actor* mHeart;

    float mPercentage;
};

