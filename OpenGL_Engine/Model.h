#pragma once
#include <vector>
#include "shaderattributes.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing fla
#include "GLTools.h"

class Model
{
public:
	Model(const aiScene* scene) { _matrix = new glm::mat4(1.0f); gltUploadModel(_meshes, scene); }
	inline glm::mat4* getMatrix() { return _matrix; }
	inline void setMatrix(glm::mat4 matrix) { *_matrix = matrix; }
	inline std::vector<ShaderAttributes*> getMeshes() { return _meshes; }
	~Model();
private:
	std::vector<ShaderAttributes*> _meshes;
	glm::mat4* _matrix;
};

