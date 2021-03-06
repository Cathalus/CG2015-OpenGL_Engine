#include "OpenGL_Engine\Engine.h"
#include "GameplayScene.h"
#include <iostream>

int main()
{
	/* General setup */
	Display* display = new Display("Render to Texture", 1280, 720, SDL_WINDOW_FULLSCREEN_DESKTOP);

	ShaderManager* shaderManager = new ShaderManager();
	TextureManager* textureManager = new TextureManager();
	ShaderUniforms* uniforms = new ShaderUniforms();
	ModelManager* modelManager = new ModelManager();
	modelManager->setTextureManager(textureManager);
	GLint* width = new GLint(0);
	GLint* height = new GLint(0);
	SDL_GetWindowSize(display->getWindow(), width, height);
	Camera* cam = new Camera(glm::vec3(0.0f, 0.0f, -3.5f), 90, display->getWidth() / display->getHeight(), 0.1, 500);
	int fps = 60;
	int fpsMill = 1000 / fps;
	int currentFrameSpeed = 0;
	
	/* Shader Setup */
	shaderManager->createShaderProgram("main",		{ "main.vert", "main.frag" });
	shaderManager->createShaderProgram("shadowMap", { "main_shadowmap.vert", "main_shadowmap.frag" });
	shaderManager->createShaderProgram("skybox",	{ "skybox.frag", "skybox.vert" });
	shaderManager->createShaderProgram("lamp",		{ "lamp.vert", "lamp.frag" });
	shaderManager->createShaderProgram("mirror",	{ "mirror.vert", "mirror.frag" });

	/* Scene Setup */
	GameplayScene scene = GameplayScene(display, shaderManager, textureManager, modelManager, uniforms);
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
		scene.render();

		display->update();


		currentFrameSpeed = SDL_GetTicks() - currentFrameTime;
		if (fpsMill > currentFrameSpeed)
			SDL_Delay(fpsMill - currentFrameSpeed);
	}
}