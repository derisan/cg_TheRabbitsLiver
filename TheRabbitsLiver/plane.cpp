#include "plane.h"

#include "mesh_component.h"
#include "box_component.h"
#include "mesh.h"
#include "random.h"

#include "tree.h"
#include "treasure.h"

Plane::Plane(Gfw* gfw, PlaneType type, int curStage, bool right, Gfw::Layer layer)
	: Actor{ gfw, layer },
	mBox{ nullptr },
	mType{ type },
	mVehicleType{ Vehicle::kCar },
	mCooldown{ Random::GetFloatRange(0.0f, 2.0f) },
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
			file = "Assets/carpet.gpmesh";
			break;
	}

	if(!right)
		SetPosition(glm::vec3{ -12.0f, yOffset, -2.0f * curStage });
	else
		SetPosition(glm::vec3{ 12.0f, yOffset, -2.0f * curStage });

	auto mc = new MeshComponent{ this, file };
	mBox = new BoxComponent{ this };
	mBox->SetObjectBox(mc->GetMesh()->GetBox());

	if (mType == PlaneType::kGrass)
		GenerateTree();
	else if (mType == PlaneType::kRailroad)
		GenerateTreasure();
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
	if (mType == PlaneType::kGrass || mType == PlaneType::kGoal || mType == PlaneType::kGoal)
		return;

	const auto& pos = GetPosition();
	auto vehicle = new Vehicle{ mGfw, mVehicleType };
	vehicle->SetPosition(glm::vec3{ mLeftOrRight * 27.0f, pos.y + 0.1f, pos.z });
	vehicle->SetSpeed(-5.0f);
	if (mLeftOrRight == -1)
		vehicle->SetRotation(180.0f);
	mCooldown = Random::GetFloatRange(2.0f, 4.0f) + vehicle->GetGenTerm();
}

void Plane::GenerateTree()
{
	const auto& pos = GetPosition();
	
	auto treeNum = Random::GetIntRange(5, 8);
	auto xPos = Random::GetShuffledArray(-12, 12, 0);

	for (int i = 0; i < treeNum; ++i)
	{
		auto tree = new Tree{ mGfw, static_cast<Tree::TreeType>(Random::GetIntRange(0, 0)) };
		tree->SetPosition(glm::vec3{ xPos[i] * 2.0f, pos.y + 0.2f, pos.z });
	}
}

void Plane::GenerateTreasure()
{
	//if (Random::GetFloatRange(0.0f, 1.0f) > 0.4f)
	//	return;

	const auto& pos = GetPosition();
	auto xPos = Random::GetShuffledArray(-12, 12, 0);

	auto treasure = new Treasure{ mGfw };
	treasure->SetPosition(glm::vec3{ xPos[0] * 2.0f, 0.0f, pos.z });
}