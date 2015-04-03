#pragma once
#include "Engine.h"
#include "GLM/gtc/matrix_transform.hpp"

class CubeMap
{
public:
	CubeMap(std::string positiveX,
			std::string negativeX,
			std::string positiveY,
			std::string negativeY, 
			std::string positiveZ,
			std::string negativeZ,
			Model* _model);
	~CubeMap();
	void Bind(unsigned int unit);
	void Render(glm::vec3 position, ShaderUniforms* uniformManager, ShaderManager* shaderManager, std::string shader);
private:
	Entity* _entity;
	GLuint* _cubeMap = new GLuint(0);
};

