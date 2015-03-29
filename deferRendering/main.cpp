
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

//---------shaders----------
shaderInfo* shaderDiffusue = NULL;
shaderInfo* shaderTexUnlit = NULL;
shaderInfo* shaderDeferred = NULL;

//-------objectsLevel-------
JMaterial* matTexUnlit = NULL;
JMaterial* matDeferred = NULL;
JMaterial* matDiffuse = NULL;

JMesh *obj1 = NULL;
JMesh *obj2 = NULL;
JMesh *obj3 = NULL;
JMesh *obj4 = NULL;

JCamera* worldCamera;
JLevel *worldLevel = NULL;

JFrameBufferObject* deferredFBO = NULL;

//------------screenLevel--------------

JMesh *screenDiffuse = NULL;
JMesh *screenPosition = NULL;

JCamera* screenCamera;
JLevel *screenLevel = NULL;

#define JScreenWidth 640
#define JScreenHeight 480

int initFBO()
{
	deferredFBO = JFBOManager::Inst()->makeCanvasWithAttribute(JFBO_BRUSHES::BRUSH_TEX1 | JFBO_BRUSHES::BRUSH_POSITION,JScreenWidth,JScreenHeight);
	return 0;
}

int initTextures()
{
	texCheck = new JTextureObject();
	if( JTextureManager::Inst()->makeTexture( *texCheck, JScreenWidth, JScreenHeight, JTEXTUREKIND_COLOR, false ) == -1 )
		return -1;

	return 0;
}

int initShaders()
{
	shaderDiffusue = JProgramManager::Inst()->setProgram_Diffuse("diffuse","../deferRendering/diffuse.vert","../deferRendering/diffuse.frag");
	shaderTexUnlit = JProgramManager::Inst()->setProgram_TexUnlit("texunlit","../deferRendering/texunlit.vert","../deferRendering/texunlit.frag");
	shaderDeferred = JProgramManager::Inst()->setProgram_Deferred("deferred","../deferRendering/deferred.vert","../deferRendering/deferred.frag");
	if( shaderDiffusue == NULL || shaderTexUnlit == NULL )
		return -1;

	return 0;
}

int initMaterial()
{
	matDeferred = new JMaterial();
	matDeferred->shaderinfo = shaderDeferred;

	matDiffuse = new JMaterial();
	matDiffuse->shaderinfo = shaderDiffusue;
	return 0;
}

int initObjects()
{
	obj1 = new JMesh();

	unsigned int smoothness1 = 1;
	float radius1 = 1;

	unsigned int smoothness2 = 3;
	float radius2 = 1;
	JVector3 tmpNormal;
	tmpNormal[0] = 0;
	tmpNormal[1] = 0;
	tmpNormal[2] = 1;
	
	if( makeSphere( radius2, smoothness2, *obj1 ) != 0 )
	{
		return -1;
	}
	if( obj1->refreshVBO() != 0 )
		return -1;
	//obj1->position[0] = 3;

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
	
	screenMaterial->texObj = deferredFBO->getTextureObjectOfCanvas( BRUSH_TEX1 );
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

	return 0;
}
int initCameras()
{
	JVector3 pos, up, at;

	pos[0] = 0;
	pos[1] = 0;
	pos[2] = 10;
	
	up[0] = 0;
	up[1] = 1;
	up[2] = 0;

	at[0] = 0;
	at[1] = 0;
	at[2] = 0;

	worldCamera = new JCameraPerspective();
	worldCamera->setTargetFBO( deferredFBO );
	worldCamera->setTransform( pos, up, at ,JScreenWidth, JScreenHeight, 1.f, 500 );
	
	screenCamera = new JCameraOrtho();
	screenCamera->setTransform( pos, up, at ,JScreenWidth, JScreenHeight, 1.f, 500 );
	
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
	worldLevel->pushCamera( worldCamera );
	
	screenLevel = new JLevel();
	screenLevel->pushMesh( screenDiffuse );
	screenLevel->pushMesh( screenPosition );
	screenLevel->pushCamera( screenCamera );
	
	return 0;
}

int draw()
{
	glClearColor( 0.5,0.5,0,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	worldLevel->draw();
/*

	glClearColor( 0,0,1,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	screenLevel->draw();
*/

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);

	glClearColor( 0,0,1,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer( GL_READ_FRAMEBUFFER, deferredFBO->bufferID );

	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glBlitFramebuffer(0,0,JScreenWidth,JScreenHeight,0,0,JScreenWidth/2,JScreenHeight/2,GL_COLOR_BUFFER_BIT,GL_LINEAR);

	glReadBuffer(GL_COLOR_ATTACHMENT1);
	glBlitFramebuffer(0,0,JScreenWidth,JScreenHeight,JScreenWidth/2,0,JScreenWidth,JScreenHeight/2,GL_COLOR_BUFFER_BIT,GL_LINEAR);


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

	//glFrontFace(GL_CCW);
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