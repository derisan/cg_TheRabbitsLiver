#pragma once

#include <string>

#include "aabb.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	bool Load(const std::string& file, class Renderer* renderer);

	class VertexArray* GetVertexArray() { return mVertexArray; }
	class Texture* GetTexture() { return mTexture; };

	float GetRadius() const { return mRadius; }
	const AABB& GetBox() const { return mBox; }
private:
	class Texture* mTexture;
	class VertexArray* mVertexArray;

	float mRadius;
	AABB mBox;
};