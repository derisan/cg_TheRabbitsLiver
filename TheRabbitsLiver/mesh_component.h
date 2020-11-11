#pragma once

#include "component.h"

#include <string>

class MeshComponent:
    public Component
{
public:
    MeshComponent(class Actor* owner, const std::string& file);
    ~MeshComponent();

    void Draw(class Shader* shader);

private:
    class Mesh* mMesh;
    class Texture* mTexture;
    class VertexArray* mVertexArray;
};

