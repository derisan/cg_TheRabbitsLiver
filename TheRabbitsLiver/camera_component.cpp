#include "camera_component.h"

#include <glm/gtc/matrix_transform.hpp>

#include "actor.h"

CameraComponent::CameraComponent(Actor* owner)
	: Component{ owner },
	mView{ 1.0f },
	mCameraPos{ 0.0f },
	mCameraTarget{ 0.0f },
	mCameraUp{ 0.0f, 1.0f, 0.0f }
{

}

void CameraComponent::OnUpdateWorldTransform()
{
	const auto& pos = mOwner->GetPosition();

	mCameraPos = glm::vec3{ pos.x, 10.0f, pos.z + 10.0f };
	mCameraTarget = pos;
	
	mView = glm::lookAt(mCameraPos, mCameraTarget, mCameraUp);
}

