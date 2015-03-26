#include "shadermanager.h"

#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <cstdlib>
#include <iostream>

inline std::string ShaderManager::loadShader(const std::string& fileName)
{
    std::string result;

    std::ifstream stream(fileName.c_str());
    if(!stream.is_open())           // if stream isn't opened -> return null string
        return result;
    result.reserve(stream.tellg()); // reserve enough space for the program
    stream.seekg(0, std::ios::beg); // seek to beginning

    // fill result
    result.assign((std::istreambuf_iterator<char>(stream)),
                   std::istreambuf_iterator<char>());
    return result;
}

inline bool ShaderManager::validateProgram(GLuint programID)
{
    if(!programID)
        return false;

    glValidateProgram(programID);
    GLint result = GL_FALSE;
    // Get validation status
    glGetProgramiv(programID, GL_VALIDATE_STATUS, &result);

    // Error has occured
    if(result == GL_FALSE)
    {
        std::cerr << "Error: Program is invalid! " << std::endl;
        int infoLogLength;
        // Get info log length
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
        if(infoLogLength > 0)
        {
            std::vector<char> buffer(infoLogLength);
            // Get info log | http://docs.gl/gl4/glGetProgramInfoLog
            glGetProgramInfoLog(programID, infoLogLength, NULL, &buffer[0]);
            fprintf(stdout, "%s\n", &buffer[0]);
        }
    }

    return result == GL_TRUE;
}

inline bool ShaderManager::checkProgram(GLuint programID)
{
    if(!programID)
        return false;

    GLint result = GL_FALSE;
    // Get link status
    glGetProgramiv(programID, GL_LINK_STATUS, &result);

    fprintf(stdout, "Linking program...\n");
    int infoLogLength;
    // Get info log length
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);

    if(infoLogLength > 0)
    {

        std::vector<char> buffer(infoLogLength < 1 ? int(1) : infoLogLength);
        // Get info log
        glGetProgramInfoLog(programID, infoLogLength, NULL, &buffer[0]);
        fprintf(stdout, "%s\n", &buffer[0]);
    }

    return result == GL_TRUE;
}

inline bool ShaderManager::checkShader(GLuint shaderName, const std::string &file)
{
    if(!shaderName)
        return false;

    GLint result = GL_FALSE;
    // Get compile status
    glGetShaderiv(shaderName, GL_COMPILE_STATUS, &result);

    fprintf(stdout, "Compiling Shader '%s' ...", file.c_str());
    int infoLogLength;
    // Get info log length
    glGetShaderiv(shaderName, GL_INFO_LOG_LENGTH, &infoLogLength);
    if(infoLogLength > 0)
    {
        std::vector<char> buffer(infoLogLength);
        // Get info log
        glGetShaderInfoLog(shaderName, infoLogLength, NULL, &buffer[0]);
        fprintf(stdout, "%s\n", &buffer[0]);
    }
    return result == GL_TRUE;
}

inline GLuint ShaderManager::createShader(GLenum type, const std::string &arguments, const std::string &source)
{
    GLuint shaderID = 0;

    if(!source.empty())
    {
        // loads the shader source content
        std::string sourceContent = loadShader(source);
        // saves pointer to the shader source
        const char* sourcePointer = sourceContent.c_str();
        // creates shader
        shaderID = glCreateShader(type);
        // defines source for the shader program
        glShaderSource(shaderID, 1, &sourcePointer, NULL);
        // starts shader compilation
        glCompileShader(shaderID);
    }
    return shaderID;
}

inline GLuint ShaderManager::createShader(GLenum type, const std::string &source)
{
    return createShader(type, std::string(), source);
}

void ShaderManager::setAttributeBindings(std::vector<std::string> bindings)
{
    _attributeBindings = bindings;
}

