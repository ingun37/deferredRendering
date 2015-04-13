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
				*(JMatrix44*)(skinningMesh.vertices[vwLink.vIdx].skinmat1) = currJ2M[cluster.skelIdx] * bindM2J[cluster.skelIdx];
			}
		}
		skinningMesh.resetVBData();
	}


	return 0;
}
