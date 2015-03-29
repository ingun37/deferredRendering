#include <math.h>
#include <time.h>

#include "JShape.h"

#define sphereVertexNumForTex(S) ((2 + S)*((2*S) + 2 + 1))
#define sphereSliceNumForTex(S) (S*2+2 + 1)
#define sphereFaceNumForTex(S) (( 2*S + 2 )*(S+2-1)*2)
#define sphereIndexNumForTex(S) (sphereFaceNumForTex(S)*3)
#define PI 3.141592f

int makeSphere( float radius, unsigned int smoothness, JMesh& mesh )
{
	unsigned long pnum = sphereVertexNumForTex(smoothness);
	unsigned long slicenum = sphereSliceNumForTex(smoothness);
	unsigned long idxnum = sphereIndexNumForTex(smoothness);

	JVertex* tmpvertices = new JVertex[pnum];
	unsigned int *tmpIndices = new unsigned int[idxnum];

	float tmpangle, tmpradius, tmpheight;

	tmpangle = PI/(smoothness+1);

	try
	{
		for(unsigned int i=0;i<smoothness + 2;i++)
		{
			tmpradius = sinf(tmpangle * i) * radius;
			tmpheight = cosf(tmpangle * i) * radius;
			for(unsigned int j=0;j<slicenum;j++)
			{
				
				tmpvertices[i*slicenum + j].position[0] = cosf(tmpangle * j) * tmpradius;
				tmpvertices[i*slicenum + j].position[1] = tmpheight;
				tmpvertices[i*slicenum + j].position[2] = sinf(tmpangle * j) * tmpradius;
				
						
				tmpvertices[i*slicenum + j].normal[0] = tmpvertices[i*slicenum + j].position[0]/radius;
				tmpvertices[i*slicenum + j].normal[1] = tmpvertices[i*slicenum + j].position[1]/radius;
				tmpvertices[i*slicenum + j].normal[2] = tmpvertices[i*slicenum + j].position[2]/radius;
				
						
				tmpvertices[i*slicenum + j].uv[0] = ((float)slicenum-1)/j;
				tmpvertices[i*slicenum + j].uv[1] = ((float)smoothness+2-1)/i;

				tmpvertices[i*slicenum + j].diffuse[0] = 0;
				tmpvertices[i*slicenum + j].diffuse[1] = 1;
				tmpvertices[i*slicenum + j].diffuse[2] = sinf(tmpangle * j);
				tmpvertices[i*slicenum + j].diffuse[3] = 1;
			}
		}

		for(unsigned int i=0;i<smoothness+2-1;i++)
		{
			for(unsigned int j=0;j<slicenum-1;j++)
			{
					tmpIndices[6*(i*(slicenum-1)+j) + 0] = i * slicenum + j;
					tmpIndices[6*(i*(slicenum-1)+j) + 1] = i * slicenum + (j+1);
					tmpIndices[6*(i*(slicenum-1)+j) + 2] = (i+1) * slicenum + j;
						
					tmpIndices[6*(i*(slicenum-1)+j) + 3] = i * slicenum + j+1;
					tmpIndices[6*(i*(slicenum-1)+j) + 4] = (i+1) * slicenum + (j+1);
					tmpIndices[6*(i*(slicenum-1)+j) + 5] = (i+1) * slicenum + j;
			}
		}

		for(unsigned int i=0;i<idxnum;i++)
		{
			if( tmpIndices[i] >= pnum )
				throw;

			mesh.pushVertex( tmpvertices[ tmpIndices[i] ] );
		}
	}
	catch(...)
	{
		puts(" make Sphere ");
		delete tmpvertices;
		delete tmpIndices;
		return -1;
	}
	
	delete tmpvertices;
	delete tmpIndices;
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
	JVector3 rotAxis = normalInit / normal;
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
		mesh.pushVertex( vertices[ indices[i] ] );
	}
	delete [] indices;
	delete [] vertices;
	return 0;
}