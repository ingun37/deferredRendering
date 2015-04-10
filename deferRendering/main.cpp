
#include <Windows.h>

#include "JMaterial.h"
#include "JLevel.h"
#include "JMesh.h"

#include "JShape.h"
#include "JCamera.h"
#include "JFBOManager.h"
#include "JGlobalVariables.h"
#include "JActor.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>

#include <time.h>


using namespace std;

#define SAFE_DELETE(p) if(p){ delete (p); }

JTextureManager mngTex;
JProgramManager mngProgram;
JFBOManager mngFBO;
//-------textures----------
JTextureObject texPool[16];
JTextureObject* texCheck = &texPool[0];
JTextureObject* texWorldmap = &texPool[1];
JTextureObject* texBlock = &texPool[2];
JTextureObject* texPlanet2 = &texPool[3];
//---------shaders----------
shaderInfo_Diffuse shaderDiffusue;
shaderInfo_TexUnlit shaderTexUnlit;
shaderInfo_Deferred shaderDeferred;
shaderInfo_FinalDeferred shaderFinalDeferred;

shaderInfo_DirShadow shaderDirShadow;
//-------objectsLevel-------
JMaterial materialPool[16];
JMaterial* matTexUnlit = &materialPool[0];
JMaterial* matDeferred = &materialPool[1];
JMaterial* matDiffuse = &materialPool[2];
JMaterial *matTable = &materialPool[3];
JMaterial *matObj2 = &materialPool[4];
JMaterial *finalMat = &materialPool[5];

JMesh *objTable = NULL;
JMesh *obj1 = NULL;
JMesh *obj2 = NULL;
JMesh *obj3 = NULL;
JMesh *obj4 = NULL;

JActor *sausage = NULL;

JCamera* shadowCamera = NULL;
JCamera* worldCamera;
JLevel *worldLevel = NULL;

JFrameBufferObject fboPool[4];
JFrameBufferObject* shadowFBO = &fboPool[0];
JFrameBufferObject* deferredFBO = &fboPool[1];

//------------screenLevel--------------

JMesh *screenQuad = NULL;

JCamera* screenCamera = NULL;
JLevel *screenLevel = NULL;

#define JScreenWidth 640
#define JScreenHeight 480

int initManagers()
{
	int err = 0;
	err |= mngFBO.initFBOManager( mngTex );
	err |= mngProgram.initProgramManager();
	//TODO : initmanagers
	//mngTex.init
	//mngProgram.init
	return 0;
}
int initFBO()
{
	int err = 0;
	err |= mngFBO.makeCanvasWithAttribute(*deferredFBO,JFBO_BRUSHES::BRUSH_DEPTH | JFBO_BRUSHES::BRUSH_DIFFUSE | JFBO_BRUSHES::BRUSH_POSITION | JFBO_BRUSHES::BRUSH_NORMAL | JFBO_BRUSHES::BRUSH_TEX | JFBO_BRUSHES::BRUSH_SHADOW,JScreenWidth,JScreenHeight);
	err |= mngFBO.makeCanvasWithAttribute(*shadowFBO, JFBO_BRUSHES::BRUSH_DEPTH, 640, 480);
	return err;
}

int initTextures()
{
	int err = 0;
	err |= mngTex.makeTexture( *texWorldmap,JTEXTURE_IMAGEFORMAT::JIMGFORMAT_BMP,"../deferRendering/earthtoon.bmp" );
	err |= mngTex.makeTexture( *texBlock,JTEXTURE_IMAGEFORMAT::JIMGFORMAT_BMP,"../deferRendering/block.bmp" );
	err |= mngTex.makeTexture( *texPlanet2,JTEXTURE_IMAGEFORMAT::JIMGFORMAT_BMP,"../deferRendering/planet2.bmp" );
	return err;
}

int initShaders()
{
	int err = 0;
	err |= mngProgram.setProgram_Diffuse(shaderDiffusue,"../deferRendering/diffuse.vert","../deferRendering/diffuse.frag");
	err |= mngProgram.setProgram_TexUnlit(shaderTexUnlit,"../deferRendering/texunlit.vert","../deferRendering/texunlit.frag");
	err |= mngProgram.setProgram_Deferred(shaderDeferred,"../deferRendering/deferred.vert","../deferRendering/deferred.frag");
	err |= mngProgram.setProgram_DirShadow(shaderDirShadow,"../deferRendering/dirShadow.vert","../deferRendering/dirShadow.frag");
	err |= mngProgram.setProgram_FinalDeferred(shaderFinalDeferred,"../deferRendering/finalDeferred.vert","../deferRendering/finalDeferred.frag");

	return err;
}

