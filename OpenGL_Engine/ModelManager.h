#pragma once
#include <map>
#include <GL\glew.h>
#include <string>
#include <iostream>
#include "Model.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing fla

class ModelManager
{
public:
	void loadModel(std::string name, std::string fileName, float scale = 1, glm::vec3 translation = glm::vec3(0), glm::vec3 rotation = glm::vec3(1), float angle = 0)
	{
		assert(_textureManager != 0);
		if (_models.find(name) == _models.end())
		{
			std::string fName = "models/";
			fName.append(fileName);

			std::cout << "Loading model \"" << fName << "\" " << std::endl;
			std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;

			Assimp::Importer* importer = new Assimp::Importer();
			const aiScene* scene = importer->ReadFile(fName, 
				aiProcess_Triangulate |
				aiProcess_GenSmoothNormals |
				//aiProcess_OptimizeMeshes |
				//aiProcess_OptimizeGraph |
				//aiProcess_JoinIdenticalVertices |
				aiProcess_ImproveCacheLocality |
				//aiProcess_SortByPType | 
				aiProcess_CalcTangentSpace);

			Model* temp = new Model(scene, _textureManager);
			temp->setMatrix(glm::scale(*temp->getMatrix(), glm::vec3(scale)));
			if (translation != glm::vec3(0))
				temp->setMatrix(glm::translate(*temp->getMatrix(), translation));
			if (angle != 0)
				temp->setMatrix(glm::rotate(*temp->getMatrix(), glm::radians(angle), rotation));
			_models[name] = temp;

			std::cout << "Model loading complete" << std::endl;
		}
	}
	void unloadModel(std::string name)
	{
		if (_models.find(name) != _models.end())
			_models.erase(name);
	}
	Model* getModel(std::string name)
	{
		if (_models.find(name) == _models.end())
		{
			return NULL;
		}
		else{
			return _models[name];
		}
	}
	inline void setTextureManager(TextureManager* textureManager) { _textureManager = textureManager; }
private:
	std::map<std::string, Model*> _models;
	TextureManager* _textureManager;
};