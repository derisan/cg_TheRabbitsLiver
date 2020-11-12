#include "plane.h"

#include "mesh_component.h"
#include "box_component.h"
#include "mesh.h"

Plane::Plane(Gfw* gfw, PlaneType type, int curStage, Gfw::Layer layer)
	: Actor{ gfw, layer },
	mBox{ nullptr },
	mType{ type }
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
			break;
		case PlaneType::kRailroad:
			file = "Assets/railroad.gpmesh";
			yOffset -= 0.1f;
			break;
		case PlaneType::kWater:
			file = "Assets/water.gpmesh";
			break;
		case PlaneType::kGoal:
			break;
	}

	SetPosition(glm::vec3{ 0.0f, yOffset, -2.0f * curStage });

	auto mc = new MeshComponent{ this, file };
	mBox = new BoxComponent{ this };
	mBox->SetObjectBox(mc->GetMesh()->GetBox());
}

void Plane::UpdateActor()
{
	Actor::UpdateActor();


}