int initMaterial()
{
	matDeferred->texObj = texWorldmap;
	matDeferred->shaderinfo = &shaderDeferred;

	matDiffuse->shaderinfo = &shaderDiffusue;

	matTexUnlit->shaderinfo = &shaderTexUnlit;

	matTable->texObj = texBlock;
	matTable->shaderinfo = &shaderDeferred;

	matObj2->texObj = texPlanet2;
	matObj2->shaderinfo = &shaderDeferred;

	finalMat->shaderinfo = &shaderFinalDeferred;

	finalMat->extexObj1 = deferredFBO->getTextureObjectOfCanvas(BRUSH_DIFFUSE);
	finalMat->extexObj2 = deferredFBO->getTextureObjectOfCanvas(BRUSH_NORMAL);
	finalMat->extexObj3 = deferredFBO->getTextureObjectOfCanvas(BRUSH_POSITION);
	finalMat->extexObj4 = deferredFBO->getTextureObjectOfCanvas(BRUSH_TEX);
	finalMat->extexObj5 = deferredFBO->getTextureObjectOfCanvas(BRUSH_SHADOW);

	return 0;
}

int initObjects()
{
	JVector3 tmpNormal;
	tmpNormal[0] = 0;
	tmpNormal[1] = 1;
	tmpNormal[2] = 0;

	sausage = new JActor();

	if(makeSausage(3,1,10,2,2,*sausage) != 0)
		return -1;

	//sausage->meshes[0]->position[0] = 3;
	sausage->meshes[0]->material = matTable;

	//sausage->meshes[1]->position[0] = 5;
	sausage->meshes[1]->material = matTable;
	sausage->meshes[2]->material = matTable;

	objTable = new JMesh();
	if( makePlane(10, 10, 1, 1, tmpNormal, *objTable, false ) != 0 )
	{
		return -1;
	}
	if( objTable->refreshVertexIndexBuffer() != 0 )
		return -1;
	objTable->position[1] = -1;
	objTable->material = matTable;

	unsigned int smoothness2 = 10;
	float radius2 = 1;

	obj1 = new JMesh();
	
	clock_t tmpT1 = clock();
	if( makeSphere( radius2, smoothness2, *obj1 ) != 0 )
	{
		return -1;
	}
	tmpT1 = clock() - tmpT1;
	printf("making sphere - %f sec\n",((float)tmpT1)/CLOCKS_PER_SEC);
	if( obj1->refreshVertexIndexBuffer() != 0 )
		return -1;
	obj1->position[2] = 1;

	obj1->setMaterial(matDeferred);


	obj2 = new JMesh();

	if(makeSphere( 2, smoothness2, *obj2))
		return -1;
	if(obj2->refreshVertexIndexBuffer() != 0)
		return -1;
	obj2->setMaterial(matObj2);
	obj2->position[2] = -2;
	obj2->position[0] = -1;
	//-------------------screen------------------
	tmpNormal[0] = 0;
	tmpNormal[1] = 0;
	tmpNormal[2] = 1;

	screenQuad = new JMesh();
	
	if( makePlane(JScreenWidth, JScreenHeight, 1, 1, tmpNormal, *screenQuad, false ) != 0 )
	{
		return -1;
	}
	if( screenQuad->refreshVertexIndexBuffer() != 0 )
		return -1;

	matTexUnlit->texObj = deferredFBO->getTextureObjectOfCanvas( BRUSH_DIFFUSE );
	if(matTexUnlit->texObj == NULL)
	{
		return -1;
	}
	
	screenQuad->setMaterial( matTexUnlit );

	return 0;
}
int initCameras()
{
	JVector3 pos, up, at;

	pos[0] = 0;
	pos[1] = 7;
	pos[2] = 11;
	
	up[0] = 0;
	up[1] = 1;
	up[2] = 0;

	at[0] = 0;
	at[1] = 0;
	at[2] = 0;

	worldCamera = new JCameraPerspective();
	worldCamera->setTargetFBO( deferredFBO );
	worldCamera->setTransform( pos, up, at ,JScreenWidth, JScreenHeight, 1.f, 100 );

	JGlobalVariables::gWorldCameraEyePos = pos;

	pos[0] = 0;
	pos[1] = 0;
	pos[2] = 10;

	up[0] = 0;
	up[1] = 1;
	up[2] = 0;

	at[0] = 0;
	at[1] = 0;
	at[2] = 0;

	screenCamera = new JCameraOrtho();
	screenCamera->setTransform( pos, up, at ,JScreenWidth, JScreenHeight, 1.f, 100 );

	pos[0] = 10;//this value must be synced with the value at JLevel::draw() case JSHADERKIND_FINALDEFERRED
	pos[1] = 8;
	pos[2] = 0;

	up[0] = 0;
	up[1] = 1;
	up[2] = 0;

	at[0] = 0;
	at[1] = 0;
	at[2] = 0;

	JGlobalVariables::gSunlightDir = pos;

	shadowCamera = new JCameraOrtho();
	shadowCamera->setTargetFBO( shadowFBO );
	shadowCamera->setTransform( pos, up, at ,16, 16, 1.f, 30 );//width and height of ground? would it be enough?
	
	return 0;
}

