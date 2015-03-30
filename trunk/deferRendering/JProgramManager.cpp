#include "JProgramManager.h"
#include "JFBOManager.h"

#define printOpenGLError() printError(__FILE__,__LINE__)

/*
GLSL compilers and linkers try to be as efficient as possible. Therefore, they do their best to eliminate code that does not affect the stage outputs. Because of this, a uniform defined in a shader file does not have to be made available in the linked program. It is only available if that uniform is used by code that affects the stage output, and that the uniform itself can change the output of the stage.
	Therefore, a uniform that is exposed by a fully linked program is called an "active" uniform; any other uniform specified by the original shaders is inactive. Inactive uniforms cannot be used to do anything in a program.*/
size_t TypeSize(GLenum type)
{
	size_t size;
#define CASE(Enum, Count, Type) \
case Enum: size = Count * sizeof(Type); break
	switch (type) {
		CASE(GL_FLOAT, 1, GLfloat);
		CASE(GL_FLOAT_VEC2, 2, GLfloat);
		CASE(GL_FLOAT_VEC3, 3, GLfloat);
		CASE(GL_FLOAT_VEC4, 4, GLfloat);
		CASE(GL_INT, 1, GLint);
		CASE(GL_INT_VEC2, 2, GLint);
		CASE(GL_INT_VEC3, 3, GLint);
		CASE(GL_INT_VEC4, 4, GLint);
		CASE(GL_UNSIGNED_INT, 1, GLuint);
		CASE(GL_UNSIGNED_INT_VEC2, 2, GLuint);
		CASE(GL_UNSIGNED_INT_VEC3, 3, GLuint);
		CASE(GL_UNSIGNED_INT_VEC4, 4, GLuint);
		CASE(GL_BOOL, 1, GLboolean);
		CASE(GL_BOOL_VEC2, 2, GLboolean);
		CASE(GL_BOOL_VEC3, 3, GLboolean);
		CASE(GL_BOOL_VEC4, 4, GLboolean);
		CASE(GL_FLOAT_MAT2, 4, GLfloat);
		CASE(GL_FLOAT_MAT2x3, 6, GLfloat);
		CASE(GL_FLOAT_MAT2x4, 8, GLfloat);
		CASE(GL_FLOAT_MAT3, 9, GLfloat);
		CASE(GL_FLOAT_MAT3x2, 6, GLfloat);
		CASE(GL_FLOAT_MAT3x4, 12, GLfloat);
		CASE(GL_FLOAT_MAT4, 16, GLfloat);
		CASE(GL_FLOAT_MAT4x2, 8, GLfloat);
		CASE(GL_FLOAT_MAT4x3, 12, GLfloat);
#undef CASE
default:
	fprintf(stderr, "Unknown type: 0x%x\n", type);
	exit(EXIT_FAILURE);
	break;
}
return size;
}

static int printError(char* file, int line)
{
	GLenum glErr;
	int retCode = 0;
	glErr = glGetError();
	if(glErr != GL_NO_ERROR)
	{
		printf("glError in file %s @ line %d: %s",file,line,gluErrorString(glErr));
		retCode = 1;
	}
	return retCode;

}
static char* getshadersourcefromfile(char* path,unsigned int* plen)
{
	FILE* fsource = fopen(path,"r");
	char c;
	unsigned int len=0;
	char* source, *pc;

	while((c=getc(fsource))!=EOF)
	{
		len++;
	}
	len++;
	source = (char*)malloc(sizeof(char)*len);
	pc = source;
	fseek(fsource,0,SEEK_SET);
	while((c=getc(fsource))!=EOF)
	{
		*(pc++)=c;
	}
	*pc = '\0';
	if(plen) *plen = len;

	fclose(fsource);

	return source;
}

unsigned int JProgramManager::currentlyRunningProgram = 0;
const shaderInfo* JProgramManager::currentlyRunningInfo = NULL;

