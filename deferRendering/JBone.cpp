#include "JBone.h"


JBone::JBone(void)
{
	pos[0] = pos[1] = pos[2] = axis[0] = axis[1] = axis[2] = 0;
	angle = 0;
	upperBoneIdx = 0;
}


JBone::~JBone(void)
{
}

JMatrix44 JBone::getTransform()// T * R
{
	return JMatrix44::GetTranslationMatrix(pos[0],pos[1],pos[2]) * JMatrix44::GetRotateMatrix(axis[0],axis[1],axis[2],angle);
}
