#include "GameplayScene.h"
#include <GLM\gtc\matrix_transform.hpp>

void GameplayScene::update(float delta)
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

	// _player camera translation
	glm::vec3 mainCam = _cameras[0]->getPosition();
	glm::vec3 mainCamForward = _cameras[0]->getForward();
	glm::vec3 mirrorCam = _mirrorCamera->getPosition();
	glm::vec3 mirrorFwd = glm::normalize(mirrorCam - (mainCam+mainCamForward));
	_mirrorCamera->setForward(glm::reflect(mirrorFwd, glm::vec3(1,0,0)));

	// Update light Direction
	_shadowCamera->setForward(-_directionalLight->getPosition());
	_flashLight->setPosition(_cameras[0]->getPosition());
	_flashLight->setDirection(_cameras[0]->getForward());

	_lamp->setTranslation(_directionalLight->getPosition());
}

void GameplayScene::updateLightSources()
{
	int pointCNT = 0;
	for (LightSource* lightSource : _lights)
	{
		if (lightSource->getType() == LightType::DIRECTIONAL)
		{
			_uniformManager->updateUniformData("directionalLight.color", _directionalLight->getColor());
			_uniformManager->updateUniformData("directionalLight.direction", _directionalLight->getDirection());
			_uniformManager->updateUniformData("directionalLight.intensity", _directionalLight->getIntensity());
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
		else if (lightSource->getType() == LightType::POINT)
		{
			std::cout << pointCNT << std::endl;
			_uniformManager->updateUniformData("pointLights[" + std::to_string(pointCNT) + "].color",
				lightSource->getColor());
			_uniformManager->updateUniformData("pointLights[" + std::to_string(pointCNT) + "].position",
				lightSource->getPosition());
			_uniformManager->updateUniformData("pointLights[" + std::to_string(pointCNT) + "].constant",
				((PointLight*)lightSource)->getConstant());
			_uniformManager->updateUniformData("pointLights[" + std::to_string(pointCNT) + "].linear",
				((PointLight*)lightSource)->getLinear());
			_uniformManager->updateUniformData("pointLights[" + std::to_string(pointCNT) + "].quadratic",
				((PointLight*)lightSource)->getQuadratic());
			_uniformManager->updateUniformData("pointLights[" + std::to_string(pointCNT) + "].active",
				true);
			pointCNT++;
		}
	}
	_uniformManager->updateUniformData("lampcolor", _directionalLight->getColor());
}

void GameplayScene::render()
{
	updateLightSources();
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
	_display->clear((float)0 / 255, (float)0 / 255, (float)50 / 255, 1);
	_uniformManager->updateUniformData("DepthMVP", biasMatrix*_shadowCamera->getCameraProjection());
	_uniformManager->updateUniformData("MVP", _activeCamera->getCameraProjection());
	draw(true); 

	// Render Cubemap
	_cubeMap->Render(_activeCamera->getPosition()+glm::vec3(0,5,0), _uniformManager, _shaderManager, "skybox");
}

void GameplayScene::drawShadowMap(std::string shader)
{	
	_uniformManager->updateUniformData("shadowTexelSize", _shadowTexelSize);
	for (Entity* e : _entities)
	{
		_uniformManager->updateUniformData("model", *e->getCombinedMatrix());
		_shaderManager->drawWithShaderProgram(shader, e->getModel(), *_uniformManager);
	}
}

void GameplayScene::draw(bool drawLightSource)
{
	_shadowBuffer.bindForReading(11);
	_uniformManager->updateUniformData("textureDepth", 11);

	_uniformManager->updateUniformData("ambientStrength", _ambientStrength);
	_uniformManager->updateUniformData("viewPosition", _activeCamera->getPosition());
	_uniformManager->updateUniformData("shadowTexelSize", _shadowTexelSize);

	for (Entity* e : _entities)
	{
		_uniformManager->updateUniformData("model", *e->getCombinedMatrix());
		_shaderManager->drawWithShaderProgram("main", e->getModel(), *_uniformManager);
	}

	// Draw Lamp
	_uniformManager->updateUniformData("model", *_lamp->getCombinedMatrix());
	_shaderManager->drawWithShaderProgram("lamp", _lamp->getModel(), *_uniformManager);
}

void GameplayScene::init()
{
	loadAssets();
	initEntities();
	SDL_ShowCursor(0);
	/* Mouse Delta */
	_lastX = _display->getWidth() / 2;
	_lastY = _display->getHeight() / 2;

	/* Set up Render to Texture */
	_rttBuffer.init(_display->getWidth(), _display->getHeight(), GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_COLOR_ATTACHMENT0,false);
	_rttTexture = _rttBuffer.getTexture();

	/* Shadow Buffering */
	_shadowBuffer.init(_display->getWidth(), _display->getHeight(), GL_TEXTURE_2D, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT, true);
	_shadowTexture = _shadowBuffer.getTexture();	
	_shadowTexelSize = glm::vec3(1.0f / 1280.0f, 1.0f / 720.0f, 0.0f);
		
	/* Set up mirror camera */
	Camera* mirrorCamera = new Camera(glm::vec3(0,0,0), 90, 1, 0.1, 500);
	glm::vec3 forward = glm::vec3(-1, 0, 0);
	mirrorCamera->setForward(forward);
	_mirrorCamera = mirrorCamera;
	_cameras.push_back(_mirrorCamera);

	/* Set up shadow camera */
	Camera* shadowCamera = new Camera(_lightDirection, -300, 300, -300, 300, -300,300);
	_shadowCamera = shadowCamera;
	_cameras.push_back(_shadowCamera);

	_cubeMap = new CubeMap(std::string("skybox/mn_front.png"), std::string("skybox/mn_back.png"),
						   std::string("skybox/mn_up.png"), std::string("skybox/mn_down.png"),
						   std::string("skybox/mn_right.png"), std::string("skybox/mn_left.png"),
						   _modelManager->getModel("skybox"));
}

void GameplayScene::loadAssets()
{
	/* Models */
	_modelManager->loadModel("skybox", "skybox/skybox.obj");
	_modelManager->loadModel("cube", "cube/cube.obj");
	_modelManager->loadModel("terrain", "level/level.obj");
	_modelManager->loadModel("tree", "level/tree/tree.obj");

	/* Initialize Lights */
	_directionalLight = new DirectionalLight(glm::vec3((float)58 / 255, (float)58 / 255, (float)135 / 255), glm::vec3(-215, 210, -245), glm::vec3(1, 0, 0),1);
	_lights.push_back(_directionalLight);
	_flashLight = new SpotLight(glm::vec3(1, 1, 0), _activeCamera->getPosition(), _activeCamera->getForward(),
		glm::cos(glm::radians(12.5f)),
		glm::cos(glm::radians(17.5f)),
		1.0f, 0.09f, 0.032f);
	_lights.push_back(_flashLight);
	_lights.push_back(new PointLight(glm::vec3((float)0 / 255, (float)255 / 255, (float)0 / 255),
		glm::vec3(44, 17, -34), 1, 0.14f, 0.07f));
	_lights.push_back(new PointLight(glm::vec3((float)255 / 255, (float)0 / 255, (float)0 / 255),
		glm::vec3(93, 42, 7), 1, 0.022f, 0.0019f));
	_lights.push_back(new PointLight(glm::vec3((float)255 / 255, (float)191 / 255, (float)0 / 255),
		glm::vec3(62, 32, -118), 1, 0.007f, 0.0002f));
}

void GameplayScene::initEntities()
{
	Entity* temp;
	
	// Terrain
	temp = new Entity(_modelManager->getModel("terrain"));
	temp->addRotation(glm::vec3(0, 1, 0), 180);
	temp->rotate();
	_entities.push_back(temp);
	
	// Lamp
	_lamp = new Entity(_modelManager->getModel("cube"));

	// Tree
	temp = new Entity(_modelManager->getModel("tree"));
	temp->setTranslation(glm::vec3(0, 0, -10));
	_entities.push_back(temp);
}

void GameplayScene::unloadAssets()
{
	_modelManager->unloadModel("skybox");
	_modelManager->unloadModel("plane");
}