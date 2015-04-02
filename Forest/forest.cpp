#include "OpenGL_Engine\Engine.h"
#include <iostream>
#include "GameplayScene.h"

int main()
{
	/* General Setup */
	Display* display = new Display("The Forest", 1280, 720);
	Camera* mainCamera = new Camera(glm::vec3(0, 0, -3.5f), 90, display->getWidth() / display->getHeight(), 1, 500);
	int targetFPS = 60;
	int fpsMill = 1000 / targetFPS;
	int currentFrameSpeed = 0;

	/* Initialize Managers */
	ShaderManager* shaderManager = new ShaderManager();
	TextureManager* textureManager = new TextureManager();
	ShaderUniforms* uniforms = new ShaderUniforms();
	ModelManager* modelManager = new ModelManager();
	modelManager->setTextureManager(textureManager);

	/* Load default textures */
	textureManager->loadTexture("default_normal", "default_normal.jpg");
	textureManager->loadTexture("default", "default.png");

	/* Shader Setup */
	shaderManager->createShaderProgram("game", { "game.vert", "game.frag" });
	shaderManager->createShaderProgram("lamp", { "lamp.vert", "lamp.frag" });

	/* Scene Setup */
	GameplayScene scene(display, shaderManager, textureManager, modelManager, uniforms);
	scene.addCamera(mainCamera);
	scene.init();

	/* Delta Timing */
	float deltaTime;
	int lastFrameTime = 0, currentFrameTime = 0;
	currentFrameTime = SDL_GetTicks();

	while (!display->isClosed())
	{
		// Calculate Delta
		lastFrameTime = currentFrameTime;
		currentFrameTime = SDL_GetTicks();
		deltaTime = (float)(currentFrameTime - lastFrameTime) / 1000;

		// Update and Render
		scene.update(deltaTime);
		scene.render();
		display->update();

		// Delay to match desired FPS
		currentFrameSpeed = SDL_GetTicks() - currentFrameTime;
		if (fpsMill > currentFrameSpeed)
			SDL_Delay(fpsMill - currentFrameSpeed);
	}
}