#include "title_scene.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "gfw.h"
#include "renderer.h"
#include "actor.h"
#include "sprite_component.h"
#include "sound_engine.h"
#include "shader.h"

TitleScene::TitleScene(Gfw* gfw)
	: Scene{ gfw },
	mRenderer{ nullptr },
	mSpriteShader{ nullptr }
{
	mRenderer = Renderer::Get();
	mSpriteShader = mRenderer->GetShader("sprite");
}

void TitleScene::Enter()
{
	SoundEngine::Get()->Play("happy.mp3");

	auto bg = new Actor{ mGfw, mGfw->kDefault };
	auto sc = new SpriteComponent{ bg, "Assets/pressenter.jpg" };

	glViewport(0, 0, mGfw->GetScrWidth(), mGfw->GetScrHeight());
}

void TitleScene::Exit()
{
	SoundEngine::Get()->Stop("happy.mp3");

	mGfw->RemoveAll();
}

void TitleScene::ProcessInput(unsigned char key)
{
	if (key == 27)
		mGfw->PopScene();
	else if (key == 13)
		mGfw->ChangeScene("main");
}

void TitleScene::Update()
{

}

void TitleScene::Draw()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Enable alpha blending
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