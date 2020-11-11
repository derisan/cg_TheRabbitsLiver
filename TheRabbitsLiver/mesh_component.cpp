#include "mesh_component.h"

#include <GL/glew.h>

#include "gfw.h"
#include "actor.h"
#include "renderer.h"

#include "shader.h"
#include "vertexarray.h"
#include "texture.h"
#include "mesh.h"

MeshComponent::MeshComponent(Actor* owner, const std::string& file)
	: Component{ owner },
	mMesh{ nullptr },
	mTexture{ nullptr },
	mVertexArray{ nullptr }
{
	mOwner->GetGfw()->AddMesh(this);

	auto renderer = Renderer::Get();
	mMesh = renderer->GetMesh(file);

	mTexture = mMesh->GetTexture();
	mVertexArray = mMesh->GetVertexArray();
}

MeshComponent::~MeshComponent()
{
	mOwner->GetGfw()->RemoveMesh(this);
}

void MeshComponent::Draw(Shader* shader)
{
	shader->SetActive();
	mTexture->SetActive();
	mVertexArray->SetActive();
	glDrawElements(GL_TRIANGLES, mVertexArray->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
}