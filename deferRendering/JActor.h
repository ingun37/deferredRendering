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
	std::vector<JBone> allBonesUpperBase;
	std::vector<JMatrix44> bindJ2M;
	std::vector<JMatrix44> bindM2J;
	std::vector<JMatrix44> currJ2M;

	std::vector<JSkinSkin*> skinners;

	int calculateRootBaseBones();
	int updateActor();

	JActor(void);
	~JActor(void);
};

