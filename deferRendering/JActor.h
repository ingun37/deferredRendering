#pragma once
#include <vector>
#include "JBone.h"
#include "JMesh.h"
#include "JSkinSkin.h"
class JActor
{
public:
	// laws
	// 1. relationship between skinners and meshes is Injective and non-surjective. ( skinners.size < meshes.size ) (no sharing mesh )
	std::vector<JMesh*> meshes;
	//TODO : avoid pointer
	std::vector<JBone> allBones;
	std::vector<JSkinSkin*> skinners;

	int updateActor();

	JActor(void);
	~JActor(void);
};

