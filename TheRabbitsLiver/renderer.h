// -----------------------------------
// renderer.h
// 2020. 11. 05
// Code by derisan (derisan@naver.com)
// -----------------------------------

#pragma once

#include <unordered_map>
#include <string>

// Renderer is a singleton class
class Renderer
{
public:
	static Renderer* Get();
	static class VertexArray* GetSpriteVertexArray();

	void Shutdown();

	class Texture* GetTexture(const std::string& file);
	class Mesh* GetMesh(const std::string& file);
	class Shader* GetShader(const std::string& file);

private:
	Renderer() = default;

	std::unordered_map<std::string, class Texture*> mTextures;
	std::unordered_map<std::string, class Mesh*> mMeshes;
	std::unordered_map<std::string, class Shader*> mShaders;

	static Renderer* mInstance;
	static class VertexArray* mSpriteVertexArray;
};

