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

class JMesh
{
public:
	int tag;
	vector<JVertex> vertices;
	
	JMaterial* material;
	JVBO* jvbo;

	JVector4 position;

	JMesh();

	int pushVertex( const JVertex&  vertex);
	JMaterial* getMaterial();
	int setMaterial( JMaterial* mat );
	
	int refreshVBO();
	int draw();
	
};

#endif
