#pragma once
class Scene
{
public:
	enum class State
	{
		kActive,
		kPaused
	};

	Scene(class Gfw* gfw);
	virtual ~Scene() = default;

	virtual void Enter() { }
	virtual void Exit() { }
	virtual void ProcessInput(unsigned char key) { }
	virtual void Update() { }
	virtual void Draw() { }
	virtual void Pause() { mState = State::kPaused; }
	virtual void Resume() { mState = State::kActive; }

	// Getters
	State GetState() const { return mState; }
	class Gfw* GetGfw() { return mGfw; }

	// Setters
	void SetState(State state) { mState = state; }

protected:
	class Gfw* mGfw;

private:
	State mState;
};

