#include "gfw.h"

#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "renderer.h"
#include "loading_scene.h"

Gfw::Gfw()
	: mScrWidth{ 0 },
	mScrHeight{ 0 },
	mShouldClose{ false }
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
	Renderer::Get()->Shutdown();

	// Scene map을 순회하면서 scene 삭제
	for (auto item : mScenesMap)
		delete item.second;
}

void Gfw::ProcessInput(unsigned char key)
{
	if (key == 0)
		return;

	mScenes.top()->ProcessInput(key);
}

void Gfw::Update()
{
	mScenes.top()->Update();
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

