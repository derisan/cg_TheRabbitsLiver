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
	mRenderer{ nullptr },
	mSpriteShader{ nullptr }
{
	mRenderer = Renderer::Get();
	mSpriteShader = mRenderer->GetShader("sprite");
}

void MainScene::Enter()
{
	SoundEngine::Get()->Play("BlueWorld.mp3", 0.75f);

	auto bg = new Actor{ mGfw };
	auto sc = new SpriteComponent{ bg, "Assets/deadscreen.png" };
}

void MainScene::Exit()
{
	SoundEngine::Get()->Stop("BlueWorld.mp3");

	mGfw->RemoveAll();
}

void MainScene::ProcessInput(unsigned char key)
{
	if (key == 27)
		mGfw->ChangeScene("title");
	else if (key == 13)
		mGfw->ChangeScene("dead");
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
	// Enable alpha blending
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);


	glViewport(0, 0, 1600, 450);
	// Draw all sprites
	mSpriteShader->SetActive();
	auto sprites = mGfw->GetSprites();
	for (auto sprite : sprites)
		sprite->Draw(mSpriteShader);


	glViewport(0, 450, 1600, 450);
	for (auto sprite : sprites)
		sprite->Draw(mSpriteShader);


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