#ifndef JShape_h
#define JShape_h

#include "JMesh.h"
#include "JLinearAlgebra.h"

int makeSphere( float radius, unsigned int smoothness, JMesh& mesh );
int makePlane(float widthLen, float heightLen, unsigned int widthSeg, unsigned int heightSeg, const JVector3& normal, JMesh& mesh, int isClockwise);
int makeCylinder( float radius, unsigned int smoothness, float length, unsigned int segment, JMesh& mesh );
#endif
