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

	if (currentKeyStates[SDL_SCANCODE_PAGEUP])
	{
		_ambientStrength += delta;
	}
	if (currentKeyStates[SDL_SCANCODE_PAGEDOWN])
	{
		_ambientStrength -= delta;
	}

	if (currentKeyStates[SDL_SCANCODE_L])
	{
		if (!_flashLightKeyLock)
		{
			_flashLightActive = !_flashLightActive;
			_flashLightKeyLock = true;
		}
	}
	else{
		_flashLightKeyLock = false;
	}

	if (currentKeyStates[SDL_SCANCODE_UP])
	{
		_directionalLight->translate(glm::vec3(-delta*_speed,0,0));
	}
	if (currentKeyStates[SDL_SCANCODE_DOWN])
	{
		_directionalLight->translate(glm::vec3(delta*_speed, 0, 0));
	}
	if (currentKeyStates[SDL_SCANCODE_LEFT])
	{
		_directionalLight->translate(glm::vec3(0, 0, delta*_speed));
	}
	if (currentKeyStates[SDL_SCANCODE_RIGHT])
	{
		_directionalLight->translate(glm::vec3(0, 0, -delta*_speed));
	}
	if (currentKeyStates[SDL_SCANCODE_PAGEUP])
	{
		_directionalLight->translate(glm::vec3(0, delta*_speed, 0));
	}
	if (currentKeyStates[SDL_SCANCODE_PAGEDOWN])
	{
		_directionalLight->translate(glm::vec3(0, -delta*_speed, 0));
	}

	if (currentKeyStates[SDL_SCANCODE_1])
	{
		_activeCamera = _cameras[0];
	}
	else if (currentKeyStates[SDL_SCANCODE_2])
	{
		_activeCamera = _mirrorCamera;
	}
	else if (currentKeyStates[SDL_SCANCODE_3])
	{
		_activeCamera = _shadowCamera;
	}



	/* Control light stuff*/
	_ambientStrength = glm::clamp(_ambientStrength, 0.0f, 1.0f);
	_directionalLight->setDirection(glm::normalize(glm::vec3(0, 0, 0) - _directionalLight->getPosition()));

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

	// _player camera translation
	_player->setTranslation(_cameras[0]->getPosition());

	_lamp->setTranslation(_directionalLight->getPosition());
	_mirrorCamera->setPosition(_billboard->getPosition());

	glm::vec3 mainCam = _cameras[0]->getPosition();
	glm::vec3 mainCamForward = _cameras[0]->getForward();
	glm::vec3 mirrorCam = _mirrorCamera->getPosition();
	glm::vec3 mirrorFwd = glm::normalize(mirrorCam - (mainCam+mainCamForward));
	_mirrorCamera->setForward(glm::reflect(mirrorFwd, glm::vec3(1,0,0)));

	// Update light Direction
	//_shadowCamera->setPosition(_lightPos);
	_shadowCamera->setForward(glm::normalize(_billboard->getPosition() - _directionalLight->getPosition()));

	_flashLight->setPosition(_cameras[0]->getPosition());
	_flashLight->setDirection(_cameras[0]->getForward());
}

