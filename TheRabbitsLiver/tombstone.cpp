#include "tombstone.h"

#include <iostream>

#include <glm/gtx/norm.hpp>

#include "mesh_component.h"
#include "mesh.h"
#include "player.h"
#include "sound_engine.h"

#include "player.h"

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

	if (mPercentage > 5.0f)
	{
		mTarget->Reincarnation();
		SetState(State::kDead);
	}
}

void Tombstone::ActorInput(unsigned char key)
{
	Actor::ActorInput(key);

	if (key == 32)
	{
		auto players = mGfw->GetActorsAt(Gfw::Layer::kPlayer);
		for (auto player : players)
		{
			auto pp = (Player*)player;

			if (pp->GetType() == mTarget->GetType())
				continue;

			if (glm::distance2(pp->GetPosition(), GetPosition()) <= 5.0f)
				mPercentage += 2.0f * mGfw->dt;
		}
	}
}