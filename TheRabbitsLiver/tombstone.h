#pragma once

#include "actor.h"

class Tombstone :
    public Actor
{
public:
    Tombstone(Gfw* gfw, class Player* target, Gfw::Layer layer = Gfw::Layer::kDefault);

    void UpdateActor() override;
    void ActorInput(unsigned char key) override;

private:
    class Player* mTarget;

    float mPercentage;
};

