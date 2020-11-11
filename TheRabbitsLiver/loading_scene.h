#pragma once

#include "scene.h"

#include <vector>
#include <string>

class LoadingScene:
    public Scene
{
public:
	LoadingScene(class Gfw* gfw);

	void Enter() override;
	void Exit() override;
	void ProcessInput(unsigned char key) override;
	void Update() override;
	void Draw() override;

	// Scene specific
	bool ReadFromJson(const std::string& file);

private:
	std::vector<std::string> mMeshFiles;
	std::vector<std::string> mSoundFiles;
	std::vector<std::string> mImgFiles;

	class Renderer* mRenderer;
	class Shader* mSpriteShader;

	unsigned int mMeshIdx;
	unsigned int mSoundIdx;
	unsigned int mImgIdx;
	unsigned int mCurIdx;
	unsigned int mTotalFiles;
	float mElapsed;
};

