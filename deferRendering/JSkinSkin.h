#pragma once
#include <vector>
#include "JSkinCluster.h"
class JSkinSkin
{
public:
	int meshIdx;
	std::vector<JSkinCluster> clusters;

	JSkinSkin(void);
	~JSkinSkin(void);
};

