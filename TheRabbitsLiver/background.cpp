#include "background.h"

#include <GL/glew.h>

#include "texture.h"
#include "shader.h"
#include "renderer.h"

Background::Background(const std::string& file)
	: mTexture{ nullptr },
	mShader{ nullptr }
{
	auto renderer = Renderer::Get();

	mTexture = renderer->GetTexture(file);
	mShader = renderer->GetShader("bg");

	Load();
}

Background::~Background()
{
	glDeleteBuffers(1, &mVertexBuffer);
	glDeleteBuffers(1, &mIndexBuffer);
	glDeleteVertexArrays(1, &mVertexArray);
}

void Background::Draw()
{
	mShader->SetActive();
	mTexture->SetActive();
	glBindVertexArray(mVertexArray);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void Background::Load()
{
	const float vertices[] = {
		// pos				// tex
		-1.0f, 1.0f, 0.999f,   0.0f, 1.0f,
		-1.0f,-1.0f, 0.999f,   0.0f, 0.0f,
		1.0f, -1.0f, 0.999f,   1.0f, 0.0f,
		1.0f,  1.0f,  0.999f,  1.0f, 1.0f
	};

	const unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	// Create vertex array
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	// Create vertex buffer
	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, 4 * 5 * sizeof(float), vertices, GL_STATIC_DRAW);

	// Create index buffer
	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, reinterpret_cast<void*>(sizeof(float) * 3));
}