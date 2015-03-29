#ifndef JProgramManager_h
#define JProgramManager_h
#include <map>
#include <string>
//#define GLEW_STATIC
#include <GL/glew.h>

#include "JLinearAlgebra.h"

#include "JTextureManager.h"

enum JShaderKinds{JSHADERKIND_DEFERRED, JSHADERKIND_TEXUNLIT, JSHADERKIND_DIFFUSE};

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

		int setOutputDrawBuffer( JFrameBufferObject* fbo );

		shaderInfo_Deferred* setProgram_Deferred(const string& name, char* vpath, char* fpath);
		int setUniformVariables_Deferred( JMatrix44 mvp );
		
		shaderInfo_TexUnlit* setProgram_TexUnlit(const string& name, char* vpath, char* fpath);
		int setUniformVariables_TexUnlit( JMatrix44 mvp, JTextureObject* aTex );

		shaderInfo_Diffuse* setProgram_Diffuse(const string& name, char* vpath, char* fpath);
		int setUniformVariables_Diffuse( JMatrix44 mvp );

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
