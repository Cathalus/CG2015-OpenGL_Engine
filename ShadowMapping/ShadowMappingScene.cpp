#include "ShadowMappingScene.h"

void ShadowMappingScene::update(float delta)
{
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
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

	if (currentKeyStates[SDL_SCANCODE_W])
	{
		_activeCamera->setPosition(_activeCamera->getPosition() + _activeCamera->getForward()*delta*_speed);
	}
	if (currentKeyStates[SDL_SCANCODE_S])
	{
		_activeCamera->setPosition(_activeCamera->getPosition() - _activeCamera->getForward()*delta*_speed);
	}
	if (currentKeyStates[SDL_SCANCODE_A])
	{
		_activeCamera->setPosition(_activeCamera->getPosition() - (glm::normalize(glm::cross(_activeCamera->getForward(), _activeCamera->getUp())))*delta*_speed);
	}
	if (currentKeyStates[SDL_SCANCODE_D])
	{
		_activeCamera->setPosition(_activeCamera->getPosition() + (glm::normalize(glm::cross(_activeCamera->getForward(), _activeCamera->getUp())))*delta*_speed);
	}
	if (currentKeyStates[SDL_SCANCODE_SPACE])
	{
		_activeCamera->setPosition(_activeCamera->getPosition() + glm::vec3(0, delta*_speed, 0));
	}
	if (currentKeyStates[SDL_SCANCODE_LSHIFT])
	{
		_activeCamera->setPosition(_activeCamera->getPosition() + glm::vec3(0, -delta*_speed, 0));
	}

	if (currentKeyStates[SDL_SCANCODE_PAGEUP])
	{
		_ambientStrength += delta;
	}
	if (currentKeyStates[SDL_SCANCODE_PAGEDOWN])
	{
		_ambientStrength -= delta;
	}

	if (currentKeyStates[SDL_SCANCODE_UP])
	{
		_entities[0]->setMatrix(glm::translate(*_entities[0]->getMatrix(), glm::vec3(delta*(-_speed), 0, 0)));
	}
	if (currentKeyStates[SDL_SCANCODE_DOWN])
	{
		_entities[0]->setMatrix(glm::translate(*_entities[0]->getMatrix(), glm::vec3(delta*(_speed), 0, 0)));
	}
	if (currentKeyStates[SDL_SCANCODE_LEFT])
	{
		_entities[0]->setMatrix(glm::translate(*_entities[0]->getMatrix(), glm::vec3(0, 0, delta*(_speed))));
	}
	if (currentKeyStates[SDL_SCANCODE_RIGHT])
	{
		_entities[0]->setMatrix(glm::translate(*_entities[0]->getMatrix(), glm::vec3(0, 0, delta*(-_speed))));
	}

	if (currentKeyStates[SDL_SCANCODE_1])
	{
		_activeCamera = _cameras[0];
	}
	else if (currentKeyStates[SDL_SCANCODE_2])
	{
		_activeCamera = _cameras[1];
	}



	/* Control light stuff*/
	_ambientStrength = glm::clamp(_ambientStrength, 0.0f, 1.0f);

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

	_acc += delta;

	/* Light color effect */
	float r = glm::abs(cos(_acc * 3));
	float g = glm::abs(sin(_acc * 5));
	float b = glm::abs(sin(_acc * 2));
	_lightColor = glm::vec3(0.98f, 0.98f, 0.825f);
}

void ShadowMappingScene::shadowMap()
{
	_display->clear(0.5294117647058824f, 0.807843137254902f, 0.9803921568627451f, 1.0f);
	
	// Bind shadow buffer
	_shadowBuffer->bindForWriting();
	_uniformManager->updateUniformData("LSP", _cameras[1]->getCameraProjection()); // Send light space position to the vertex shader
	draw(false);

	// Unbind shadowbuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMappingScene::render()
{
	_display->clear(0.5294117647058824f, 0.807843137254902f, 0.9803921568627451f, 1.0f);
	_uniformManager->updateUniformData("MVP", _activeCamera->getCameraProjection());
	_uniformManager->updateUniformData("LSP", _cameras[1]->getCameraProjection()); // Send light space position to the vertex shader
	draw(true);
}

void ShadowMappingScene::draw(bool drawLightSource)
{

	/* Render Entities */
	for (Entity* e : _entities)
	{
		_uniformManager->updateUniformData("model", *e->getCombinedMatrix());
		_shaderManager->drawWithShaderProgram("main", e->getModel(), *_uniformManager);
	}
	if (drawLightSource)
	{
		// Lamp
		_uniformManager->updateUniformData("model", *_lamp->getCombinedMatrix());
		_shaderManager->drawWithShaderProgram("lamp", _lamp->getModel(), *_uniformManager);
	}

	/* Update Uniforms */
	_uniformManager->updateUniformData("lightColor", _lightColor);
	_uniformManager->updateUniformData("lightPosition", _lightPos);
	_uniformManager->updateUniformData("ambientStrength", _ambientStrength);
	_uniformManager->updateUniformData("viewPosition", _activeCamera->getPosition());
}

void ShadowMappingScene::init()
{
	loadAssets();
	SDL_ShowCursor(0);
	/* Mouse Delta */
	_lastX = _display->getWidth() / 2;
	_lastY = _display->getHeight() / 2;
	
	/* Intialize Shadowbuffer */
	_shadowBuffer = new Framebuffer();
	_shadowBuffer->init(_display->getWidth(), _display->getHeight());

	_shaderManager->setShadowBuffer(_shadowBuffer);

	Camera* lightCamera = new Camera(*_activeCamera);
	glm::vec3 forward = glm::vec3(-0.8f, -0.6f, 0.0f);
	lightCamera->setForward(forward);
	lightCamera->setPosition(_lightPos);
	_cameras.push_back(lightCamera);
	_uniformManager->updateUniformData("LSP", _cameras[1]->getCameraProjection()); // Send light space position to the vertex shader
	_activeCamera = _cameras[1];

	_modelManager->loadModel("cube", "cube/cube.obj");
	_modelManager->loadModel("plane", "plane/plane.obj");

	_entities.push_back(new Entity(_modelManager->getModel("cube")));
	_entities.push_back(new Entity(_modelManager->getModel("plane"),glm::vec3(0,-1,0)));
	_lamp = new Entity(_modelManager->getModel("cube"), _lightPos);
}

void ShadowMappingScene::loadAssets()
{
}

void ShadowMappingScene::unloadAssets()
{
}