#pragma once

#include <string>

class Texture
{
public:
	Texture();
	~Texture();

	bool Load(const std::string& file);

	void SetActive();

	int GetWidth() const { return mTextureWidth; }
	int GetHeight() const { return mTextureHeight; }

private:
	unsigned int mTextureID;
	int mTextureWidth;
	int mTextureHeight;
};