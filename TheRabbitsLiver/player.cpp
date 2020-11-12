#include "player.h"

#include <iostream>

#include "mesh_component.h"
#include "box_component.h"
#include "camera_component.h"
#include "mesh.h"

Player::Player(Gfw* gfw, PlayerType type, Gfw::Layer layer)
	: Actor{ gfw, layer },
	mBox{ nullptr },
	mCamera{ nullptr },
	mType{ type }
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

