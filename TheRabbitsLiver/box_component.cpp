#include "box_component.h"

#include "actor.h"

BoxComponent::BoxComponent(Actor* owner)
	: Component{ owner },
	mObjectBox{ glm::vec3{0.0f}, glm::vec3{0.0f} },
	mWorldBox{ glm::vec3{0.0f}, glm::vec3{0.0f} },
	mShouldRotate{ true }
{

}

void BoxComponent::OnUpdateWorldTransform()
{
	mWorldBox = mObjectBox;
	mWorldBox.mMin *= mOwner->GetScale();
	mWorldBox.mMax *= mOwner->GetScale();

	if (mShouldRotate)
		mWorldBox.Rotate(mOwner->GetRotation());

	mWorldBox.mMin += mOwner->GetPosition();
	mWorldBox.mMax += mOwner->GetPosition();
}
