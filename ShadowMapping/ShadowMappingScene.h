#pragma once
#include "OpenGL_Engine\Engine.h"
class ShadowMappingScene :
	public Scene
{
public:
	ShadowMappingScene(Display* display, ShaderManager* shaderManager, TextureManager* textureManager,
		ModelManager* modelManager, ShaderUniforms* uniformManager)
		: Scene(display, shaderManager, textureManager, modelManager, uniformManager){};
	void update(float delta);
	void shadowMap();
	void render();
	void draw(bool drawLightSource);
	void init();
	void loadAssets();
	void unloadAssets();
	~ShadowMappingScene() { unloadAssets(); }
private:
	GLfloat _lastX, _lastY;
	GLfloat _yaw = -90.0f, _pitch = 0;
	GLfloat _ambientStrength = 0.1f;
	GLfloat _acc = 0;
	glm::vec3 _lightColor = glm::vec3(0);
	glm::vec3 _lightPos = glm::vec3(10, 20, 0);
	std::vector<Entity*> _entities;
	Entity* _lamp;
	float _speed = 20.0f;
	Framebuffer* _shadowBuffer;
};

