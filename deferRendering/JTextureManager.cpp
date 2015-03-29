#include "JTextureManager.h"

JTextureObject::JTextureObject()
{
	bufID = -1;
}
static GLubyte imginfos[640*480*4];
int JTextureManager::makeTexture( JTextureObject& texObj, GLsizei width, GLsizei height, JTEXTURE_KINDS kind, bool fillWithRandomColor )
{
	GLuint textureID;
	int internalFormat;
	int justFormat;
	switch( kind )
	{
	case JTEXTUREKIND_COLOR:
		internalFormat = GL_RGBA8;
		justFormat = GL_RGBA;
		break;
	case JTEXTUREKIND_DEPTH:
		internalFormat = GL_DEPTH_COMPONENT16;
		justFormat = GL_DEPTH_COMPONENT16;
		break;
	default:
		return -1;
		break;
	}
	GLubyte* randomPixelDatas = new GLubyte[ width * height * 4 ];

	for(int i=0;i<width*height;i++)
	{
		randomPixelDatas[i*4 + 0] = 0;//rand() % 255;
		randomPixelDatas[i*4 + 1] = 0;//rand() % 255;
		randomPixelDatas[i*4 + 2] = 255;//rand() % 255;
		randomPixelDatas[i*4 + 3] = 255;//rand() % 255;
	}

	glGenTextures(1,&textureID);
	if(glGetError() != GL_NO_ERROR)
	{
		fillWithRandomColor = fillWithRandomColor;
	}
	glBindTexture(GL_TEXTURE_2D, textureID);

	if(glGetError() != GL_NO_ERROR)
	{
		fillWithRandomColor = fillWithRandomColor;
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	if( fillWithRandomColor )
	{
		
		glTexImage2D(GL_TEXTURE_2D,0,internalFormat,width,height,0,justFormat,GL_UNSIGNED_BYTE,randomPixelDatas);
		if(int theerror = glGetError())
		{
			fillWithRandomColor = fillWithRandomColor;
		}
		delete [] randomPixelDatas;
	}
	else
	{
		glTexStorage2D(GL_TEXTURE_2D,1,internalFormat,width,height);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	if(glGetError() != GL_NO_ERROR)
	{
		fillWithRandomColor = fillWithRandomColor;
	}
	
	JTextureObject* tObj = new JTextureObject();
	tObj->bufID = textureID;
	
	texObj.bufID= textureID;
	
	return 0;
}

int JTextureManager::deleteTexture( JTextureObject& texObj )
{
	glDeleteTextures(1, &texObj.bufID );
	return 0;
}