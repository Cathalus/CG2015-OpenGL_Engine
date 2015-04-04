#include "GLTools.h"
#include "shaderattributes.h"
#include <math.h> /* fabs */
#include <glm/glm.hpp>
#include <vector>
#include <iostream>



void gltMakeTorus(ShaderAttributes &pA, GLfloat majorRadius,
                  GLfloat minorRadius, GLint numMajor, GLint numMinor) {

  float PI = 3.141592;

  std::vector<glm::vec3> positions;
  std::vector<glm::vec3> normals;
  glm::vec3 tempPosition;
  glm::vec3 tempNormal;

  double majorStep = 2.0f * PI / numMajor;
  double minorStep = 2.0f * PI / numMinor;
  int i, j;
  for (i = 0; i < numMajor; ++i) {
    double a0 = i * majorStep;
    double a1 = a0 + majorStep;
    GLfloat x0 = (GLfloat)cos(a0);
    GLfloat y0 = (GLfloat)sin(a0);
    GLfloat x1 = (GLfloat)cos(a1);
    GLfloat y1 = (GLfloat)sin(a1);

    for (j = 0; j <= numMinor; ++j) {
      double b = j * minorStep;
      GLfloat c = (GLfloat)cos(b);
      GLfloat r = minorRadius * c + majorRadius;
      GLfloat z = minorRadius * (GLfloat)sin(b);

      // First point
      tempNormal.x = x0 * c;
      tempNormal.y = y0 * c;
      tempNormal.z = z / minorRadius;
      glm::normalize(tempNormal);

      tempPosition.x = x0 * r;
      tempPosition.y = y0 * r;
      tempPosition.z = z;

      positions.push_back(tempPosition);
      normals.push_back(tempNormal);

      // Second point
      tempNormal.x = x1 * c;
      tempNormal.y = y1 * c;
      tempNormal.z = z / minorRadius;
      glm::normalize(tempNormal);

      tempPosition.x = x1 * r;
      tempPosition.y = y1 * r;
      tempPosition.z = z;

      positions.push_back(tempPosition);
      normals.push_back(tempNormal);

      // Next one over
      b = (j + 1) * minorStep;
      c = (GLfloat)cos(b);
      r = minorRadius * c + majorRadius;
      z = minorRadius * (GLfloat)sin(b);

      // Third (based on first)
      tempNormal.x = x0 * c;
      tempNormal.y = y0 * c;
      tempNormal.z = z / minorRadius;
      glm::normalize(tempNormal);

      tempPosition.x = x0 * r;
      tempPosition.y = y0 * r;
      tempPosition.z = z;

      positions.push_back(tempPosition);
      normals.push_back(tempNormal);

      // Fourth (based on second)
      tempNormal.x = x1 * c;
      tempNormal.y = y1 * c;
      tempNormal.z = z / minorRadius;
      glm::normalize(tempNormal);

      tempPosition.x = x1 * r;
      tempPosition.y = y1 * r;
      tempPosition.z = z;

      positions.push_back(tempPosition);
      normals.push_back(tempNormal);
    }
  }
  pA.uploadBufferForIndex(0, positions);
  pA.uploadBufferForIndex(1, normals);
  pA.setDrawMethod(GL_TRIANGLE_STRIP);
}

