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
#include "treasure.h"
#include "item.h"
#include "camera_component.h"
#include "mesh_component.h"
#include "box_component.h"
#include "background.h"

MainScene::MainScene(Gfw* gfw)
	: Scene{ gfw },
	mRenderer{ nullptr },
	mSpriteShader{ nullptr },
	mPhongShader{ nullptr },
	mPlayer1{ nullptr },
	mPlayer2{ nullptr },
	mBackground{ nullptr },
	mCurStage{ 0 },
	mDirLightYPos{ -20.0f },
	mIsNight{ false },
	mChangesLightYpos{ false }
{
	mRenderer = Renderer::Get();
	mSpriteShader = mRenderer->GetShader("sprite");
	mPhongShader = mRenderer->GetShader("phong");
}

void MainScene::Enter()
{
	SoundEngine::Get()->Play("BlueWorld.mp3", 0.75f);

	// Set projection matrix in advance. Because it rarely changes.
	glm::mat4 proj{ 1.0f };
	proj = glm::perspective(45.0f, static_cast<float>(GetGfw()->GetScrWidth()) / GetGfw()->GetScrHeight(),
		0.1f, 100.0f);
	mPhongShader->SetActive();
	mPhongShader->SetMatrix4Uniform("uProj", proj);

	mBackground = new Background{ "Assets/earth.jpg" };

	mCurStage = 0;
	mStage.clear();

	// Load game data
	LoadData();
}

void MainScene::Exit()
{
	SoundEngine::Get()->Stop("BlueWorld.mp3");

	delete mBackground;

	mGfw->RemoveAllActors();
}

void MainScene::ProcessInput(unsigned char key)
{
	switch (key)
	{
		case 27: // esc
			mGfw->ChangeScene("title");
			break;
		case 'g': case 'G':
		{
			mIsNight = !mIsNight;
			mChangesLightYpos = !mChangesLightYpos;

			if (mIsNight)
				SoundEngine::Get()->Play("owl.mp3");
			else
				SoundEngine::Get()->Play("chicken.mp3");
		}
			break;
		case 'p': case 'P':
			Pause();
			break;
		case 'r': case 'R':
			Resume();
			break;
		case 't': case 'T':
			// TODO: Make players invincible
			mPlayer1->SetInvincible( !mPlayer1->GetInvincible() );
			mPlayer2->SetInvincible( !mPlayer2->GetInvincible() );
			break;
		case 49: // Numpad 1
			mPlayer1->GetCamera()->SetTopView();
			mPlayer2->GetCamera()->SetTopView();
			break;
		case 50: // Numpad 2
			mPlayer1->GetCamera()->SetFPS();
			mPlayer2->GetCamera()->SetFPS();
			break;
		case 51: // Numpad 3
			mPlayer1->GetCamera()->SetTPS();
			mPlayer2->GetCamera()->SetTPS();
			break;
	}
}

void MainScene::Update()
{
	if (mCurStage < mStage.size())
		CreatePlane();

	CollisionCheck();

	if (IsWin())
		mGfw->ChangeScene("winning");

	else if (mPlayer1->GetIsDead() && mPlayer2->GetIsDead())
		mGfw->ChangeScene("dead");

	if (mChangesLightYpos)
	{
		if (mIsNight)
			mDirLightYPos += mGfw->dt * 12.5f;
		else
			mDirLightYPos -= mGfw->dt * 12.5f;

		if (mDirLightYPos < -20.0f || mDirLightYPos > 20.0f)
			mChangesLightYpos = false;
	}
}

void MainScene::Draw()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// Render bottom screen
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	mPhongShader->SetActive();
	auto camera = mPlayer1->GetCamera();
	SetLightUniforms(camera->GetCameraPos());
	glViewport(0, 0, mGfw->GetScrWidth(), mGfw->GetScrHeight() / 2);
	mPhongShader->SetMatrix4Uniform("uView", camera->GetView());
	for (auto& mesh : mGfw->GetMeshes())
		mesh->Draw(mPhongShader);

	mBackground->Draw();

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
	mSpriteShader->SetActive();
	auto& sprites = mGfw->GetSpritesAt(Gfw::SpriteLayer::kBottom);
	for (auto sprite : sprites)
		sprite->Draw(mSpriteShader);

	// Render Top screen
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	mPhongShader->SetActive();
	camera = mPlayer2->GetCamera();
	SetLightUniforms(camera->GetCameraPos());
	glViewport(0, mGfw->GetScrHeight() / 2, mGfw->GetScrWidth(), mGfw->GetScrHeight() / 2);
	mPhongShader->SetMatrix4Uniform("uView", camera->GetView());
	for (auto& mesh : mGfw->GetMeshes())
		mesh->Draw(mPhongShader);

	mBackground->Draw();

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
	mSpriteShader->SetActive();
	auto& spritez = mGfw->GetSpritesAt(Gfw::SpriteLayer::kTop);
	for (auto sprite : spritez)
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

