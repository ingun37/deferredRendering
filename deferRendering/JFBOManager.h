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
	BRUSH_DIFFUSE = 1,
	BRUSH_POSITION = 2,
	BRUSH_NORMAL = 4,
	BRUSH_TEX = 8,
	BRUSH_SHADOW = 16,
	BRUSH_DEPTH = 32,
	BRUSH_STENCIL = 64
};


class JFrameBufferObject //TODO : Consider about turning it into smart pointer
{
public:
	GLsizei width;
	GLsizei height;

	GLuint bufferID;

	JTextureObject colorTex;
	JTextureObject positionTex;
	JTextureObject normalTex;
	JTextureObject texTex;
	JTextureObject shadowTex;
	JTextureObject depthTex;

	GLuint stencilID;

	int brushes;

	JTextureManager* mngTex;
	int initFBO( JTextureManager& argMngTex );
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
	JTextureManager *mngTex;
	int makeCanvasWithAttribute( JFrameBufferObject& fbo, int jfbo_brushes, GLsizei width, GLsizei height );
	int initFBOManager( JTextureManager& argMngTex );
	JFBOManager():mngTex(0){};
};



#endif