void gltMakeCylinder(ShaderAttributes &pA, GLfloat baseRadius,
                     GLfloat topRadius, GLfloat fLength, GLint numSlices,
                     GLint numStacks) {

  std::vector<glm::vec3> positions;
  std::vector<glm::vec3> normals;
  glm::vec3 tempPosition;
  glm::vec3 tempNormal;

  float fRadiusStep = (topRadius - baseRadius) / float(numStacks);

  GLfloat fStepSizeSlice = (3.1415926536f * 2.0f) / float(numSlices);

  GLfloat ds = 1.0f / float(numSlices);
  GLfloat dt = 1.0f / float(numStacks);
  GLfloat s;
  GLfloat t;

  for (int i = 0; i < numStacks; i++) {
    if (i == 0)
      t = 0.0f;
    else
      t = float(i) * dt;

    float tNext;
    if (i == (numStacks - 1))
      tNext = 1.0f;
    else
      tNext = float(i + 1) * dt;

    float fCurrentRadius = baseRadius + (fRadiusStep * float(i));
    float fNextRadius = baseRadius + (fRadiusStep * float(i + 1));
    float theyta;
    float theytaNext;

    float fCurrentZ = float(i) * (fLength / float(numStacks));
    float fNextZ = float(i + 1) * (fLength / float(numStacks));

    float zNormal = 0.0f;

    zNormal = (baseRadius - topRadius);

    for (int j = 0; j < numSlices; j++) {
      if (j == 0)
        s = 0.0f;
      else
        s = float(j) * ds;

      float sNext;
      if (j == (numSlices - 1))
        sNext = 1.0f;
      else
        sNext = float(j + 1) * ds;

      theyta = fStepSizeSlice * float(j);
      if (j == (numSlices - 1))
        theytaNext = 0.0f;
      else
        theytaNext = fStepSizeSlice * (float(j + 1));

      // Inner First
      tempPosition.x = cos(theyta) * fCurrentRadius;
      tempPosition.y = sin(theyta) * fCurrentRadius;
      tempPosition.z = fCurrentZ;

      tempNormal.x = tempPosition.x;
      tempNormal.y = tempPosition.y;
      tempNormal.z = zNormal;
      glm::normalize(tempNormal);

      positions.push_back(tempPosition);
      normals.push_back(tempNormal);

      // Outer First
      tempPosition.x = cos(theyta) * fNextRadius;
      tempPosition.y = sin(theyta) * fNextRadius;
      tempPosition.z = fNextZ;

      tempNormal.x = tempPosition.x;
      tempNormal.y = tempPosition.y;
      tempNormal.z = zNormal;
      glm::normalize(tempNormal);

      positions.push_back(tempPosition);
      normals.push_back(tempNormal);

      // Inner second
      tempPosition.x = cos(theytaNext) * fCurrentRadius;
      tempPosition.y = sin(theytaNext) * fCurrentRadius;
      tempPosition.z = fCurrentZ;

      tempNormal.x = tempPosition.x;
      tempNormal.y = tempPosition.y;
      tempNormal.z = zNormal;
      glm::normalize(tempNormal);

      positions.push_back(tempPosition);
      normals.push_back(tempNormal);

      // Outer second
      tempPosition.x = cos(theytaNext) * fNextRadius;
      tempPosition.y = sin(theytaNext) * fNextRadius;
      tempPosition.z = fNextZ;

      tempNormal.x = tempPosition.x;
      tempNormal.y = tempPosition.y;
      tempNormal.z = zNormal;
      glm::normalize(tempNormal);

      positions.push_back(tempPosition);
      normals.push_back(tempNormal);
    }
  }
  pA.uploadBufferForIndex(0, positions);
  pA.uploadBufferForIndex(1, normals);
  pA.setDrawMethod(GL_TRIANGLE_STRIP);
}

void gltMakeDisk(ShaderAttributes &pA, GLfloat innerRadius,
                 GLfloat outerRadius, GLint nSlices, GLint nStacks) {

  std::vector<glm::vec3> positions;
  std::vector<glm::vec3> normals;
  glm::vec3 tempPosition;
  glm::vec3 tempNormal;

  // How much to step out each stack
  GLfloat fStepSizeRadial = outerRadius - innerRadius;
  if (fStepSizeRadial < 0.0f) // Dum dum...
    fStepSizeRadial *= -1.0f;

  fStepSizeRadial /= float(nStacks);

  GLfloat fStepSizeSlice = (3.1415926536f * 2.0f) / float(nSlices);

  float fRadialScale = 1.0f / outerRadius;

  for (GLint i = 0; i < nStacks; i++) // Stacks
  {
    float theyta;
    float theytaNext;
    for (GLint j = 0; j < nSlices; j++) // Slices
    {
      float inner = innerRadius + (float(i)) * fStepSizeRadial;
      float outer = innerRadius + (float(i + 1)) * fStepSizeRadial;

      theyta = fStepSizeSlice * float(j);
      if (j == (nSlices - 1))
        theytaNext = 0.0f;
      else
        theytaNext = fStepSizeSlice * (float(j + 1));

      // Inner First
      tempPosition.x = cos(theyta) * inner;
      tempPosition.y = sin(theyta) * inner;
      tempPosition.z = 0.0;

      tempNormal.x = 0.0;
      tempNormal.y = 0.0;
      tempNormal.z = 1.0;

      positions.push_back(tempPosition);
      normals.push_back(tempNormal);

      // Outer First
      tempPosition.x = cos(theyta) * outer;
      tempPosition.y = sin(theyta) * outer;
      tempPosition.z = 0.0;

      tempNormal.x = 0.0;
      tempNormal.y = 0.0;
      tempNormal.z = 1.0;

      positions.push_back(tempPosition);
      normals.push_back(tempNormal);

      // Inner Second
      tempPosition.x = cos(theytaNext) * inner;
      tempPosition.y = sin(theytaNext) * inner;
      tempPosition.z = 0.0;

      tempNormal.x = 0.0;
      tempNormal.y = 0.0;
      tempNormal.z = 1.0;

      positions.push_back(tempPosition);
      normals.push_back(tempNormal);

      // Outer First
      tempPosition.x = cos(theytaNext) * outer;
      tempPosition.y = sin(theytaNext) * outer;
      tempPosition.z = 0.0;

      tempNormal.x = 0.0;
      tempNormal.y = 0.0;
      tempNormal.z = 1.0;

      positions.push_back(tempPosition);
      normals.push_back(tempNormal);
    }
  }
  pA.uploadBufferForIndex(0, positions);
  pA.uploadBufferForIndex(1, normals);
  pA.setDrawMethod(GL_TRIANGLE_STRIP);
}

