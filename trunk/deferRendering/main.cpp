
#include <Windows.h>

#include "JMaterial.h"
#include "JLevel.h"
#include "JMesh.h"

#include "JShape.h"
#include "JCamera.h"
#include "JFBOManager.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
using namespace std;

#define SAFE_DELETE(p) if(p){ delete (p); }
//-------textures----------
JTextureObject* texCheck = NULL;
JTextureObject* texWorldmap = NULL;
JTextureObject* texBlock = NULL;
//---------shaders----------
shaderInfo* shaderDiffusue = NULL;
shaderInfo* shaderTexUnlit = NULL;
shaderInfo* shaderDeferred = NULL;
shaderInfo_DirShadow* shaderDirShadow = NULL;
//-------objectsLevel-------
JMaterial* matTexUnlit = NULL;
JMaterial* matDeferred = NULL;
JMaterial* matDiffuse = NULL;

JMesh *objTable = NULL;
JMaterial *matTable = NULL;

JMesh *obj1 = NULL;
JMesh *obj2 = NULL;
JMesh *obj3 = NULL;
JMesh *obj4 = NULL;

JFrameBufferObject* shadowFBO = NULL;
JCamera* shadowCamera = NULL;
JCamera* worldCamera;
JLevel *worldLevel = NULL;

JFrameBufferObject* deferredFBO = NULL;

//------------screenLevel--------------

JMesh *screenDiffuse = NULL;
JMesh *screenPosition = NULL;
JMesh *screenNormal = NULL;
JMesh *screenTex = NULL;

JCamera* screenCamera;
JLevel *screenLevel = NULL;

#define JScreenWidth 640
#define JScreenHeight 480

int initFBO()
{
	deferredFBO = JFBOManager::Inst()->makeCanvasWithAttribute( JFBO_BRUSHES::BRUSH_DEPTH | JFBO_BRUSHES::BRUSH_DIFFUSE | JFBO_BRUSHES::BRUSH_POSITION | JFBO_BRUSHES::BRUSH_NORMAL | JFBO_BRUSHES::BRUSH_TEX | JFBO_BRUSHES::BRUSH_SHADOW,JScreenWidth,JScreenHeight);

	shadowFBO = JFBOManager::Inst()->makeCanvasWithAttribute( JFBO_BRUSHES::BRUSH_DEPTH , 640, 480);
	return 0;
}

int initTextures()
{
	texWorldmap = new JTextureObject();
	if( JTextureManager::Inst()->makeTexture( *texWorldmap,JTEXTURE_IMAGEFORMAT::JIMGFORMAT_BMP,"../deferRendering/earthtoon.bmp" ) == -1 )
		return -1;

	texBlock = new JTextureObject();
	if( JTextureManager::Inst()->makeTexture( *texBlock,JTEXTURE_IMAGEFORMAT::JIMGFORMAT_BMP,"../deferRendering/block.bmp" ) == -1 )
		return -1;


	return 0;
}

int initShaders()
{
	shaderDiffusue = JProgramManager::Inst()->setProgram_Diffuse("diffuse","../deferRendering/diffuse.vert","../deferRendering/diffuse.frag");
	shaderTexUnlit = JProgramManager::Inst()->setProgram_TexUnlit("texunlit","../deferRendering/texunlit.vert","../deferRendering/texunlit.frag");
	shaderDeferred = JProgramManager::Inst()->setProgram_Deferred("deferred","../deferRendering/deferred.vert","../deferRendering/deferred.frag");
	shaderDirShadow = JProgramManager::Inst()->setProgram_DirShadow("dirshadow","../deferRendering/dirShadow.vert","../deferRendering/dirShadow.frag");
	if( shaderDiffusue == NULL || shaderTexUnlit == NULL || shaderDirShadow == NULL)
		return -1;

	return 0;
}

int initMaterial()
{
	matDeferred = new JMaterial();
	matDeferred->texObj = texWorldmap;
	matDeferred->shaderinfo = shaderDeferred;

	matDiffuse = new JMaterial();
	matDiffuse->shaderinfo = shaderDiffusue;

	matTexUnlit = new JMaterial();
	matTexUnlit->texObj = texWorldmap;
	matTexUnlit->shaderinfo = shaderTexUnlit;

	matTable = new JMaterial();
	matTable->texObj = texBlock;
	matTable->shaderinfo = shaderDeferred;
	return 0;
}

