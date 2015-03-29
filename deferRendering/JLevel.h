#ifndef JLevel_h
#define JLevel_h

#include "JMesh.h"
#include "JCamera.h"

#include <vector>
#include <string>

using namespace std;

class JLevel
{
public:
	vector<JMesh*> meshes;
	vector<JCamera*> cameras;

	int draw();

	
	int pushMesh( JMesh* pMesh );
	int pushCamera( JCamera* pCamera );

	JLevel();
};

#endif
