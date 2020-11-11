#include "texture.h"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <GL/glew.h>

Texture::Texture()
	: mTextureID{ 0 },
	mTextureWidth{ 0 },
	mTextureHeight{ 0 }
{

}

Texture::~Texture()
{
	glDeleteTextures(1, &mTextureID);
}

bool Texture::Load(const std::string& file)
{
	int channels{ 0 };

	stbi_set_flip_vertically_on_load(true);
	auto image = stbi_load(file.c_str(), &mTextureWidth,
		&mTextureHeight, &channels, 0);

	if (!image)
	{
		std::cout << "STB failed to load image " << file << std::endl;
		return false;
	}

	int format = GL_RGB;
	if (channels == 4)
		format = GL_RGBA;

	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, format, mTextureWidth, mTextureHeight,
		0, format, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(image);

	return true;
}

void Texture::SetActive()
{
	glBindTexture(GL_TEXTURE_2D, mTextureID);
}