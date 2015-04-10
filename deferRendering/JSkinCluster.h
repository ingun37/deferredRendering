#pragma once
#include <vector>

typedef struct _vtxWeight
{
	unsigned int vIdx;
	float weight;
	_vtxWeight( unsigned int _vidx, float _w ):vIdx(_vidx),weight(_w){}
} vtxWeight;

class JSkinCluster
{
public:
	std::vector<vtxWeight> affectingVtxIdxs;
	int skelIdx;

	JSkinCluster():skelIdx(-1){};
	~JSkinCluster();
};

