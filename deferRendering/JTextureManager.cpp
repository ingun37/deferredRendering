#include "JTextureManager.h"
#include <iostream>
JTextureObject::JTextureObject():bufID(-1),width(0),height(0)
{
}

int JTextureManager::makeTexture( JTextureObject& texObj, GLsizei width, GLsizei height, JTEXTURE_KINDS kind )
{
	GLuint textureID;
	int internalFormat;
	int justFormat;
	switch( kind )
	{
	case JTEXTUREKIND_VECTOR:
		internalFormat = GL_RGBA32F;
		justFormat = GL_RGBA;
		break;
	case JTEXTUREKIND_COLOR:
		internalFormat = GL_RGBA8;
		justFormat = GL_RGBA;
		break;
	case JTEXTUREKIND_DEPTH:
		internalFormat = GL_DEPTH_COMPONENT32F;
		justFormat = GL_DEPTH_COMPONENT;
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
	
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	
	glTexStorage2D(GL_TEXTURE_2D,1,internalFormat,width,height);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	
	texObj.bufID= textureID;
	texObj.width = width;
	texObj.height = height;
	return 0;
}

int JTextureManager::makeTexture(JTextureObject& texObj, JTEXTURE_IMAGEFORMAT imgFmt, string path)
{//24 bmp only
	int i;
	FILE* f = fopen(path.c_str(), "rb");

	if(f == NULL)
		return -1;

	unsigned char info[54];
	fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

	// extract image height and width from header
	int width = *(int*)&info[18];
	int height = *(int*)&info[22];

	std::cout << "reading bmp file..." << endl;
	std::cout << "width: " << width << endl;
	std::cout << "height: " << height << endl;

	int row_padded = (width*3 + 3) & (~3);
	unsigned char* data = new unsigned char[row_padded];
	unsigned char tmp;

	GLubyte *texData = new GLubyte[width*height*4];

	for(int i = 0; i < height; i++)
	{
		fread(data, sizeof(unsigned char), row_padded, f);
		for(int j = 0; j < width*3; j += 3)
		{
			texData[i*(width*4) + 4*j/3 + 0] = data[j+2];
			texData[i*(width*4) + 4*j/3 + 1] = data[j+1];
			texData[i*(width*4) + 4*j/3 + 2] = data[j+0];
			texData[i*(width*4) + 4*j/3 + 3] = 255;
		}
	}
	delete [] data;
	fclose(f);


	glGenTextures(1,&texObj.bufID);
	glBindTexture(GL_TEXTURE_2D, texObj.bufID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,texData);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	delete [] texData;

	texObj.width = width;
	texObj.height = height;

	return 0;
}

int JTextureManager::deleteTexture( JTextureObject& texObj )
{
	glDeleteTextures(1, &texObj.bufID );
	return 0;
}