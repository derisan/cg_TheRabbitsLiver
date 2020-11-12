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

    // Collision process
    void Fall();
    void OnBoard(class Vehicle* log);

    // Getters
    class BoxComponent* GetBox() const { return mBox; }
    class CameraComponent* GetCamera() const { return mCamera; }
    PlayerType GetType() const { return mType; }

    // Setters
    void SetZBorder(const glm::vec2 z) { mBorder.z = z; }

private:
    class BoxComponent* mBox;
    class CameraComponent* mCamera;

    PlayerType mType;

    struct Border
    {
        const glm::vec2 x;
        glm::vec2 z;
    };
    Border mBorder;

    glm::vec3 mPrevMovement;
};

