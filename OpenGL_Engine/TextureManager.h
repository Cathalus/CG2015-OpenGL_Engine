#pragma once
#include <map>
#include <GL\glew.h>
#include <string>
#include <iostream>
#include "Texture.h"

class TextureManager
{
public:
	void loadTexture(std::string name, std::string fileName, bool& success)
	{
		if (_textures.find(name) == _textures.end())
		{
			std::cout << "Loading Texture: " << fileName << std::endl;
			_textures[name] = new Texture(fileName);
			success = true;
		}
		success = false;
	}
	void loadTexture(std::string name, Texture* texture)
	{
		if (_textures.find(name) == _textures.end())
			_textures[name] = texture;
	}
	void unloadTexture(std::string name)
	{
		if (_textures.find(name) != _textures.end())
			_textures.erase(name);
	}
	Texture* getTexture(std::string name)
	{
		if (_textures.find(name) == _textures.end())
		{
			return NULL;
		}
		else{
			return _textures[name];
		}
	}
private:
	std::map<std::string, Texture*> _textures;
};