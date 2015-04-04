// GLSL Vertex Shader
#version 330 core

layout(location = 0) out vec4 FragColor;

in vec2 TexCoord0;

uniform vec3 lampColor;

uniform sampler2D textureDiffuse;
uniform sampler2D textureNormal;

void main()
{
	vec4 diffuseColor = texture2D(textureDiffuse, TexCoord0);
	if(diffuseColor.rgb != vec3(0,0,0));
		FragColor = vec4(lampColor, 1.0f)*diffuseColor;
	FragColor = vec4(lampColor,1);
}