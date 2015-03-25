#include "OpenGL_Engine\Engine.h"
#include <iostream>
#include "ShadowMappingScene.h"

int main()
{
	/* General setup */
	Display* display = new Display("SceneBasedGame", 1280, 720);

	ShaderManager* shaderManager = new ShaderManager();
	TextureManager* textureManager = new TextureManager();
	ShaderUniforms* uniforms = new ShaderUniforms();
	ModelManager* modelManager = new ModelManager();
	GLint* width = new GLint(0);
	GLint* height = new GLint(0);
	SDL_GetWindowSize(display->getWindow(), width, height);
	Camera* cam = new Camera(glm::vec3(0.0f, 0.0f, -3.5f), 90, 16 / 9, 0.1, 200);
	int fps = 60;
	int fpsMill = 1000 / fps;
	int currentFrameSpeed = 0;

	/* Shader Setup */
	shaderManager->createShaderProgram("main", { "main.vert", "main.frag" });
	shaderManager->createShaderProgram("lamp", { "lamp.vert", "lamp.frag" });

	/* Scene Setup */
	ShadowMappingScene scene(display, shaderManager, textureManager, modelManager, uniforms);
	scene.addCamera(cam);
	scene.init();

	/* Delta Timing */
	float deltaTime;
	int lastFrameTime = 0, currentFrameTime = 0;
	currentFrameTime = SDL_GetTicks();
	while (!display->isClosed())
	{
		/* Delta calculation */
		lastFrameTime = currentFrameTime;
		currentFrameTime = SDL_GetTicks();
		deltaTime = (float)(currentFrameTime - lastFrameTime) / 1000;

		scene.update(deltaTime);
		scene.shadowMap();
		scene.render();

		display->update();

		currentFrameSpeed = SDL_GetTicks() - currentFrameTime;
		if (fpsMill > currentFrameSpeed)
			SDL_Delay(fpsMill - currentFrameSpeed);
	}
}