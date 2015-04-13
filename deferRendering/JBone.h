#pragma once
#include "JLinearAlgebra.h"
class JBone
{
public:
	JVector3 pos;

	JVector3 axis;
	float angle;
	unsigned int upperBoneIdx;

	//TODO : avoid copy
	JMatrix44 getTransform();

	JBone(void);
	~JBone(void);
};

