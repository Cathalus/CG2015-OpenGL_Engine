#include "shaderattributes.h"
#include <iostream>

ShaderAttributes::ShaderAttributes()
{
    // generate vertex array
    glGenVertexArrays(1, &_vao);
}

void ShaderAttributes::uploadEBOForIndex(int index, std::vector<GLuint> indices)
{
	_eboCount = indices.size();
	GLuint tempEBOIndex;

	glBindVertexArray(_vao);

	/* EBO Buffer */
	glGenBuffers(1, &tempEBOIndex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tempEBOIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0])*indices.size(), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void ShaderAttributes::uploadBufferForIndex(int index, std::vector<glm::vec4> vertices)
{
	_vboCount = vertices.size();

	// bind vertex array
	glBindVertexArray(_vao);

	GLuint tempVBOIndex;
	
	// generate buffers
	glGenBuffers(1, &tempVBOIndex);

	/* VBO Buffer */
	glBindBuffer(GL_ARRAY_BUFFER, tempVBOIndex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0])*vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)0);

	// Unbind VAO
	glBindVertexArray(0);

	_attributeBuffers.push_back(tempVBOIndex);
}

void ShaderAttributes::uploadBufferForIndex(int index, std::vector<glm::vec3> vertices)
{
	_vboCount = vertices.size();
    // bind vertex array
    glBindVertexArray(_vao);

    GLuint tempVBOIndex;
	GLuint tempEBOIndex;
    // generate buffers
    glGenBuffers(1, &tempVBOIndex);

	/* VBO Buffer */
    glBindBuffer(GL_ARRAY_BUFFER, tempVBOIndex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0])*vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)0);

    // Unbind VAO
    glBindVertexArray(0);

    _attributeBuffers.push_back(tempVBOIndex);
}

void ShaderAttributes::uploadBufferForIndex(int index, std::vector<glm::vec2> vertices)
{
	_vboCount = vertices.size();
	// bind vertex array
	glBindVertexArray(_vao);

	GLuint tempVBOIndex;
	GLuint tempEBOIndex;
	// generate buffers
	glGenBuffers(1, &tempVBOIndex);

	/* VBO Buffer */
	glBindBuffer(GL_ARRAY_BUFFER, tempVBOIndex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0])*vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)0);

	// Unbind VAO
	glBindVertexArray(0);

	_attributeBuffers.push_back(tempVBOIndex);
}

void ShaderAttributes::uploadBufferForIndex(int index, int dim, std::vector<float> vertices)
{
	_vboCount = vertices.size() / dim;

	// bind vertex array
	glBindVertexArray(_vao);

	GLuint tempVBOIndex;
	// generate buffers
	glGenBuffers(1, &tempVBOIndex);

	/* VBO Buffer */
	glBindBuffer(GL_ARRAY_BUFFER, tempVBOIndex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, dim, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)0);

	// Unbind VAO
	glBindVertexArray(0);

	_attributeBuffers.push_back(tempVBOIndex);
}

void ShaderAttributes::bindBuffers() { 
	glBindVertexArray(_vao); 
}

ShaderAttributes::~ShaderAttributes()
{
	glDeleteVertexArrays(1, &_vao);
	if (_hasTexture)
		delete _texture;
}