#ifndef JMaterial_h
#define JMaterial_h

#include <string>

#include "JTextureManager.h"
#include "JProgramManager.h"
using namespace std;

class JMaterial
{
public:
	JTextureObject* texObj;

	JTextureObject* extexObj1;
	JTextureObject* extexObj2;
	JTextureObject* extexObj3;
	JTextureObject* extexObj4;
	JTextureObject* extexObj5;

	shaderInfo* shaderinfo;

	JMaterial();
};

#endif
