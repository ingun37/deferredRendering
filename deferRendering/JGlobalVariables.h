#pragma once
#include "JLinearAlgebra.h"
class JGlobalVariables
{
public:
	static JVector3 gWorldCameraEyePos;
	static JVector3 gSunlightDir;
	JGlobalVariables(void);
	~JGlobalVariables(void);
};

