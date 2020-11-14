#pragma once
#include "actor.h"
class Ballon :
    public Actor
{
public:
    Ballon(Gfw* gfw, class Player* target, Gfw::Layer layer = Gfw::Layer::kDefault);

    void UpdateActor() override;

private:
    class Player* mTarget;

    glm::vec3 mVelocity;
    float mDistance;
};

