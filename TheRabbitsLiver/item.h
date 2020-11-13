#pragma once
#include "actor.h"
class Item :
    public Actor
{
public:
    enum ItemType
    {
        kPotion,
        kPoison
    };

    Item(Gfw* gfw, Gfw::Layer layer = Gfw::Layer::kItem);

    // Getters
    class BoxComponent* GetBox() const { return mBox; }
    ItemType GetType() const { return mType; }

private:
    class BoxComponent* mBox;

    ItemType mType;
};

