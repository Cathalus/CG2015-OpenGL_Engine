#pragma once
#include "OpenGL_Engine\Engine.h"

class GameplayScene :
	public Scene
{
public:
	GameplayScene(Display* display, ShaderManager* shaderManager, TextureManager* textureManager,
		ModelManager* modelManager, ShaderUniforms* uniformManager)
		: Scene(display, shaderManager, textureManager, modelManager, uniformManager){};
	void update(float delta);
	void render();
	void draw(std::string shader);
	void init();
	void loadAssets();
	void unloadAssets();
	~GameplayScene();
private:
	// Input variables
	float _yaw = 0, _pitch = 90;
	int _lastX = 0, _lastY = 0;
	float _speed = 25.0f;

	// Entities
	std::vector<Entity*> _entities;
	Entity* _lightSource;

	// Light Settings
	GLfloat _ambientStrength = 1.0f;
	glm::vec3 _lightColor = glm::vec3(1,1,1);
	glm::vec3 _lightPos = glm::vec3(0, 30, 0);
	glm::vec3 _lightDirection = glm::vec3(0, 0, 0) - _lightPos;
};