void gltMakeSphere(ShaderAttributes &pA, GLfloat fRadius, GLint iSlices,
                   GLint iStacks) {

  std::vector<glm::vec3> positions;
  std::vector<glm::vec3> normals;
  glm::vec3 tempPosition;
  glm::vec3 tempNormal;

  GLfloat drho = (GLfloat)(3.141592653589) / (GLfloat)iStacks;
  GLfloat dtheta = 2.0f * (GLfloat)(3.141592653589) / (GLfloat)iSlices;
  GLfloat ds = 1.0f / (GLfloat)iSlices;
  GLfloat dt = 1.0f / (GLfloat)iStacks;
  GLfloat t = 1.0f;
  GLfloat s = 0.0f;
  GLint i, j; // Looping variables

  for (i = 0; i < iStacks; i++) {
    GLfloat rho = (GLfloat)i * drho;
    GLfloat srho = (GLfloat)(sin(rho));
    GLfloat crho = (GLfloat)(cos(rho));
    GLfloat srhodrho = (GLfloat)(sin(rho + drho));
    GLfloat crhodrho = (GLfloat)(cos(rho + drho));

    // Many sources of OpenGL sphere drawing code uses a triangle fan
    // for the caps of the sphere. This however introduces texturing
    // artifacts at the poles on some OpenGL implementations
    s = 0.0f;

    for (j = 0; j < iSlices; j++) {
      GLfloat theta = (j == iSlices) ? 0.0f : j * dtheta;
      GLfloat stheta = (GLfloat)(-sin(theta));
      GLfloat ctheta = (GLfloat)(cos(theta));

      GLfloat x = stheta * srho;
      GLfloat y = ctheta * srho;
      GLfloat z = crho;

      tempNormal.x = x;
      tempNormal.y = y;
      tempNormal.z = z;

      tempPosition.x = x * fRadius;
      tempPosition.y = y * fRadius;
      tempPosition.z = z * fRadius;

      positions.push_back(tempPosition);
      normals.push_back(tempNormal);

      x = stheta * srhodrho;
      y = ctheta * srhodrho;
      z = crhodrho;

      // vTexture.x = s;
      // vTexture.y = t - dt;
      tempNormal.x = x;
      tempNormal.y = y;
      tempNormal.z = z;

      tempPosition.x = x * fRadius;
      tempPosition.y = y * fRadius;
      tempPosition.z = z * fRadius;

      positions.push_back(tempPosition);
      normals.push_back(tempNormal);

      theta = ((j + 1) == iSlices) ? 0.0f : (j + 1) * dtheta;
      stheta = (GLfloat)(-sin(theta));
      ctheta = (GLfloat)(cos(theta));

      x = stheta * srho;
      y = ctheta * srho;
      z = crho;

      s += ds;
      // vTexture.x = s;
      // vTexture.y = t;
      tempNormal.x = x;
      tempNormal.y = y;
      tempNormal.z = z;

      tempPosition.x = x * fRadius;
      tempPosition.y = y * fRadius;
      tempPosition.z = z * fRadius;

      positions.push_back(tempPosition);
      normals.push_back(tempNormal);

      x = stheta * srhodrho;
      y = ctheta * srhodrho;
      z = crhodrho;

      tempNormal.x = x;
      tempNormal.y = y;
      tempNormal.z = z;

      tempPosition.x = x * fRadius;
      tempPosition.y = y * fRadius;
      tempPosition.z = z * fRadius;

      positions.push_back(tempPosition);
      normals.push_back(tempNormal);
    }
    t -= dt;
  }
  pA.uploadBufferForIndex(0, positions);
  pA.uploadBufferForIndex(1, normals);
  pA.setDrawMethod(GL_TRIANGLE_STRIP);
}

