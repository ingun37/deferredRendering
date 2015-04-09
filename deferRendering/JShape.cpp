#include <math.h>
#include <time.h>

#include "JShape.h"

#define sphereVertexNumForTex(S) ((2 + S)*((2*S) + 2 + 1))
#define sphereSliceNumForTex(S) (S*2+2 + 1)
#define sphereFaceNumForTex(S) (( 2*S + 2 )*(S+2-1)*2)
#define sphereIndexNumForTex(S) (sphereFaceNumForTex(S)*3)
#define PI 3.141592f

JVertex vertexPool[ sphereVertexNumForTex(500) ];
unsigned int indexPool[ sphereIndexNumForTex(500) ];
int makeSphere( float radius, unsigned int smoothness, JMesh& mesh )
{
	unsigned long pnum = sphereVertexNumForTex(smoothness);
	unsigned long slicenum = sphereSliceNumForTex(smoothness);
	unsigned long idxnum = sphereIndexNumForTex(smoothness);

	mesh.clearAll();
	mesh.reserveVertexLen(pnum);
	mesh.reserveIndexLen(idxnum);

	JVertex* tmpvertices = vertexPool; //new JVertex[pnum]
	unsigned int *tmpIndices = indexPool ;//new unsigned int[idxnum];

	float tmpangle, tmpradius, tmpheight;

	tmpangle = PI/(smoothness+1);

	int cnt = 0;
	try
	{
		for(unsigned int i=0;i<smoothness + 2;i++)
		{
			tmpradius = sinf(tmpangle * i) * radius;
			tmpheight = cosf(tmpangle * i) * radius;
			for(unsigned int j=0;j<slicenum;j++)
			{
				tmpvertices[cnt].position[0] = cosf(tmpangle * j) * tmpradius;
				tmpvertices[cnt].position[1] = tmpheight;
				tmpvertices[cnt].position[2] = sinf(tmpangle * j) * tmpradius;
				
				tmpvertices[cnt].normal[0] = tmpvertices[cnt].position[0]/radius;
				tmpvertices[cnt].normal[1] = tmpvertices[cnt].position[1]/radius;
				tmpvertices[cnt].normal[2] = tmpvertices[cnt].position[2]/radius;
						
				tmpvertices[cnt].uv[0] = ((float)j)/(slicenum-1);
				tmpvertices[cnt].uv[1] = 1- ((float)i)/(smoothness+2-1);

				tmpvertices[cnt].diffuse[0] = 0;
				tmpvertices[cnt].diffuse[1] = 1;
				tmpvertices[cnt].diffuse[2] = sinf(tmpangle * j);
				tmpvertices[cnt].diffuse[3] = 1;

				cnt++;
			}
		}

		if(cnt != pnum)
			puts("odd... not correct");
		cnt = 0;

		for(unsigned int i=0;i<pnum;i++)
			mesh.pushVertex( tmpvertices[i] );

		for(unsigned int i=0;i<smoothness+2-1;i++)
		{
			for(unsigned int j=0;j<slicenum-1;j++)
			{
				tmpIndices[cnt + 0] = i * slicenum + j;
				tmpIndices[cnt + 1] = i * slicenum + (j+1);
				tmpIndices[cnt + 2] = (i+1) * slicenum + j;
				tmpIndices[cnt + 3] = i * slicenum + j+1;
				tmpIndices[cnt + 4] = (i+1) * slicenum + (j+1);
				tmpIndices[cnt + 5] = (i+1) * slicenum + j;

				cnt+=6;
			}
		}
		if(cnt!=idxnum)
			puts("odd... not correct");
		cnt=0;

		for(unsigned int i=0;i<idxnum;i++)
			mesh.pushIndex( tmpIndices[i] );
	}
	catch(...)
	{
		puts(" make Sphere ");
		//delete tmpvertices;
		//delete tmpIndices;
		return -1;
	}
	
	//delete [] tmpvertices;
	//delete [] tmpIndices;
	return 0;
}


