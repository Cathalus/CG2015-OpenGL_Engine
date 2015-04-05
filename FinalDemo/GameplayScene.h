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
	void init();
	~GameplayScene() { unloadAssets(); }
private:
	void updateLightSources();
	void draw(bool drawLightSource);
	void drawShadowMap(std::string shader);
	void loadAssets();
	void unloadAssets();
	void initEntities();

	/* Controls */
	GLfloat _lastX, _lastY;
	GLfloat _yaw = -90.0f, _pitch = 0;
	float _speed = 60.0f;
	float _movementSpeed = 5.0f;
	bool _flashLightActive = false;
	bool _flashLightKeyLock = false;
	
	/* Light */
	GLfloat _ambientStrength = 0.7f;
	glm::vec3 _lightColor = glm::vec3((float)58 / 255, (float)58 / 255, (float)135 / 255);
	glm::vec3 _lightDirection;
	glm::vec3 _shadowTexelSize;

	/* Entities */
	std::vector<Entity*> _entities;
	Entity* _lamp;
	Entity* _mirror;
	Entity* temp;
	Entity* _camera;
	Entity* _bounceCube;

	/* Cameras */
	Camera* _mirrorCamera;
	Camera* _shadowCamera;

	/* Rendering Effects */
	Framebuffer _rttBuffer;
	Framebuffer _shadowBuffer;
	Texture* _rttTexture;
	Texture* _shadowTexture;

	/* Lights */
	DirectionalLight* _directionalLight;
	SpotLight* _flashLight;
	
	CubeMap* _cubeMap;

	float _acc = 0.0f;
};