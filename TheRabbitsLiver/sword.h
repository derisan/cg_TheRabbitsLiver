#pragma once
#include "actor.h"
class Sword :
    public Actor
{
public:
    Sword(Gfw* gfw, class Player* target, Gfw::Layer layer = Gfw::Layer::kDefault);

    void UpdateActor() override;
    
    // Getters
    class BoxComponent* GetBox() const { return mBox; }

private:
    class BoxComponent* mBox;
    class Player* mTarget;

    float mSpeed;
};

