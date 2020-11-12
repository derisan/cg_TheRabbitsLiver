#include "winning_scene.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "gfw.h"
#include "renderer.h"
#include "actor.h"
#include "sprite_component.h"
#include "sound_engine.h"
#include "shader.h"

WinningScene::WinningScene(Gfw* gfw)
	: Scene{ gfw },
	mRenderer{ nullptr },
	mSpriteShader{ nullptr },
	mMeshShader{ nullptr }
{
	mRenderer = Renderer::Get();
	mSpriteShader = mRenderer->GetShader("sprite");
	mMeshShader = mRenderer->GetShader("basicMesh");
}

void WinningScene::Enter()
{
	SoundEngine::Get()->Play("victory.mp3");

	auto img = new Actor{ mGfw };
	auto sc = new SpriteComponent{ img, "Assets/youwin.png" };
	img->SetScale(1.0f);
}

void WinningScene::Exit()
{
	SoundEngine::Get()->Stop("victory.mp3");

	mGfw->RemoveAllActors();
}

void WinningScene::ProcessInput(unsigned char key)
{
	if (key == 27)
		mGfw->PopScene();
	else if (key == 13)
		mGfw->ChangeScene("main");
}

void WinningScene::Update()
{

}

void WinningScene::Draw()
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