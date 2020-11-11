#pragma once

#include <string>

#include <glm/glm.hpp>

using GLenum = unsigned int;
using GLuint = unsigned int;

class Shader
{
public:
	Shader();
	~Shader();

	bool Load(const std::string& vertName, const std::string& fragName);
	void SetActive();

	void SetMatrix4Uniform(const std::string& name, const glm::mat4& mat);
	void SetMatrix3Uniform(const std::string& name, const glm::mat3& mat);
	void SetVectorUniform(const std::string& name, const glm::vec3& vec);
	void SetFloatUniform(const std::string& name, float value);

private:
	bool CompileShader(const std::string& fileName,
		GLenum shaderType,
		GLuint& outShader);
	bool IsCompiled(GLuint shader);
	bool IsValidProgram();

private:
	GLuint mVertexShader;
	GLuint mFragShader;
	GLuint mShaderProgram;
};
