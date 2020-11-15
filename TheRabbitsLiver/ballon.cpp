#include "ballon.h"

#include "mesh_component.h"
#include "mesh.h"
#include "sound_engine.h"

#include "player.h"
#include "vehicle.h"

Ballon::Ballon(Gfw* gfw, Player* target, Gfw::Layer layer)
	: Actor{ gfw, layer },
	mTarget{ target },
	mVelocity{ 0.0f, 3.0f, 0.0f },
	mDistance{ 0.0f }
{
	auto mc = new MeshComponent{ this, "Assets/ballon.gpmesh" };
	SetScale(0.3f);
}

void Ballon::UpdateActor()
{
	Actor::UpdateActor();

	auto pos = GetPosition();
	pos += mVelocity * mGfw->dt;
	mDistance += fabs(mVelocity.z * mGfw->dt);

	if (pos.y > 10.0f)
	{
		pos.y = 10.0f;
		mVelocity = glm::vec3{ 0.0f, 0.0f, -5.0f };
	}
	if (mDistance > 20.0f)
	{
		SoundEngine::Get()->Play("pop.mp3", 3.0f);
		SetState(State::kDead);
	}

	mTarget->SetPosition(glm::vec3{ pos.x, pos.y - 2.0f, pos.z });
	SetPosition(pos);
}