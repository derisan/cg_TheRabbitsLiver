#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "gfw.h"

class Actor
{
public:
	enum class State
	{
		kActive,
		kPaused,
		kDead
	};

	Actor(Gfw* Gfw, Gfw::Layer layer);
	virtual ~Actor();

	void Update();
	void UpdateComponents();
	virtual void UpdateActor() { };

	void ProcessInput(unsigned char key);
	virtual void ActorInput(unsigned char key) { };

	void ComputeWorldTransform();

	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

	// Getters
	State GetState() const { return mState; }
	const glm::mat4& GetWorldTransform() const { return mWorldTransform; }
	const glm::vec3& GetPosition() const { return mPosition; }
	float GetScale() const { return mScale; }
	float GetRotation() const { return mRotation; }
	glm::vec3 GetForward() const;
	glm::vec3 GetRight() const;
	class Gfw* GetGfw() { return mGfw; }
	int GetLayer() const { return mLayer; }

	// Setters
	void SetState(State state) { mState = state; }
	void SetPosition(const glm::vec3& position) { mPosition = position; mRecomputeWorldTransform = true; }
	void SetScale(float scale) { mScale = scale; mRecomputeWorldTransform = true; }
	void SetRotation(float rotation) { mRotation = rotation; mRecomputeWorldTransform = true; }

protected:
	class Gfw* mGfw;
	
private:
	std::vector<class Component*> mComponents;

	State mState;

	// Transforms
	glm::mat4 mWorldTransform;
	glm::vec3 mPosition;
	float mScale;
	float mRotation;

	bool mRecomputeWorldTransform;
	Gfw::Layer mLayer;
};

