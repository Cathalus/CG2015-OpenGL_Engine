#include "GameplayScene.h"

void GameplayScene::init()
{
	SDL_ShowCursor(0);
	loadAssets();

	_lastX = _display->getWidth() / 2;
	_lastY = _display->getHeight() / 2;

	_entities.push_back(new Entity(_modelManager->getModel("plane")));
	_entities.back()->setScale(5);
	_entities.push_back(new Entity(_modelManager->getModel("house")));
	_entities.push_back(new Entity(_modelManager->getModel("level")));
	_entities.push_back(new Entity(_modelManager->getModel("cube"),glm::vec3(15,20,25)));
	_lightSource = new Entity(_modelManager->getModel("cube"), _lightPos, glm::vec3(1, 1, 1), 0, 0.5f);
}

GameplayScene::~GameplayScene()
{
	unloadAssets();
}

void GameplayScene::update(float delta)
{
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

	/* Input */
	glm::vec3 deltaMovement;
	glm::vec3 forward = _activeCamera->getForward();

	/* Mouse */
	int xpos, ypos;
	SDL_GetMouseState(&xpos, &ypos);
	GLfloat xoffset = xpos - _lastX;
	GLfloat sensitivity = 0.1f;
	GLfloat yoffset = _lastY - ypos; // Reversed since y-coordinates range from bottom to top

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	_yaw += xoffset;
	_pitch -= -yoffset;

	if (_pitch > 89.0f)
		_pitch = 89.0f;
	if (_pitch < -89.0f)
		_pitch = -89.0f;

	// Calculate new camera forward
	glm::vec3 fwd;
	fwd.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	fwd.y = sin(glm::radians(_pitch));
	fwd.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	forward = glm::normalize(fwd);
	if (_activeCamera == _cameras[0])
		_activeCamera->setForward(forward);

	if (xpos <= 5 || xpos >= _display->getWidth() - 5 ||
		ypos <= 5 || ypos >= _display->getHeight() - 5)
	{
		_lastX = _display->getWidth() / 2;
		_lastY = _display->getHeight() / 2;

		SDL_WarpMouseInWindow(_display->getWindow(), _lastX, _lastY);
	}
	else{
		_lastX = xpos;
		_lastY = ypos;
	}

	/* Keyboard */
	if (currentKeyStates[SDL_SCANCODE_W])
	{
		_cameras[0]->setPosition(_cameras[0]->getPosition() + _cameras[0]->getForward()*delta*_speed);
	}
	if (currentKeyStates[SDL_SCANCODE_S])
	{
		_cameras[0]->setPosition(_cameras[0]->getPosition() - _cameras[0]->getForward()*delta*_speed);
	}
	if (currentKeyStates[SDL_SCANCODE_A])
	{
		_cameras[0]->setPosition(_cameras[0]->getPosition() - (glm::normalize(glm::cross(_cameras[0]->getForward(), _cameras[0]->getUp())))*delta*_speed);
	}
	if (currentKeyStates[SDL_SCANCODE_D])
	{
		_cameras[0]->setPosition(_cameras[0]->getPosition() + (glm::normalize(glm::cross(_cameras[0]->getForward(), _cameras[0]->getUp())))*delta*_speed);
	}
	if (currentKeyStates[SDL_SCANCODE_SPACE])
	{
		_cameras[0]->setPosition(_cameras[0]->getPosition() + glm::vec3(0, delta*_speed, 0));
	}
	if (currentKeyStates[SDL_SCANCODE_LSHIFT])
	{
		_cameras[0]->setPosition(_cameras[0]->getPosition() + glm::vec3(0, -delta*_speed, 0));
	}

	_lightSource->setTranslation(_lightPos);
}

void GameplayScene::render()
{
	_display->clear((float)0 / 255, (float)0 / 255, (float) 50 / 255, 1);
	_uniformManager->updateUniformData("MVP", _activeCamera->getCameraProjection());
	draw("game");
}

void GameplayScene::draw(std::string shader)
{
	/* Update Uniforms */
	_uniformManager->updateUniformData("lightColor", _lightColor);
	_uniformManager->updateUniformData("lightPosition", _lightPos);
	_uniformManager->updateUniformData("ambientStrength", _ambientStrength);
	_uniformManager->updateUniformData("viewPosition", _activeCamera->getPosition());

	/* Render Entities */
	for (Entity* e : _entities)
	{
		_uniformManager->updateUniformData("model", *e->getCombinedMatrix());
		_shaderManager->drawWithShaderProgram(shader, e->getModel(), *_uniformManager);
	}

	_uniformManager->updateUniformData("model", *_lightSource->getCombinedMatrix());
	_shaderManager->drawWithShaderProgram("lamp", _lightSource->getModel(), *_uniformManager);
}

void GameplayScene::loadAssets()
{
	/* Textures */

	/* Models */
	_modelManager->loadModel("plane", "plane/plane.obj");
	_modelManager->loadModel("cube", "cube/cube.obj");
	_modelManager->loadModel("house", "house/house.obj");
	_modelManager->loadModel("level", "level/main.obj");
}

void GameplayScene::unloadAssets()
{
	// Unload Models
	_modelManager->unloadModel("plane");
	_modelManager->unloadModel("cube");
	_modelManager->unloadModel("house");
	_modelManager->unloadModel("level");
}