int JProgramManager::makeShader(char* path, GLenum type, GLint* perror, GLuint& shaderID)
{
	char* source;
	char** sources;
	char buflog[1024]={0,};
	GLint result;
	GLuint iShader;


	iShader = glCreateShader(type);

	sources = (char**)malloc(sizeof(char*)*1);

	source = getshadersourcefromfile(path,NULL);

	//printf("%s\n",source);

	sources[0] = source;

	glShaderSource(iShader,1,(const char**)sources,NULL);
	puts("shader source complete");
	glCompileShader(iShader);
	puts("compile complete");
	glGetShaderiv(iShader,GL_COMPILE_STATUS,&result);
	if(result != GL_TRUE)
	{
		if(perror)
			*perror = result;
		puts("compile error");
		glGetShaderInfoLog(iShader, 1024, NULL, buflog);
		printf("%s\n",buflog);
		return -999;
	}
	free(source);
	free(sources);
	printf("shader %d complete.\n",iShader);

	shaderID = iShader;
	return 0;

}

int JProgramManager::makeVertexShader(char* path, GLint *perror, GLuint& shaderID)
{
	return makeShader(path, GL_VERTEX_SHADER, perror, shaderID);
}

int JProgramManager::makeFragmentShader(char* path, GLint *perror, GLuint& shaderID)
{
	return makeShader(path, GL_FRAGMENT_SHADER, perror, shaderID);
}

int JProgramManager::makeProgram(GLuint attachingVertexShader, GLuint attachingFragmentShader, GLuint& programID)
{
	programID = glCreateProgram();
	printOpenGLError();
	glAttachShader(programID, attachingVertexShader);
	printOpenGLError();
	glAttachShader(programID, attachingFragmentShader);
	printOpenGLError();
	glLinkProgram(programID);
	printOpenGLError();

	return 0;
}

int JProgramManager::useProgram(const string& shaderName)
{
	shaderInfo* sInfo = programs[shaderName];
	if(sInfo == NULL) return -1;
	currentlyRunningInfo = sInfo;
	glUseProgram(sInfo->p);

	return 0;
}

int JProgramManager::useProgram(const shaderInfo* sInfo)
{
	if(sInfo == NULL) return -1;

	currentlyRunningInfo = sInfo;
	glUseProgram( sInfo->p );

	return 0;
}

int JProgramManager::useProgram_Deferred()
{
	//TODO
	return 0;
}

//////////////////////////////
//---------------------DEFERRED
//////////////////////////////
shaderInfo_Deferred* JProgramManager::setProgram_Deferred(const string& name, char* vpath, char* fpath)
{

	shaderInfo_Deferred* info = new shaderInfo_Deferred();

	if(makeVertexShader(vpath,NULL, info->v) != 0)
		return NULL;
	if(makeFragmentShader(fpath,NULL, info->f) != 0)
		return NULL;
	if(makeProgram(info->v, info->f, info->p) != 0)
		return NULL;

	info->lmvp = 4;
	info->ltex = 5;
	info->lShadowPV = 6;
	info->lShadowTex = 7;
	info->lTrans = 8;

	if(info->p > 0)
		programs[name] = info;

	return info;
}

int JProgramManager::setUniformVariables_Deferred( JMatrix44 mvp, JTextureObject* aTex,JMatrix44& shadowPV, JTextureObject* shadowTex, JMatrix44& trans )
{
	if(JProgramManager::currentlyRunningInfo->shaderKind != JSHADERKIND_DEFERRED)
		return -1;

	shaderInfo_Deferred* shaderinfo = (shaderInfo_Deferred*)JProgramManager::currentlyRunningInfo;

	glUniformMatrix4fv( shaderinfo->lmvp, 1, GL_TRUE, &mvp[0][0] );
	glUniformMatrix4fv( shaderinfo->lShadowPV, 1, GL_TRUE, &shadowPV[0][0] );
	glUniformMatrix4fv( shaderinfo->lTrans, 1, GL_TRUE, &trans[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, aTex->bufID );
	glUniform1i(shaderinfo->ltex,0);

	if( shadowTex )
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, shadowTex->bufID );
		glUniform1i(shaderinfo->lShadowTex,1);
	}
	return 0;
}

