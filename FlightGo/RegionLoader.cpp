#include "stdafx.h"
#include "RegionLoader.h"

RectRegion* createRectRegion(string desc) {
	int x, y, width, height;
	sscanf_s(desc.c_str(), "%d%*[^0-9]%d%*[^0-9]%d%*[^0-9]%d", &x,  &y, &width, &height);
	return new RectRegion(x, y, width, height);
}

CircleRegion* createCircleRegion(string desc) {
	int x, y, radius;
	sscanf_s(desc.c_str(), "%d%*[^0-9]%d%*[^0-9]%d", &x, &y, &radius);
	return new CircleRegion(x, y, radius);
}

PolygonRegion* createPolygonRegion(string desc) {
	int pointNum = 0;
	for (char c : desc) {
		if (c == '(') {
			pointNum++;
		}
	}

	const char *str = desc.c_str();
	CPoint* points = new CPoint[pointNum];

	for (int i = 0; i < pointNum; ++i) {
		int x, y;
		sscanf_s(str, "%*c(%d%*[^0-9]%d)", &x, &y);
		while (*str != ')') {
			++str;
		}
		++str;
		points[i].SetPoint(x, y);
	}

	PolygonRegion* rt = new PolygonRegion(points, pointNum);
	delete[] points;
	return rt;
}

SimpleRegion* RegionLoader::loadRegion(string path, string name)
{
	map<string, string> config = loadConfig(path, "[Region]");
	string desc;
	auto iter = config.find(name);
	if (iter != config.end()) {
		desc = iter->second;
	}
	else {
		return nullptr;
	}

	switch (desc[0]) {
	case 'e': case 'E':
		return new EmptyRegion();
	case 'r': case 'R':
		return createRectRegion(desc.substr(2));
	case 'c': case 'C':
		return createCircleRegion(desc.substr(2));
	case 'p': case 'P':
		return createPolygonRegion(desc.substr(1));
	default:
		return nullptr;
	}
}
