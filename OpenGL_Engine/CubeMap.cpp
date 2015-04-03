#include "CubeMap.h"
#include "stb_image.h"

CubeMap::CubeMap(	std::string positiveX,
					std::string negativeX,
					std::string positiveY,
					std::string negativeY,
					std::string positiveZ,
					std::string negativeZ,
					Model* model)
{
	std::string fileNames[] = { positiveX, negativeX,
								positiveY, negativeY,
								positiveZ, negativeZ};
	GLenum types[] = {	GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
						GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
						GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };
	glGenTextures(1, _cubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, *_cubeMap);

	for (int i = 0; i < 6; i++)
	{
		int width, height, numComponents;
		unsigned char* imageData = stbi_load(std::string("textures/").append(fileNames[i]).c_str(), &width, &height, &numComponents, 4);

		if (imageData == NULL)
			std::cerr << "Texture loading failed for cubeMap: " << fileNames[i] << std::endl;

		glTexImage2D(types[i], 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

		delete imageData;
	}

	_entity = new Entity(model);
	_entity->getModel()->getMeshes()[0]->setTexture(new Texture(_cubeMap));

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void CubeMap::Render(glm::vec3 position, ShaderUniforms* uniformManager, ShaderManager* shaderManager, std::string shader)
{
	GLint oldCullFace;
	GLint oldDepthFunc;
	glGetIntegerv(GL_CULL_FACE, &oldCullFace);
	glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFunc);

	// Change face culling so that the cubemap can be rendered
	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);

	_entity->setTranslation(position);
	uniformManager->updateUniformData("model", *_entity->getCombinedMatrix());
	shaderManager->drawWithShaderProgram(shader, _entity->getModel(), *uniformManager);

	// Restore old face culling and depth functions
	glCullFace(oldCullFace);
	glDepthFunc(oldDepthFunc);
}

CubeMap::~CubeMap()
{
}
