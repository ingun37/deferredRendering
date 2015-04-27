#ifndef JFBOManager_h
#define JFBOManager_h
//#define GLEW_STATIC
#include <GL/glew.h>


#include <map>
#include <string>
#include <vector>


#include "JTextureManager.h"

using namespace std;

enum JFBO_BRUSHE_IDX
{
	BRUSHIDX_DIFFUSE = 0,
	BRUSHIDX_POSITION,
	BRUSHIDX_NORMAL,
	BRUSHIDX_TEX,
	BRUSHIDX_SHADOW,
	BRUSHIDX_DEPTH,
	BRUSHIDX_STENCIL,
	BRUSHIDX_NUM
};

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

const int tableBrush_flag[BRUSHIDX_NUM]={BRUSH_DIFFUSE,BRUSH_POSITION,BRUSH_NORMAL,BRUSH_TEX,BRUSH_SHADOW,BRUSH_DEPTH,BRUSH_STENCIL};
const JTEXTURE_KINDS tableBrush_kind[BRUSHIDX_NUM] = {JTEXTUREKIND_COLOR,JTEXTUREKIND_VECTOR,JTEXTUREKIND_COLOR,JTEXTUREKIND_COLOR,JTEXTUREKIND_COLOR,JTEXTUREKIND_DEPTH,JTEXTUREKIND_STENCIL};
const int tableBrush_attachIdx[BRUSHIDX_NUM] = {0,1,2,3,4,0,0};





class JFrameBufferObject //TODO : Consider about turning it into smart pointer
{
public:
	GLsizei width;
	GLsizei height;

	GLuint bufferID;

	JTextureObject texs[BRUSHIDX_NUM];
	/*JTextureObject colorTex;
	JTextureObject positionTex;
	JTextureObject normalTex;
	JTextureObject texTex;
	JTextureObject shadowTex;
	JTextureObject depthTex;*/

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
