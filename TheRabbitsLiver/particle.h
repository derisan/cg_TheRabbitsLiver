#pragma once
#include "actor.h"
class Particle :
    public Actor
{
public:
    enum ParticleType
    {
        kFire,
        kCarrot,
        kHeart
    };

    Particle(Gfw* gfw, ParticleType type, const glm::vec3 startPoint, Gfw::Layer layer = Gfw::kDefault);

    void UpdateActor() override;
    
private:
    const glm::vec3 mGravity;
    glm::vec3 mVelocity;
    float mLife;

    ParticleType mType;
};

