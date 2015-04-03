#pragma once
#include "OpenGL_Engine\Engine.h"

class RenderToTextureScene :
	public Scene
{
public:
	RenderToTextureScene(Display* display, ShaderManager* shaderManager, TextureManager* textureManager,
		ModelManager* modelManager, ShaderUniforms* uniformManager)
		: Scene(display, shaderManager, textureManager, modelManager, uniformManager){};
	void update(float delta);
	void render();
	void draw(bool drawLightSource);
	void drawShadowMap(std::string shader);
	void init();
	void loadAssets();
	void unloadAssets();
	~RenderToTextureScene() { unloadAssets(); }
private:
	GLfloat _lastX, _lastY;
	GLfloat _yaw = -90.0f, _pitch = 0;
	GLfloat _ambientStrength = 0.1f;
	GLfloat _acc = 0;
	glm::vec3 _lightColor = glm::vec3((float)58 / 255, (float)58 / 255, (float)135 / 255);
	//glm::vec3 _lightColor = glm::vec3((float)255 / 255, (float)255 / 255, (float)255 / 255);
	glm::vec3 _lightDirection;
	glm::vec3 _shadowTexelSize;
	std::vector<Entity*> _entities;
	Entity* _lamp;
	float _speed = 20.0f;

	Camera* _mirrorCamera;
	Camera* _shadowCamera;

	Framebuffer _rttBuffer;
	Texture* _rttTexture;
	Entity* _billboard;
	Entity* _player;

	Framebuffer _shadowBuffer;
	Texture* _shadowTexture;

	DirectionalLight* _directionalLight;
};