void gltMakeCube(ShaderAttributes &pA, GLfloat fRadius) {

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;

    /////////////////////////////////////////////
  // Top of cube
  normals.push_back(glm::vec3(0.0f, fRadius, 0.0f));
  positions.push_back(glm::vec3(fRadius, fRadius, fRadius));
  texCoords.push_back(glm::vec2(1, 1));

  normals.push_back(glm::vec3(0.0f, fRadius, 0.0f));
  positions.push_back(glm::vec3(fRadius, fRadius, -fRadius));
  texCoords.push_back(glm::vec2(1, 0));

  normals.push_back(glm::vec3(0.0f, fRadius, 0.0f));
  positions.push_back(glm::vec3(-fRadius, fRadius, -fRadius));
  texCoords.push_back(glm::vec2(0,0));

  normals.push_back(glm::vec3(0.0f, fRadius, 0.0f));
  positions.push_back(glm::vec3(fRadius, fRadius, fRadius));
  texCoords.push_back(glm::vec2(1, 1));

  normals.push_back(glm::vec3(0.0f, fRadius, 0.0f));
  positions.push_back(glm::vec3(-fRadius, fRadius, -fRadius));
  texCoords.push_back(glm::vec2(0,0));

  normals.push_back(glm::vec3(0.0f, fRadius, 0.0f));
  positions.push_back(glm::vec3(-fRadius, fRadius, fRadius));
  texCoords.push_back(glm::vec2(0, 1));

  ////////////////////////////////////////////
  // Bottom of cube
  normals.push_back(glm::vec3(0.0f, -fRadius, 0.0f));
  positions.push_back(glm::vec3(-fRadius, -fRadius, -fRadius));
  texCoords.push_back(glm::vec2(0, 0));

  normals.push_back(glm::vec3(0.0f, -fRadius, 0.0f));
  positions.push_back(glm::vec3(fRadius, -fRadius, -fRadius));
  texCoords.push_back(glm::vec2(1, 0));

  normals.push_back(glm::vec3(0.0f, -fRadius, 0.0f));
  positions.push_back(glm::vec3(fRadius, -fRadius, fRadius));
  texCoords.push_back(glm::vec2(1, 1));

  normals.push_back(glm::vec3(0.0f, -fRadius, 0.0f));
  positions.push_back(glm::vec3(-fRadius, -fRadius, fRadius));
  texCoords.push_back(glm::vec2(0, 1));

  normals.push_back(glm::vec3(0.0f, -fRadius, 0.0f));
  positions.push_back(glm::vec3(-fRadius, -fRadius, -fRadius));
  texCoords.push_back(glm::vec2(0, 0));

  normals.push_back(glm::vec3(0.0f, -fRadius, 0.0f));
  positions.push_back(glm::vec3(fRadius, -fRadius, fRadius));
  texCoords.push_back(glm::vec2(1, 1));

  ///////////////////////////////////////////
  // Left side of cube
  normals.push_back(glm::vec3(-fRadius, 0.0f, 0.0f));
  positions.push_back(glm::vec3(-fRadius, fRadius, fRadius));
  texCoords.push_back(glm::vec2(0, 1));

  normals.push_back(glm::vec3(-fRadius, 0.0f, 0.0f));
  positions.push_back(glm::vec3(-fRadius, fRadius, -fRadius));
  texCoords.push_back(glm::vec2(1, 1));

  normals.push_back(glm::vec3(-fRadius, 0.0f, 0.0f));
  positions.push_back(glm::vec3(-fRadius, -fRadius, -fRadius));
  texCoords.push_back(glm::vec2(1, 0));

  normals.push_back(glm::vec3(-fRadius, 0.0f, 0.0f));
  positions.push_back(glm::vec3(-fRadius, fRadius, fRadius));
  texCoords.push_back(glm::vec2(0, 1));

  normals.push_back(glm::vec3(-fRadius, 0.0f, 0.0f));
  positions.push_back(glm::vec3(-fRadius, -fRadius, -fRadius));
  texCoords.push_back(glm::vec2(1, 0));

  normals.push_back(glm::vec3(-fRadius, 0.0f, 0.0f));
  positions.push_back(glm::vec3(-fRadius, -fRadius, fRadius));
  texCoords.push_back(glm::vec2(0, 0));

  // Right side of cube
  normals.push_back(glm::vec3(fRadius, 0.0f, 0.0f));
  positions.push_back(glm::vec3(fRadius, -fRadius, -fRadius));
  texCoords.push_back(glm::vec2(1, 1));

  normals.push_back(glm::vec3(fRadius, 0.0f, 0.0f));
  positions.push_back(glm::vec3(fRadius, fRadius, -fRadius));
  texCoords.push_back(glm::vec2(1, 1));

  normals.push_back(glm::vec3(fRadius, 0.0f, 0.0f));
  positions.push_back(glm::vec3(fRadius, fRadius, fRadius));
  texCoords.push_back(glm::vec2(1, 1));

  normals.push_back(glm::vec3(fRadius, 0.0f, 0.0f));
  positions.push_back(glm::vec3(fRadius, fRadius, fRadius));
  texCoords.push_back(glm::vec2(1, 1));

  normals.push_back(glm::vec3(fRadius, 0.0f, 0.0f));
  positions.push_back(glm::vec3(fRadius, -fRadius, fRadius));
  texCoords.push_back(glm::vec2(1, 1));

  normals.push_back(glm::vec3(fRadius, 0.0f, 0.0f));
  positions.push_back(glm::vec3(fRadius, -fRadius, -fRadius));
  texCoords.push_back(glm::vec2(1, 1));

  // Front and Back
  // Front
  normals.push_back(glm::vec3(0.0f, 0.0f, fRadius));
  positions.push_back(glm::vec3(fRadius, -fRadius, fRadius));
  texCoords.push_back(glm::vec2(1, 1));

  normals.push_back(glm::vec3(0.0f, 0.0f, fRadius));
  positions.push_back(glm::vec3(fRadius, fRadius, fRadius));
  texCoords.push_back(glm::vec2(1, 1));

  normals.push_back(glm::vec3(0.0f, 0.0f, fRadius));
  positions.push_back(glm::vec3(-fRadius, fRadius, fRadius));
  texCoords.push_back(glm::vec2(1, 1));

  normals.push_back(glm::vec3(0.0f, 0.0f, fRadius));
  positions.push_back(glm::vec3(-fRadius, fRadius, fRadius));
  texCoords.push_back(glm::vec2(1, 1));

  normals.push_back(glm::vec3(0.0f, 0.0f, fRadius));
  positions.push_back(glm::vec3(-fRadius, -fRadius, fRadius));
  texCoords.push_back(glm::vec2(1, 1));

  normals.push_back(glm::vec3(0.0f, 0.0f, fRadius));
  positions.push_back(glm::vec3(fRadius, -fRadius, fRadius));
  texCoords.push_back(glm::vec2(1, 1));

  // Back
  normals.push_back(glm::vec3(0.0f, 0.0f, -fRadius));
  positions.push_back(glm::vec3(fRadius, -fRadius, -fRadius));
  texCoords.push_back(glm::vec2(1, 1));

  normals.push_back(glm::vec3(0.0f, 0.0f, -fRadius));
  positions.push_back(glm::vec3(-fRadius, -fRadius, -fRadius));
  texCoords.push_back(glm::vec2(1, 1));

  normals.push_back(glm::vec3(0.0f, 0.0f, -fRadius));
  positions.push_back(glm::vec3(-fRadius, fRadius, -fRadius));
  texCoords.push_back(glm::vec2(1, 1));

  normals.push_back(glm::vec3(0.0f, 0.0f, -fRadius));
  positions.push_back(glm::vec3(-fRadius, fRadius, -fRadius));
  texCoords.push_back(glm::vec2(1, 1));

  normals.push_back(glm::vec3(0.0f, 0.0f, -fRadius));
  positions.push_back(glm::vec3(fRadius, fRadius, -fRadius));
  texCoords.push_back(glm::vec2(1, 1));

  normals.push_back(glm::vec3(0.0f, 0.0f, -fRadius));
  positions.push_back(glm::vec3(fRadius, -fRadius, -fRadius));
  texCoords.push_back(glm::vec2(1, 1));


  pA.uploadBufferForIndex(0, positions);
  pA.uploadBufferForIndex(1, normals);
  pA.uploadBufferForIndex(2, texCoords);
  pA.setDrawMethod(GL_TRIANGLES);
}

