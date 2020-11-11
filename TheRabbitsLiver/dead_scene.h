#pragma once
#include "scene.h"
class DeadScene:
    public Scene
{
public:
	DeadScene(class Gfw* gfw);

	void Enter() override;
	void Exit() override;
	void ProcessInput(unsigned char key) override;
	void Update() override;
	void Draw() override;

private:
	class Renderer* mRenderer;
	class Shader* mSpriteShader;
};

