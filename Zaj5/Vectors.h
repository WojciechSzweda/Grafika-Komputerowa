#pragma once
#include <GL/glut.h>
#include <math.h>

typedef GLfloat GLTVector3[3];


void gltGetNormalVector(const GLTVector3 vP1, const GLTVector3 vP2, const GLTVector3 vP3, GLTVector3 vNormal);
void gltSubtractVectors(const GLTVector3 vFirst, const GLTVector3 vSecond, GLTVector3 vResult);
void gltVectorCrossProduct(const GLTVector3 vU, const GLTVector3 vV, GLTVector3 vResult);
void gltGetNormalVector(const GLTVector3 vP1, const GLTVector3 vP2, const GLTVector3 vP3, GLTVector3 vNormal);
void gltNormalizeVector(GLTVector3 vNormal);
GLfloat gltGetVectorLength(const GLTVector3 vVector);
GLfloat gltGetVectorLengthSqrd(const GLTVector3 vVector);
void gltScaleVector(GLTVector3 vVector, const GLfloat fScale);