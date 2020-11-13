#pragma once
#include "actor.h"
class Tree :
    public Actor
{
public:
    enum TreeType
    {
        kBasic
    };

    Tree(Gfw* gfw, TreeType type, Gfw::Layer layer = Gfw::Layer::kTree);

    // Getters
    class BoxComponent* GetBox() const { return mBox; }
    TreeType GetType() const { return mType; }

private:
    class BoxComponent* mBox;

    TreeType mType;
};

