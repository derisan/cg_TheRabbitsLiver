#pragma once

#include <string>
#include <stack>
#include <unordered_map>
#include <vector>
#include <array>

class Gfw
{
public:
	enum Layer
	{
		kPlayer,
		kVehicle,
		kPlane,
		kTree,
		kSprite
	};

	Gfw();

	bool Init(int* argc, char** argv, int w = 1600, int h = 900);

	void Run(unsigned char key);
	void Shutdown();

	void ChangeScene(const std::string& scene);
	void PushScene(const std::string& scene);
	void PopScene();

	void AddActorAt(class Actor* actor, Layer layer);
	void RemoveActorAt(class Actor* actor, Layer layer);
	void RemoveAll();
	void AddMesh(class MeshComponent* mesh);
	void RemoveMesh(class MeshComponent* mesh);
	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	// Getters
	bool GetShouldClose() const { return mShouldClose; }
	int GetScrWidth() const { return mScrWidth; }
	int GetScrHeight() const { return mScrHeight; }

	std::vector<class Actor*>& GetActorsAt(Layer layer) { return mActors[layer]; }
	std::vector<class MeshComponent*>& GetMeshes() { return mMeshes; }
	std::vector<class SpriteComponent*> GetSprites() { return mSprites; }

	// Setters
	void SetShouldClose(bool value) { mShouldClose = value; }

	const float dt{ 16.0f / 1000.0f };

private:
	void ProcessInput(unsigned char key);
	void Update();
	void Draw();

	std::stack<class Scene*> mScenes;
	std::unordered_map<std::string, class Scene*> mScenesMap;

	std::vector<std::vector<class Actor*>> mActors;
	std::vector<class Actor*> mPendingActors;

	std::vector<class MeshComponent*> mMeshes;
	std::vector<class SpriteComponent*> mSprites;

	int mScrWidth;
	int mScrHeight;

	bool mShouldClose;
	bool mIsUpdating;
};

