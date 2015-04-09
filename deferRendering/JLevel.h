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
	JCamera* shadowCamera;
	shaderInfo_DirShadow* shadowShader;
	int draw();

	int pushMesh( JMesh* pMesh );
	int pushCamera( JCamera* pCamera );

	int removeMesh( JMesh* pMesh );
	int removeCamera( JCamera* pCamera );

	JProgramManager* mngProg;
	int initJLevel( JProgramManager& argMngProg );
	JLevel();
};

#endif
