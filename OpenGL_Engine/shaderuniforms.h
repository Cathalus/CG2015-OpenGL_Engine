#ifndef SHADERUNIFORMS_H
#define SHADERUNIFORMS_H

#define GLM_FORCE_RADIANS
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <map>

class ShaderManager;

class ShaderUniforms
{
private:
    friend class ShaderManager;

public:
    void updateUniformData(std::string uniformName, glm::mat4& mat);
    void updateUniformData(std::string uniformName, glm::mat3& mat);
    void updateUniformData(std::string uniformName, glm::vec4& mat);
    void updateUniformData(std::string uniformName, glm::vec3& mat);
    void updateUniformData(std::string uniformName, glm::vec2& mat);
    void updateUniformData(std::string uniformName, float f);
    void updateUniformData(std::string uniformName, unsigned int i);
	void updateUniformData(std::string uniformName, int i);

private:
    void uploadUniformData(unsigned int programID);
    std::map<std::string, glm::mat4> _mat4;
    std::map<std::string, glm::mat3> _mat3;
    std::map<std::string, glm::vec4> _vec4;
    std::map<std::string, glm::vec3> _vec3;
    std::map<std::string, glm::vec2> _vec2;
    std::map<std::string, float> _float;
    std::map<std::string, unsigned int> _uint;
	std::map<std::string, int> _int;
};

#endif // SHADERUNIFORMS_H
