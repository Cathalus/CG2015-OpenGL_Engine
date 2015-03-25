#include "RenderToTextureScene.h"
#include <GLM\gtc\matrix_transform.hpp>

void RenderToTextureScene::update(float delta)
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
		_entities[0]->translate(glm::vec3(delta*(-_speed), 0, 0));
		//_entities[0]->setMatrix(glm::translate(*_entities[0]->getMatrix(), glm::vec3(delta*(-_speed), 0, 0)));
	}
	if (currentKeyStates[SDL_SCANCODE_DOWN])
	{
		_entities[0]->translate(glm::vec3(delta*(_speed), 0, 0));
		//_entities[0]->setMatrix(glm::translate(*_entities[0]->getMatrix(), glm::vec3(delta*(_speed), 0, 0)));
	}
	if (currentKeyStates[SDL_SCANCODE_LEFT])
	{
		_entities[0]->translate(glm::vec3(0, 0, delta*(_speed)));
		//_entities[0]->setMatrix(glm::translate(*_entities[0]->getMatrix(), glm::vec3(0, 0, delta*(_speed))));
	}
	if (currentKeyStates[SDL_SCANCODE_RIGHT])
	{
		_entities[0]->translate(glm::vec3(0, 0, delta*(-_speed)));
		//_entities[0]->setMatrix(glm::translate(*_entities[0]->getMatrix(), glm::vec3(0, 0, delta*(-_speed))));
	}

	if (currentKeyStates[SDL_SCANCODE_1])
	{
		_activeCamera = _cameras[0];
	}
	else if (currentKeyStates[SDL_SCANCODE_2])
	{
		_activeCamera = _mirrorCamera;
	}
	else if (currentKeyStates[SDL_SCANCODE_4])
	{
		_activeCamera = _shadowCamera;
	}
	else if (currentKeyStates[SDL_SCANCODE_3])
	{
		_activeCamera = _mirrorCamera2;
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

	// _player camera translation
	_player->setTranslation(_cameras[0]->getPosition());

	_lamp->setTranslation(_lightPos);
	_mirrorCamera->setPosition(_billboard->getPosition());
	_mirrorCamera2->setPosition(_billboard2->getPosition());

	glm::vec3 mainCam = _cameras[0]->getPosition();
	glm::vec3 mainCamForward = _cameras[0]->getForward();
	glm::vec3 mirrorCam = _mirrorCamera->getPosition();
	glm::vec3 mirrorFwd = glm::normalize(mirrorCam - (mainCam+mainCamForward));
	_mirrorCamera->setForward(glm::reflect(mirrorFwd, glm::vec3(1,0,0)));

	mirrorCam = _mirrorCamera2->getPosition();
	mirrorFwd = glm::normalize(mirrorCam - (mainCam + mainCamForward));
	_mirrorCamera2->setForward(glm::reflect(mirrorFwd, glm::vec3(1, 0, 0)));
}

void RenderToTextureScene::render()
{
	// Render to Texture (Mirror)
	_rttBuffer.bindForWriting();
	_display->clear(0, 0, 0, 1.0f);
	_activeCamera = _mirrorCamera;
	_uniformManager->updateUniformData("MVP", _mirrorCamera->getCameraProjection());
	draw(false);

	// Render to Texture (Mirror2)
	_rttBuffer2.bindForWriting();
	_display->clear(1, 1, 1, 1.0f);
	_activeCamera = _mirrorCamera2;
	_uniformManager->updateUniformData("MVP", _mirrorCamera2->getCameraProjection());
	draw(false);

	// Render to Window
	_display->bindRenderTarget();
	_display->clear(0.5294117647058824f, 0.807843137254902f, 0.9803921568627451f, 1.0f);
	_activeCamera = _cameras[0];
	_uniformManager->updateUniformData("MVP", _activeCamera->getCameraProjection());
	draw(true);
}

