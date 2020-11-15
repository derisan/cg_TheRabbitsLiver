#include "sprite_component.h"

#include <GL/glew.h>

#include "actor.h"
#include "renderer.h"

#include "shader.h"
#include "vertexarray.h"
#include "texture.h"

SpriteComponent::SpriteComponent(Actor* owner, const std::string& file, Gfw::SpriteLayer layer)
	: Component{ owner },
	mVertexArray{ nullptr },
	mTexture{ nullptr },
	mLayer{ layer }
{
	mOwner->GetGfw()->AddSpriteAt(this, layer);

	auto renderer = Renderer::Get();

	mTexture = renderer->GetTexture(file);
	mVertexArray = renderer->GetSpriteVertexArray();
}

SpriteComponent::~SpriteComponent()
{
	mOwner->GetGfw()->RemoveSpriteAt(this, mLayer);
}

void SpriteComponent::Draw(Shader* shader)
{
	shader->SetMatrix4Uniform("uWorld", mOwner->GetWorldTransform());
	mTexture->SetActive();
	mVertexArray->SetActive();
	glDrawElements(GL_TRIANGLES, mVertexArray->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
}