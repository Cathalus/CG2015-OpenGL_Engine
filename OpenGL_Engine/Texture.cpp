#include "texture.h"
#include "stb_image.h"
#include <cassert>
#include <iostream>
#include <string>

Texture::Texture(const std::string& fileName, GLenum textureTarget, GLfloat filter)
{
	int width, height, numComponents;
	unsigned char* imageData = stbi_load(std::string("textures/").append(fileName).c_str(), &width, &height, &numComponents, 4);

	if (imageData == NULL)
		std::cerr << "Texture loading failed for texture: " << fileName << std::endl;

	_textureTarget = textureTarget;

	glGenTextures(1, _texture);
	glBindTexture(_textureTarget, *_texture);

	/* Setting texture parameters */
	// texture wrapping
	glTexParameteri(_textureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(_textureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// GLint level = mipmapping
	glTexImage2D(_textureTarget, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

	// texture interpolation (filtering)
	glTexParameteri(_textureTarget, GL_TEXTURE_MIN_FILTER, filter);	// minification
	glTexParameteri(_textureTarget, GL_TEXTURE_MAG_FILTER, filter);	// magnification

	if (filter == GL_NEAREST_MIPMAP_LINEAR ||
		filter == GL_NEAREST_MIPMAP_NEAREST ||
		filter == GL_LINEAR_MIPMAP_LINEAR ||
		filter == GL_LINEAR_MIPMAP_NEAREST)
	{
		// Generates MipMap and sets anisotropic filtering to a maximum of 8
		glGenerateMipmap(_textureTarget);
		// Check if anisotropic filtering is supported
		GLfloat maxAnisotropy;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
		glTexParameterf(_textureTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
	}
	else{
		glTexParameteri(_textureTarget, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(_textureTarget, GL_TEXTURE_MAX_LEVEL, 0);
	}

	// Free CPU
	stbi_image_free(imageData);

	_width = width;
	_height = height;
}

Texture::Texture(GLuint width, GLuint height, GLenum textureTarget, GLenum internalFormat, GLenum format, GLfloat filter)
{
	glGenTextures(1, _texture);
	_textureTarget = textureTarget;
	glBindTexture(textureTarget, *_texture);
	glTexParameterf(textureTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(textureTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(textureTarget, 0, internalFormat, width, height, 0, format, GL_FLOAT, NULL);
	_width = width;
	_height = height;
}

Texture::Texture(GLuint* id)
{
	_texture = id;
}

void Texture::Bind(unsigned int unit)
{
	assert(unit >= 0 && unit <= 31);
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(_textureTarget, *_texture);
}

Texture::~Texture(void)
{
	if(*_texture) glDeleteTextures(1, _texture);
	if (_texture) delete _texture;
}
