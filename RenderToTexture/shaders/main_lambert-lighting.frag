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

uniform sampler2D textureDiffuse;		// DiffuseMap
uniform sampler2D textureNormal;		// NormalMap
uniform sampler2D textureDepth;			// ShadowMap
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

float sampleShadowMap(sampler2D shadowMap, vec2 coords, float compare)
{
	// Returns 0 or 1 compare is farther away than the tex value
	return step(compare, texture2D(shadowMap, coords.xy).r);
}

float calcShadowAmount(sampler2D shadowMap, vec4 shadowMapCoords)
{
	vec3 shadowMapCoords0 = (shadowMapCoords.xyz/shadowMapCoords.w)* vec3(0.5) + vec3(0.5);	// Depth-bias -1|1 to 0|1
	return sampleShadowMap(shadowMap, shadowMapCoords0.xy, shadowMapCoords0.z);
}

void main()
{
	vec4 shadowCoordinateWdivide = ShadowCoord0 / ShadowCoord0.w;
		
	// Ambient
	vec3 ambient = ambientStrength * lightColor * material.ambient;
	
	// Diffuse
	vec3 lightDir = normalize(lightPosition - FragPos0);
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
	FragColor = vec4(result,1.0) * diffuseColor * (0.5+0.5*calcShadowAmount(textureDepth, ShadowCoord0));
	
	//float Depth = texture(textureDepth, ShadowCoord0.xy).r;
    //FragColor = vec4(texture2D(textureDepth, ShadowCoord0.xy).r);
	
	// Show depth buffer
	//FragColor = vec4(gl_FragCoord.z);
	
	//fragmentDepth = gl_FragCoord.z;
}