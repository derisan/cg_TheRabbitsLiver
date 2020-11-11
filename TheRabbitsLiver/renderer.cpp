// -----------------------------------
// renderer.cpp
// 2020. 11. 05
// Code by derisan (derisan@naver.com)
// -----------------------------------

#include "renderer.h"

#include "texture.h"
#include "mesh.h"
#include "shader.h"
#include "vertexarray.h"

Renderer* Renderer::Get()
{
	if (!mInstance)
		mInstance = new Renderer{};

	return mInstance;
}

VertexArray* Renderer::GetSpriteVertexArray()
{
	if (!mSpriteVertexArray)
	{
		const float vertices[] = {
		-0.5f,  0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 1.f, // top left
		-0.5f, -0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 0.f, // bottome left
		 0.5f, -0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 0.f, // bottom right
		 0.5f,  0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 1.f  // top right
		};

		const unsigned int indices[] = {
			0, 1, 2,
			0, 2, 3
		};

		mSpriteVertexArray = new VertexArray{ vertices, 4, indices, 6 };
	}

	return mSpriteVertexArray;
}

void Renderer::Shutdown()
{
	for (auto item : mTextures)
		delete item.second;

	for (auto item : mMeshes)
		delete item.second;

	for (auto item : mShaders)
		delete item.second;
}

Texture* Renderer::GetTexture(const std::string& file)
{
	Texture* tex = nullptr;
	auto iter = mTextures.find(file);
	if (iter != mTextures.end())
		tex = iter->second;
	else
	{
		tex = new Texture{};
		if (tex->Load(file))
			mTextures.emplace(file, tex);
		else
		{
			delete tex;
			tex = nullptr;
		}
	}
	return tex;
}

Mesh* Renderer::GetMesh(const std::string& file)
{
	Mesh* m = nullptr;
	auto iter = mMeshes.find(file);
	if (iter != mMeshes.end())
		m = iter->second;
	else
	{
		m = new Mesh{};
		if (m->Load(file, this))
			mMeshes.emplace(file, m);
		else
		{
			delete m;
			m = nullptr;
		}
	}
	return m;
}

Shader* Renderer::GetShader(const std::string& file)
{
	auto vertFilePath = "Shaders/" + file + ".vert";
	auto fragFilePath = "Shaders/" + file + ".frag";

	Shader* sh = nullptr;
	auto iter = mShaders.find(file);
	if (iter != mShaders.end())
		sh = iter->second;
	else
	{
		sh = new Shader{};
		if (sh->Load(vertFilePath, fragFilePath))
			mShaders.emplace(file, sh);
		else
		{
			delete sh;
			sh = nullptr;
		}
	}
	return sh;
}

Renderer* Renderer::mInstance;
VertexArray* Renderer::mSpriteVertexArray;