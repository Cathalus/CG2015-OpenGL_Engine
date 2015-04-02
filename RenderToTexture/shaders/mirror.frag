// GLSL Vertex Shader
#version 330 core

layout(location = 0) out vec4 FragColor;

in vec4 Color0;
in vec2 TexCoord0;

uniform sampler2D textureDiffuse;
uniform sampler2D textureNormal;
uniform sampler2D textureDepth;

void main()
{
	FragColor = texture2D(textureDiffuse, TexCoord0)+vec4(0.05,0.05,0.05,0.05);	
}