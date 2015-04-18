#include <math.h>
#include <time.h>

#include "JShape.h"

#define sphereVertexNumForTex(S) ((2 + S)*((2*S) + 2 + 1))
#define sphereSliceNumForTex(S) (S*2+2 + 1)
#define sphereFaceNumForTex(S) (( 2*S + 2 )*(S+2-1)*2)
#define sphereIndexNumForTex(S) (sphereFaceNumForTex(S)*3)
#define PI 3.141592f

//TODO : lets prevent below pool overflow.
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

			vertices[idx].normal[0] = normal[0];
			vertices[idx].normal[1] = normal[1];
			vertices[idx].normal[2] = normal[2];

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

#define cylinderCNum(SM) ((SM*2)+3)
#define cylinderHNum(SG) (SG+1)
#define cylinderVNum(SM,SG) ((cylinderCNum(SM)*cylinderHNum(SG)) + (2*(cylinderCNum(SM)-1)))
#define cylinderINum(SM,SG) ((((cylinderCNum(SM)-1)*(SG))*6) + (((cylinderCNum(SM)-3)*3)*2))
int makeCylinder( float radius, unsigned int smoothness, float length, unsigned int segment, JMesh& mesh, float yBase )
{
	unsigned long pnum = cylinderVNum(smoothness,segment);
	unsigned long idxnum = cylinderINum(smoothness,segment);
	unsigned int verticalNum = cylinderHNum(segment);
	unsigned int circlenum = cylinderCNum(smoothness);

	mesh.clearAll();
	mesh.reserveVertexLen(pnum);
	mesh.reserveIndexLen(idxnum);

	JVertex* tmpvertices = vertexPool; //new JVertex[pnum]
	unsigned int *tmpIndices = indexPool ;//new unsigned int[idxnum];

	float tmpangle, tmpheight;

	tmpangle = PI/(smoothness+1);

	int cnt = 0;
	try
	{
		for (unsigned int i=0;i<circlenum-1;i++)
		{
			tmpvertices[cnt].position[0] = cosf(tmpangle * i) * radius;
			tmpvertices[cnt].position[1] = -length/2 + yBase;
			tmpvertices[cnt].position[2] = sinf(tmpangle * i) * radius;

			tmpvertices[cnt].normal[0] = 0;
			tmpvertices[cnt].normal[1] = -1;
			tmpvertices[cnt].normal[2] = 0;

			tmpvertices[cnt].uv[0] = 0;
			tmpvertices[cnt].uv[1] = 0;

			tmpvertices[cnt].diffuse[0] = 0;
			tmpvertices[cnt].diffuse[1] = 0;
			tmpvertices[cnt].diffuse[2] = 0;
			tmpvertices[cnt].diffuse[3] = 1;
			cnt++;
		}
		for(unsigned int i=0;i<verticalNum;i++)
		{
			tmpheight = -(length/2) + ((length/(verticalNum-1))*i) + yBase;
			for(unsigned int j=0;j<circlenum;j++)
			{
				tmpvertices[cnt].position[0] = cosf(tmpangle * j) * radius;
				tmpvertices[cnt].position[1] = tmpheight;
				tmpvertices[cnt].position[2] = sinf(tmpangle * j) * radius;

				tmpvertices[cnt].normal[0] = tmpvertices[cnt].position[0]/radius;
				tmpvertices[cnt].normal[1] = 0;
				tmpvertices[cnt].normal[2] = tmpvertices[cnt].position[2]/radius;

				tmpvertices[cnt].uv[0] = ((float)j)/(circlenum-1);
				tmpvertices[cnt].uv[1] = 1- ((float)i)/(verticalNum-1);

				tmpvertices[cnt].diffuse[0] = 0;
				tmpvertices[cnt].diffuse[1] = 0;
				tmpvertices[cnt].diffuse[2] = 0;
				tmpvertices[cnt].diffuse[3] = 1;
				cnt++;
			}
		}
		for (unsigned int i=0;i<circlenum-1;i++)
		{
			tmpvertices[cnt].position[0] = -cosf(tmpangle * i) * radius;
			tmpvertices[cnt].position[1] = length/2 + yBase;
			tmpvertices[cnt].position[2] = sinf(tmpangle * i) * radius;

			tmpvertices[cnt].normal[0] = 0;
			tmpvertices[cnt].normal[1] = 1;
			tmpvertices[cnt].normal[2] = 0;

			tmpvertices[cnt].uv[0] = 0;
			tmpvertices[cnt].uv[1] = 0;

			tmpvertices[cnt].diffuse[0] = 0;
			tmpvertices[cnt].diffuse[1] = 0;
			tmpvertices[cnt].diffuse[2] = 0;
			tmpvertices[cnt].diffuse[3] = 1;
			cnt++;
		}

		/*//test code..
		JMatrix44 idm = JMatrix44::GetIdentityMatrix();
		for(unsigned int i=0;i<pnum;i++)
			memcpy(tmpvertices[i].skinmat1,&idm,sizeof(JMatrix44));
			//tmpvertices[i].skinmat1 = JMatrix44::GetIdentityMatrix();
		//*/
		if(cnt != pnum)
			puts("odd... not correct");
		cnt = 0;

		for(unsigned int i=0;i<pnum;i++)
			mesh.pushVertex( tmpvertices[i] );

		unsigned int mark = 0;
		for(unsigned int i=0;i<circlenum-1-2;i++)
		{
			tmpIndices[cnt+0] = 0;
			tmpIndices[cnt+1] = i+1;
			tmpIndices[cnt+2] = i+2;
			cnt+=3;
		}
		mark = (circlenum - 1);
		for(unsigned int i=0;i<segment;i++)
		{
			for(unsigned int j=0;j<circlenum-1;j++)
			{
				tmpIndices[cnt + 0] = mark+ i * circlenum + j;
				tmpIndices[cnt + 1] = mark+ i * circlenum + (j+1);
				tmpIndices[cnt + 2] = mark+ (i+1) * circlenum + j;
				tmpIndices[cnt + 3] = mark+ i * circlenum + j+1;
				tmpIndices[cnt + 4] = mark+ (i+1) * circlenum + (j+1);
				tmpIndices[cnt + 5] = mark+ (i+1) * circlenum + j;

				cnt+=6;
			}
		}
		mark = pnum - (circlenum-1);
		for(unsigned int i=0;i<circlenum-1-2;i++)
		{
			tmpIndices[cnt+0] = mark+ 0;
			tmpIndices[cnt+1] = mark+i+1;
			tmpIndices[cnt+2] = mark+i+2;
			cnt+=3;
		}
		if(cnt!=idxnum)
			puts("odd... not correct");
		cnt=0;

		for(unsigned int i=0;i<idxnum;i++)
			mesh.pushIndex( tmpIndices[i] );
	}
	catch(...)
	{
		puts(" make cylinder ");
		//delete tmpvertices;
		//delete tmpIndices;
		return -1;
	}

	//delete [] tmpvertices;
	//delete [] tmpIndices;
	return 0;
}

