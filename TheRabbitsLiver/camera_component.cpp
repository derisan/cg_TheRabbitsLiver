#include "camera_component.h"

#include <glm/gtc/matrix_transform.hpp>

#include "actor.h"

CameraComponent::CameraComponent(Actor* owner)
	: Component{ owner },
	mView{ 1.0f },
	mCameraPos{ 0.0f },
	mCameraTarget{ 0.0f },
	mCameraUp{ 0.0f, 1.0f, 0.0f },
	mViewOption{ 0 }
{

}

void CameraComponent::OnUpdateWorldTransform()
{
	switch (mViewOption)
	{
		case 0:
			TopView();
			break;
		case 1:
			FPS();
			break;
		case 2:
			TPS();
			break;
	}
}

void CameraComponent::TopView()
{
	const auto& pos = mOwner->GetPosition();

	mCameraPos = glm::vec3{ pos.x + 3.0f, 12.0f, pos.z + 10.0f };
	mCameraTarget = pos;

	mView = glm::lookAt(mCameraPos, mCameraTarget, mCameraUp);
}

void CameraComponent::FPS()
{
	const auto& pos = mOwner->GetPosition();

	mCameraPos = glm::vec3{ pos.x, pos.y + 1.0f, pos.z - 1.0f};
	mCameraTarget = mCameraPos + 3.0f * mOwner->GetForward();

	mView = glm::lookAt(mCameraPos, mCameraTarget, mCameraUp);
}

void CameraComponent::TPS()
{
	const auto& pos = mOwner->GetPosition();

	mCameraPos = glm::vec3{ pos.x, 3.0f, pos.z + 5.0f };
	mCameraTarget = pos + 3.0f * mOwner->GetForward();

	mView = glm::lookAt(mCameraPos, mCameraTarget, mCameraUp);
}