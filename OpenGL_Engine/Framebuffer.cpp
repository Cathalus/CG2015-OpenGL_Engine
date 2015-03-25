#include "Framebuffer.h"
#include <iostream>

bool Framebuffer::init(Texture* texture, GLenum attachment)
{
	// Create FBO
	glGenFramebuffers(1, &_fbo);

	// Create buffer
	_attachment = attachment;
	_texture = texture;
	GLuint* texID = _texture->getID();
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, attachment, _texture->getTextureTarget(), *texID, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	if (_fbo == 0)
		return false;

	if (!_hasDepth)
	{
		glGenRenderbuffers(1, &_rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, _rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _texture->getWidth(), _texture->getHeight());
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _rbo);
	}

	// ?
	glDrawBuffers(*texID, &attachment);

	// check for errors
	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB error, status: 0x%x\n", Status);
		return false;
	}

	return true;
}

void Framebuffer::bindForWriting()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);
}

void Framebuffer::bindForReading(GLenum textureUnit)
{
	_texture->Bind(textureUnit);
}

Framebuffer::~Framebuffer()
{
	if (_fbo != 0)
		glDeleteFramebuffers(1, &_fbo);
	if (_texture != nullptr)
		delete _texture;
}
