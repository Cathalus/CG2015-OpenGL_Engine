#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include "shaderuniforms.h"
#include "shaderattributes.h"
#include "shaderuniforms.h"
#include "Model.h"
#include "Framebuffer.h"

#include <GL/glew.h>
#include <map>
#include <string>
#include <vector>

class ShaderAttributes;
class ShaderUniforms;

class ShaderManager
{
private:
    // friend classes | https://en.wikipedia.org/wiki/Friend_class
    friend class ShaderAttributes;
    friend class ShaderUniforms;

public:
    void setAttributeBindings(std::vector<std::string> bindings);
    GLuint createShaderProgram(std::string programName,
                               std::vector<std::string> shaderList);
    void drawWithShaderProgram(std::string programName,
                               ShaderAttributes& attributes,
                               ShaderUniforms& uniforms);
	void drawWithShaderProgram(std::string programName,
							   Model* model,
							   ShaderUniforms& uniforms);

	inline void setShadowBuffer(Framebuffer* shadowBuffer){ _hasShadowBuffer = true; _shadowBuffer = shadowBuffer; }
private:
    std::string loadShader(const std::string& filename);
    GLuint createShader(GLenum type,
                        const std::string& source);
    GLuint createShader(GLenum type,
                        const std::string& arguments,
                        const std::string& source);
    bool checkError(const char* title);
    bool checkProgram(GLuint programID);
    bool checkShader(GLuint shaderName,
                     const std::string& file);
    bool validateProgram(GLuint programID);

    std::map<std::string, GLuint> _programs;
    std::vector<std::string> _attributeBindings;

    std::string _defaultShaderLocation = "shaders/";

	Framebuffer* _shadowBuffer;
	bool _hasShadowBuffer = false;
};

#endif // SHADERMANAGER_H
