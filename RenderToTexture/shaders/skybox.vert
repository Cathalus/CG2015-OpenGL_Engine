// GLSL Vertex Shader
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 normal;
layout (location = 2) in vec2 texCoord;

uniform mat4 MVP;
uniform mat4 model;

out vec4 Color0;
out vec4 Normal0;
out vec3 TexCoord0;

void main()
{
	Color0 = normal;
	vec4 pos = MVP*model*vec4(position, 1.0);
	gl_Position = pos.xyww;
	TexCoord0 = position;
	Normal0 = normal;
}