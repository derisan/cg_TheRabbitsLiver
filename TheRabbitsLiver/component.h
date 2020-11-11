#pragma once

class Component
{
public:
	Component(class Actor* owner);
	virtual ~Component();

	virtual void Update() {};
	virtual void ProcessKeyboardInput(unsigned char key) { };
	virtual void OnUpdateWorldTransform() { };

protected:
	class Actor* mOwner;
};

