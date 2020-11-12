#pragma once
#include "actor.h"
class Player :
    public Actor
{
public:
    enum PlayerType
    {
        kP1,
        kP2
    };

    Player(class Gfw* gfw, PlayerType type, Gfw::Layer layer = Gfw::Layer::kPlayer);

    void UpdateActor() override;
    void ActorInput(unsigned char key) override;

    void PlayerOneInput(unsigned char key);
    void PlayerTwoInput(unsigned char key);

    // Getters
    class BoxComponent* GetBox() const { return mBox; }
    class CameraComponent* GetCamera() const { return mCamera; }
    PlayerType GetType() const { return mType; }

private:
    class BoxComponent* mBox;
    class CameraComponent* mCamera;

    PlayerType mType;
};
