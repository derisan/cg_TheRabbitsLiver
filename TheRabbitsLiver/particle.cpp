#include "particle.h"

#include "mesh_component.h"
#include "mesh.h"
#include "random.h"

Particle::Particle(Gfw* gfw, ParticleType type, const glm::vec3 startPoint, Gfw::Layer layer)
	: Actor{ gfw, layer },
	mGravity{ 0.0f, -5.0f, 0.0f },
	mVelocity{ Random::GetFloatRange(-1.0f, 1.0f), Random::GetFloatRange(3.0f, 5.0f), Random::GetFloatRange(-1.0f, 1.0f) },
	mLife{ Random::GetFloatRange(1.0f, 2.5f) },
	mType{ type }
{
	std::string file;

	switch (type)
	{
		case kFire:
			file = "Assets/fireball.gpmesh";
			SetScale(0.075f);
			break;
		case kCarrot:
			file = "Assets/carrot.gpmesh";
			SetScale(0.075f);
			SetRotation(Random::GetFloatRange(0.0f, 360.0f));
			break;
		case kHeart:
			file = "Assets/heart.gpmesh";
			SetScale(0.01f);
			break;
	}
	auto pos = startPoint + Random::GetVec3(-0.5f, 0.5f);
	SetPosition(pos);
	
	auto mc = new MeshComponent{ this, file };
}

void Particle::UpdateActor()
{
	if (mLife < 0)
	{
		SetState(State::kDead);
		return;
	}

	mLife -= mGfw->dt;
	mVelocity += mGravity * mGfw->dt;
	auto pos = GetPosition() + mVelocity * mGfw->dt;
	SetPosition(pos);
}