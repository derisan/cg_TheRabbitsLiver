#include "vehicle.h"

#include "mesh_component.h"
#include "box_component.h"
#include "camera_component.h"
#include "mesh.h"

Vehicle::Vehicle(Gfw* gfw, VehicleType type, Gfw::Layer layer)
	: Actor{ gfw, layer },
	mBox{ nullptr },
	mSpeed{ 0.0f },
	mGenTerm{ 0.0f },
	mType{ type }
{
	std::string file;

	switch (type)
	{
		case kCar:
			file = "Assets/car.gpmesh";
			SetScale(0.1f);
			mGenTerm = 2.0f;
			break;
		case kTruck:
			file = "Assets/truck.gpmesh";
			SetScale(0.15f);
			mGenTerm = 3.0f;
			break;
		case kBus:
			file = "Assets/bus.gpmesh";
			SetScale(0.1f);
			mGenTerm = 5.0f;
			break;
		case kLog:
			file = "Assets/log.gpmesh";
			break;
	}

	auto mc = new MeshComponent{ this, file };
	mBox = new BoxComponent{ this };
	mBox->SetObjectBox(mc->GetMesh()->GetBox());
}

void Vehicle::UpdateActor()
{
	Actor::UpdateActor();

	auto right = GetRight();

	auto pos = GetPosition();
	pos += right * mSpeed * mGfw->dt;

	if (pos.x > 40.0f || pos.x < -15.0f)
		SetState(State::kDead);

	SetPosition(pos);
}