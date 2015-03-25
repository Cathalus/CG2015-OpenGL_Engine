#include "shaderuniforms.h"
#include <iostream>

void ShaderUniforms::uploadUniformData(unsigned int programID)
{
    for (auto e : _mat4) {
      glUniformMatrix4fv(glGetUniformLocation(programID, e.first.c_str()), 1,
                         GL_FALSE, glm::value_ptr(e.second));
    }

    for (auto e : _mat3) {
      glUniformMatrix3fv(glGetUniformLocation(programID, e.first.c_str()), 1,
                         GL_FALSE, glm::value_ptr(e.second));
    }

   for(auto e : _vec4){
      glUniform4fv(glGetUniformLocation(programID, e.first.c_str()), 1,
                   glm::value_ptr(e.second));
   }

   for(auto e : _vec3){
      glUniform3fv(glGetUniformLocation(programID, e.first.c_str()), 1,
                   glm::value_ptr(e.second));
   }

   for(auto e : _vec2){
      glUniform2fv(glGetUniformLocation(programID, e.first.c_str()), 1,
                   glm::value_ptr(e.second));
   }

   for(auto e : _float){
      glUniform1f(glGetUniformLocation(programID, e.first.c_str()), e.second);
   }

   for(auto e : _uint){
      glUniform1ui(glGetUniformLocation(programID, e.first.c_str()), e.second);
   }

   for (auto e : _int)
   {
	   glUniform1i(glGetUniformLocation(programID, e.first.c_str()), e.second);
   }
}

void ShaderUniforms::updateUniformData(std::string uniformName, glm::mat4& mat)
{
    _mat4[uniformName] = mat;
}

void ShaderUniforms::updateUniformData(std::string uniformName, glm::mat3& mat)
{
    _mat3[uniformName] = mat;
}

void ShaderUniforms::updateUniformData(std::string uniformName, glm::vec4& mat)
{
    _vec4[uniformName] = mat;
}

void ShaderUniforms::updateUniformData(std::string uniformName, glm::vec3& mat)
{
    _vec3[uniformName] = mat;
}

void ShaderUniforms::updateUniformData(std::string uniformName, glm::vec2& mat)
{
    _vec2[uniformName] = mat;
}

void ShaderUniforms::updateUniformData(std::string uniformName, float f)
{
    _float[uniformName] = f;
}

void ShaderUniforms::updateUniformData(std::string uniformName, unsigned int ui)
{
    _uint[uniformName] = ui;
}

void ShaderUniforms::updateUniformData(std::string uniformName, int i)
{
	_int[uniformName] = i;
}