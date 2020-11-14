#include "item.h"

#include "mesh_component.h"
#include "box_component.h"
#include "mesh.h"
#include "random.h"

Item::Item(Gfw* gfw, Gfw::Layer layer)
	: Actor{ gfw, layer },
	mType{ static_cast<Item::ItemType>(Random::GetIntRange(3, 3)) },
	mBox{ nullptr }
{
	std::string file;
	switch (mType)
	{
		case kPotion:
			file = "Assets/meat.gpmesh";
			SetScale(1 / 40.0f);
			SetRotation(180.0f);
			break;
		case kPoison:
			file = "Assets/frog.gpmesh";
			SetScale(5.0f);
			SetRotation(180.0f);
			break;
		case kBomb:
			file = "Assets/penguin.gpmesh";
			SetScale(0.1f);
			break;
		case kBallon:
			file = "Assets/ballon.gpmesh";
			SetScale(0.3f);
			break;
	}

	auto mc = new MeshComponent{ this, file };
	mBox = new BoxComponent{ this };
	mBox->SetObjectBox(mc->GetMesh()->GetBox());
}