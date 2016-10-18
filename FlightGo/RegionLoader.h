#pragma once
#include "ConfigLoader.h"
#include "Region.h"

class RegionLoader: public ConfigLoader
{
public:
	static SimpleRegion* loadRegion(string path, string name);
};