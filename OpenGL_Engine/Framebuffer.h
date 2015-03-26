#pragma once
#include "GL\glew.h"
#include "Texture.h"

class Framebuffer
{
public:
	~Framebuffer();
	bool init(GLuint width, GLuint height, GLenum textureTarget, GLenum internalFormat, GLenum format, GLenum attachment = GL_NONE);
	void addDepthTexture(Texture* texture);
	void bindForWriting();
	void bindForReading(GLenum TextureUnit);
	inline Texture* getTexture() { return _texture; }
private:
	GLenum _component;
	GLenum _attachment;
	GLuint _fbo = 0; // Frame Buffer
	GLuint _rbo = 0; // Render Buffer
	Texture* _texture;
	bool _hasDepth = false;
};

