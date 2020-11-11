#include "mesh.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <limits>

#include <rapidjson/document.h>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

#include "renderer.h"
#include "texture.h"
#include "vertexarray.h"


Mesh::Mesh()
	: mTexture{ nullptr },
	mVertexArray{ nullptr },
	mRadius{ 0.0f },
	mBox{ glm::vec3{std::numeric_limits<float>::max()}, glm::vec3{std::numeric_limits<float>::min()} }
{

}

Mesh::~Mesh()
{
	delete mVertexArray;
	mVertexArray = nullptr;
}

bool Mesh::Load(const std::string& file, Renderer* renderer)
{
	std::ifstream f{ file };
	if (!f.is_open())
	{
		std::cout << "Failed to open file " << file << std::endl;
		return false;
	}

	std::stringstream fileStream;
	fileStream << f.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	if (!doc.IsObject())
	{
		std::cout << file << " is not valid json" << std::endl;
		return false;
	}

	int ver = doc["version"].GetInt();

	// Check the version
	if (ver != 1)
	{
		std::cout << file << " is not version 1" << std::endl;
		return false;
	}


	size_t vertSize = 8;

	// Load textures
	const rapidjson::Value& textures = doc["textures"];
	if (!textures.IsArray() || textures.Size() < 1)
	{
		std::cout << file << " has no textures." << std::endl;
		return false;
	}

	for (rapidjson::SizeType i = 0; i < textures.Size(); i++)
	{
		// Is this texture already loaded?
		std::string texName = textures[i].GetString();
		Texture* t = renderer->GetTexture(texName);
		if (t == nullptr)
		{
			// Try loading the texture
			t = renderer->GetTexture(texName);
			if (t == nullptr)
			{
				// If it's still null, just use the default texture
				t = renderer->GetTexture("Assets/Default.png");
			}
		}
		mTexture = t;
	}

	// Load in the vertices
	const rapidjson::Value& vertsJson = doc["vertices"];
	if (!vertsJson.IsArray() || vertsJson.Size() < 1)
	{
		std::cout << file << " has no vertices" << std::endl;
		return false;
	}

	std::vector<float> vertices;
	vertices.reserve(vertsJson.Size() * vertSize);
	mRadius = 0.0f;
	for (rapidjson::SizeType i = 0; i < vertsJson.Size(); i++)
	{
		// For now, just assume we have 8 elements
		const rapidjson::Value& vert = vertsJson[i];
		if (!vert.IsArray() || vert.Size() != 8)
		{
			std::cout << "Unexpected vertex format" << file << std::endl;
			return false;
		}

		glm::vec3 pos{ vert[0].GetDouble(), vert[1].GetDouble(), vert[2].GetDouble() };
		mRadius = glm::max(mRadius, glm::distance2(pos, glm::vec3{ 0.0f }));
		mBox.UpdateMinMax(pos);

		// Add the floats
		for (rapidjson::SizeType i = 0; i < vert.Size(); i++)
		{
			vertices.emplace_back(static_cast<float>(vert[i].GetDouble()));
		}
	}

	mRadius = glm::sqrt(mRadius);

	// Load in the indices
	const rapidjson::Value& indJson = doc["indices"];
	if (!indJson.IsArray() || indJson.Size() < 1)
	{
		std::cout << file << " has no indices" << std::endl;
		return false;
	}

	std::vector<unsigned int> indices;
	indices.reserve(indJson.Size() * 3);
	for (rapidjson::SizeType i = 0; i < indJson.Size(); i++)
	{
		const rapidjson::Value& ind = indJson[i];
		if (!ind.IsArray() || ind.Size() != 3)
		{
			std::cout << "Invalid indices " << file << std::endl;
			return false;
		}

		indices.emplace_back(ind[0].GetUint());
		indices.emplace_back(ind[1].GetUint());
		indices.emplace_back(ind[2].GetUint());
	}

	// Now create a vertex array
	mVertexArray = new VertexArray(vertices.data(), static_cast<unsigned>(vertices.size()) / vertSize,
		indices.data(), static_cast<unsigned>(indices.size()));
	return true;
}

