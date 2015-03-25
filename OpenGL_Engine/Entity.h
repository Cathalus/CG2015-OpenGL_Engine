#pragma once

#include "Model.h"
#include "GL\glew.h"
#include "GLM\glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"
#include <vector>

class Entity
{
public:
	Entity(Model* model, glm::vec3 translation = glm::vec3(0), glm::vec3 rotation = glm::vec3(1), float angle = 0, float scale = 1)
	{
		_model = model;
		*_matrix = glm::scale(*_matrix, glm::vec3(scale));
		*_matrix = glm::rotate(*_matrix, glm::radians(angle), rotation);
		*_matrix = glm::translate(*_matrix, translation);

		_translation = translation;
		_scale = scale;
		update();
	}
	inline glm::mat4* getMatrix() { return _matrix; }
	/*inline void setMatrix(glm::mat4& matrix) { *_matrix = matrix;  }*/
	inline Model* getModel() { return _model; }
	inline glm::mat4* getCombinedMatrix() {
		glm::mat4* result = new glm::mat4(1.0f);
		*result = (*_entityMatrix)*(*_matrix)*(*_model->getMatrix());
		return result;
	}
	inline void translate(glm::vec3 translation)
	{
		_translation += translation;
		*_entityMatrix = glm::translate(*_entityMatrix, translation);
	}
	inline void setTranslation(glm::vec3 translation)
	{
		_translation = translation;
		update();
	}
	inline void addRotation(glm::vec3 rotation, float angle)
	{	
		_rotations.push_back(glm::vec4(rotation.x, rotation.y, rotation.z, angle));
	}
	inline void clearRotation()
	{
		_rotations.clear();
	}
	inline void rotate()
	{
		update();
	}
	inline void scale(float scale)
	{
		_scale *= scale;
		*_entityMatrix = glm::scale(*_entityMatrix, glm::vec3(scale));
		*_matrix = glm::scale(*_matrix, glm::vec3(scale));
	}
	inline void setScale(float scale)
	{
		_scale = scale;
		update();
	}
	inline glm::vec3 getPosition()
	{
		return _translation;
	}
private:
	glm::mat4* _entityMatrix = new glm::mat4(1.0f);
	glm::mat4* _matrix = new glm::mat4(1.0f);
	Model* _model;
	// components of vec4: x, y, z, angle
	std::vector<glm::vec4> _rotations;
	glm::vec3 _translation;
	float _scale;
	inline void update()
	{
		_entityMatrix = new glm::mat4(1.0f);
		*_entityMatrix = glm::scale(*_entityMatrix, glm::vec3(_scale));
		*_entityMatrix = glm::translate(*_entityMatrix, _translation);
		if (!_rotations.empty())
		{
			for (glm::vec4 rotation : _rotations)
			{
				std::cout << rotation.x << " " << rotation.y << " " << rotation.z << " " << rotation.w << std::endl;
				*_entityMatrix = glm::rotate(*_entityMatrix, glm::radians(rotation.w), glm::vec3(rotation.x, rotation.y, rotation.z));
			}
		}
	}
};

