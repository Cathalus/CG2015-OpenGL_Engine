#include "texture.h"
#include "stb_image.h"
#include <cassert>
#include <iostream>
#include <string>

Texture::Texture(const std::string& fileName, GLenum textureTarget)
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
	glGenerateMipmap(_textureTarget);

	// texture interpolation (filtering)
	glTexParameteri(_textureTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	// minification
	glTexParameteri(_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// magnification

	// Free CPU
	stbi_image_free(imageData);

	std::cout << "Texture " << fileName << " loaded!" << std::endl;
	_width = width;
	_height = height;
}

Texture::Texture(GLuint width, GLuint height, GLenum textureTarget, GLenum internalFormat, GLenum format)
{
	glGenTextures(1, _texture);
	_textureTarget = textureTarget;
	glBindTexture(textureTarget, *_texture);
	glTexImage2D(textureTarget, 0, internalFormat, width, height, 0, format, GL_FLOAT, NULL);
	glTexParameterf(textureTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(textureTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	_width = width;
	_height = height;
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
