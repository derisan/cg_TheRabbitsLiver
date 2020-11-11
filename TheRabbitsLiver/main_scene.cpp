#include "main_scene.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "gfw.h"
#include "renderer.h"
#include "actor.h"
#include "sprite_component.h"
#include "sound_engine.h"
#include "shader.h"

MainScene::MainScene(Gfw* gfw)
	: Scene{ gfw },
	mRenderer{ nullptr }
{
	mRenderer = Renderer::Get();
}

void MainScene::Enter()
{
	SoundEngine::Get()->Play("BlueWorld.mp3", 0.75f);
}

void MainScene::Exit()
{
	SoundEngine::Get()->Stop("BlueWorld.mp3");

	mGfw->RemoveAll();
}

void MainScene::ProcessInput(unsigned char key)
{
	if (key == 27)
		mGfw->ChangeScene("start");
	else if (key == 'p' || key == 'P')
		Pause();
	else if (key == 'r' || key == 'R')
		Resume();
}

void MainScene::Update()
{

}

void MainScene::Draw()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	glutSwapBuffers();
}

void MainScene::Pause()
{
	Scene::Pause();
}

void MainScene::Resume()
{
	Scene::Resume();
}