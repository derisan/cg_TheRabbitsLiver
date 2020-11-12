#pragma once
#include "actor.h"
class Vehicle :
    public Actor
{
public:
    enum VehicleType
    {
        kCar,
        kTruck,
        kBus,
        kLog
    };

    Vehicle(class Gfw* gfw, VehicleType type, Gfw::Layer layer = Gfw::Layer::kVehicle);

    void UpdateActor() override;
    
    // Getters
    VehicleType GetType() const { return mType; }
    class BoxComponent* GetBox() const { return mBox; }
    float GetGenTerm() const { return mGenTerm; }

    // Setters
    void SetSpeed(float speed) { mSpeed = speed; }
    void SetGenTerm(float gen) { mGenTerm = gen; }

private:
    class BoxComponent* mBox;

    float mSpeed;
    float mGenTerm;

    VehicleType mType;
};

