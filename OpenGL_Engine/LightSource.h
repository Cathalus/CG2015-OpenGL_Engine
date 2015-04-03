#pragma once
#include "Engine.h"

enum LightType { DIRECTIONAL, SPOT, POINT };

class LightSource
{
public:
	LightSource(glm::vec3 lightColor, glm::vec3 lightPosition, glm::vec3 lightDirection) :
		_lightColor(lightColor),
		_lightPosition(lightPosition),
		_lightDirection(lightDirection){};
	inline glm::vec3 getColor()		{ return _lightColor; }
	inline glm::vec3 getPosition()	{ return _lightPosition; }
	inline glm::vec3 getDirection()	{ return _lightDirection; }

	inline void setPosition(glm::vec3 lightPosition) { _lightPosition = lightPosition; }
	inline void setDirection(glm::vec3 lightDirection) { _lightDirection = lightDirection; }
	inline void setColor(glm::vec3 lightColor) { _lightColor = lightColor; }

	inline void translate(glm::vec3 lightPositionDelta) { _lightPosition += lightPositionDelta; }

	virtual inline LightType getType() = 0;
protected:
	glm::vec3 _lightPosition;
	glm::vec3 _lightDirection;
	glm::vec3 _lightColor;
};

class DirectionalLight : public LightSource
{
public:
	DirectionalLight(glm::vec3 lightColor, glm::vec3 lightPosition, glm::vec3 lightDirection) :
		LightSource(lightColor, lightPosition, lightDirection) {};
	inline LightType getType() { return LightType::DIRECTIONAL; }
};

class SpotLight : public LightSource
{
public:
	SpotLight(glm::vec3 lightColor, glm::vec3 lightPosition, glm::vec3 lightDirection, float cutOff, float outerCutOff, float constant, float linear, float quadratic) :
		LightSource(lightColor, lightPosition, lightDirection),
		_cutOff(cutOff),
		_outerCutOff(outerCutOff),
		_constant(constant),
		_linear(linear),
		_quadratic(quadratic) {};
	inline LightType getType() { return LightType::SPOT; }
	inline float getCutOff() { return _cutOff; }
	inline float getOuterCutOff() { return _outerCutOff; }
	inline float getConstant() { return _constant; }
	inline float getLinear() { return _linear; }
	inline float getQuadratic() { return _quadratic; }
private:
	float _cutOff;
	float _outerCutOff;
	float _constant;
	float _linear;
	float _quadratic;
};

class PointLight : public LightSource
{
public:
	PointLight(glm::vec3 lightColor, glm::vec3 lightPosition, float constant, float linear, float quadratic) :
		LightSource(lightColor, lightPosition, glm::vec3(0,0,0)),
		_constant(constant),
		_linear(linear),
		_quadratic(quadratic) {};
	inline LightType getType() { return LightType::POINT; }
	inline float getConstant() { return _constant; }
	inline float getLinear() { return _linear; }
	inline float getQuadratic() { return _quadratic; }
private:
	float _constant;
	float _linear;
	float _quadratic;
};