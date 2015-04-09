#ifndef JTextureManager_h
#define JTextureManager_h
//#define GLEW_STATIC
#include <GL/glew.h>

#include <map>
#include <string>


using namespace std;

class JTextureObject
{
public:
	int width;
	int height;
	GLuint bufID;
	JTextureObject();
};

enum JTEXTURE_KINDS { JTEXTUREKIND_VECTOR, JTEXTUREKIND_COLOR, JTEXTUREKIND_DEPTH };

enum JTEXTURE_IMAGEFORMAT { JIMGFORMAT_BMP };

class JTextureManager
{
public:
	
	int makeTexture( JTextureObject& texObj, GLsizei width, GLsizei height, JTEXTURE_KINDS kind );
	int makeTexture( JTextureObject& texObj, JTEXTURE_IMAGEFORMAT imgFmt, string path);

	int deleteTexture( JTextureObject& texObj );

	JTextureManager(){};
};



#endif
