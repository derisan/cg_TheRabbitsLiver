#pragma once
#include "actor.h"
class Bomb :
    public Actor
{
public:
    Bomb(Gfw* gfw, class Vehicle* target, Gfw::Layer layer = Gfw::Layer::kDefault);

    void UpdateActor() override;

    // Getters
    class BoxComponent* GetBox() const { return mBox; }

private:
    class BoxComponent* mBox;
    class Vehicle* mTarget;

    float mSpeed;
};

