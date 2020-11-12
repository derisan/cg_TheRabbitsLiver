#include "plane.h"

#include "mesh_component.h"
#include "box_component.h"
#include "mesh.h"
#include "random.h"

Plane::Plane(Gfw* gfw, PlaneType type, int curStage, bool right, Gfw::Layer layer)
	: Actor{ gfw, layer },
	mBox{ nullptr },
	mType{ type },
	mVehicleType{ Vehicle::kCar },
	mCooldown{ 0.0f },
	mLeftOrRight{ Random::GetChoice(-1, 1) },
	mDisabled{ false }
{
	std::string file;

	auto yOffset{ -0.1f };
	switch (type)
	{
		case PlaneType::kGrass:
			file = "Assets/grass.gpmesh";
			yOffset -= 0.1f;
			break;
		case PlaneType::kRoad:
			file = "Assets/road.gpmesh";
			mVehicleType = static_cast<Vehicle::VehicleType>(Random::GetIntRange(0, 1));
			break;
		case PlaneType::kRailroad:
			file = "Assets/railroad.gpmesh";
			mVehicleType = Vehicle::kBus;
			yOffset -= 0.1f;
			break;
		case PlaneType::kWater:
			file = "Assets/water.gpmesh";
			mVehicleType = Vehicle::kLog;
			break;
		case PlaneType::kGoal:
			break;
	}

	if(!right)
		SetPosition(glm::vec3{ -12.0f, yOffset, -2.0f * curStage });
	else
		SetPosition(glm::vec3{ 12.0f, yOffset, -2.0f * curStage });

	auto mc = new MeshComponent{ this, file };
	mBox = new BoxComponent{ this };
	mBox->SetObjectBox(mc->GetMesh()->GetBox());
}

void Plane::UpdateActor()
{
	Actor::UpdateActor();

	mCooldown -= mGfw->dt;
	if (!mDisabled && mCooldown < 0)
		GenerateVehicle();
}

void Plane::GenerateVehicle()
{
	if (mType == PlaneType::kGrass || mType == PlaneType::kGoal)
		return;

	const auto& pos = GetPosition();
	auto vehicle = new Vehicle{ mGfw, mVehicleType };
	vehicle->SetPosition(glm::vec3{ mLeftOrRight * 27.0f, pos.y + 0.1f, pos.z });
	vehicle->SetSpeed(-5.0f);
	if (mLeftOrRight == -1)
		vehicle->SetRotation(180.0f);
	mCooldown = Random::GetFloatRange(2.0f, 4.0f) + vehicle->GetGenTerm();
}