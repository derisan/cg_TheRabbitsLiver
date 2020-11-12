#include "main_scene.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/gtc/matrix_transform.hpp>

#include "gfw.h"
#include "renderer.h"
#include "actor.h"
#include "sprite_component.h"
#include "sound_engine.h"
#include "shader.h"
#include "player.h"
#include "plane.h"
#include "camera_component.h"
#include "mesh_component.h"

MainScene::MainScene(Gfw* gfw)
	: Scene{ gfw },
	mRenderer{ nullptr },
	mSpriteShader{ nullptr },
	mMeshShader{ nullptr },
	mPlayer1{ nullptr },
	mPlayer2{ nullptr }
{
	mRenderer = Renderer::Get();
	mSpriteShader = mRenderer->GetShader("sprite");
	mMeshShader = mRenderer->GetShader("basicMesh");
}

void MainScene::Enter()
{
	SoundEngine::Get()->Play("BlueWorld.mp3", 0.75f);

	// Set projection matrix in advance. Because it rarely changes.
	glm::mat4 proj{ 1.0f };
	proj = glm::perspective(45.0f, static_cast<float>(GetGfw()->GetScrWidth()) / GetGfw()->GetScrHeight(),
		0.1f, 100.0f);
	mMeshShader->SetActive();
	mMeshShader->SetMatrix4Uniform("uProj", proj);

	// Load game data
	LoadData();
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
	else if (key == 't')
		mGfw->ChangeScene("winning");
	else if (key == 'p' || key == 'P')
		Pause();
	else if (key == 'r' || key == 'R')
		Resume();
}

void MainScene::Update()
{
	if (mCurStage < mStage.size())
		CreatePlane();

	RemoveBehind();
}

void MainScene::Draw()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	mMeshShader->SetActive();

	glViewport(0, 0, 1600, 450);
	mMeshShader->SetMatrix4Uniform("uView", mPlayer1->GetCamera()->GetView());
	for (auto mesh : mGfw->GetMeshes())
		mesh->Draw(mMeshShader);

	glViewport(0, 450, 1600, 450);
	mMeshShader->SetMatrix4Uniform("uView", mPlayer2->GetCamera()->GetView());
	for (auto mesh : mGfw->GetMeshes())
		mesh->Draw(mMeshShader);

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

void MainScene::LoadData()
{
	mPlayer1 = new Player{ mGfw, Player::kP1 };
	mPlayer2 = new Player{ mGfw, Player::kP2 };
	mPlayer2->SetPosition(glm::vec3{ 3.0f, 0.0f, 0.0f });

	// Read stage from file
	std::ifstream file{ "Assets/stage.txt" };
	if (!file.is_open())
	{
		std::cout << "Failed to read stage file" << std::endl;
		return;
	}

	std::stringstream s;
	s << file.rdbuf();
	std::string contents = s.str();

	for (auto ch : contents)
		if (ch != '\n' && ch != ' ')
			mStage.emplace_back(ch - 65);
}

void MainScene::CreatePlane()
{
	auto maxZ = GetMaxZ();
	for (; mCurStage < -maxZ + 10; ++mCurStage)
		new Plane{ mGfw, static_cast<Plane::PlaneType>(mStage[mCurStage]), mCurStage };
}

void MainScene::RemoveBehind()
{
	auto maxZ = GetMaxZ();

	for (auto plane : mGfw->GetActorsAt(Gfw::Layer::kPlane))
	{
		auto z = plane->GetPosition().z;
		if (maxZ + 6.0f < z)
			plane->SetState(Actor::State::kDead);
	}

	for (auto vehicle : mGfw->GetActorsAt(Gfw::Layer::kVehicle))
	{
		auto z = vehicle->GetPosition().z;
		if (maxZ + 6.0f < z)
			vehicle->SetState(Actor::State::kDead);
	}

	for (auto tree : mGfw->GetActorsAt(Gfw::Layer::kTree))
	{
		auto z = tree->GetPosition().z;
		if (maxZ + 6.0f < z)
			tree->SetState(Actor::State::kDead);
	}
}

float MainScene::GetMaxZ()
{
	const auto& p1Pos = mPlayer1->GetPosition();
	const auto& p2Pos = mPlayer2->GetPosition();

	auto max = glm::max(p1Pos.z, p2Pos.z);

	return max;
}