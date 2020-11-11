#include "component.h"

#include "actor.h"

Component::Component(Actor* owner)
	: mOwner{ owner }
{
	mOwner->AddComponent(this);
}

Component::~Component()
{
	mOwner->RemoveComponent(this);
}