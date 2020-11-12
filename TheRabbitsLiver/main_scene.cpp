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
#include "vehicle.h"
#include "tree.h"
#include "camera_component.h"
#include "mesh_component.h"
#include "box_component.h"

MainScene::MainScene(Gfw* gfw)
	: Scene{ gfw },
	mRenderer{ nullptr },
	mSpriteShader{ nullptr },
	mMeshShader{ nullptr },
	mPlayer1{ nullptr },
	mPlayer2{ nullptr },
	mCurStage{ 0 }
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

	mCurStage = 0;

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

	CollisionCheck();

	if (IsWin())
		mGfw->ChangeScene("winning");
}

void MainScene::Draw()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// Render bottom screen
	glEnable(GL_DEPTH_TEST);
	mMeshShader->SetActive();
	glViewport(0, 0, mGfw->GetScrWidth(), mGfw->GetScrHeight() / 2);
	mMeshShader->SetMatrix4Uniform("uView", mPlayer1->GetCamera()->GetView());
	for (auto mesh : mGfw->GetMeshes())
		mesh->Draw(mMeshShader);

	// Render Top screen
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	mMeshShader->SetActive();
	glViewport(0, mGfw->GetScrHeight() / 2, mGfw->GetScrWidth(), mGfw->GetScrHeight() / 2);
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
	mPlayer1->SetPosition(glm::vec3{ -12.0f, 0.0f, 0.0f });
	mPlayer2->SetPosition(glm::vec3{ 12.0f, 0.0f, 0.0f });

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
	{
		auto plane = new Plane{ mGfw, static_cast<Plane::PlaneType>(mStage[mCurStage]), mCurStage };
		plane->SetDisabled(true);
		new Plane{ mGfw, static_cast<Plane::PlaneType>(mStage[mCurStage]), mCurStage, true };
	}
}

float MainScene::GetMaxZ()
{
	const auto& p1Pos = mPlayer1->GetPosition();
	const auto& p2Pos = mPlayer2->GetPosition();

	auto max = glm::max(p1Pos.z, p2Pos.z);

	return max;
}

void MainScene::CollisionCheck()
{
	const auto& p1Box = mPlayer1->GetBox()->GetWorldBox();
	const auto& p2Box = mPlayer2->GetBox()->GetWorldBox();
	auto maxZ = GetMaxZ();

	// Collision check with vehicles
	for (auto vehicle : mGfw->GetActorsAt(Gfw::Layer::kVehicle))
	{
		if (vehicle->GetState() != Actor::State::kActive)
			continue;

		auto vp = (Vehicle*)vehicle;

		auto z = vp->GetPosition().z;
		if (maxZ + 6.0f < z)
		{
			vp->SetState(Actor::State::kDead);
			continue;
		}

		const auto& vehicleBox = vp->GetBox()->GetWorldBox();
		if (Intersects(p1Box, vehicleBox))
		{
			if (vp->GetType() == Vehicle::VehicleType::kLog)
			{
				mPlayer1->OnBoard(vp);
			}
			else
				mPlayer1->HitByCar();
		}

		if (Intersects(p2Box, vehicleBox))
		{
			if (vp->GetType() == Vehicle::VehicleType::kLog)
			{
				mPlayer2->OnBoard(vp);
			}
			else
				mPlayer2->HitByCar();
		}
	}

	// Collision check with water planes
	for (auto plane : mGfw->GetActorsAt(Gfw::Layer::kPlane))
	{
		if (plane->GetState() != Actor::State::kActive)
			continue;

		auto pp = (Plane*)plane;

		auto z = pp->GetPosition().z;
		if (maxZ + 6.0f < z)
		{
			pp->SetState(Actor::State::kDead);
			mPlayer1->SetZBorder(glm::vec2{ z - 2.0f, z - 16.0f });
			mPlayer2->SetZBorder(glm::vec2{ z - 2.0f, z - 16.0f });
			continue;
		}
		
		if (pp->GetType() != Plane::PlaneType::kWater)
			continue;

		if (Intersects(p1Box, pp))	
			mPlayer1->Fall();
		
		if (Intersects(p2Box, pp))
			mPlayer2->Fall();
	}

	// Tree must die
	for (auto tree : mGfw->GetActorsAt(Gfw::Layer::kTree))
	{
		if (tree->GetState() != Actor::State::kActive)
			continue;

		auto tp = (Tree*)tree;
		auto z = tp->GetPosition().z;

		if (maxZ + 6.0f < z)
		{
			tp->SetState(Actor::State::kDead);
			continue;
		}
	}
}

bool MainScene::IsWin()
{
	if (GetMaxZ() == -2.0f * (mStage.size() - 1))
		return true;

	return false;
}