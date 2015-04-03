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