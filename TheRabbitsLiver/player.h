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

    Player(Gfw* gfw, PlayerType type, Gfw::Layer layer = Gfw::Layer::kPlayer);

    void UpdateActor() override;
    void ActorInput(unsigned char key) override;
    void PlayerOneInput(unsigned char key);
    void PlayerTwoInput(unsigned char key);

    void GenerateLifeSprite(const std::string& file);

    // Collision detect
    void CheckCollisionWithTree();

    // Collision process
    void Fall();
    void OnBoard(class Vehicle* log);
    void HitByCar();
    void YouDie();
    void Reincarnation();
    
    // Getters
    class BoxComponent* GetBox() const { return mBox; }
    class CameraComponent* GetCamera() const { return mCamera; }
    PlayerType GetType() const { return mType; }
    bool GetIsDead() const { return mIsDead; }
    
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
    int mLives;
    float mInvincibleTime;
    bool mIsDead;
    bool mIsFalling;

    std::vector<class Actor*> mLifeGauges;
};

