// GLTools.h
// OpenGL SuperBible
/* Copyright 1998 - 2003 Richard S. Wright Jr.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list
of conditions and the following disclaimer.

Redistributions in binary form must reproduce the above copyright notice, this
list
of conditions and the following disclaimer in the documentation and/or other
materials provided with the distribution.

Neither the name of Richard S. Wright Jr. nor the names of other contributors
may be used
to endorse or promote products derived from this software without specific prior
written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
EVENT
SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED
TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
OR
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
IN
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.
*/

//Adapted for Simple OpenGL Framework
#ifndef GLTOOLS
#include <GL/glew.h>
#include <GLM\glm.hpp>
#include "obj_loader.h"
#include "tiny_obj_loader.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing fla
#include <ASSIMP\material.h>
#include <ASSIMP\cfileio.h>
#include "TextureManager.h"
#define GLTOOLS

class ShaderAttributes;

// Make Objects
void gltMakeTorus(ShaderAttributes &pA, GLfloat majorRadius,
                  GLfloat minorRadius, GLint numMajor, GLint numMinor);
void gltMakeSphere(ShaderAttributes &pA, GLfloat fRadius, GLint iSlices,
                   GLint iStacks);
void gltMakeDisk(ShaderAttributes &pA, GLfloat innerRadius,
                 GLfloat outerRadius, GLint nSlices, GLint nStacks);
void gltMakeCylinder(ShaderAttributes &pA, GLfloat baseRadius,
                     GLfloat topRadius, GLfloat fLength, GLint numSlices,
                     GLint numStacks);
void gltMakeCube(ShaderAttributes &pA, GLfloat fRadius);
void gltMakeTriangle(ShaderAttributes &pA, glm::vec3 a, glm::vec3 b, glm::vec3 c);
void gltMakeTestObject(ShaderAttributes &pA);
void gltUploadModel(ShaderAttributes &pA, std::vector<tinyobj::shape_t> &shape);
void gltUploadModel(std::vector<ShaderAttributes*> &pA, const aiScene* scene, TextureManager* textureManager);
void gltMakePlane(ShaderAttributes &pA);
#endif
