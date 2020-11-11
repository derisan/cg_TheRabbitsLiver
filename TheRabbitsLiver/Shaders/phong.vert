#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

out vec2 texCoord;
out vec3 fragPos;
out vec3 fragNormal;

uniform mat4 uWorld;
uniform mat4 uView;
uniform mat4 uProj;

void main()
{
	fragPos = vec3(uWorld * vec4(inPos, 1.0f));
	fragNormal = mat3(transpose(inverse(uWorld))) * inNormal;
	texCoord = inTexCoord;

	gl_Position = uProj * uView * vec4(fragPos, 1.0f);
}