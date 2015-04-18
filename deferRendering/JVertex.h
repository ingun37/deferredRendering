#pragma once
typedef struct JVertexAttributeInfoRaw
{
	int location;
	int elementnum;
	int elementSize;
	int nameitSize;
	int chunkSize;
	int willNormalize;
	int offsetBuffer;
	int offsetObject;
	int offsetFirst;
	int stride;
}JVertexAttributeInfo;

#define JVERTEXSKINMATNUM 3
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

#define JSIZE_FLOAT 4
const int tableElementSize[JVERTEXATTNUM]   = {
	JSIZE_FLOAT, JSIZE_FLOAT, JSIZE_FLOAT, JSIZE_FLOAT,
	JSIZE_FLOAT, JSIZE_FLOAT, JSIZE_FLOAT, JSIZE_FLOAT,
	JSIZE_FLOAT, JSIZE_FLOAT, JSIZE_FLOAT, JSIZE_FLOAT,
	JSIZE_FLOAT, JSIZE_FLOAT, JSIZE_FLOAT, JSIZE_FLOAT
};
const int tableElementNum[JVERTEXATTNUM]    = { 3, 3, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};
const int tableNormalize[JVERTEXATTNUM]     = { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int tableZeroMemory[JVERTEXATTNUM]	= { 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
const int tableNthSkinMatAttribute[JVERTEXSKINMATNUM][4] = {
	{JVERTEXATTSKINMAT1_R1, JVERTEXATTSKINMAT1_R2, JVERTEXATTSKINMAT1_R3, JVERTEXATTSKINMAT1_R4},
	{JVERTEXATTSKINMAT2_R1, JVERTEXATTSKINMAT2_R2, JVERTEXATTSKINMAT2_R3, JVERTEXATTSKINMAT2_R4},
	{JVERTEXATTSKINMAT3_R1, JVERTEXATTSKINMAT3_R2, JVERTEXATTSKINMAT3_R3, JVERTEXATTSKINMAT3_R4},
};


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

	//fixit : gl type is fixed to float. fixit
	static JVertexAttributeInfo* getFixedVertexAttributeInfoArray(int atti, unsigned int vNum)
	{
		static JVertexAttributeInfo fixedparameters[JVERTEXATTNUM];
		static int inited = 0;
		if(inited == 0)
		{
			JVertex tmpv;
			int offsetInObj = 0;
			for(int i=0;i<JVERTEXATTNUM;i++)
			{
				fixedparameters[i].location = i;
				fixedparameters[i].elementnum = tableElementNum[i];
				fixedparameters[i].elementSize = tableElementSize[i];
				fixedparameters[i].willNormalize = tableNormalize[i];
				fixedparameters[i].nameitSize = tableElementSize[i] * tableElementNum[i];
				fixedparameters[i].offsetObject = offsetInObj;
				
				offsetInObj += tableElementNum[i]*tableElementSize[i];
				
				fixedparameters[i].offsetBuffer = 0;
				fixedparameters[i].stride = 0;
				fixedparameters[i].chunkSize = tableElementSize[i] * tableElementNum[i];
			}
			inited = 1;
		}

		unsigned int accuOffset = 0;
		for(int i=0;i<JVERTEXATTNUM;i++)
		{
			fixedparameters[i].offsetBuffer = accuOffset;
			fixedparameters[i].offsetFirst = accuOffset;
			accuOffset += tableElementNum[i] * tableElementSize[i] * vNum;
		}

		if(atti >= 0 && atti<JVERTEXATTNUM)
			return &(fixedparameters[atti]);
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