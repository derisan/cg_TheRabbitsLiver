#include "gfw.h"

#include <iostream>
#include <algorithm>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "renderer.h"
#include "loading_scene.h"
#include "actor.h"
#include "mesh_component.h"
#include "sprite_component.h"

Gfw::Gfw()
	: mScrWidth{ 0 },
	mScrHeight{ 0 },
	mShouldClose{ false },
	mIsUpdating{ false }
{

}

bool Gfw::Init(int* argc, char** argv, int w, int h)
{
	mScrWidth = w;
	mScrHeight = h;

	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(mScrWidth, mScrHeight);
	glutCreateWindow("The Rabbit's Liver");

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Unable to initialize GLEW" << std::endl;
		return false;
	}

	mScenesMap.emplace("loading", new LoadingScene{ this });

	PushScene("loading");

	return true;
}

void Gfw::Run(unsigned char key)
{
	if (!mShouldClose)
	{
		ProcessInput(key);
		Update();
		Draw();
	}
}

void Gfw::Shutdown()
{
	// Delete cached things in renderer
	Renderer::Get()->Shutdown();

	// Delete scene
	for (auto item : mScenesMap)
		delete item.second;

	// Delete all actors
	for(auto actors : mActors)
		while (!actors.empty())
			delete actors.back();
}

void Gfw::ProcessInput(unsigned char key)
{
	if (key == 0)
		return;

	for (auto actors : mActors)
		for (auto actor : actors)
		{
			actor->ProcessInput(key);
		}

	mScenes.top()->ProcessInput(key);
}

void Gfw::Update()
{
	auto curScene = mScenes.top();

	if (curScene->GetState() == Scene::State::kPaused)
		return;
	curScene->Update();

	std::vector<Actor*> deads;
	mIsUpdating = true;
	for(auto actors : mActors)
		for (auto actor : actors)
		{
			actor->Update();
			if (actor->GetState() == Actor::State::kDead)
				deads.emplace_back(actor);
		}
	mIsUpdating = false;

	for (auto pending : mPendingActors)
	{
		pending->ComputeWorldTransform();
		auto layer = pending->GetLayer();
		mActors[layer].emplace_back(pending);
	}
	mPendingActors.clear();

	for (auto actor : deads)
	{
		delete actor;
	}
}

void Gfw::Draw()
{
	mScenes.top()->Draw();
}

void Gfw::ChangeScene(const std::string& scene)
{
	auto iter = mScenesMap.find(scene);
	if (iter == std::end(mScenesMap))
	{
		std::cout << "There's no scene named " << scene << std::endl;
		return;
	}

	if (mScenes.size() > 0)
	{
		mScenes.top()->Exit();
		mScenes.pop();
	}
	mScenes.push(iter->second);
	mScenes.top()->Enter();
}

void Gfw::PushScene(const std::string& scene)
{
	auto iter = mScenesMap.find(scene);
	if (iter == std::end(mScenesMap))
	{
		std::cout << "There's no scene named " << scene << std::endl;
		return;
	}

	if (mScenes.size() > 0)
		mScenes.top()->Pause();
	mScenes.push(iter->second);
	mScenes.top()->Enter();
}

void Gfw::PopScene()
{
	if (mScenes.size() == 1)
		mShouldClose = true;
	else if (mScenes.size() > 1)
	{
		mScenes.top()->Exit();
		mScenes.pop();
		mScenes.top()->Resume();
	}
}

void Gfw::AddActorAt(Actor* actor, int layer)
{
	if (mIsUpdating)
		mPendingActors.emplace_back(actor);
	else
		mActors[layer].emplace_back(actor);
}

void Gfw::RemoveActorAt(Actor* actor, int layer)
{
	auto iter = std::find(std::begin(mPendingActors), std::end(mPendingActors), actor);
	if (iter != std::end(mPendingActors))
		mPendingActors.erase(iter);

	iter = std::find(std::begin(mActors[layer]), std::end(mActors[layer]), actor);
	if (iter != std::end(mActors[layer]))
		mActors[layer].erase(iter);
}

void Gfw::RemoveAll()
{
	for (auto actors : mActors)
		while (!actors.empty())
			delete actors.back();
}

void Gfw::AddMesh(MeshComponent* mesh)
{
	mMeshes.emplace_back(mesh);
}

void Gfw::RemoveMesh(MeshComponent* mesh)
{
	auto iter = std::find(std::begin(mMeshes), std::end(mMeshes), mesh);
	if (iter != std::end(mMeshes))
		mMeshes.erase(iter);
}

void Gfw::AddSprite(SpriteComponent* sprite)
{
	mSprites.emplace_back(sprite);
}

void Gfw::RemoveSprite(SpriteComponent* sprite)
{
	auto iter = std::find(std::begin(mSprites), std::end(mSprites), sprite);
	if (iter != std::end(mSprites))
		mSprites.erase(iter);
}