int initObjects()
{
	JVector3 tmpNormal;
	tmpNormal[0] = 0;
	tmpNormal[1] = 1;
	tmpNormal[2] = 0;

	objTable = new JMesh();
	if( makePlane(10, 10, 1, 1, tmpNormal, *objTable, false ) != 0 )
	{
		return -1;
	}
	if( objTable->refreshVBO() != 0 )
		return -1;
	objTable->position[1] = -1;
	objTable->material = matTable;
	
	obj1 = new JMesh();

	unsigned int smoothness2 = 7;
	float radius2 = 1;
	
	tmpNormal[0] = 0;
	tmpNormal[1] = 0;
	tmpNormal[2] = 1;
	
	if( makeSphere( radius2, smoothness2, *obj1 ) != 0 )
	{
		return -1;
	}
	if( obj1->refreshVBO() != 0 )
		return -1;
	//obj1->position[1] = 3;

	obj1->setMaterial(matDeferred);

	//-------------------screen------------------

	screenDiffuse = new JMesh();
	
	if( makePlane(JScreenWidth/2, JScreenHeight/2, 1, 1, tmpNormal, *screenDiffuse, false ) != 0 )
	{
		return -1;
	}
	screenDiffuse->position[0] = -JScreenWidth/4;
	screenDiffuse->position[1] = JScreenHeight/4;
	if( screenDiffuse->refreshVBO() != 0 )
		return -1;

	

	JMaterial* screenMaterial = new JMaterial();
	screenMaterial->shaderinfo = shaderTexUnlit;
	
	screenMaterial->texObj = deferredFBO->getTextureObjectOfCanvas( BRUSH_DIFFUSE );
	if(screenMaterial->texObj == NULL)
	{
		return -1;
	}

	screenDiffuse->setMaterial( screenMaterial );


	screenPosition = new JMesh();

	if( makePlane(JScreenWidth/2, JScreenHeight/2, 1, 1, tmpNormal, *screenPosition, false ) != 0 )
	{
		return -1;
	}
	screenPosition->position[0] = JScreenWidth/4;
	screenPosition->position[1] = JScreenHeight/4;
	if( screenPosition->refreshVBO() != 0 )
		return -1;

	JMaterial* screenPositionMat = new JMaterial();
	screenPositionMat->shaderinfo = shaderTexUnlit;
	screenPositionMat->texObj = deferredFBO->getTextureObjectOfCanvas( BRUSH_POSITION );
	if(screenPositionMat->texObj == NULL)
		return -1;

	screenPosition->setMaterial( screenPositionMat );


	screenNormal = new JMesh();

	if( makePlane(JScreenWidth/2, JScreenHeight/2, 1, 1, tmpNormal, *screenNormal, false ) != 0 )
	{
		return -1;
	}
	screenNormal->position[0] = -JScreenWidth/4;
	screenNormal->position[1] = -JScreenHeight/4;
	if( screenNormal->refreshVBO() != 0 )
		return -1;

	JMaterial* screenNormalMat = new JMaterial();
	screenNormalMat->shaderinfo = shaderTexUnlit;
	screenNormalMat->texObj = deferredFBO->getTextureObjectOfCanvas( BRUSH_NORMAL );
	if(screenNormalMat->texObj == NULL)
		return -1;

	screenNormal->setMaterial( screenNormalMat );


	screenTex = new JMesh();

	if( makePlane(JScreenWidth/2, JScreenHeight/2, 1, 1, tmpNormal, *screenTex, false ) != 0 )
	{
		return -1;
	}
	screenTex->position[0] = JScreenWidth/4;
	screenTex->position[1] = -JScreenHeight/4;
	if( screenTex->refreshVBO() != 0 )
		return -1;

	JMaterial* screenTexMat = new JMaterial();
	screenTexMat->shaderinfo = shaderTexUnlit;
	screenTexMat->texObj = deferredFBO->getTextureObjectOfCanvas( BRUSH_SHADOW );
	if(screenTexMat->texObj == NULL)
		return -1;

	screenTex->setMaterial( screenTexMat );

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

	pos[0] = 10;
	pos[1] = 8;
	pos[2] = 0;

	up[0] = 0;
	up[1] = 1;
	up[2] = 0;

	at[0] = 0;
	at[1] = 0;
	at[2] = 0;

	shadowCamera = new JCameraOrtho();
	shadowCamera->setTargetFBO( shadowFBO );
	shadowCamera->setTransform( pos, up, at ,16, 16, 1.f, 30 );//width and height of ground? would it be enough?
	
	return 0;
}

int init()
{
	glewInit();
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
	worldLevel->pushMesh( objTable );
	worldLevel->pushCamera( worldCamera );
	worldLevel->shadowCamera = shadowCamera;
	worldLevel->shadowShader = shaderDirShadow;

	screenLevel = new JLevel();
	screenLevel->pushMesh( screenDiffuse );
	screenLevel->pushMesh( screenPosition );
	screenLevel->pushMesh( screenNormal );
	screenLevel->pushMesh(screenTex);
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
	SAFE_DELETE( screenDiffuse );
	SAFE_DELETE( screenLevel );
	SAFE_DELETE( worldLevel );
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_1 && action == GLFW_RELEASE)
	{

	}
	if (key == GLFW_KEY_2 && action == GLFW_RELEASE)
	{

	}
	if (key == GLFW_KEY_3 && action == GLFW_RELEASE)
	{

	}
	if (key == GLFW_KEY_4 && action == GLFW_RELEASE)
	{

	}
	if (key == GLFW_KEY_5 && action == GLFW_RELEASE)
	{

	}
	if (key == GLFW_KEY_6 && action == GLFW_RELEASE)
	{

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