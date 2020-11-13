#include "tombstone.h"

#include "mesh_component.h"
#include "mesh.h"
#include "player.h"
#include "sound_engine.h"

Tombstone::Tombstone(Gfw* gfw, Player* target, Gfw::Layer layer)
	: Actor{ gfw, layer },
	mTarget{ target },
	mPercentage{ 0.0f }
{
	auto mc = new MeshComponent{ this, "Assets/skull.gpmesh" };

	const auto& pos = mTarget->GetPosition();

	SetScale(0.3f);
	SetRotation(180.0f);
	SetPosition(glm::vec3{ pos.x - 1.0f, pos.y, pos.z });
}

void Tombstone::UpdateActor()
{
	Actor::UpdateActor();


}

void Tombstone::ActorInput(unsigned char key)
{
	Actor::ActorInput(key);
}