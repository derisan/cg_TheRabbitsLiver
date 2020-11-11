#pragma once

#include <string>
#include <stack>
#include <unordered_map>

class Gfw
{
public:
	Gfw();

	bool Init(int* argc, char** argv, int w = 1600, int h = 900);

	void Run(unsigned char key);
	void Shutdown();

	void ChangeScene(const std::string& scene);
	void PushScene(const std::string& scene);
	void PopScene();

	// Getters
	bool GetShouldClose() const { return mShouldClose; }
	int GetScrWidth() const { return mScrWidth; }
	int GetScrHeight() const { return mScrHeight; }

	// Setters
	void SetShouldClose(bool value) { mShouldClose = value; }

	const float dt{ 16.0f / 1000.0f };

private:
	void ProcessInput(unsigned char key);
	void Update();
	void Draw();

	std::stack<class Scene*> mScenes;
	std::unordered_map<std::string, class Scene*> mScenesMap;

	int mScrWidth;
	int mScrHeight;

	bool mShouldClose;
};

