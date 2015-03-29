#ifndef JFBOManager_h
#define JFBOManager_h
//#define GLEW_STATIC
#include <GL/glew.h>


#include <map>
#include <string>
#include <vector>


#include "JTextureManager.h"

using namespace std;

enum JFBO_BRUSHES
{
	BRUSH_TEX1 = 1,
	BRUSH_POSITION = 2,
	BRUSH_NORMAL = 4,
	BRUSH_DEPTH = 16,
	BRUSH_STENCIL = 32
};


class JFrameBufferObject //TODO : Consider about turning it into smart pointer
{
public:
	GLsizei width;
	GLsizei height;

	GLuint bufferID;

	JTextureObject* colorTex;
	JTextureObject* positionTex;
	JTextureObject* normalTex;
	
	GLuint depthRBO;

	GLuint stencilID;

	int brushes;

	JFrameBufferObject();

	int reset( int jfbo_brushes, GLsizei width, GLsizei height);
	int bind();//binding FBO also glDrawBuffer
	int unbind();
	int setOutputDrawBuffer();
	JTextureObject* getTextureObjectOfCanvas( JFBO_BRUSHES whichTex );//textures made in middle of creating fbo has no "string" key but only "unsigned int" key.
};

class JFBOManager
{
public:
	JFrameBufferObject* makeCanvasWithAttribute( int jfbo_brushes, GLsizei width, GLsizei height );
	
	
	static JFBOManager* Inst()
	{
		static JFBOManager* instance = NULL;
		if(instance == NULL)
			instance = new JFBOManager();
		return instance;
	}

	JFBOManager();
};



#endif