void gltMakeTriangle(ShaderAttributes &pA, glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;

	positions.push_back(a);
	positions.push_back(b);
	positions.push_back(c);

	texCoords.push_back(glm::vec2(0, 0));
	texCoords.push_back(glm::vec2(1, 0));
	texCoords.push_back(glm::vec2(0.5f, 1));

	pA.uploadBufferForIndex(0, positions);
	pA.uploadBufferForIndex(2, texCoords);

}

void gltMakeTestObject(ShaderAttributes &pA)
{
	std::vector<glm::vec3> positions = 
	{
		glm::vec3(0.5f, 0.5f, 0.0f),  // Top Right
		glm::vec3(0.5f, -0.5f, 0.0f),  // Bottom Right
		glm::vec3(-0.5f, -0.5f, 0.0f),  // Bottom Left
		glm::vec3(-0.5f, 0.5f, 0.0f)   // Top Left 
	};

	std::vector<glm::vec2> texCoords =
	{
		glm::vec2(1,1),
		glm::vec2(1, 0),
		glm::vec2(0, 0),
		glm::vec2(0, 1)
	};

	std::vector<glm::vec4> colors =
	{
		glm::vec4(0, 0, 1, 1),
		glm::vec4(0, 1, 0, 1),
		glm::vec4(1, 0, 0, 1),
		glm::vec4(1, 1, 1, 1)
	};

	std::vector<GLuint> indices = { 
		0,1,3,
		1,2,3
	};

	pA.uploadBufferForIndex(0, positions);
	pA.uploadEBOForIndex(0, indices);
	pA.uploadBufferForIndex(1, colors);
	pA.uploadBufferForIndex(2, texCoords);

	pA.setDrawMethod(GL_TRIANGLES);
}

