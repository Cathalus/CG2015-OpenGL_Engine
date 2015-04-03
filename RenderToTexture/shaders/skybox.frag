// GLSL Vertex Shader
#version 330 core

layout(location = 0) out vec4 FragColor;

in vec3 TexCoord0;

uniform vec3 lampColor;

uniform samplerCube textureCubeMap;
void main()
{
	FragColor = texture(textureCubeMap, TexCoord0);
}