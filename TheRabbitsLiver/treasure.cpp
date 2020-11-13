#include "treasure.h"

#include <glm/gtx/norm.hpp>

#include "mesh_component.h"
#include "mesh.h"
#include "player.h"
#include "sound_engine.h"
#include "item.h"
#include "random.h"

Treasure::Treasure(Gfw* gfw, Gfw::Layer layer)
	: Actor{ gfw, layer },
	mHeart{ nullptr },
	mPercentage{ 0.0f }
{
	auto mc = new MeshComponent{ this, "Assets/chest.gpmesh" };

	SetScale(0.02f);
	SetRotation(180.0f);
}

void Treasure::UpdateActor()
{
	Actor::UpdateActor();

	if (mPercentage > 3.0f)
	{
		SoundEngine::Get()->Play("chest_opening.wav", 2.0f);

		GenerateItem();

		// Pop Items
		mHeart->SetState(State::kDead);
		SetState(State::kDead);
	}
}

void Treasure::ActorInput(unsigned char key)
{
	if (key == 32)
	{
		if (!mHeart)
		{
			mHeart = new Actor{ mGfw };
			auto mc = new MeshComponent{ mHeart, "Assets/heart.gpmesh" };
			mHeart->SetScale(0.0f);

			const auto& pos = GetPosition();
			mHeart->SetPosition(glm::vec3{ pos.x, 3.0f, pos.z });
		}

		auto players = mGfw->GetActorsAt(Gfw::Layer::kPlayer);
		for (auto player : players)
		{
			if (player->GetState() != Actor::State::kActive)
				continue;

			auto pp = (Player*)player;

			if (glm::distance2(pp->GetPosition(), GetPosition()) <= 6.0f)
			{
				mPercentage += 2.0f * mGfw->dt;
				mHeart->SetScale(mPercentage * 0.005f);
			}
		}
	}
}

void Treasure::GenerateItem()
{
	auto item = new Item{ mGfw };
	
	auto yOffset{ 0 };
	switch (item->GetType())
	{
		case Item::kPotion:
			yOffset = 1.0f;
			break;
	}

	const auto& pos = GetPosition();
	item->SetPosition(glm::vec3{ pos.x, pos.y + yOffset, pos.z });

}