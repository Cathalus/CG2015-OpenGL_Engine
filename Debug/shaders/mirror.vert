// GLSL Vertex Shader
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;

uniform mat4 MVP;
uniform mat4 model;

out vec4 Color0;
out vec2 TexCoord0;
out vec3 FragPos0;
out vec3 Tangent0;
out vec3 Normal0;

void main()
{
	Color0 = normal;
	gl_Position = MVP*model*vec4(position, 1.0);
	TexCoord0 = vec2(texCoord.x, 1.0-texCoord.y);
}