void RenderToTextureScene::render()
{
	for (LightSource* lightSource : _lights)
	{
		if (lightSource->getType() == LightType::DIRECTIONAL)
		{
			_uniformManager->updateUniformData("directionalLight.color", _directionalLight->getColor());
			_uniformManager->updateUniformData("directionalLight.direction", _directionalLight->getDirection());
			_uniformManager->updateUniformData("lampColor", _directionalLight->getColor());
		}
		else if (lightSource->getType() == LightType::SPOT)
		{
			_uniformManager->updateUniformData("spotLight.color", _flashLight->getColor());
			_uniformManager->updateUniformData("spotLight.position", _flashLight->getPosition());
			_uniformManager->updateUniformData("spotLight.direction", _flashLight->getDirection());
			_uniformManager->updateUniformData("spotLight.cutOff", _flashLight->getCutOff());
			_uniformManager->updateUniformData("spotLight.cutOuterOff", _flashLight->getOuterCutOff());
			_uniformManager->updateUniformData("spotLight.constant", _flashLight->getConstant());
			_uniformManager->updateUniformData("spotLight.linear", _flashLight->getLinear());
			_uniformManager->updateUniformData("spotLight.quadratic", _flashLight->getQuadratic());
			_uniformManager->updateUniformData("spotLight.active", _flashLightActive);
		}
	}

	Camera* temp = _activeCamera;
	// Shadow
	glCullFace(GL_FRONT);			// remove peter panning
	_activeCamera = _shadowCamera;
	_shadowBuffer.bindForWriting();
	//_display->clear(0.5294117647058824f, 0.807843137254902f, 0.9803921568627451f, 1.0f);
	_display->clear((float)0 / 255, (float)0 / 255, (float)50 / 255, 1);
	_uniformManager->updateUniformData("MVP", _shadowCamera->getCameraProjection());
	drawShadowMap("shadowMap");
	_activeCamera = temp;
	glCullFace(GL_BACK);

	// Render to Texture (Mirror)
	_rttBuffer.bindForWriting();
	//_display->clear(0.5294117647058824f, 0.807843137254902f, 0.9803921568627451f, 1.0f);
	_display->clear((float)0 / 255, (float)0 / 255, (float)50 / 255, 1);
	_uniformManager->updateUniformData("MVP", _mirrorCamera->getCameraProjection());
	draw(true);

	temp = _activeCamera;

	// Depth-bias -1|1 to 0|1
	glm::mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);

	// Render to Window
	_display->bindRenderTarget();
	//_display->clear(0.5294117647058824f, 0.807843137254902f, 0.9803921568627451f, 1.0f);
	_display->clear((float)0 / 255, (float)0 / 255, (float)50 / 255, 1);
	_uniformManager->updateUniformData("DepthMVP", biasMatrix*_shadowCamera->getCameraProjection());
	_uniformManager->updateUniformData("MVP", _activeCamera->getCameraProjection());
	draw(true); 
}

void RenderToTextureScene::drawShadowMap(std::string shader)
{	
	_uniformManager->updateUniformData("shadowTexelSize", _shadowTexelSize);
	for (Entity* e : _entities)
	{
		_uniformManager->updateUniformData("model", *e->getCombinedMatrix());
		_shaderManager->drawWithShaderProgram(shader, e->getModel(), *_uniformManager);
	}
	// Billboard
	_uniformManager->updateUniformData("model", *_billboard->getCombinedMatrix());
	_shaderManager->drawWithShaderProgram(shader, _billboard->getModel(), *_uniformManager);
}

void RenderToTextureScene::draw(bool drawLightSource)
{
	_shadowBuffer.bindForReading(11);
	_uniformManager->updateUniformData("textureDepth", 11);

	/* Update Uniforms */
	_uniformManager->updateUniformData("ambientStrength", _ambientStrength);
	_uniformManager->updateUniformData("viewPosition", _activeCamera->getPosition());
	_uniformManager->updateUniformData("shadowTexelSize", _shadowTexelSize);
	

	/* Render Entities */
	for (Entity* e : _entities)
	{
		_uniformManager->updateUniformData("model", *e->getCombinedMatrix());
		_shaderManager->drawWithShaderProgram("main", e->getModel(), *_uniformManager);
	}
	if (drawLightSource)
	{
		if (_activeCamera != _shadowCamera)
		{
			// Lamp
			_uniformManager->updateUniformData("model", *_lamp->getCombinedMatrix());
			_shaderManager->drawWithShaderProgram("lamp", _lamp->getModel(), *_uniformManager);
		}
	}else{
		// Player
		_uniformManager->updateUniformData("model", *_player->getCombinedMatrix());
		_shaderManager->drawWithShaderProgram("main", _player->getModel(), *_uniformManager);
	}

	// Billboard
	_uniformManager->updateUniformData("model", *_billboard->getCombinedMatrix());
	_shaderManager->drawWithShaderProgram("mirror", _billboard->getModel(), *_uniformManager);
}

