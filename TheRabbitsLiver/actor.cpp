#include "actor.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "component.h"

Actor::Actor(Gfw* gfw, Gfw::Layer layer)
	: mGfw{ gfw },
	mState{ State::kActive },
	mWorldTransform{ 1.0f },
	mPosition{ 0.0f },
	mScale{ 1.0f },
	mRotation{ 0.0f },
	mRecomputeWorldTransform{ true },
	mLayer{ layer }
{
	mGfw->AddActorAt(this, layer);
}

Actor::~Actor()
{
	mGfw->RemoveActorAt(this, mLayer);

	while (!mComponents.empty())
		delete mComponents.back();
}

void Actor::Update()
{
	if (mState == State::kActive)
	{
		ComputeWorldTransform();

		UpdateComponents();
		UpdateActor();

		ComputeWorldTransform();
	}
}

void Actor::UpdateComponents()
{
	for (auto comp : mComponents)
		comp->Update();
}

void Actor::ProcessInput(unsigned char key)
{
	if (mState == State::kActive)
	{
		for (auto comp : mComponents)
			comp->ProcessKeyboardInput(key);

		ActorInput(key);
	}
}

void Actor::ComputeWorldTransform()
{
	if (mRecomputeWorldTransform)
	{
		mRecomputeWorldTransform = false;

		mWorldTransform = glm::mat4{ 1.0f };
		mWorldTransform = glm::translate(mWorldTransform, mPosition);
		mWorldTransform = glm::rotate(mWorldTransform, glm::radians(mRotation), glm::vec3{ 0.0f, 1.0f, 0.0f });
		mWorldTransform = glm::scale(mWorldTransform, glm::vec3{ mScale });

		for (auto comp : mComponents)
			comp->OnUpdateWorldTransform();
	}
}

void Actor::AddComponent(Component* component)
{
	mComponents.emplace_back(component);
}

void Actor::RemoveComponent(Component* component)
{
	auto iter = std::find(std::begin(mComponents), std::end(mComponents), component);
	if (iter != std::end(mComponents))
		mComponents.erase(iter);
}

glm::vec3 Actor::GetForward() const
{
	return glm::rotateY(glm::vec3{ 0.0, 0.0f, -1.0f }, glm::radians(mRotation));
}

glm::vec3 Actor::GetRight() const
{
	return glm::rotateY(glm::vec3{ 0.0f, 0.0f, -1.0f }, glm::radians(mRotation - 90.0f));
}