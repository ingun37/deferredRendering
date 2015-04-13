#pragma once
#include "JLinearAlgebra.h"
class JView
{
public:
	JVector3 axisY;
	JVector3 axisX;
	JVector3 axisZ;
	JVector3 origin;

	JVector3 recoordToBase(JVector3 vec);

	JView(void);
	~JView(void);
};

