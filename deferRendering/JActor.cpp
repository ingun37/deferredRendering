#include "JActor.h"


JActor::JActor(void)
{
}


JActor::~JActor(void)
{
}

int JActor::updateActor()
{
	for(int i=0;i<allBonesUpperBase.size();i++)
	{
		int upperBoneIdx = i;
		int currBoneIdx = i;

		//todo avoid copy
		//todo rename this var
		JMatrix44 &matName = currJ2M[i];
		matName = JMatrix44::GetIdentityMatrix();

		while(upperBoneIdx != -1)
		{
			currBoneIdx = upperBoneIdx;
			matName = allBonesUpperBase[currBoneIdx].getTransform() * matName;
			upperBoneIdx = allBonesUpperBase[currBoneIdx].upperBoneIdx;
		}
	}

	for( int i=0;i<skinners.size();i++ )
	{
		JSkinSkin& skinner = *skinners[i];
		JMesh& skinningMesh = *meshes[skinner.meshIdx];
		//for (int j=0;j<skinner.clusters.size();j++)
		{
			JSkinCluster& cluster = skinner.clusters[1];
			JBone& attachedBone = allBonesUpperBase[cluster.skelIdx];

			for(int linki = 0;linki < cluster.affectingVtxIdxs.size();linki++)
			{
				vtxWeight& vwLink = cluster.affectingVtxIdxs[linki];
				//todo testcode
				JMatrix44 skinningMat = currJ2M[cluster.skelIdx] * bindM2J[cluster.skelIdx];
				//JMatrix44 skinningMat = JMatrix44::GetIdentityMatrix();
				skinningMesh.resetSubData(JVERTEXATTSKINMAT1_R1, vwLink.vIdx, &skinningMat[0][0]);
				skinningMesh.resetSubData(JVERTEXATTSKINMAT1_R2, vwLink.vIdx, &skinningMat[1][0]);
				skinningMesh.resetSubData(JVERTEXATTSKINMAT1_R3, vwLink.vIdx, &skinningMat[2][0]);
				skinningMesh.resetSubData(JVERTEXATTSKINMAT1_R4, vwLink.vIdx, &skinningMat[3][0]);
/*
				skinningMesh.resetSubData(JVERTEXATTSKINMAT2_R1, vwLink.vIdx, &skinningMat[0][0]);
				skinningMesh.resetSubData(JVERTEXATTSKINMAT2_R2, vwLink.vIdx, &skinningMat[1][0]);
				skinningMesh.resetSubData(JVERTEXATTSKINMAT2_R3, vwLink.vIdx, &skinningMat[2][0]);
				skinningMesh.resetSubData(JVERTEXATTSKINMAT2_R4, vwLink.vIdx, &skinningMat[3][0]);

				skinningMesh.resetSubData(JVERTEXATTSKINMAT3_R1, vwLink.vIdx, &skinningMat[0][0]);
				skinningMesh.resetSubData(JVERTEXATTSKINMAT3_R2, vwLink.vIdx, &skinningMat[1][0]);
				skinningMesh.resetSubData(JVERTEXATTSKINMAT3_R3, vwLink.vIdx, &skinningMat[2][0]);
				skinningMesh.resetSubData(JVERTEXATTSKINMAT3_R4, vwLink.vIdx, &skinningMat[3][0]);*/
				//*(JMatrix44*)(skinningMesh.vertices[vwLink.vIdx].skinmat1) = currJ2M[cluster.skelIdx] * bindM2J[cluster.skelIdx];
			}
		}
		skinningMesh.resetVBData();
	}


	return 0;
}
