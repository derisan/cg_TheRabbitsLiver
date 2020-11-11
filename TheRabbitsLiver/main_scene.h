#pragma once
#include "scene.h"
class MainScene:
    public Scene
{
public:
	MainScene(class Gfw* gfw);

	void Enter() override;
	void Exit() override;
	void ProcessInput(unsigned char key) override;
	void Update() override;
	void Draw() override;
	void Pause() override;
	void Resume() override;

private:
	class Renderer* mRenderer;
	class Shader* mSpriteShader;
};

