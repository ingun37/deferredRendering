#pragma once
typedef struct JVertexAttributeInfoRaw
{
	int location;
	int elementnum;
	int willNormalize;
	int offset;
	int stride;
}JVertexAttributeInfo;

enum JVERTEXATTRIBUTE
{
	JVERTEXATTPOSITION,
	JVERTEXATTNORMAL,
	JVERTEXATTUV,
	JVERTEXATTDIFFUSE,

	JVERTEXATTSKINMAT1_R1,
	JVERTEXATTSKINMAT1_R2,
	JVERTEXATTSKINMAT1_R3,
	JVERTEXATTSKINMAT1_R4,

	JVERTEXATTSKINMAT2_R1,
	JVERTEXATTSKINMAT2_R2,
	JVERTEXATTSKINMAT2_R3,
	JVERTEXATTSKINMAT2_R4,

	JVERTEXATTSKINMAT3_R1,
	JVERTEXATTSKINMAT3_R2,
	JVERTEXATTSKINMAT3_R3,
	JVERTEXATTSKINMAT3_R4,
	JVERTEXATTNUM
};

#define defAtt(elenum,off,bNormalize)\
	fixedparameters[locationCnt].location = locationCnt;\
	fixedparameters[locationCnt].elementnum = (elenum);\
	fixedparameters[locationCnt].willNormalize = (bNormalize);\
	fixedparameters[locationCnt].offset = (off);\
	fixedparameters[locationCnt].stride = sizeof(JVertex);\
	locationCnt++;

class JVertex
{
public:
	//char data[JVTX_VERTEX_SIZE];
	float position[3];
	float normal[3];
	float uv[2];
	float diffuse[4];
	float skinmat1[16];
	float skinmat2[16];
	float skinmat3[16];

	static JVertexAttributeInfo* getFixedVertexAttributeInfoArray(int i)
	{
		static JVertexAttributeInfo fixedparameters[JVERTEXATTNUM];
		static int inited = 0;
		if(inited == 0)
		{
			int locationCnt = 0;
			JVertex tmpv;

			defAtt(	3, (int)tmpv.position - (int)(&tmpv),0);
			defAtt(	3, (int)tmpv.normal - (int)(&tmpv),1);
			defAtt(	2, (int)tmpv.uv - (int)(&tmpv),0);
			defAtt(	4, (int)tmpv.diffuse - (int)(&tmpv),0);

			defAtt(	4, (int)tmpv.skinmat1 - (int)(&tmpv)					,0);
			defAtt(	4, (int)tmpv.skinmat1 - (int)(&tmpv) + sizeof(float)*4	,0);
			defAtt(	4, (int)tmpv.skinmat1 - (int)(&tmpv) + sizeof(float)*8	,0);
			defAtt(	4, (int)tmpv.skinmat1 - (int)(&tmpv) + sizeof(float)*12	,0);

			defAtt( 4, (int)tmpv.skinmat2 - (int)(&tmpv)					,0);
			defAtt( 4, (int)tmpv.skinmat2 - (int)(&tmpv) + sizeof(float)*4	,0);
			defAtt( 4, (int)tmpv.skinmat2 - (int)(&tmpv) + sizeof(float)*8	,0);
			defAtt( 4, (int)tmpv.skinmat2 - (int)(&tmpv) + sizeof(float)*12	,0);

			defAtt( 4, (int)tmpv.skinmat3 - (int)(&tmpv)					,0);
			defAtt( 4, (int)tmpv.skinmat3 - (int)(&tmpv) + sizeof(float)*4	,0);
			defAtt( 4, (int)tmpv.skinmat3 - (int)(&tmpv) + sizeof(float)*8	,0);
			defAtt( 4, (int)tmpv.skinmat3 - (int)(&tmpv) + sizeof(float)*12	,0);
			inited = 1;
		}
		if(i >= 0 && i<JVERTEXATTNUM)
			return &(fixedparameters[i]);
		return 0;
	}

	JVertex(){};
};

/*original
class JVertex
{
public:
	JVector3 position;
	JVector3 normal;
	JVector2 uv;
	JVector4 diffuse;
	JMatrix44 skinmat[3];

	static JVertexAttributeInfo* getFixedVertexAttributeInfoArray(int i);

	JVertex( float x, float y, float z, float nx, float ny, float nz, float r, float g, float b, float a, float u, float v);
	JVertex();
};*/