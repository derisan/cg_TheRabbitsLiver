#include "player.h"

#include "mesh_component.h"
#include "box_component.h"
#include "sprite_component.h"
#include "camera_component.h"
#include "mesh.h"
#include "sound_engine.h"

#include "vehicle.h"
#include "tree.h"
#include "particle.h"
#include "sword.h"


Player::Player(Gfw* gfw, PlayerType type, Gfw::Layer layer)
	: Actor{ gfw, layer },
	mBox{ nullptr },
	mCamera{ nullptr },
	mType{ type },
	mBorder{ {-24.0f, 24.0f}, {0.0f, -16.0f} },
	mPrevMovement{ 0.0f },
	mLives{ 3 },
	mInvincibleTime{ 0.0f },
	mIsDead{ false }
{
	std::string meshFile;
	std::string lifeImgFile;
	if (type == PlayerType::kP1)
	{
		meshFile = "Assets/bunny.gpmesh";
		lifeImgFile = "Assets/life_carrot.png";
	}
	else
	{
		meshFile = "Assets/bird.gpmesh";
		lifeImgFile = "Assets/life_heart.png";
		SetScale(0.1f);
	}

	auto mc = new MeshComponent{ this, meshFile };

	mBox = new BoxComponent{ this };
	mBox->SetObjectBox(mc->GetMesh()->GetBox());

	mCamera = new CameraComponent{ this };

	GenerateLifeSprite(lifeImgFile);
}

void Player::UpdateActor()
{
	mInvincibleTime -= mGfw->dt;

	CheckCollisionWithTree();

	auto pos = GetPosition();
	if (pos.x < mBorder.x.x)
		pos.x = mBorder.x.x;
	else if (pos.x > mBorder.x.y)
		pos.x = mBorder.x.y;

	if (pos.z > mBorder.z.x)
		pos.z = mBorder.z.x;
	else if (pos.z < mBorder.z.y)
		pos.z = mBorder.z.y;

	SetPosition(pos);
}

void Player::ActorInput(unsigned char key)
{
	if (mType == PlayerType::kP1)
		PlayerOneInput(key);
	else
		PlayerTwoInput(key);
}

void Player::PlayerOneInput(unsigned char key)
{
	auto pos = GetPosition();
	const auto& forward = GetForward();
	const auto& right = GetRight();

	switch (key)
	{
		case 'w': case 'W':
			pos += forward * 2.0f;
			mPrevMovement = -(forward * 2.0f);
			break;
		case 's': case 'S':
			pos -= forward * 2.0f;
			mPrevMovement = (forward * 2.0f);
			break;
		case 'a': case 'A':
			pos -= right * 1.2f;
			mPrevMovement = (right * 1.2f);
			break;
		case 'd': case 'D':
			pos += right * 1.2f;
			mPrevMovement = -(right * 1.2f);
			break;
	}

	SetPosition(pos);
}

void Player::PlayerTwoInput(unsigned char key)
{
	auto pos = GetPosition();
	const auto& forward = GetForward();
	const auto& right = GetRight();

	switch (key)
	{
		case 'i': case 'I':
			pos += forward * 2.0f;
			mPrevMovement = -(forward * 2.0f);
			break;
		case 'k': case 'K':
			pos -= forward * 2.0f;
			mPrevMovement = (forward * 2.0f);
			break;
		case 'j': case 'J':
			pos -= right * 1.2f;
			mPrevMovement = (right * 1.2f);
			break;
		case 'l': case 'L':
			pos += right * 1.2f;
			mPrevMovement = -(right * 1.2f);
			break;
	}

	SetPosition(pos);
}

void Player::Fall()
{
	// Do something when player steps into water
}

void Player::OnBoard(Vehicle* log)
{
	auto pos = GetPosition();
	pos.x = log->GetPosition().x;
	pos.y = 0.1f;

	SetPosition(pos);
}

void Player::HitByCar()
{
	if (mInvincibleTime < 0.0f)
	{
		--mLives;
		mInvincibleTime = 1.5f;
		SoundEngine::Get()->Stop("hit_sound.mp3");
		SoundEngine::Get()->Play("hit_sound.mp3");

		for (int i = 0; i < 5; ++i)
			new Particle{ mGfw, Particle::kCarrot, GetPosition() };

		if (mLives >= 0)
		{
			mLifeGauges[mLives]->SetState(State::kDead);
			mLifeGauges.pop_back();
		}

		if (mLives == 0)
		{
			YouDie();
		}
	}
}

void Player::CheckCollisionWithTree()
{
	// Check collision with tree
	const auto& playerBox = mBox->GetWorldBox();
	for (auto tree : mGfw->GetActorsAt(Gfw::kTree))
	{
		if (tree->GetState() != Actor::State::kActive)
			continue;

		auto tp = (Tree*)tree;

		const auto& treeBox = tp->GetBox()->GetWorldBox();
		if (Intersects(playerBox, treeBox))
		{
			auto pos = GetPosition() + mPrevMovement;
			SetPosition(pos);
			break;
		}
	}
}

void Player::GenerateLifeSprite(const std::string& file)
{
	for (int i = 0; i < mLives; ++i)
	{
		auto life = new Actor{ mGfw };
		auto sc = new SpriteComponent{ life, file, static_cast<Gfw::SpriteLayer>(mType) };
		life->SetScale(0.3f);
		life->SetPosition(glm::vec3{ -0.9f, 0.4f - 0.3f * i, 0.0f });
		mLifeGauges.emplace_back(life);
	}
}

void Player::YouDie()
{
	new Sword{ mGfw, this };
	mIsDead = true;
	SetState(State::kPaused);
}