#define weightEquMid(SG,X) ((1 - abs( (-2.f/(SG))*(X) + 1 ))*0.5 + 0.5) //0.5~1~0.5
#define weightEquPrev(SG,X) ((-1.f/(SG)) * (X) + 0.5) //0.5~0
#define weightEquNext(SG,X) (( 1.f/(SG)) * (X) - 0.5) //0~0.5

int makeSausage(int bonenum, float radius, unsigned int cySegnum, int cySmooth, float cylen, JActor& actor)
{
	

	actor.allBonesUpperBase.reserve(bonenum);
	for(int i=0;i<bonenum;i++)
	{
		JBone tmpBone;
		tmpBone.upperBoneIdx = i-1;
		tmpBone.axis[2] = 1;
		tmpBone.pos[1] = i==0?0:cylen;
		actor.allBonesUpperBase.push_back( tmpBone );
		//TODO : avoid copy
	}

	std::vector<vtxWeight> beginWeightsPerVtxs;
	std::vector<vtxWeight> midWeightsPerVtxs;
	std::vector<vtxWeight> endWeightsPerVtxs;

	unsigned int endNum = cylinderCNum(cySmooth) - 1;
	unsigned int sliceNum = endNum + 1;
	unsigned int cnt=0;

	unsigned int meshVNum = cylinderVNum(cySmooth,cySegnum);
	unsigned int halfMeshVNum = ((cySegnum+1)/2)*sliceNum + endNum;

	beginWeightsPerVtxs.reserve( halfMeshVNum );
	midWeightsPerVtxs.reserve( meshVNum );
	endWeightsPerVtxs.reserve( halfMeshVNum );

	//begining bone cluster..........
	for(int j=0;j<endNum;j++)
		beginWeightsPerVtxs.push_back(vtxWeight(cnt++,0.5));

	for(int j=0;j<((cySegnum+1)/2);j++)
	{
		float weight = weightEquPrev(cySegnum,j);
		for( int k=0;k<sliceNum;k++ )
			beginWeightsPerVtxs.push_back(vtxWeight(cnt++,weight));
	}

	if( halfMeshVNum != cnt )
		puts("err!!!err!!!err!!!");
	cnt=0;

	//mid bone cluster..........
	for(int j=0;j<endNum;j++)
		midWeightsPerVtxs.push_back(vtxWeight(cnt++,0));

	for(int j=0;j<cySegnum+1;j++)
	{
		float weight = weightEquMid(cySegnum,j);
		for( int k=0;k<sliceNum;k++ )
			midWeightsPerVtxs.push_back(vtxWeight(cnt++,weight));
	}

	for(int j=0;j<endNum;j++)
		midWeightsPerVtxs.push_back(vtxWeight(cnt++,0));

	if( meshVNum != cnt )
		puts("err!!!err!!!err!!!2");
	cnt=0;

	//end bone cluster..........
	unsigned int baseCnt = meshVNum - halfMeshVNum;
	for(int j=((cySegnum+1)/2)+1;j<cySegnum+1;j++)
	{
		float weight = weightEquNext(cySegnum,j);
		for( int k=0;k<sliceNum;k++ )
			endWeightsPerVtxs.push_back(vtxWeight(baseCnt + (cnt++),weight));
	}

	for(int j=0;j<endNum;j++)
		endWeightsPerVtxs.push_back(vtxWeight(baseCnt + (cnt++),0.5));

	if( halfMeshVNum != cnt )
		puts("err!!!err!!!err!!!3");

	cnt=0;

	//TODO : consider for sharing skinners for same sausages
	JSkinSkin* skinners = new JSkinSkin[bonenum];
	actor.skinners.reserve(bonenum);
	for(int i=0;i<bonenum;i++)
	{
		JSkinSkin& skinner = skinners[i];
		skinner.meshIdx = i;
		
		skinner.clusters.reserve(3);
		skinner.clusters.push_back( JSkinCluster() );
		skinner.clusters.push_back( JSkinCluster() );
		skinner.clusters.push_back( JSkinCluster() );
		
		//--------
		JSkinCluster& prevSkinCluster = skinner.clusters[0];
		prevSkinCluster.skelIdx = max(0,i-1);
		prevSkinCluster.affectingVtxIdxs.reserve( halfMeshVNum );

		for(int j=0;j<halfMeshVNum;j++)
			prevSkinCluster.affectingVtxIdxs.push_back( beginWeightsPerVtxs[j] );

		//-------
		JSkinCluster& midSkinCluster = skinner.clusters[1];
		midSkinCluster.skelIdx = i;
		midSkinCluster.affectingVtxIdxs.reserve( meshVNum );

		for(int j=0;j<meshVNum;j++)
			midSkinCluster.affectingVtxIdxs.push_back( midWeightsPerVtxs[j] );

		//---------
		JSkinCluster& nextSkinCluster = skinner.clusters[2];
		nextSkinCluster.skelIdx = min(bonenum-1,i+1);
		nextSkinCluster.affectingVtxIdxs.reserve( halfMeshVNum );

		for(int j=0;j<halfMeshVNum;j++)
			nextSkinCluster.affectingVtxIdxs.push_back( endWeightsPerVtxs[j] );

		actor.skinners.push_back(&skinner);
	}

	//getting j2m, m2j bind matrix.
	actor.bindJ2M.reserve(bonenum);
	actor.bindM2J.reserve(bonenum);
	actor.currJ2M.reserve(bonenum);
	for(int i=0;i<bonenum;i++)
	{
		JMatrix44 j2m = JMatrix44::GetIdentityMatrix();
		JMatrix44 m2j = JMatrix44::GetIdentityMatrix();

		int upperBoneIdx = i;
		int currBoneIdx = i;

		while(upperBoneIdx != -1)
		{
			currBoneIdx = upperBoneIdx;
			j2m = j2m * actor.allBonesUpperBase[currBoneIdx].getTransform();
			upperBoneIdx = actor.allBonesUpperBase[currBoneIdx].upperBoneIdx;
		}

		JMatrix44::InverseMatrix( &j2m[0][0], &m2j[0][0] );
		actor.bindJ2M.push_back(j2m);
		actor.bindM2J.push_back(m2j);
		//todo avoid copy
		actor.currJ2M.push_back(JMatrix44::GetIdentityMatrix());
	}

	JMesh* meshes = new JMesh[bonenum];
	actor.meshes.reserve(bonenum);
	for(int i=0;i<bonenum;i++)
	{
		makeCylinder(radius,cySmooth,cylen,cySegnum,meshes[i],cylen/2 + i*cylen);//TODO : optimizing skinning : no yBase could work.
		actor.meshes.push_back(&meshes[i]);
	}

//testcode
	for( int i=0;i<actor.skinners.size() ;i++)
	{
		JSkinSkin& skinner = *actor.skinners[i];

		JMesh& skinningMesh = *actor.meshes[skinner.meshIdx];

		for( int j=0;j<skinner.clusters[2].affectingVtxIdxs.size();j++)
		{
			vtxWeight& vwLink = skinner.clusters[2].affectingVtxIdxs[j];
			//testcode TODO : 
			skinningMesh.vertices[vwLink.vIdx].diffuse[0] = skinningMesh.vertices[vwLink.vIdx].diffuse[1] = skinningMesh.vertices[vwLink.vIdx].diffuse[2] = vwLink.weight;
		}
	}

	for(int i=0;i<bonenum;i++)
	{
		meshes[i].refreshVertexIndexBuffer();
	}

	
	return 0;
}