GLuint ShaderManager::createShaderProgram(std::string programName, std::vector<std::string> shaderList)
{
    bool validated = true;

    GLuint programID;
    GLuint shaderID;

    // create program
    programID = glCreateProgram();


    std::size_t found;
    for(auto a : shaderList)
    {
        // found vertex shader
        found = a.find(".vert");
        if(found != std::string::npos)
        {
            // create vertex shader
            shaderID = createShader(GL_VERTEX_SHADER, (_defaultShaderLocation + a).c_str());
            validated = validated && checkShader(shaderID, (_defaultShaderLocation + a).c_str());
            // attach shader
            glAttachShader(programID, shaderID);
            // delete shader
            glDeleteShader(shaderID);
        }
        // found fragment shader
        found = a.find(".frag");
        if(found != std::string::npos)
        {
            // create fragment shader
            shaderID = createShader(GL_FRAGMENT_SHADER, (_defaultShaderLocation + a).c_str());
            validated = validated && checkShader(shaderID, (_defaultShaderLocation + a).c_str());
            // attach shader
            glAttachShader(programID, shaderID);
            // delete shader
            glDeleteShader(shaderID);
        }
    }

    // bind attrib locations
    for(int i = 0; i < _attributeBindings.size(); i++)
    {
        glBindAttribLocation(programID, i, _attributeBindings[i].c_str());
    }

    // link the program
    glLinkProgram(programID);
    validated = validated && checkProgram(programID);

    if(validated)
    {
        _programs[programName] = programID;
    }

    return programID;
}

void ShaderManager::drawWithShaderProgram(std::string programName, ShaderAttributes &attributes, ShaderUniforms &uniforms)
{
    // set the shader program that is going to be used
    glUseProgram(_programs[programName]);
    // bind the attribute buffers
    attributes.bindBuffers();
    // upload uniform data
    uniforms.uploadUniformData(_programs[programName]);
	// check if texture is available and bind that texture
	if (attributes.hasTexture())
		attributes.getTexture()->Bind(0);
    // draw
	if (attributes.getEBOCount() > 0)
	{		
		glDrawElements(attributes.getDrawMethod(), attributes.getEBOCount(), GL_UNSIGNED_INT, 0);
	}
	else{
		glDrawArrays(attributes.getDrawMethod(), 0, attributes.getVBOCount());
	}
	glBindVertexArray(0);
}

void ShaderManager::drawWithShaderProgram(std::string programName, Model* model, ShaderUniforms &uniforms)
{
	// set the shader program that is going to be used
	glUseProgram(_programs[programName]);

	for (ShaderAttributes* attrib : model->getMeshes())
	{
		// bind attribute buffer
		attrib->bindBuffers();
		// check for texture and bind it
		if (attrib->hasTexture())
		{
			attrib->getTexture()->Bind(0);
			uniforms.updateUniformData("textureDiffuse", 0);
		}
		if (attrib->_hasNormalTexture)
		{
			attrib->getNormalTexture()->Bind(10);
			uniforms.updateUniformData("textureNormal", 10);
		}
		// send material data
		uniforms.updateUniformData("material.ambient", attrib->Material.ambient);
		uniforms.updateUniformData("material.diffuse", attrib->Material.diffuse);
		uniforms.updateUniformData("material.specular", attrib->Material.specular);
		uniforms.updateUniformData("material.shininess", attrib->Material.shininess);
		uniforms.updateUniformData("material.shininessStrength", attrib->Material.shininessStrength);

		// upload uniform data
		uniforms.uploadUniformData(_programs[programName]);
		// draw
		if (attrib->getEBOCount() > 0)
		{
			glDrawElements(attrib->getDrawMethod(), attrib->getEBOCount(), GL_UNSIGNED_INT, 0);
		}
		else{
			glDrawArrays(attrib->getDrawMethod(), 0, attrib->getVBOCount());
		}
		glBindVertexArray(0);
		// Unbind Textures
		/*glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE0 + 10);
		glBindTexture(GL_TEXTURE_2D, 0);*/
	}
}

