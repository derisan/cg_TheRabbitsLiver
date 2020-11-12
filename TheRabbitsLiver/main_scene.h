#pragma once

#include "scene.h"

#include <vector>

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

	// Scene specific
	void LoadData();
	void CreatePlane();
	void CollisionCheck();

	bool IsWin();
	float GetMaxZ();

private:
	std::vector<int> mStage;

	class Renderer* mRenderer;
	class Shader* mSpriteShader;
	class Shader* mMeshShader;

	class Player* mPlayer1;
	class Player* mPlayer2;

	int mCurStage;
};