int init()
{
	glewInit();

	if( initManagers() != 0 )
		return -1;
	if( initFBO() != 0 )
		return -1;
	if( initTextures() != 0)
		return -1;
	if( initShaders() != 0)
		return -1;
	if ( initMaterial() != 0 )
		return -1;
	
	if( initCameras() != 0 )
		return -1;
	if( initObjects() != 0 )
		return -1;

	worldLevel = new JLevel();
	worldLevel->pushMesh( obj1 );
	worldLevel->pushMesh( obj2 );
	worldLevel->pushMesh( objTable );
	worldLevel->pushMesh( sausage->meshes[0] );
	worldLevel->pushMesh( sausage->meshes[1] );
	worldLevel->pushMesh( sausage->meshes[2] );
	worldLevel->pushCamera( worldCamera );
	worldLevel->shadowCamera = shadowCamera;
	worldLevel->shadowShader = &shaderDirShadow;

	screenLevel = new JLevel();
	screenLevel->pushMesh( screenQuad );
	screenLevel->pushCamera( screenCamera );
	
	return 0;
}

int draw()
{
	glClearColor( 0.5,0.5,0.5,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	worldLevel->draw();


	glClearColor( 0,0.2,0.2,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	screenLevel->draw();

/*

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);

	glClearColor( 0,0,1,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer( GL_READ_FRAMEBUFFER, deferredFBO->bufferID );

	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glBlitFramebuffer(0,0,JScreenWidth,JScreenHeight,0,0,JScreenWidth/2,JScreenHeight/2,GL_COLOR_BUFFER_BIT,GL_LINEAR);

	glReadBuffer(GL_COLOR_ATTACHMENT1);
	glBlitFramebuffer(0,0,JScreenWidth,JScreenHeight,JScreenWidth/2,0,JScreenWidth,JScreenHeight/2,GL_COLOR_BUFFER_BIT,GL_LINEAR);
*/


	return 0;
}

int release()
{
	SAFE_DELETE( obj1 );
	SAFE_DELETE( screenQuad );
	SAFE_DELETE( screenLevel );
	SAFE_DELETE( worldLevel );
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_1 && action == GLFW_RELEASE)
	{
		screenQuad->material = matTexUnlit;
		screenQuad->material->texObj = deferredFBO->getTextureObjectOfCanvas( BRUSH_DIFFUSE );
	}
	if (key == GLFW_KEY_2 && action == GLFW_RELEASE)
	{
		screenQuad->material = matTexUnlit;
		screenQuad->material->texObj = deferredFBO->getTextureObjectOfCanvas( BRUSH_NORMAL );
	}
	if (key == GLFW_KEY_3 && action == GLFW_RELEASE)
	{
		screenQuad->material = matTexUnlit;
		screenQuad->material->texObj = deferredFBO->getTextureObjectOfCanvas( BRUSH_POSITION );
	}
	if (key == GLFW_KEY_4 && action == GLFW_RELEASE)
	{
		screenQuad->material = matTexUnlit;
		screenQuad->material->texObj = deferredFBO->getTextureObjectOfCanvas( BRUSH_TEX );
	}
	if (key == GLFW_KEY_5 && action == GLFW_RELEASE)
	{
		screenQuad->material = matTexUnlit;
		screenQuad->material->texObj = deferredFBO->getTextureObjectOfCanvas( BRUSH_SHADOW );
	}
	if (key == GLFW_KEY_6 && action == GLFW_RELEASE)
	{
		screenQuad->material = finalMat;
	}
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(JScreenWidth, JScreenHeight, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
	
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);

	glfwSetKeyCallback(window, key_callback);

	init();
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
		draw();
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

    }
	release();
    glfwTerminate();
    return 0;
}