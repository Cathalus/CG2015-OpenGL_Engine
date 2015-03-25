#pragma once

#include <string>
#include <GL/glew.h>

class Texture
{
public:
	Texture(const std::string& fileName, GLenum textureTarget = GL_TEXTURE_2D);
	Texture(GLuint width, GLuint height, GLenum textureTarget = GL_TEXTURE_2D, GLenum textureComponent = GL_RGB);
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

