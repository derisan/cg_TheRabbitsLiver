#pragma once
#include "actor.h"
class Plane :
    public Actor
{
public:
    enum PlaneType
    {
        kGrass,
        kRoad,
        kRailroad,
        kWater,
        kGoal
    };

    Plane(class Gfw* gfw, PlaneType type, int curStage, Gfw::Layer layer = Gfw::Layer::kPlane);

    void UpdateActor() override;

    // Getters
    class BoxComponent* GetBox() const { return mBox; }
    PlaneType GetType() const { return mType; }

private:
    class BoxComponent* mBox;

    PlaneType mType;
};

