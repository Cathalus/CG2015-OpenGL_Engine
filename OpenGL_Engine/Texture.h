#pragma once

#include <string>
#include <GL/glew.h>

class Texture
{
public:
	// default: trilinear filtering
	Texture(const std::string& fileName, GLenum textureTarget = GL_TEXTURE_2D, GLfloat filter = GL_LINEAR_MIPMAP_LINEAR);
	Texture(GLuint width, GLuint height, GLenum textureTarget, GLenum internalFormat, GLenum format, GLfloat filter = GL_LINEAR_MIPMAP_LINEAR);
	Texture(GLuint* id);
	void Bind(unsigned int unit);
	inline GLuint* getID() { return _texture; }
	inline GLenum getTextureTarget() { return _textureTarget;  }
	inline int getWidth() { return _width; }
	inline int getHeight() { return _height; }
	virtual ~Texture(void);
protected:
private:
	GLuint* _texture = new GLuint(0);
	GLenum _textureTarget;
	int _width = 0;
	int _height = 0;
};

