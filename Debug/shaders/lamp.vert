// GLSL Vertex Shader
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 normal;
layout (location = 2) in vec2 texCoord;

uniform mat4 MVP;
uniform mat4 model;

out vec4 oColor;
out vec4 oNormal;
out vec2 oTexCoord;

void main()
{
	oColor = normal;
	gl_Position = MVP*model*vec4(position, 1.0);
	oTexCoord = vec2(texCoord.x, 1.0-texCoord.y);
	oNormal = normal;
}