void MainScene::LoadData()
{
	mPlayer1 = new Player{ mGfw, Player::kP1 };
	mPlayer2 = new Player{ mGfw, Player::kP2 };

	auto frame = new Actor{ mGfw };
	frame->SetScale(2.0f);
	auto sc = new SpriteComponent{ frame, "Assets/rabbit_frame.png", Gfw::SpriteLayer::kBottom };

	frame = new Actor{ mGfw };
	frame->SetScale(2.0f);
	sc = new SpriteComponent{ frame, "Assets/crow_frame.png", Gfw::SpriteLayer::kTop };

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

	assert(mStage.size() >= 10 && "Stage must be more than 10!");
	assert(mStage.size() % 10 == 0 && "Stage must be divided by 10!");
}

void MainScene::CreatePlane()
{
	auto zPos = GetBehindPlayerZPos();
	for (; mCurStage < -zPos + 10; ++mCurStage)
	{
		auto plane = new Plane{ mGfw, static_cast<Plane::PlaneType>(mStage[mCurStage]), mCurStage };
		plane->SetDisabled(true);
		new Plane{ mGfw, static_cast<Plane::PlaneType>(mStage[mCurStage]), mCurStage, true };
	}
}

float MainScene::GetBehindPlayerZPos()
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
	auto zPos = GetBehindPlayerZPos();

	// Collision check with vehicles
	for (auto vehicle : mGfw->GetActorsAt(Gfw::Layer::kVehicle))
	{
		if (vehicle->GetState() != Actor::State::kActive)
			continue;

		auto vp = (Vehicle*)vehicle;

		auto z = vp->GetPosition().z;
		if (zPos + 6.0f < z)
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

	// Collision check with planes
	for (auto plane : mGfw->GetActorsAt(Gfw::Layer::kPlane))
	{
		if (plane->GetState() != Actor::State::kActive)
			continue;

		auto pp = (Plane*)plane;

		auto z = pp->GetPosition().z;
		if (zPos + 6.0f < z)
		{
			pp->SetState(Actor::State::kDead);
			mPlayer1->SetZBorder(glm::vec2{ z - 2.0f, z - 16.0f });
			mPlayer2->SetZBorder(glm::vec2{ z - 2.0f, z - 16.0f });
			continue;
		}

		if (pp->GetType() == Plane::PlaneType::kWater)
			continue;

		if (Intersects(p1Box, pp))
		{
			mPlayer1->NotToFall();
		}

		if (Intersects(p2Box, pp))
		{
			mPlayer2->NotToFall();
		}
	}

	// Tree must die
	for (auto tree : mGfw->GetActorsAt(Gfw::Layer::kTree))
	{
		if (tree->GetState() != Actor::State::kActive)
			continue;

		auto tp = (Tree*)tree;
		auto z = tp->GetPosition().z;

		if (zPos + 6.0f < z)
		{
			tp->SetState(Actor::State::kDead);
			continue;
		}
	}

	// Treasure must die
	for (auto trea : mGfw->GetActorsAt(Gfw::Layer::kTreasure))
	{
		if (trea->GetState() != Actor::State::kActive)
			continue;

		auto tp = (Treasure*)trea;
		auto z = tp->GetPosition().z;

		if (zPos + 6.0f < z)
		{
			tp->SetState(Actor::State::kDead);
			continue;
		}
	}

	// Check with items
	for (auto item : mGfw->GetActorsAt(Gfw::Layer::kItem))
	{
		if (item->GetState() != Actor::State::kActive)
			continue;

		auto ip = (Item*)item;
		auto z = ip->GetPosition().z;

		if (zPos + 6.0f < z)
		{
			ip->SetState(Actor::State::kDead);
			continue;
		}

		const auto& itemBox = ip->GetBox()->GetWorldBox();
		if (Intersects(p1Box, itemBox))
		{
			auto type = ip->GetType();
			switch (type)
			{
				case Item::kPotion:
					mPlayer1->IncreaseHp();
					break;
				case Item::kPoison:
					mPlayer1->DecreaseHp();
					break;
				case Item::kBomb:
					mPlayer1->DropBomb();
					break;
				case Item::kBallon:
					if (mPlayer1->GetState() == Actor::State::kActive && mPlayer2->GetState() == Actor::State::kActive)
					{
						mPlayer1->Fly();
						mPlayer2->Fly();
					}
					break;
			}

			ip->SetState(Actor::State::kDead);
		}

		if (Intersects(p2Box, itemBox))
		{
			auto type = ip->GetType();
			switch (type)
			{
				case Item::kPotion:
					mPlayer2->IncreaseHp();
					break;
				case Item::kPoison:
					mPlayer2->DecreaseHp();
					break;
				case Item::kBomb:
					mPlayer2->DropBomb();
					break;
				case Item::kBallon:
					if (mPlayer1->GetState() == Actor::State::kActive && mPlayer2->GetState() == Actor::State::kActive)
					{
						mPlayer2->Fly();
						mPlayer1->Fly();
					}
					break;
			}

			ip->SetState(Actor::State::kDead);
		}
	}
}

