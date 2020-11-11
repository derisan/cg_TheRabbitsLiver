#pragma once

#include "component.h"

#include <string>

class SpriteComponent:
    public Component
{
public:
    SpriteComponent(class Actor* owner, const std::string& file);
    ~SpriteComponent();

    void Draw(class Shader* shader);
    
private:
    class VertexArray* mVertexArray;
    class Texture* mTexture;
};

