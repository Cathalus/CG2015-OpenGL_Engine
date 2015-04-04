#ifndef SHADERATTRIBUTES_H
#define SHADERATTRIBUTES_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include "Texture.h"

class ShaderManager;

class ShaderAttributes
{
private:
    friend class ShaderManager;
public:
    ShaderAttributes();
	~ShaderAttributes();
	void uploadEBOForIndex(int index, std::vector<GLuint> indices);
	void uploadBufferForIndex(int index, std::vector<glm::vec4> vertices);
	void uploadBufferForIndex(int index, std::vector<glm::vec3> vertices);
	void uploadBufferForIndex(int index, std::vector<glm::vec2> vertices);
	void uploadBufferForIndex(int index, int dim, std::vector<float> vertices);
    void setDrawMethod(GLenum method) { _drawMethod = method; }
	inline Texture* getTexture(){ return _texture; }
	inline Texture* getNormalTexture() { return _normalTexture; }
	inline void setTexture(Texture* texture) { _hasTexture = true; _texture = texture; }
	inline void setNormalTexture(Texture* texture) { _hasNormalTexture = true; _normalTexture = texture; }
	inline bool hasTexture() { return _hasTexture; }
	inline bool hasNormalTexture() { return _hasNormalTexture; }

	struct Material {
		glm::vec3 ambient = glm::vec3(1, 1, 1);
		glm::vec3 diffuse = glm::vec3(1, 1, 1);
		glm::vec3 specular = glm::vec3(1, 1, 1);
		float shininess = 32;
		float shininessStrength = 0.5;
	} Material;
private:
    void bindBuffers();
    unsigned int getVBOCount() { return _vboCount; }
	unsigned int getEBOCount() { return _eboCount; }
    GLenum getDrawMethod() { return _drawMethod; }

    std::vector<GLuint> _attributeBuffers;
	GLuint _vao;
	GLuint _ebo;
    unsigned int _vboCount = 0;
	unsigned int _eboCount = 0;
    GLenum _drawMethod = GL_TRIANGLES;
	Texture* _texture;
	Texture* _normalTexture;
	bool _hasTexture = false;
	bool _hasNormalTexture = false;
};

#endif // SHADERATTRIBUTES_H
