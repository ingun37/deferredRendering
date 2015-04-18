#include "JActor.h"


JActor::JActor(void)
{
}


JActor::~JActor(void)
{
}

int JActor::updateActor()
{
	for(unsigned int i=0;i<allBonesUpperBase.size();i++)
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
	if(skinners.size()>0)
	{
		for(unsigned int i=0;i<meshes.size();i++)
		{
			for(int j=0;j<JVERTEXATTNUM;j++)
			{
				if(tableZeroMemory[j])
					meshes[i]->zeroMemorySubDataAttribute((JVERTEXATTRIBUTE)j);
			}
		}
	}
	for( unsigned int i=0;i<skinners.size();i++ )
	{
		JSkinSkin& skinner = *skinners[i];
		JMesh& skinningMesh = *meshes[skinner.meshIdx];
		for (unsigned int j=0;j<skinner.clusters.size();j++)//todo : there's skinning mat num limit. and it is same as skinner.clusters.size()
		{
			JSkinCluster& cluster = skinner.clusters[j];
			JBone& attachedBone = allBonesUpperBase[cluster.skelIdx];

			for(unsigned int linki = 0;linki < cluster.affectingVtxIdxs.size();linki++)
			{
				vtxWeight& vwLink = cluster.affectingVtxIdxs[linki];
				//todo testcode
				JMatrix44 skinningMat = currJ2M[cluster.skelIdx] * bindM2J[cluster.skelIdx];
				skinningMat*=vwLink.weight;
				
				for(int ri = 0;ri<4;ri++)
					skinningMesh.resetSubData((JVERTEXATTRIBUTE)tableNthSkinMatAttribute[j][ri], vwLink.vIdx, &skinningMat[ri][0]);

				//*(JMatrix44*)(skinningMesh.vertices[vwLink.vIdx].skinmat1) = currJ2M[cluster.skelIdx] * bindM2J[cluster.skelIdx];
			}
		}
		skinningMesh.resetVBData();
	}


	return 0;
}
