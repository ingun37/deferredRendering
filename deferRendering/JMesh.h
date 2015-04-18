#ifndef JMesh_h
#define JMesh_h

#include "JLinearAlgebra.h"
#include "JMaterial.h"
#include "JVertex.h"

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
	int clear();
	JVBO():vbo(0),vao(0),structSize(0),vertexNum(0){};
};

class JIBO
{
public:
	unsigned int ibo;
	unsigned int indexNum;
	int allocIBO( int aIndexNum );
	int setIBO( unsigned int* data );
	int clear();
	JIBO():ibo(0),indexNum(0){};
};

class JMesh
{
public:
	int tag;

	vector<JVertex> vertices;
	char* vbData;
	vector<unsigned int> indices;
	JMaterial* material;
	JVBO jvbo;
	JIBO jibo;

	JVector4 position;

	JMesh();

	int reserveVertexLen( const unsigned int len );
	int pushVertex( const JVertex&  vertex);
	int reserveIndexLen( const unsigned int len );
	int pushIndex( const unsigned int index);
	JMaterial* getMaterial();
	int setMaterial( JMaterial* mat );
	
	int refreshVertexIndexBuffer();
	int resetVBData();
	int rearrangeData();
	int resetSubData(JVERTEXATTRIBUTE att, unsigned int idx, void* data);
	int draw();
	
	int clearGLBuffers();
	int clearData();
	int clearAll();
};

#endif
