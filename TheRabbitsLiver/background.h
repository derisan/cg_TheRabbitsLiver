#pragma once

#include <string>

class Background
{
public:
	Background(const std::string& file);
	~Background();

	void Draw();
	void Load();

private:
	class Texture* mTexture;
	class Shader* mShader;

	unsigned int mVertexArray;
	unsigned int mVertexBuffer;
	unsigned int mIndexBuffer;
};

