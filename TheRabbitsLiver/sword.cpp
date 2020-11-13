#include "sword.h"

#include "mesh_component.h"
#include "box_component.h"
#include "mesh.h"
#include "sound_engine.h"

#include "player.h"

Sword::Sword(Gfw* gfw, Player* target, Gfw::Layer layer)
	: Actor{ gfw, layer },
	mBox{ nullptr },
	mTarget{ target },
	mSpeed{ -9.8f }
{
	auto mc = new MeshComponent{ this, "Assets/sword.gpmesh" };

	mBox = new BoxComponent{ this };
	mBox->SetObjectBox(mc->GetMesh()->GetBox());

	const auto& pos = target->GetPosition();
	
	SetPosition(glm::vec3{ pos.x, pos.y + 8.0f, pos.z });
}

void Sword::UpdateActor()
{
	Actor::UpdateActor();

	auto pos = GetPosition();
	pos.y = mSpeed * mGfw->dt;

	SetPosition(pos);
	
	const auto& playerBox = mTarget->GetBox()->GetWorldBox();
	if (Intersects(playerBox, mBox->GetWorldBox()))
	{
		SoundEngine::Get()->Stop("slashing.wav");
		SoundEngine::Get()->Play("slashing.wav");

		SetState(State::kDead);
	}
}