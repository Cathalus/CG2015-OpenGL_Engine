#pragma once
#include <vector>
#include "ModelManager.h"

class Scene
{
public:
	Scene(Display* display, ShaderManager* shaderManager, TextureManager* textureManager, ModelManager* modelManager, ShaderUniforms* uniformManager)
	{
		_display = display;
		_shaderManager = shaderManager;
		_textureManager = textureManager;
		_uniformManager = uniformManager;
		_modelManager = modelManager;
	}
	virtual void update(float delta) = 0;
	virtual void render() = 0;
	virtual void init() = 0;
	virtual void loadAssets() = 0;
	virtual void unloadAssets() = 0;
	inline void addCamera(Camera* cam)
	{
		if (_activeCamera == nullptr)
			_activeCamera = cam;
		_cameras.push_back(cam);
	}
protected:
	Camera* _activeCamera;
	std::vector<Camera*> _cameras;
	Display* _display;
	ShaderManager* _shaderManager;
	TextureManager* _textureManager;
	ModelManager* _modelManager;
	ShaderUniforms* _uniformManager;
};