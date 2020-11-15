#include "bomb.h"

#include "mesh_component.h"
#include "box_component.h"
#include "mesh.h"
#include "sound_engine.h"

#include "particle.h"
#include "vehicle.h"

Bomb::Bomb(Gfw* gfw, Vehicle* target, Gfw::Layer layer)
	: Actor{ gfw, layer },
	mBox{ nullptr },
	mTarget{ target },
	mSpeed{ -10.8f }
{
	auto mc = new MeshComponent{ this, "Assets/penguin.gpmesh" };

	mBox = new BoxComponent{ this };
	mBox->SetObjectBox(mc->GetMesh()->GetBox());

	const auto& pos = target->GetPosition();

	SetPosition(glm::vec3{ pos.x, pos.y + 10.0f, pos.z });
	SetScale(0.1f);
	SoundEngine::Get()->Play("penguin_drop.wav");

	mTarget->SetSpeed(0.0f);
}

void Bomb::UpdateActor()
{
	Actor::UpdateActor();

	auto pos = GetPosition();
	pos.y += mSpeed * mGfw->dt;

	SetPosition(pos);

	const auto& vehicleBox = mTarget->GetBox()->GetWorldBox();
	if (Intersects(vehicleBox, mBox->GetWorldBox()))
	{
		SoundEngine::Get()->Stop("penguin_drop.wav");
		SoundEngine::Get()->Play("penguin_boom.wav");

		for (int i = 0; i < 5; ++i)
			new Particle{ mGfw, Particle::kFire, mTarget->GetPosition() };

		SetState(State::kDead);
		mTarget->SetState(State::kDead);
	}
}