bool MainScene::IsWin()
{
	if (GetBehindPlayerZPos() <= -2.0f * (mStage.size() - 1))
		return true;

	return false;
}

void MainScene::SetLightUniforms(const glm::vec3& cameraPos)
{
	// Directional light
	mPhongShader->SetVectorUniform("uViewPos", cameraPos);
	mPhongShader->SetVectorUniform("uDirLight.direction", glm::vec3{ -24.0f, mDirLightYPos, -0.1f });
	mPhongShader->SetVectorUniform("uDirLight.ambient", glm::vec3{ 0.0f });
	mPhongShader->SetVectorUniform("uDirLight.diffuse", glm::vec3{ 1.0f });
	mPhongShader->SetVectorUniform("uDirLight.specular", glm::vec3{ 1.0f });


	// Player spot light
	int curLights{ 0 };
	auto& players = mGfw->GetActorsAt(Gfw::Layer::kPlayer);
	for (auto player : players)
	{
		auto pp = (Player*)player;

		const auto& pos = pp->GetPosition();
		std::string path = "uSpotLights[" + std::to_string(curLights) + "].";
		mPhongShader->SetVectorUniform(path + "position", glm::vec3{ pos.x, pos.y + 5.0f, pos.z });
		mPhongShader->SetVectorUniform(path + "direction", glm::vec3{ 0.0f, pos.y - 0.1f, 0.0f });
		mPhongShader->SetVectorUniform(path + "ambient", glm::vec3{ 0.3f });
		mPhongShader->SetVectorUniform(path + "diffuse", glm::vec3{ 1.0f });
		mPhongShader->SetVectorUniform(path + "specular", glm::vec3{ 1.0f });
		mPhongShader->SetFloatUniform(path + "constant", 1.0f);
		mPhongShader->SetFloatUniform(path + "linear", 0.0045f);
		mPhongShader->SetFloatUniform(path + "quadratic", 0.0075f);
		mPhongShader->SetFloatUniform(path + "cutOff", glm::cos(glm::radians(12.5f)));
		mPhongShader->SetFloatUniform(path + "outerCutOff", glm::cos(glm::radians(17.5f)));

		++curLights;
	}


	// Vehicle spot light
	const int spotLimit{ 20 };
	auto& vehicles = mGfw->GetActorsAt(Gfw::Layer::kVehicle);
	for (auto vehicle : vehicles)
	{
		if (curLights >= spotLimit)
			break;

		auto vp = (Vehicle*)vehicle;
		if (vp->GetType() == Vehicle::kLog)
			continue;

		const auto& pos = vp->GetPosition();
		if (fabs(pos.z - GetBehindPlayerZPos() > 12.0f))
			continue;

		std::string path = "uSpotLights[" + std::to_string(curLights) + "].";
		mPhongShader->SetVectorUniform(path + "position", vp->GetPosition());
		auto direction = -vp->GetRight() + glm::vec3{ 0.0f, -0.1f, 0.0f };
		mPhongShader->SetVectorUniform(path + "direction", direction);
		mPhongShader->SetVectorUniform(path + "ambient", glm::vec3{ 0.3f });
		mPhongShader->SetVectorUniform(path + "diffuse", glm::vec3{ 1.0f });
		mPhongShader->SetVectorUniform(path + "specular", glm::vec3{ 1.0f });
		mPhongShader->SetFloatUniform(path + "constant", 1.0f);
		mPhongShader->SetFloatUniform(path + "linear", 0.0045f);
		mPhongShader->SetFloatUniform(path + "quadratic", 0.0075f);
		mPhongShader->SetFloatUniform(path + "cutOff", glm::cos(glm::radians(12.5f)));
		mPhongShader->SetFloatUniform(path + "outerCutOff", glm::cos(glm::radians(17.5f)));

		++curLights;
	}

	for (int i = curLights; i < spotLimit; ++i)
	{
		std::string path = "uSpotLights[" + std::to_string(i) + "].";
		mPhongShader->SetVectorUniform(path + "position", glm::vec3{ 0.0f });
		mPhongShader->SetVectorUniform(path + "direction", glm::vec3{ 0.0f });
		mPhongShader->SetVectorUniform(path + "ambient", glm::vec3{ 0.0f });
		mPhongShader->SetVectorUniform(path + "diffuse", glm::vec3{ 1.0f });
		mPhongShader->SetVectorUniform(path + "specular", glm::vec3{ 1.0f });
		mPhongShader->SetFloatUniform(path + "constant", 1.0f);
		mPhongShader->SetFloatUniform(path + "linear", 0.027f);
		mPhongShader->SetFloatUniform(path + "quadratic", 0.0028f);
		mPhongShader->SetFloatUniform(path + "cutOff", glm::cos(glm::radians(12.5f)));
		mPhongShader->SetFloatUniform(path + "outerCutOff", glm::cos(glm::radians(17.5f)));
	}
}