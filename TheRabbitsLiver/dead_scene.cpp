#include "dead_scene.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "gfw.h"
#include "renderer.h"
#include "actor.h"
#include "sprite_component.h"
#include "sound_engine.h"
#include "shader.h"

DeadScene::DeadScene(Gfw* gfw)
	: Scene{ gfw },
	mRenderer{ nullptr },
	mSpriteShader{ nullptr }
{
	mRenderer = Renderer::Get();
	mSpriteShader = mRenderer->GetShader("sprite");
}

void DeadScene::Enter()
{
	SoundEngine::Get()->Play("gameover.wav");

	auto img = new Actor{ mGfw };
	auto sc = new SpriteComponent{ img, "Assets/deadscreen.png" };
	img->SetScale(2.0f);
}

void DeadScene::Exit()
{
	SoundEngine::Get()->Stop("gameover.wav");

	mGfw->RemoveAll();
}

void DeadScene::ProcessInput(unsigned char key)
{
	if (key == 27)
		mGfw->PopScene();
	else if (key == 13)
		mGfw->ChangeScene("main");
}

void DeadScene::Update()
{

}

void DeadScene::Draw()
{
	glViewport(0, 0, mGfw->GetScrWidth(), mGfw->GetScrHeight());

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Enable alpha blending
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	// Draw all sprites
	mSpriteShader->SetActive();
	auto sprites = mGfw->GetSprites();
	for (auto sprite : sprites)
		sprite->Draw(mSpriteShader);

	glutSwapBuffers();
}