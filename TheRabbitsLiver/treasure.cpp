#include "treasure.h"

#include "mesh_component.h"
#include "box_component.h"
#include "mesh.h"

Treasure::Treasure(Gfw* gfw, TreasureType type, Gfw::Layer layer)
	: Actor{ gfw, layer },
	mBox{ nullptr },
	mType{ type }
{
	auto mc = new MeshComponent{ this, "Assets/chest.gpmesh" };

	mBox = new BoxComponent{ this };
	mBox->SetObjectBox(mc->GetMesh()->GetBox());

	SetScale(0.02f);
	SetRotation(180.0f);
}