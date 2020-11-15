#include "tree.h"

#include "mesh_component.h"
#include "box_component.h"
#include "mesh.h"

Tree::Tree(Gfw* gfw, TreeType type, Gfw::Layer layer)
	: Actor{gfw, layer},
	mBox{ nullptr },
	mType{ type }
{
	std::string file;

	switch (type)
	{
		case kBasic:
			file = "Assets/basic_tree.gpmesh";
			SetScale(0.1f);
			break;
	}

	auto mc = new MeshComponent{ this, file };
	mBox = new BoxComponent{ this };
	mBox->SetObjectBox(mc->GetMesh()->GetBox());
}