/*
      y
      ^
      |
-----------
\     |____\___> x
 \          \
  \__________\
*/
int makePlane(float widthLen, float heightLen, unsigned int widthSeg, unsigned int heightSeg, const JVector3& normal, JMesh& mesh, int isClockwise)
{
	int i, j;
	int widthvnum = widthSeg + 1;
	int heightvnum = heightSeg + 1;
	unsigned int vnum = widthvnum * heightvnum;
	JVertex *vertices = new JVertex[vnum];
	JMatrix44 rotMat;

	JVector3 normalInit;
	normalInit[0]=0;
	normalInit[1]=1;
	normalInit[2]=0;
	JVector3 rotAxis = normal / normalInit;
	float rotAngle;

	rotAngle = normal.angleBetween( normalInit );

	rotMat = JMatrix44::GetRotateMatrix( rotAxis[0], rotAxis[1], rotAxis[2], rotAngle );

	for (i=0; i<heightvnum; i++)
	{
		for(j=0;j<widthvnum;j++)
		{
			int idx = i*widthvnum + j;
			JVector3 positionInit;
			positionInit[0] = j*(widthLen/widthSeg) - widthLen/2;
			positionInit[1] = 0;
			positionInit[2] = i*(heightLen/heightSeg) - heightLen/2;

			JVector4 tmpPos4 = rotMat * positionInit.getContravariantVector();
			vertices[idx].position[0] = tmpPos4[0];
			vertices[idx].position[1] = tmpPos4[1];
			vertices[idx].position[2] = tmpPos4[2];

			vertices[idx].diffuse[0] = ((float)(rand()%10))/10;
			vertices[idx].diffuse[1] = ((float)(rand()%10))/10;
			vertices[idx].diffuse[2] = ((float)(rand()%10))/10;
			vertices[idx].diffuse[3] = 1;

			vertices[idx].normal = normal;

			vertices[idx].uv[0] = ((float)j)/(widthvnum-1);
			vertices[idx].uv[1] = ((float)i)/(heightvnum-1);

			mesh.pushVertex( vertices[idx] );
		}
	}
	unsigned int numIdx = widthSeg * heightSeg * 6;
	unsigned int *indices = new unsigned int[numIdx];
		
	for (i=0; i<heightSeg; i++)
	{
		for(j=0;j<widthSeg;j++)
		{
			if(isClockwise)
			{
				indices[6*(i*widthSeg + j) + 0] = (i+1)*widthvnum + j;
				indices[6*(i*widthSeg + j) + 1] = i*widthvnum + j;
				indices[6*(i*widthSeg + j) + 2] = i*widthvnum + j+1;
				indices[6*(i*widthSeg + j) + 3] = i*widthvnum + j+1;
				indices[6*(i*widthSeg + j) + 4] = (i+1)*widthvnum + j+1;
				indices[6*(i*widthSeg + j) + 5] = (i+1)*widthvnum + j;
			}
			else
			{
				indices[6*(i*widthSeg + j) + 0] = (i+1)*widthvnum + j;
				indices[6*(i*widthSeg + j) + 1] = (i+1)*widthvnum + j+1;
				indices[6*(i*widthSeg + j) + 2] = i*widthvnum + j+1;
				indices[6*(i*widthSeg + j) + 3] = i*widthvnum + j+1;
				indices[6*(i*widthSeg + j) + 4] = i*widthvnum + j;
				indices[6*(i*widthSeg + j) + 5] = (i+1)*widthvnum + j;
			}
		}
	}

	for(i=0;i<numIdx;i++)
	{
		if( indices[i] >= vnum )
		{
			
			delete [] indices;
			delete [] vertices;
			return -1;
		}
		mesh.pushIndex( indices[i] );
	}
	delete [] indices;
	delete [] vertices;
	return 0;
}