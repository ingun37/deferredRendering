#ifndef JMesh_h
#define JMesh_h

#include "JLinearAlgebra.h"
#include "JMaterial.h"

#include <vector>
#include <string>

using namespace std;
class JVBO
{
public:
	unsigned int vbo;
	unsigned int vao;
	unsigned int structSize;
	unsigned int vertexNum;
	int allocVBO( int aVertexNum, int aStructSize );
	int setVBO( void* data );
	JVBO();
};

class JIBO
{
public:
	unsigned int ibo;
	unsigned int indexNum;
	int allocIBO( int aIndexNum );
	int setIBO( unsigned int* data );
	JIBO();
};

class JMesh
{
public:
	int tag;
	vector<JVertex> vertices;
	vector<unsigned int> indices;
	JMaterial* material;
	JVBO* jvbo;
	JIBO* jibo;

	JVector4 position;

	JMesh();

	int pushVertex( const JVertex&  vertex);
	int pushIndex( const unsigned int index);
	JMaterial* getMaterial();
	int setMaterial( JMaterial* mat );
	
	int refreshVertexIndexBuffer();
	int draw();
	
};

#endif
