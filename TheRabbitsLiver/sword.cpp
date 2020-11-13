#include "sword.h"

#include "mesh_component.h"
#include "box_component.h"
#include "mesh.h"
#include "sound_engine.h"

#include "player.h"
#include "particle.h"

Sword::Sword(Gfw* gfw, Player* target, Gfw::Layer layer)
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
}

void Sword::UpdateActor()
{
	Actor::UpdateActor();

	auto pos = GetPosition();
	pos.y += mSpeed * mGfw->dt;

	SetPosition(pos);
	
	const auto& playerBox = mTarget->GetBox()->GetWorldBox();
	if (Intersects(playerBox, mBox->GetWorldBox()))
	{
		SoundEngine::Get()->Stop("penguin_drop.wav");
		SoundEngine::Get()->Play("penguin_boom.wav");
		
		for (int i = 0; i < 5; ++i)
			new Particle{ mGfw, Particle::kFire, mTarget->GetPosition() };

		std::string soundFile;
		if (mTarget->GetType() == Player::PlayerType::kP1)
			soundFile = "p1_dead.mp3";
		else
			soundFile = "p2_dead.mp3";

		SoundEngine::Get()->Play(soundFile);

		SetState(State::kDead);
	}
}