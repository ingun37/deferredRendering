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
	shaderInfo* shaderinfo;
	JMaterial();
};

#endif
