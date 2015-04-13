#include "JView.h"


JView::JView(void)
{
	axisX[0] = 1; axisX[1] = axisX[2] = 0;
	axisY[0] = 0; axisY[1] = 1; axisY[2] = 0;
	axisZ[0] = axisZ[1] = 0; axisZ[2] = 1;
	origin[0] = origin[1] = origin[2] = 0;
}


JView::~JView(void)
{
}

JVector3 JView::recoordToBase(JVector3 vec)
{
	//TODO : no copying, inline
	JVector3 recoorded;
	vec = vec - origin;

	recoorded[0] = axisX * vec;
	recoorded[1] = axisY * vec;
	recoorded[2] = axisZ * vec;

	return recoorded;
}
