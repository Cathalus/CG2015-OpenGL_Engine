#pragma once
#include <GLM\glm.hpp>

class Vertex
{
public:
	Vertex(const glm::vec3& pos, const glm::vec2& texCoord, const glm::vec3& normal = glm::vec3(0, 0, 0))
	{
		this->pos = pos;
		this->texCoord = texCoord;
		this->normal = normal;
	}

	inline glm::vec3* getPos()		{ return &pos; };
	inline glm::vec2* getTexCoord() { return &texCoord; };
	inline glm::vec3* getNormal()   { return &normal; };
private:
	glm::vec3 pos;
	glm::vec2 texCoord;
	glm::vec3 normal;
};

