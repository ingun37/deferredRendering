#pragma once
#include <vector>

typedef struct _vtxWeight
{
	unsigned int vIdx;
	float weight;
} vtxWeight;

class JSkinCluster
{
public:
	std::vector<vtxWeight> affectingVtxIdxs;
	int skelIdx;

	JSkinCluster():skelIdx(-1){};
	~JSkinCluster();
};

