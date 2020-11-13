#pragma once

#include "actor.h"
#include "vehicle.h"

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

    Plane(Gfw* gfw, PlaneType type, int curStage, bool right = false, Gfw::Layer layer = Gfw::Layer::kPlane);

    void UpdateActor() override;

    void GenerateVehicle();
    void GenerateTree();
    void GenerateTreasure();

    // Getters
    class BoxComponent* GetBox() const { return mBox; }
    PlaneType GetType() const { return mType; }

    // Setters
    void SetDisabled(bool value) { mDisabled = value; }

private:
    class BoxComponent* mBox;

    PlaneType mType;
    Vehicle::VehicleType mVehicleType;

    float mCooldown;
    int mLeftOrRight;
    bool mDisabled;
};

