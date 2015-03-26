// GLSL Vertex Shader
#version 330 core

//layout(location = 0) out float fragmentDepth;
layout(location = 0) out vec4 FragColor;

in vec4 Color0;
in vec2 TexCoord0;
in vec3 FragPos0;
in vec3 Tangent0;
in vec3 Normal0;
in vec4 ShadowCoord0; 

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
	float shininessStrength;
};

uniform sampler2D textureDiffuse;
uniform sampler2D textureNormal;
uniform sampler2D textureDepth;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 viewPosition;
uniform float ambientStrength;
uniform Material material;
uniform vec3 lightDirection;

vec3 calcBumpMap()
{
	vec3 result;
	
	vec3 normal = normalize(Normal0);
	vec3 tangent = normalize(Tangent0);
	tangent = normalize(tangent - dot(tangent,normal)*normal);
	vec3 bitangent = cross(tangent,normal);
	
	vec3 bumpmapnormal = texture(textureNormal, TexCoord0).xyz;
	bumpmapnormal = 2 * bumpmapnormal - vec3(1);
	mat3 TBN = mat3(tangent, bitangent, normal);
	result = TBN * bumpmapnormal;
	result = normalize(result);
	
	return result;	
}

void main()
{
	float visibility = 1.0;
	if ( texture2D( textureDepth, ShadowCoord0.xy ).z  <  ShadowCoord0.z){
		visibility = 0.5;
	}

	// Ambient
	vec3 ambient = ambientStrength * lightColor * material.ambient;
	
	// Diffuse
	vec3 lightDir = normalize(-lightDirection);
	vec3 normal = calcBumpMap();
	float lamberFactor = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = lamberFactor * lightColor * material.diffuse;
	
	// Specular
	float specularStrength = material.shininessStrength;
	vec3 viewDir = normalize(viewPosition - FragPos0);
	vec3 reflectDir = normalize(reflect(-lightDir, normal));		
	float shininess = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = specularStrength * shininess * lightColor * material.specular;  	
	
	vec3 result = ambient;
	if(lamberFactor > 0.0)
	{
		result = (diffuse + specular); 
	}
	vec4 diffuseColor = texture2D(textureDiffuse, TexCoord0);
	FragColor = vec4(result,1.0) * diffuseColor * visibility;
	
	float Depth = texture(textureDepth, TexCoord0).x;
    Depth = 1.0 - (1.0 - Depth) * 25.0;
    //FragColor = vec4(Depth);
	
	//fragmentDepth = gl_FragCoord.z;
}