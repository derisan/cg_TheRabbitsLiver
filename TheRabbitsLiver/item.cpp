#include "item.h"

#include "mesh_component.h"
#include "box_component.h"
#include "mesh.h"
#include "random.h"

Item::Item(Gfw* gfw, Gfw::Layer layer)
	: Actor{ gfw, layer },
	mType{ static_cast<Item::ItemType>(Random::GetIntRange(0, 0)) },
	mBox{ nullptr }
{
	std::string file;
	switch (mType)
	{
		case kPotion:
			file = "Assets/heart.gpmesh";
			SetScale(0.02f);
			break;
		case kPoison:
			break;
	}

	auto mc = new MeshComponent{ this, file };
	mBox = new BoxComponent{ this };
	mBox->SetObjectBox(mc->GetMesh()->GetBox());
}