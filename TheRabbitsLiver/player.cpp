#include "player.h"

#include <iostream>

#include "mesh_component.h"
#include "box_component.h"
#include "camera_component.h"
#include "mesh.h"

#include "vehicle.h"

Player::Player(Gfw* gfw, PlayerType type, Gfw::Layer layer)
	: Actor{ gfw, layer },
	mBox{ nullptr },
	mCamera{ nullptr },
	mType{ type },
	mBorder{ {-24.0f, 24.0f}, {0.0f, -16.0f} },
	mPrevMovement{ 0.0f }
{
	std::string file;
	if (type == PlayerType::kP1)
	{
		file = "Assets/bunny.gpmesh";
	}
	else
	{
		file = "Assets/bird.gpmesh";
		SetScale(0.1f);
	}

	auto mc = new MeshComponent{ this, file };

	mBox = new BoxComponent{ this };
	mBox->SetObjectBox(mc->GetMesh()->GetBox());

	mCamera = new CameraComponent{ this };
}

void Player::UpdateActor()
{
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
			break;
		case 's': case 'S':
			pos -= forward * 2.0f;
			break;
		case 'a': case 'A':
			pos -= right * 1.2f;
			break;
		case 'd': case 'D':
			pos += right * 1.2f;
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
			break;
		case 'k': case 'K':
			pos -= forward * 2.0f;
			break;
		case 'j': case 'J':
			pos -= right * 1.2f;
			break;
		case 'l': case 'L':
			pos += right * 1.2f;
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