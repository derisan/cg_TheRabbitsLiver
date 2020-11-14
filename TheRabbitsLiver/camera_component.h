#pragma once

#include "component.h"

#include <glm/glm.hpp>

class CameraComponent:
    public Component
{
public:
    CameraComponent(class Actor* owner);

    void OnUpdateWorldTransform() override;

    void TopView();
    void FPS();
    void TPS();

    // Getters
    const glm::mat4& GetView() const { return mView; }
    const glm::vec3& GetCameraPos() const { return mCameraPos; }
    const glm::vec3& GetCameraTarget() const { return mCameraTarget; }
   
    // Setters
    void SetView(const glm::mat4& view) { mView = view; }
    void SetCameraPos(const glm::vec3& pos) { mCameraPos = pos; }
    void SetCameraTarget(const glm::vec3& target) { mCameraTarget = target; }
    void SetTopView() { mViewOption = 0; }
    void SetFPS() { mViewOption = 1; }
    void SetTPS() { mViewOption = 2; }

private:
    glm::mat4 mView;

    glm::vec3 mCameraPos;
    glm::vec3 mCameraTarget;
    glm::vec3 mCameraUp;

    unsigned int mViewOption;
};

