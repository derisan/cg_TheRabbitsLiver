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
#include "bomb.h"
#include "ballon.h"


Player::Player(Gfw* gfw, PlayerType type, Gfw::Layer layer)
	: Actor{ gfw, layer },
	mBox{ nullptr },
	mCamera{ nullptr },
	mType{ type },
	mBorder{ {-24.0f, 24.0f}, {0.0f, -16.0f} },
	mPrevMovement{ 0.0f },
	mLives{ 3 },
	mInvincibleTime{ 0.0f },
	mIsInvincible{ false },
	mIsDead{ false }
{
	std::string meshFile;
	if (type == PlayerType::kP1)
	{
		meshFile = "Assets/bunny.gpmesh";
		mLifeImgFile = "Assets/life_carrot.png";
	}
	else
	{
		meshFile = "Assets/bird.gpmesh";
		mLifeImgFile = "Assets/life_heart.png";
		SetScale(0.1f);
	}

	auto mc = new MeshComponent{ this, meshFile };

	mBox = new BoxComponent{ this };
	mBox->SetObjectBox(mc->GetMesh()->GetBox());

	mCamera = new CameraComponent{ this };

	GenerateLifeSprite(mLifeImgFile);
}

void Player::UpdateActor()
{
	mInvincibleTime -= mGfw->dt;
	if( mIsInvincible )
		mInvincibleTime = 0.0f;

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

	pos.y -= 5.0f * mGfw->dt;
	if (pos.y < -2.0f)
	{
		SoundEngine::Get()->Play("waterfall.mp3", 2.0f);
		YouDie();
	}



	SetPosition(pos);

	if (mLives <= 0)
	{
		YouDie();
	}
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

void Player::NotToFall()
{
	auto pos = GetPosition();
	pos.y = 0.0f;
	SetPosition(pos);
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

		Particle::ParticleType type;
		if (mType == Player::kP1)
			type = Particle::kCarrot;
		else
			type = Particle::kHeart;

		for (int i = 0; i < 5; ++i)
			new Particle{ mGfw, type, GetPosition() };

		if (mLives >= 0)
		{
			mLifeGauges[mLives]->SetState(State::kDead);
			mLifeGauges.pop_back();
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
	for (size_t i = mLifeGauges.size(); i < mLives; ++i)
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

void Player::Reincarnation()
{
	SoundEngine::Get()->Play("reincarnation.wav");

	SetState(State::kActive);
	mIsDead = false;
	mLives = 3;
	mInvincibleTime = 1.5f;

	GenerateLifeSprite(mLifeImgFile);

	auto& players = mGfw->GetActorsAt(Gfw::Layer::kPlayer);
	for (auto player : players)
	{
		auto pp = (Player*)player;
		if (pp->GetType() == mType)
			continue;

        const auto& pos = pp->GetPosition();
		SetPosition(pos);
	}
}

void Player::IncreaseHp()
{
	SoundEngine::Get()->Play("drinking.wav", 2.0f);

	if (mLives < 3)
	{
		++mLives;
		GenerateLifeSprite(mLifeImgFile);
	}
}

void Player::DecreaseHp()
{
	SoundEngine::Get()->Play("drinking_poison.mp3", 2.0f);

	if (mLives > 0)
	{
		--mLives;
		mLifeGauges[mLives]->SetState(State::kDead);
		mLifeGauges.pop_back();
		GenerateLifeSprite(mLifeImgFile);
	}
}

void Player::DropBomb()
{
	auto& vehicles = mGfw->GetActorsAt(Gfw::Layer::kVehicle);
	int bombs{ 10 };
	for (auto vehicle : vehicles)
	{
		if (vehicle->GetState() != State::kActive)
			continue;

		auto vp = (Vehicle*)vehicle;

		if (vp->GetType() == Vehicle::kLog)
			continue;

		const auto& vPos = vp->GetPosition();
		const auto& pPos = GetPosition();
		if (vPos.z > pPos.z || fabs(vPos.z - pPos.z) > 10.0f)
			continue;

		// Generate bomb
		new Bomb{ mGfw, vp };

		--bombs;

		if (bombs == 0)
			break;
	}
}

void Player::Fly()
{
	auto ballon = new Ballon{ mGfw, this };
	const auto& pos = GetPosition();
	SetPosition(glm::vec3{ pos.x, 5.0f, pos.z });
	ballon->SetPosition(glm::vec3{ pos.x, pos.y + 1.0f, pos.z });
}