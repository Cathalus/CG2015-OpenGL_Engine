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
struct DirectionalLight {
	vec3 direction;
	vec3 color;
};
struct SpotLight {
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;
    float constant;
    float linear;
    float quadratic;
	vec3 color;
	bool active;
};
struct PointLight {
	vec3 position;
    float constant;
    float linear;
    float quadratic;
	vec3 color;
	bool active;
};
uniform sampler2D textureDiffuse;				// DiffuseMap
uniform sampler2D textureNormal;				// NormalMap
uniform sampler2D textureDepth;					// ShadowMap

#define NUM_POINT_LIGHTS 2

uniform DirectionalLight directionalLight;
uniform SpotLight spotLight;
uniform PointLight pointLights[NUM_POINT_LIGHTS];
	
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 viewPosition;
uniform float ambientStrength;
uniform Material material;
uniform vec3 lightDirection;
uniform vec3 shadowTexelSize;

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

/* Bilinear Filtering 
sampler2DShadow is not supported by all devices -> software implementation of bilinear filtering */
float sampleShadowMapLinear(sampler2D shadowMap, vec2 coords, float compare, vec2 texelSize)
{
	vec2 pixelPos = coords/texelSize + vec2(0.5);
	vec2 fracPart = fract(pixelPos);
	vec2 startTexel = (pixelPos - fracPart) * texelSize;
	
	float bottomLeftTexel 	= sampleShadowMap(shadowMap, startTexel, compare);
	float bottomRightTexel 	= sampleShadowMap(shadowMap, startTexel + vec2(texelSize.x, 0.0), compare);
	float topLeftTexel 		= sampleShadowMap(shadowMap, startTexel + vec2(0.0, texelSize.y), compare);
	float topRightTexel 	= sampleShadowMap(shadowMap, startTexel + texelSize, compare);
	
	// bilinear interpolation
	float leftTexel 	= mix(bottomLeftTexel, topLeftTexel, fracPart.y);
	float rightTexel	= mix(bottomRightTexel, topRightTexel, fracPart.y);
	float result 		= mix(leftTexel, rightTexel, fracPart.x);
	return result;
}

// Percentage Close Filtering (PCF)
float sampleShadowMapPCF(sampler2D shadowMap, vec2 coords, float compare, vec2 texelSize)
{
	const float NUM_SAMPLES = 4.0f;
	const float SAMPLES_START = (NUM_SAMPLES-1.0f)/2.0f;
	
	float result = 0.0f;
	
	for(float y = -SAMPLES_START; y <= SAMPLES_START; y += 1.0f)
	{
		for(float x = -SAMPLES_START; x <= SAMPLES_START; x += 1.0f)
		{
			vec2 offsetCoords = vec2(x,y)*texelSize;
			result += sampleShadowMapLinear(shadowMap, coords + offsetCoords, compare, texelSize);
		}
	}
	return result/pow(NUM_SAMPLES,2);
}

float calcShadowAmount(sampler2D shadowMap, vec4 shadowMapCoords, vec3 normal, vec3 lightDirection)
{
	vec3 shadowMapCoords0 = (shadowMapCoords.xyz/shadowMapCoords.w);	
	float bias = (shadowTexelSize.x);
	return sampleShadowMapPCF(shadowMap, shadowMapCoords0.xy, shadowMapCoords0.z-bias, shadowTexelSize.xy);
}

/*	LIGHT CALCULATION */
// DirectionalLight
vec3 calcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir, Material material)
{
	float specularStrength = material.shininessStrength;
	vec3 lightDir = normalize(-light.direction);
	// Diffuse Lighting
	float lamberFactor = max(dot(normal, lightDir), 0.0);
	// Specular Lighting
	vec3 reflectDir = normalize(reflect(-lightDir, normal));
	float shininess = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// Calculate result
	vec3 diffuse = lamberFactor * light.color * material.diffuse;
	vec3 ambient = ambientStrength * light.color * material.ambient;
	vec3 specular = specularStrength * shininess * light.color * material.specular;
	
	float shadowAmount = calcShadowAmount(textureDepth, ShadowCoord0, normal, lightDir);
	
	return (ambient+diffuse+specular*shadowAmount)*(0.5+0.5*shadowAmount);
}

// Spotlight
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, Material material)
{
	float specularStrength = material.shininessStrength;

	vec3 lightDir = normalize(light.position-fragPos);
	// Diffuse Lighting
	float lamberFactor = max(dot(normal, lightDir), 0.0);
	// Specular Lighting
	vec3 reflectDir = normalize(reflect(-lightDir, normal));
	float shininess = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// Attenuation
	float distance = length(light.position-fragPos);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	// Intensity
	float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	
	vec3 diffuse = lamberFactor * light.color * material.diffuse;
	vec3 ambient = ambientStrength * light.color * material.ambient;
	vec3 specular = specularStrength * shininess * light.color * material.specular;
	
	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;
	return (ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, Material material)
{
	float specularStrength = material.shininessStrength;

	vec3 lightDir = normalize(light.position-fragPos);
	// Diffuse Lighting
	float lamberFactor = max(dot(normal, lightDir), 0.0);
	// Specular Lighting
	vec3 reflectDir = normalize(reflect(-lightDir, normal));
	float shininess = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// Attenuation
	float distance = length(light.position-fragPos);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	vec3 diffuse = lamberFactor * light.color * material.diffuse;
	vec3 ambient = ambientStrength * light.color * material.ambient;
	vec3 specular = specularStrength * shininess * light.color * material.specular;
	
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}

void main()
{
	vec4 shadowCoordinateWdivide = ShadowCoord0 / ShadowCoord0.w;
	
	vec4 diffuseColor = texture2D(textureDiffuse, TexCoord0);
	vec3 normal = calcBumpMap();
	vec3 viewDir = normalize(viewPosition - FragPos0);
	vec3 dirLight = calcDirLight(directionalLight, normal, viewDir, material);
	vec3 lightSpot = vec3(0);
	if(spotLight.active) lightSpot = calcSpotLight(spotLight, normal, FragPos0, viewDir, material);
	vec4 result = vec4(dirLight,1.0);
	result += vec4(lightSpot,1.0);
	for(int i = 0; i < NUM_POINT_LIGHTS; i++)
	{
		result += max(vec4(0),vec4(calcPointLight(pointLights[i], normal, FragPos0, viewDir, material),1.0));
		//result += vec4(0.25f);
	}
	FragColor = result * diffuseColor;
}