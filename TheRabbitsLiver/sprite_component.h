#pragma once

#include "component.h"

#include <string>

class SpriteComponent:
    public Component
{
public:
    SpriteComponent(class Actor* owner, const std::string& file, Gfw::SpriteLayer layer = Gfw::SpriteLayer::kOther);
    ~SpriteComponent();

    void Draw(class Shader* shader);
    
private:
    class VertexArray* mVertexArray;
    class Texture* mTexture;

    Gfw::SpriteLayer mLayer;
};