//////////////////////////////
//---------------------TEXUNLIT
//////////////////////////////
shaderInfo_TexUnlit* JProgramManager::setProgram_TexUnlit(const string& name, char* vpath, char* fpath)
{
	shaderInfo_TexUnlit* info = new shaderInfo_TexUnlit();

	if(makeVertexShader(vpath,NULL, info->v) != 0)
		return NULL;
	if(makeFragmentShader(fpath,NULL, info->f) != 0)
		return NULL;
	if(makeProgram(info->v, info->f, info->p) != 0)
		return NULL;

	/*GLuint uniformBlockIndex;

	uniformBlockIndex = glGetUniformBlockIndex(info->p, "Uniforms");
	glGetActiveUniformBlockiv(info->p, uniformBlockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &(info->uniformBlockSize));
	info->buffer = malloc(info->uniformBlockSize);

	glGetUniformIndices(info->p, info->uniformVarNum, info->uniformVarNames, info->uniformVarIndices);
	glGetActiveUniformsiv(info->p, info->uniformVarNum, info->uniformVarIndices, GL_UNIFORM_OFFSET, info->uniformVarOffset);
	glGetActiveUniformsiv(info->p, info->uniformVarNum, info->uniformVarIndices, GL_UNIFORM_SIZE, info->uniformVarSize);
	glGetActiveUniformsiv(info->p, info->uniformVarNum, info->uniformVarIndices, GL_UNIFORM_TYPE, info->uniformVarType);

	glGenBuffers(1, &(info->uniformBlockBufferObject));
	glBindBuffer(GL_UNIFORM_BUFFER, info->uniformBlockBufferObject);
	glBufferData(GL_UNIFORM_BUFFER, info->uniformBlockSize, info->buffer, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, uniformBlockIndex, info->uniformBlockBufferObject);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);*/
	info->lmvp = 4;
	info->ltex = 5;

	if(info->lmvp == -1 || info->ltex == -1)
		return NULL;

	if(info->p > 0)
	{
		info->name = name;
		programs[name] = info;
	}

	return info;
}

int JProgramManager::setUniformVariables_TexUnlit( JMatrix44 mvp, JTextureObject* aTex )
{
	if(JProgramManager::currentlyRunningInfo->shaderKind != JSHADERKIND_TEXUNLIT)
		return -1;

	shaderInfo_TexUnlit* shaderinfo = (shaderInfo_TexUnlit*)JProgramManager::currentlyRunningInfo;

	/*size_t size;
	size = shaderinfo->uniformVarSize[0] * TypeSize(shaderinfo->uniformVarType[0]);
	memcpy( (char*)(shaderinfo->buffer) + shaderinfo->uniformVarOffset[0], &mvp,size);

	glBindBuffer(GL_UNIFORM_BUFFER, shaderinfo->uniformBlockBufferObject);
	glBufferData(GL_UNIFORM_BUFFER, shaderinfo->uniformBlockSize, shaderinfo->buffer, GL_DYNAMIC_DRAW);
	*/
	
	glUniformMatrix4fv(shaderinfo->lmvp, 1, GL_TRUE, (GLfloat*)(&mvp));
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, aTex->bufID );
	glUniform1i(shaderinfo->ltex,0);

	return 0;
}

//////////////////////////////
//---------------------DIFFUSE
//////////////////////////////
shaderInfo_Diffuse* JProgramManager::setProgram_Diffuse(const string& name, char* vpath, char* fpath)
{
	shaderInfo_Diffuse* info = new shaderInfo_Diffuse();

	if(makeVertexShader(vpath,NULL, info->v) != 0)
		return NULL;
	if(makeFragmentShader(fpath,NULL, info->f) != 0)
		return NULL;
	if(makeProgram(info->v, info->f, info->p) != 0)
		return NULL;


	info->lmvp = 4;

	if(info->p > 0)
	{
		programs[name] = info;
		info->name = name;
	}

	return info;
}