void gltMakePlane(ShaderAttributes &pA)
{
	std::vector<glm::vec3> positions =
	{
		glm::vec3(10,  0, 10),  // Top Right
		glm::vec3(10,  0, -10),  // Bottom Right
		glm::vec3(-10, 0, -10),  // Bottom Left
		glm::vec3(-10, 0, 10)   // Top Left 
	};

	std::vector<glm::vec2> texCoords =
	{
		glm::vec2(1, 1),
		glm::vec2(1, 0),
		glm::vec2(0, 0),
		glm::vec2(0, 1)
	};

	std::vector<glm::vec4> colors =
	{
		glm::vec4(0, 0, 1, 1),
		glm::vec4(0, 1, 0, 1),
		glm::vec4(1, 0, 0, 1),
		glm::vec4(1, 1, 1, 1)
	};

	std::vector<GLuint> indices = {
		0, 1, 3,
		1, 2, 3
	};

	pA.uploadBufferForIndex(0, positions);
	pA.uploadEBOForIndex(0, indices);
	pA.uploadBufferForIndex(1, colors);
	pA.uploadBufferForIndex(2, texCoords);

	pA.setDrawMethod(GL_TRIANGLES);
}

void gltUploadModel(ShaderAttributes &pA, std::vector<tinyobj::shape_t> &shapes)
{	
	for (tinyobj::shape_t shape : shapes)
	{
		if (!shape.mesh.positions.empty())
			pA.uploadBufferForIndex(0, 3, shape.mesh.positions);
		if (!shape.mesh.normals.empty())
			pA.uploadBufferForIndex(1, 3, shape.mesh.normals);
		if (!shape.mesh.texcoords.empty())
			pA.uploadBufferForIndex(2, 2, shape.mesh.texcoords);
		if (!shape.mesh.indices.empty())
			pA.uploadEBOForIndex(0, shape.mesh.indices);
	}
	/*
	pA.uploadBufferForIndex(0, model.positions);
	pA.uploadBufferForIndex(1, model.normals);
	pA.uploadBufferForIndex(2, model.texCoords);
	pA.uploadEBOForIndex(0, model.indices);
	*/
}

