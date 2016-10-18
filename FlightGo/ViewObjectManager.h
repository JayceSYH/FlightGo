#pragma once
#include "ViewObject.h"
#include <set>

class ViewObjectManager
{
private:
	map<int, ViewObject*> objects;
	map<string, set<string>> collisionSet;
	void clearInvalid();
	static ViewObjectManager* instance;
	static void onDraw();
	bool arangedCollision;
	ViewObjectManager() {};
public:
	static ViewObjectManager& getInstance();
	~ViewObjectManager();
	static void addViewObject(ViewObject* object);
	static void addToLayout(LayoutView& layout);
	static void removeViewObject(int id);
	static void removeAllViewObject();
	static void collisionTest();
	static void invalidateObject(int objectId);
	static ViewObject& getOjbect(int id);
	static void setCollision(string objType1, string objType2);
};