int JProgramManager::setUniformVariables_Diffuse( JMatrix44 mvp )
{
	if(JProgramManager::currentlyRunningInfo->shaderKind != JSHADERKIND_DIFFUSE)
		return -1;

	shaderInfo_Diffuse* shaderinfo = (shaderInfo_Diffuse*)JProgramManager::currentlyRunningInfo;
	
	/*size_t size;
	size = shaderinfo->uniformVarSize[0] * TypeSize(shaderinfo->uniformVarType[0]);
	memcpy( (char*)(shaderinfo->buffer) + shaderinfo->uniformVarOffset[0], &mvp,size);

	glBindBuffer(GL_UNIFORM_BUFFER, shaderinfo->uniformBlockBufferObject);
	glBufferData(GL_UNIFORM_BUFFER, shaderinfo->uniformBlockSize, shaderinfo->buffer, GL_DYNAMIC_DRAW);
*/

	glUniformMatrix4fv(shaderinfo->lmvp, 1, GL_TRUE, (GLfloat*)(&mvp));

	return 0;
}


//////////////////////////////
//---------------------shadow
//////////////////////////////
shaderInfo_DirShadow* JProgramManager::setProgram_DirShadow(const string& name, char* vpath, char* fpath)
{
	shaderInfo_DirShadow* info = new shaderInfo_DirShadow();

	if(makeVertexShader(vpath,NULL, info->v) != 0)
		return NULL;
	if(makeFragmentShader(fpath,NULL, info->f) != 0)
		return NULL;
	if(makeProgram(info->v, info->f, info->p) != 0)
		return NULL;


	info->lmvp = 4;

	if(info->p > 0)
	{
		programs[name] = info;
		info->name = name;
	}

	return info;
}

int JProgramManager::setUniformVariables_DirShadow( JMatrix44 mvp )
{
	if(JProgramManager::currentlyRunningInfo->shaderKind != JSHADERKIND_DIRSHADOW)
		return -1;

	shaderInfo_DirShadow* shaderinfo = (shaderInfo_DirShadow*)JProgramManager::currentlyRunningInfo;
	
	
	glUniformMatrix4fv(shaderinfo->lmvp, 1, GL_TRUE, (GLfloat*)(&mvp));

	return 0;
}

//////////////////////////////
//---------------------shadow
//////////////////////////////
shaderInfo_FinalDeferred* JProgramManager::setProgram_FinalDeferred(const string& name, char* vpath, char* fpath)
{
	shaderInfo_FinalDeferred* info = new shaderInfo_FinalDeferred();

	if(makeVertexShader(vpath,NULL, info->v) != 0)
		return NULL;
	if(makeFragmentShader(fpath,NULL, info->f) != 0)
		return NULL;
	if(makeProgram(info->v, info->f, info->p) != 0)
		return NULL;


	info->lmvp = 4;
	info->lLightDir = 5;
	info->lDiffuseMap = 6;
	info->lNormalMap = 7;
	info->lPositionMap = 8;
	info->lTextureMap = 9;
	info->lShadowMap = 10;
	info->lEyePos = 11;

	if(info->p > 0)
	{
		programs[name] = info;
		info->name = name;
	}

	return info;
}

int JProgramManager::setUniformVariables_FinalDeferred( JMatrix44 mvp, JVector3 eyepos, JVector3 lightDir, JTextureObject* diffuseMap, JTextureObject* normalMap, JTextureObject* positionMap, JTextureObject* textureMap, JTextureObject* shadowMap )
{
	if(JProgramManager::currentlyRunningInfo->shaderKind != JSHADERKIND_FINALDEFERRED)
		return -1;

	shaderInfo_FinalDeferred* shaderinfo = (shaderInfo_FinalDeferred*)JProgramManager::currentlyRunningInfo;
	

	glUniformMatrix4fv(shaderinfo->lmvp, 1, GL_TRUE, (GLfloat*)(&mvp));
	glUniform4f( shaderinfo->lLightDir, lightDir[0], lightDir[1], lightDir[2], 0 );

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap->bufID );
	glUniform1i(shaderinfo->lDiffuseMap,0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normalMap->bufID );
	glUniform1i(shaderinfo->lNormalMap,1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, positionMap->bufID );
	glUniform1i(shaderinfo->lPositionMap,2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, textureMap->bufID );
	glUniform1i(shaderinfo->lTextureMap,3);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, shadowMap->bufID );
	glUniform1i(shaderinfo->lShadowMap,4);

	glUniform4f( shaderinfo->lEyePos, eyepos[0], eyepos[1], eyepos[2], 0 );
	return 0;
}