void gltUploadModel(std::vector<ShaderAttributes*> &pA, const aiScene* scene)
{
	float vX, vY, vZ;
	float nX, nY, nZ;
	float iX, iY, iZ;
	for (GLuint i = 0; i < scene->mNumMeshes; i++)
	{
		ShaderAttributes* mesh = new ShaderAttributes();
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> texCoords;
		std::vector<glm::vec3> tangents;
		std::vector<GLuint> indices;

		aiMesh* a = scene->mMeshes[i];
		if (scene->HasMaterials())
		{
			if (a->mMaterialIndex > 0)
			{
				const aiMaterial* material = scene->mMaterials[a->mMaterialIndex];
				/* Get Material Properties*/
				// Diffuse Color
				aiColor3D color(0.f, 0.f, 0.f);
				if(material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
					mesh->Material.diffuse = glm::vec3(color.r, color.g, color.b);
				// Ambient Color
				aiColor3D ambient(0, 0, 0);
				if (material->Get(AI_MATKEY_COLOR_AMBIENT, ambient) == AI_SUCCESS)
					mesh->Material.ambient = glm::vec3(ambient.r, ambient.g, ambient.b);
				// Specular Color
				aiColor3D specular(0, 0, 0);
				if (material->Get(AI_MATKEY_COLOR_AMBIENT, specular) == AI_SUCCESS)
					mesh->Material.specular = glm::max(glm::vec3(specular.r, specular.g, specular.b), glm::vec3(0.1,0.1,0.1));
				// Shininess
				float shininess = 0;
				if (material->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS)
					mesh->Material.shininess = glm::max(shininess,8.0f);
				// Shininess Strength
				float shininessStrength = 0;
				if (material->Get(AI_MATKEY_SHININESS_STRENGTH, shininessStrength) == AI_SUCCESS)
					mesh->Material.shininessStrength = shininessStrength;
					

				std::cout << "Shiniess = " << shininess << " strength = " << shininessStrength << std::endl;
				
				aiString texturePath;
				if(material->GetTextureCount(aiTextureType_DIFFUSE) > 0 &&
					material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS)
				{
					std::cout << "\tDiffuse Texture Found!" << std::endl;
					mesh->setTexture(new Texture(texturePath.data));
				}
				if (material->GetTextureCount(aiTextureType_NORMALS) > 0 &&
					material->GetTexture(aiTextureType_NORMALS, 0, &texturePath) == AI_SUCCESS)
				{
					std::cout << "\tNormal Texture Found!" << std::endl;
					mesh->setNormalTexture(new Texture(texturePath.data));
				}else{
					std::cout << "\tLoading default normal texture..." << std::endl;
					mesh->setNormalTexture(new Texture("default_normal.jpg"));
				}
			}else{
				std::cout << "\tLoading default normal texture..." << std::endl;
				mesh->setNormalTexture(new Texture("default_normal.jpg"));
			}
		}
		else{
			std::cout << "\tLoading default normal texture..." << std::endl;
			mesh->setNormalTexture(new Texture("default_normal.jpg"));
		}


		for (int i = 0; i < a->mNumVertices; i++)
		{
			/* Vertices */
			positions.push_back(glm::vec3(a->mVertices[i].x, a->mVertices[i].y, a->mVertices[i].z));
			
			/* Normals */
			normals.push_back(glm::vec3(a->mNormals[i].x, a->mNormals[i].y, a->mNormals[i].z));

			/* Tangents & Bitangents */
			if (a->HasTangentsAndBitangents())
			{
				tangents.push_back(glm::vec3(a->mTangents[i].x, a->mTangents[i].y, a->mTangents[i].z));
			}

			/* Texcoords */
			if (a->HasTextureCoords(0))
			{
				texCoords.push_back(glm::vec2(a->mTextureCoords[0][i].x, a->mTextureCoords[0][i].y));
			}
		}

		/* Indices */
		for (int i = 0; i < a->mNumFaces; i++)
		{
			indices.push_back(a->mFaces[i].mIndices[0]);
			indices.push_back(a->mFaces[i].mIndices[1]);
			indices.push_back(a->mFaces[i].mIndices[2]);
		}

		if (positions.size() > 0)
			mesh->uploadBufferForIndex(0, positions);	
		if (normals.size() > 0)
			mesh->uploadBufferForIndex(1, normals);
		if (texCoords.size() > 0)
			mesh->uploadBufferForIndex(2, texCoords);
		if (tangents.size() > 0)
			mesh->uploadBufferForIndex(3, tangents);
		if (indices.size() > 0)
			mesh->uploadEBOForIndex(0, indices);
		pA.push_back(mesh);

		positions.clear();
		normals.clear();
		texCoords.clear();
		tangents.clear();
		indices.clear();
	}
}