void RenderToTextureScene::init()
{
	loadAssets();
	SDL_ShowCursor(0);
	/* Mouse Delta */
	_lastX = _display->getWidth() / 2;
	_lastY = _display->getHeight() / 2;

	/* Set up Render to Texture */
	_rttBuffer.init(_display->getWidth(), _display->getHeight(), GL_TEXTURE_2D, GL_RGB32F, GL_RGB, GL_COLOR_ATTACHMENT0,false);
	_rttTexture = _rttBuffer.getTexture();

	/* Shadow Buffering */
	_shadowBuffer.init(_display->getWidth(), _display->getHeight(), GL_TEXTURE_2D, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT, true);
	_shadowTexture = _shadowBuffer.getTexture();
	_shadowTexelSize = glm::vec3(1.0f / 1280.0f, 1.0f / 720.0f, 0.0f);
		
	_modelManager->loadModel("cube", "cube/cube.obj");
	_modelManager->loadModel("plane", "plane/plane.obj");
	_modelManager->loadModel("screen", "screen/screen.obj");
	_modelManager->loadModel("screen2", "screen/screen.obj");
	_modelManager->loadModel("house", "house/house.obj");

	_billboard = new Entity(_modelManager->getModel("screen"));
	_billboard->addRotation(glm::vec3(1, 0, 0), -90);
	_billboard->addRotation(glm::vec3(0, 0, 1), 90);
	_billboard->setTranslation(glm::vec3(10 , 10, 0));
	_billboard->setScale(1);

	/* Set up mirror camera */
	Camera* mirrorCamera = new Camera(_billboard->getPosition(), 90, 1, 0.1, 500);
	glm::vec3 forward = glm::vec3(-1, 0, 0);
	mirrorCamera->setForward(forward);
	_mirrorCamera = mirrorCamera;
	_cameras.push_back(_mirrorCamera);

	Camera* shadowCamera = new Camera(_lightDirection, -70, 70, -70, 70, -100,100);
	_shadowCamera = shadowCamera;
	_cameras.push_back(_shadowCamera);

	_textureManager->loadTexture("default", "default.png");

	_modelManager->getModel("screen")->getMeshes()[0]->setTexture(_rttTexture);
	_modelManager->getModel("screen2")->getMeshes()[0]->setTexture(_textureManager->getTexture("rttTextureNew"));
	_entities.push_back(new Entity(_modelManager->getModel("cube")));
	_entities.push_back(new Entity(_modelManager->getModel("house")));
	//_entities.push_back(new Entity(_modelManager->getModel("cam"), glm::vec3(-14, 10, 0)));
	_player = new Entity(_modelManager->getModel("lakitu"), glm::vec3(0, 3, 0), glm::vec3(1, 1, 1), 0, 1.0f);
	
	Entity* plane = new Entity(_modelManager->getModel("plane"));
	plane->scale(5);
	plane->translate(glm::vec3(5, -0, 0));
	_entities.push_back(plane);
	
	_lamp = new Entity(_modelManager->getModel("cube"));

	/* Initialize Lights */
	_directionalLight = new DirectionalLight(glm::vec3((float)58 / 255, (float)58 / 255, (float)135 / 255), glm::vec3(0, 40, 0), glm::vec3(1, 0, 0));
	_lights.push_back(_directionalLight);
	_flashLight = new SpotLight(glm::vec3(1, 0, 0), _activeCamera->getPosition(), _activeCamera->getForward(), 
		glm::cos(glm::radians(12.5f)),
		glm::cos(glm::radians(13.5f)),
		1.0f,0.07f,0.017f);
	_lights.push_back(_flashLight);
}

void RenderToTextureScene::loadAssets()
{
}

void RenderToTextureScene::unloadAssets()
{
}