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
	//TODO must hold string key
	GLuint bufID;
	JTextureObject();
};

enum JTEXTURE_KINDS { JTEXTUREKIND_COLOR, JTEXTUREKIND_DEPTH };

class JTextureManager
{
public:
	
	int makeTexture( JTextureObject& texObj, GLsizei width, GLsizei height, JTEXTURE_KINDS kind, bool fillWithRandomColor = false );

	JTextureObject* getTexture( const string& key );
	JTextureObject* getTexture( const GLuint bufID );
	

	int deleteTexture( JTextureObject& texObj );

	JTextureManager(){};

	static JTextureManager* Inst()
	{
		static JTextureManager* instance = NULL;
		if(instance == NULL)
			instance = new JTextureManager();
		return instance;
	}
};



#endif