void RenderToTextureScene::draw(bool drawLightSource)
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
	}else{
		// Player
		_uniformManager->updateUniformData("model", *_player->getCombinedMatrix());
		_shaderManager->drawWithShaderProgram("main", _player->getModel(), *_uniformManager);
	}

	// Billboard
	_uniformManager->updateUniformData("model", *_billboard->getCombinedMatrix());
	_shaderManager->drawWithShaderProgram("mirror", _billboard->getModel(), *_uniformManager);
	// Billboard2
	_uniformManager->updateUniformData("model", *_billboard2->getCombinedMatrix());
	_shaderManager->drawWithShaderProgram("mirror", _billboard2->getModel(), *_uniformManager);

	/* Update Uniforms */
	_uniformManager->updateUniformData("lightColor", _lightColor);
	_uniformManager->updateUniformData("lightPosition", _lightPos);
	_uniformManager->updateUniformData("ambientStrength", _ambientStrength);
	_uniformManager->updateUniformData("viewPosition", _activeCamera->getPosition());
}

void RenderToTextureScene::init()
{
	loadAssets();
	SDL_ShowCursor(0);
	/* Mouse Delta */
	_lastX = _display->getWidth() / 2;
	_lastY = _display->getHeight() / 2;

	/* Set up Render to Texture */
	_rttTexture = new Texture(_display->getWidth(), _display->getHeight(), GL_TEXTURE_2D, GL_RGB);
	_textureManager->loadTexture("rttTexture", _rttTexture);
	_rttBuffer.init(_rttTexture, GL_COLOR_ATTACHMENT0);

	_rttTextureNew = new Texture(_display->getWidth(), _display->getHeight(), GL_TEXTURE_2D, GL_RGB);
	_textureManager->loadTexture("rttTextureNew", _rttTextureNew);
	_rttBuffer2.init(_rttTextureNew, GL_COLOR_ATTACHMENT1);

	/* Set up shadow camera */
	Camera* lightCamera = new Camera(*_activeCamera);
	glm::vec3 forward = glm::vec3(-0.8f, -0.6f, 0.0f);
	lightCamera->setForward(forward);
	lightCamera->setPosition(_lightPos);
	_shadowCamera = lightCamera;
	_cameras.push_back(_shadowCamera);

	/* Set up mirror camera */
	Camera* mirrorCamera = new Camera(*_activeCamera);
	forward = glm::vec3(-1, 0, 0);
	mirrorCamera->setForward(forward);
	mirrorCamera->setPosition(_lightPos);
	_mirrorCamera = mirrorCamera;
	_cameras.push_back(_mirrorCamera);

	/* Set up mirror camera */
	Camera* mirrorCamera2 = new Camera(*_activeCamera);
	forward = glm::vec3(1, 0, 0);
	mirrorCamera2->setForward(forward);
	mirrorCamera2->setPosition(_lightPos);
	_mirrorCamera2 = mirrorCamera2;
	_cameras.push_back(_mirrorCamera2);

	_modelManager->loadModel("cube", "cube/cube.obj");
	_modelManager->loadModel("plane", "plane/plane.obj");
	_modelManager->loadModel("screen", "screen/screen.obj");
	_modelManager->loadModel("screen2", "screen/screen.obj");
	_modelManager->loadModel("lakitu", "lakitu/lakitu.obj");

	_billboard = new Entity(_modelManager->getModel("screen"));
	_billboard->addRotation(glm::vec3(1, 0, 0), -90);
	_billboard->addRotation(glm::vec3(0, 0, 1), 90);
	_billboard->setTranslation(glm::vec3(10 , 8, 0));

	_billboard2 = new Entity(_modelManager->getModel("screen2"));
	_billboard2->addRotation(glm::vec3(1, 0, 0), -90);
	_billboard2->addRotation(glm::vec3(0, 0, 1), -90);
	_billboard2->setTranslation(glm::vec3(-10, 8, 0));

	_modelManager->getModel("screen")->getMeshes()[0]->setTexture(_textureManager->getTexture("rttTexture"));
	_modelManager->getModel("screen2")->getMeshes()[0]->setTexture(_textureManager->getTexture("rttTextureNew"));
	_entities.push_back(new Entity(_modelManager->getModel("cube"),glm::vec3(0,0,0),glm::vec3(1,1,1),0,2));
	_player = new Entity(_modelManager->getModel("lakitu"), glm::vec3(0, 3, 0), glm::vec3(1, 1, 1), 0, 1.0f);
	
	Entity* plane = new Entity(_modelManager->getModel("plane"));
	plane->scale(5);
	plane->translate(glm::vec3(0, -5, 0));
	_entities.push_back(plane);
	
	_lamp = new Entity(_modelManager->getModel("cube"));
}

void RenderToTextureScene::loadAssets()
{
}

void RenderToTextureScene::unloadAssets()
{
}