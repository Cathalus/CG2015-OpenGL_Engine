#include "GameplayScene.h"
#include <GLM\gtc\matrix_transform.hpp>

void GameplayScene::update(float delta)
{
	_acc += delta;

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
		_cameras[0]->setPosition(_cameras[0]->getPosition() + _cameras[0]->getForward()*delta*_movementSpeed);
	}
	if (currentKeyStates[SDL_SCANCODE_S])
	{
		_cameras[0]->setPosition(_cameras[0]->getPosition() - _cameras[0]->getForward()*delta*_movementSpeed);
	}
	if (currentKeyStates[SDL_SCANCODE_A])
	{
		_cameras[0]->setPosition(_cameras[0]->getPosition() - (glm::normalize(glm::cross(_cameras[0]->getForward(), _cameras[0]->getUp())))*delta*_movementSpeed);
	}
	if (currentKeyStates[SDL_SCANCODE_D])
	{
		_cameras[0]->setPosition(_cameras[0]->getPosition() + (glm::normalize(glm::cross(_cameras[0]->getForward(), _cameras[0]->getUp())))*delta*_movementSpeed);
	}
	if (currentKeyStates[SDL_SCANCODE_SPACE])
	{
		_cameras[0]->setPosition(_cameras[0]->getPosition() + glm::vec3(0, delta*_movementSpeed, 0));
	}
	if (currentKeyStates[SDL_SCANCODE_LSHIFT])
	{
		_cameras[0]->setPosition(_cameras[0]->getPosition() + glm::vec3(0, -delta*_movementSpeed, 0));
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

	if (temp != NULL)
	{
		if (currentKeyStates[SDL_SCANCODE_T])
		{
			temp->translate(glm::vec3(-delta, 0, 0));
		}
		if (currentKeyStates[SDL_SCANCODE_G])
		{
			temp->translate(glm::vec3(delta, 0, 0));
		}
		if (currentKeyStates[SDL_SCANCODE_F])
		{
			temp->translate(glm::vec3(0, 0, delta));
		}
		if (currentKeyStates[SDL_SCANCODE_H])
		{
			temp->translate(glm::vec3(0, 0, -delta));
		}
		if (currentKeyStates[SDL_SCANCODE_U])
		{
			temp->translate(glm::vec3(0, delta, 0));
		}
		if (currentKeyStates[SDL_SCANCODE_J])
		{
			temp->translate(glm::vec3(0, -delta, 0));
		}
		if (currentKeyStates[SDL_SCANCODE_Z])
		{
			std::cout << temp->getPosition().x << " " << temp->getPosition().y << " " <<  temp->getPosition().z << std::endl;
		}
	}
	if (currentKeyStates[SDL_SCANCODE_X])
	{
		std::cout << _activeCamera->getPosition().x << " " << _activeCamera->getPosition().y << " " << _activeCamera->getPosition().z << std::endl;
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

	glm::vec3 mirrorCam = _mirror->getPosition();
	float yValue = glm::clamp(_cameras[0]->getPosition().y, 0.0f, 21.0f);
	mirrorCam.y = yValue;
	glm::vec3 mirrorFwd = glm::normalize(mirrorCam-mainCam);
	_mirrorCamera->setForward(glm::reflect(mirrorFwd, glm::vec3(1,0,0)));

	// Update light Direction
	_shadowCamera->setForward(-_directionalLight->getPosition());
	_flashLight->setPosition(_cameras[0]->getPosition());
	_flashLight->setDirection(_cameras[0]->getForward());

	_lamp->setTranslation(_directionalLight->getPosition());

	// Bouncing cube animation
	float val = glm::sin(_acc) * 4;
	_bounceCube->setTranslation(glm::vec3(_bounceCube->getPosition().x, 7+val, _bounceCube->getPosition().z));

	// Translate and rotate player
	_player->setTranslation(_cameras[0]->getPosition());
	_player->clearRotation();
	glm::vec3 camFwd = glm::vec3(_cameras[0]->getForward().x, 0, _cameras[0]->getForward().z);
	glm::vec3 cubeFwd = glm::vec3(0, 0, -1);
	float angle = glm::dot(cubeFwd, camFwd);
	_player->addRotation(glm::vec3(0, 1, 0), glm::degrees(angle));
	_player->rotate();
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
	_display->clear((float)0 / 255, (float)0 / 255, (float)50 / 255, 1);
	_uniformManager->updateUniformData("MVP", _shadowCamera->getCameraProjection());
	drawShadowMap("shadowMap");
	_activeCamera = temp;
	glCullFace(GL_BACK);

	// Render to Texture (Mirror)
	_activeCamera = _mirrorCamera;
	_rttBuffer.bindForWriting();
	_display->clear((float)0 / 255, (float)0 / 255, (float)50 / 255, 1);
	_uniformManager->updateUniformData("MVP", _mirrorCamera->getCameraProjection());
	draw(true, true);

	_activeCamera = temp;

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
	draw(true, false); 

	// Render Cubemap
	_cubeMap->Render(_activeCamera->getPosition() + glm::vec3(0, 5, 0), _uniformManager, _shaderManager, "skybox");
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

void GameplayScene::draw(bool drawLightSource, bool drawPlayer)
{
	_shadowBuffer.bindForReading(11);
	_uniformManager->updateUniformData("textureDepth", 11);

	_uniformManager->updateUniformData("ambientStrength", _ambientStrength);
	_uniformManager->updateUniformData("viewPosition", _activeCamera->getPosition());
	_uniformManager->updateUniformData("shadowTexelSize", _shadowTexelSize);

	for (Entity* e : _entities)
	{
		if (e == _mirror)
		{
			_uniformManager->updateUniformData("model", *e->getCombinedMatrix());
			_shaderManager->drawWithShaderProgram("mirror", e->getModel(), *_uniformManager);
		}
		else{
			_uniformManager->updateUniformData("model", *e->getCombinedMatrix());
			_shaderManager->drawWithShaderProgram("main", e->getModel(), *_uniformManager);
		}
	}

	// Draw Lamp
	_uniformManager->updateUniformData("model", *_lamp->getCombinedMatrix());
	_shaderManager->drawWithShaderProgram("lamp", _lamp->getModel(), *_uniformManager);

	// Draw Player
	if (drawPlayer)
		_uniformManager->updateUniformData("model", *_player->getCombinedMatrix());
	_shaderManager->drawWithShaderProgram("main", _player->getModel(), *_uniformManager);
}

void GameplayScene::init()
{
	SDL_ShowCursor(0);
	/* Mouse Delta */
	_lastX = _display->getWidth() / 2;
	_lastY = _display->getHeight() / 2;

	/* Set up Render to Texture */
	_rttBuffer.init(_display->getWidth(), _display->getHeight(), GL_TEXTURE_2D, GL_RGB, GL_RGB, GL_COLOR_ATTACHMENT0, false);
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

	loadAssets();
	initEntities();

	_cubeMap = new CubeMap(std::string("skybox/mn_front.png"), std::string("skybox/mn_back.png"),
		std::string("skybox/mn_up.png"), std::string("skybox/mn_down.png"),
		std::string("skybox/mn_right.png"), std::string("skybox/mn_left.png"),
		_modelManager->getModel("skybox"));
}


void GameplayScene::initEntities()
{
	// Terrain
	temp = new Entity(_modelManager->getModel("terrain"));
	temp->addRotation(glm::vec3(0, 1, 0), 180);
	temp->rotate();
	_entities.push_back(temp);
	
	// Lamp
	_lamp = new Entity(_modelManager->getModel("cube"));

	// Mirror
	temp = new Entity(_modelManager->getModel("plane"));
	temp->setScale(1);
	temp->addRotation(glm::vec3(0, 0, 1), 90);
	temp->addRotation(glm::vec3(0, 1, 0), 90);
	temp->setTranslation(glm::vec3(20, 11, -10));
	temp->rotate();
	_mirrorCamera->setPosition(temp->getPosition());
	temp->getModel()->getMeshes()[0]->setTexture(_textureManager->getTexture("mirrorTexture"));
	_mirror = temp;
	_entities.push_back(_mirror);

	// StreetLamp 1
	temp = new Entity(_modelManager->getModel("lamp"));
	temp->setTranslation(glm::vec3(-3.72f, 1.342f, -13.2791f));
	temp->addRotation(glm::vec3(0, 1, 0), 30);
	temp->rotate();
	_lights.push_back(new PointLight(glm::vec3((float)255 / 255, (float)214 / 255, (float)170 / 255), temp->getPosition(), 1, 0.09f, 0.032f));
	_entities.push_back(temp);

	// StreetLamp 2
	temp = new Entity(_modelManager->getModel("lamp"));
	temp->setTranslation(glm::vec3(18.755f, 1.66527f, -20.0f));
	temp->addRotation(glm::vec3(0, 1, 0), 30);
	temp->rotate();
	_lights.push_back(new PointLight(glm::vec3((float)255 / 255, (float)214 / 255, (float)170 / 255), temp->getPosition(), 1, 0.027f, 0.0028f));
	_entities.push_back(temp);

	// StreetLamp 3
	temp = new Entity(_modelManager->getModel("lamp"));
	temp->setTranslation(glm::vec3(18.755f, 2.0f, 0.0f));
	temp->addRotation(glm::vec3(0, 1, 0), 30);
	temp->rotate();
	_lights.push_back(new PointLight(glm::vec3((float)255 / 255, (float)214 / 255, (float)170 / 255), temp->getPosition(), 1, 0.027f, 0.0028f));
	_entities.push_back(temp);

	// In House Light
	_lights.push_back(new PointLight(glm::vec3((float)255 / 255, (float)214 / 255, (float)170 / 255), glm::vec3(0.54903f,4.12578f, 2.50576f), 1, 0.09f, 0.032f));

	// Play Zone Light
	_lights.push_back(new PointLight(glm::vec3((float)200 / 255, (float)200 / 255, (float) 200 / 255), glm::vec3(-62, 15, -54), 1, 0.022f, 0.0019f));

	// Cube
	temp = new Entity(_modelManager->getModel("cube_bricks"));
	_bounceCube = temp;
	_bounceCube->setTranslation(glm::vec3(-54, 7, -49));
	_entities.push_back(_bounceCube);

	// Player Model
	_player = new Entity(_modelManager->getModel("cube_awesome"));
	_player->setTranslation(_cameras[0]->getPosition());
}

void GameplayScene::loadAssets()
{
	/* Models */
	_modelManager->loadModel("skybox", "skybox/skybox.obj");
	_modelManager->loadModel("cube", "cube/cube.obj");
	_modelManager->loadModel("cube_bricks", "cube/cube_brick.obj");
	_modelManager->loadModel("cube_awesome", "cube/cube_awesome.obj");
	_modelManager->loadModel("terrain", "level/level.obj");
	_modelManager->loadModel("plane", "plane/plane.obj");
	_modelManager->loadModel("lamp", "lamp/untitled.obj");
	_modelManager->loadModel("cam", "kamera/KameraNew.obj");

	/* Textures */
	bool success;
	_textureManager->loadTexture("landTexture", "landTexture.png", success);
	_textureManager->loadTexture("mirrorTexture", _rttTexture);

	/* Initialize Lights */
	_directionalLight = new DirectionalLight(glm::vec3((float)58 / 255, (float)58 / 255, (float)135 / 255), glm::vec3(-215, 210, -245), glm::vec3(1, 0, 0), 1);
	_lights.push_back(_directionalLight);
	_flashLight = new SpotLight(glm::vec3(1, 1, 1), _activeCamera->getPosition(), _activeCamera->getForward(),
		glm::cos(glm::radians(12.5f)),
		glm::cos(glm::radians(17.5f)),
		1.0f, 0.09f, 0.032f);
	_lights.push_back(_flashLight);
}

void GameplayScene::unloadAssets()
{
	_modelManager->unloadModel("skybox");
	_modelManager->unloadModel("plane");
	_modelManager->unloadModel("cube");
}