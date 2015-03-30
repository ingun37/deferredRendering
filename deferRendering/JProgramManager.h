#ifndef JProgramManager_h
#define JProgramManager_h
#include <map>
#include <string>
//#define GLEW_STATIC
#include <GL/glew.h>

#include "JLinearAlgebra.h"

#include "JTextureManager.h"

enum JShaderKinds{JSHADERKIND_DEFERRED, JSHADERKIND_TEXUNLIT, JSHADERKIND_DIFFUSE, JSHADERKIND_DIRSHADOW, JSHADERKIND_FINALDEFERRED};

using namespace std;

class shaderInfo
{
public:
	string name;

	unsigned int p;
	unsigned int v;
	unsigned int f;
		
	unsigned int lmvp;
	JShaderKinds shaderKind;
};

class shaderInfo_Deferred : public shaderInfo
{ 
public:
	unsigned int lTrans;
	unsigned int ltex;
	unsigned int lShadowPV;
	unsigned int lShadowTex;
	shaderInfo_Deferred()
	{
		//TODO set shaderkind to JShader_Deferred
		shaderKind = JSHADERKIND_DEFERRED;
	}
};

class shaderInfo_TexUnlit : public shaderInfo
{
public:
	unsigned int ltex;
	shaderInfo_TexUnlit()
	{
		shaderKind = JSHADERKIND_TEXUNLIT;
	}
};

class shaderInfo_Diffuse : public shaderInfo
{
public:
	shaderInfo_Diffuse()
	{
		shaderKind = JSHADERKIND_DIFFUSE;
	}
};

class shaderInfo_DirShadow : public shaderInfo
{
public:
	shaderInfo_DirShadow()
	{
		shaderKind = JSHADERKIND_DIRSHADOW;
	}
};

class shaderInfo_FinalDeferred : public shaderInfo
{
public:
	unsigned int lLightDir;
	unsigned int lDiffuseMap;
	unsigned int lNormalMap;
	unsigned int lPositionMap;
	unsigned int lTextureMap;
	unsigned int lShadowMap;
	unsigned int lEyePos;
	shaderInfo_FinalDeferred()
	{
		shaderKind = JSHADERKIND_FINALDEFERRED;
	}
};

typedef std::map<string, shaderInfo*> mapProgram;

class JFrameBufferObject;
class JProgramManager
{
		mapProgram programs;
		int makeShader(char* path, GLenum type, GLint* perror, GLuint& shaderID);
		int makeVertexShader(char* path, GLint *perror, GLuint& shaderID);
		int makeFragmentShader(char* path, GLint *perror, GLuint& shaderID);
		int makeProgram(GLuint attachingVertexShader, GLuint attachingFragmentShader, GLuint& programID);
public:
		static unsigned int currentlyRunningProgram;
		static const shaderInfo* currentlyRunningInfo;
		
		

		int useProgram(const string& shaderName);
		int useProgram(const shaderInfo* aShaderInfo);
		int useProgram_Deferred();


		shaderInfo_Deferred* setProgram_Deferred(const string& name, char* vpath, char* fpath);
		int setUniformVariables_Deferred( JMatrix44 mvp, JTextureObject* tex,JMatrix44& shadowPV, JTextureObject* shadowTex, JMatrix44& trans );
		
		shaderInfo_TexUnlit* setProgram_TexUnlit(const string& name, char* vpath, char* fpath);
		int setUniformVariables_TexUnlit( JMatrix44 mvp, JTextureObject* aTex );

		shaderInfo_Diffuse* setProgram_Diffuse(const string& name, char* vpath, char* fpath);
		int setUniformVariables_Diffuse( JMatrix44 mvp );

		shaderInfo_DirShadow* setProgram_DirShadow(const string& name, char* vpath, char* fpath);
		int setUniformVariables_DirShadow( JMatrix44 mvp );

		shaderInfo_FinalDeferred* setProgram_FinalDeferred(const string& name, char* vpath, char* fpath);
		int setUniformVariables_FinalDeferred( JMatrix44 mvp, JVector3 eyepos, JVector3 lightDir, JTextureObject* diffuseMap, JTextureObject* normalMap, JTextureObject* positionMap, JTextureObject* textureMap, JTextureObject* shadowMap );

		JProgramManager(){};

		static JProgramManager* Inst()
		{
			static JProgramManager* instance = NULL;
			if(instance == NULL)
				instance = new JProgramManager();
			return instance;
		}
};



#endif