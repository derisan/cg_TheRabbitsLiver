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

}

