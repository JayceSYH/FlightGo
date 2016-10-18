#include "stdafx.h"
#include "ViewObjectManager.h"
#include "AppManager.h"
#include <algorithm>

void ViewObjectManager::clearInvalid()
{
	while (1) {
		int flag = 0;
		auto iter = objects.begin();
		while (iter != objects.end()) {
			if (iter->second->getViewState() == ViewObject::ViewState::INVALID) {
				delete iter->second;
				objects.erase(iter);
				break;
				flag = 1;
			}
			++iter;
		}
		if (flag == 0) {
			break;
		}
	}
}

void ViewObjectManager::onDraw()
{
	list<int> idList;
	auto iter = getInstance().objects.begin();
	while (iter != getInstance().objects.end()) {
		if (iter->second->getViewState() == ViewObject::ViewState::SHOW) {
			idList.push_back(iter->first);
		}
		++iter;
	}

	for (auto id : idList) {
		getInstance().objects[id]->onDraw();
	}
}

ViewObjectManager* ViewObjectManager::instance = nullptr;

ViewObjectManager & ViewObjectManager::getInstance()
{
	if (ViewObjectManager::instance == nullptr) {
		ViewObjectManager::instance = new ViewObjectManager();
		instance->arangedCollision = false;
	}

	return *ViewObjectManager::instance;
}

ViewObjectManager::~ViewObjectManager()
{
	for (auto iter : objects) {
		delete iter.second;
	}
}

void ViewObjectManager::addViewObject(ViewObject * object)
{
	getInstance().objects[object->getId()] = object;
}

void ViewObjectManager::addToLayout(LayoutView & layout)
{
	onDraw();
	collisionTest();
	getInstance().clearInvalid();
	for (auto iter : getInstance().objects) {
		iter.second->addToLayout(layout);
	}
}

void ViewObjectManager::removeViewObject(int id)
{
	auto iter = getInstance().objects.find(id);
	if (iter != getInstance().objects.end()) {
		delete iter->second;
		getInstance().objects.erase(iter);
	}
}

void ViewObjectManager::removeAllViewObject()
{
	for (auto iter : getInstance().objects) {
		delete iter.second;
	}

	getInstance().objects.clear();
}

void ViewObjectManager::collisionTest()
{
	ViewObjectManager& instance = getInstance();
	auto iter = instance.objects.begin();
	while (iter != instance.objects.end()) {
		if (iter->second->getViewState() == ViewObject::ViewState::INVALID) {
			iter++;
			continue;
		}

		auto iter2 = iter;
		iter2++;
		while (iter2 != getInstance().objects.end()) {
			if (iter->second->getViewState() == ViewObject::ViewState::INVALID) {
				iter++;
				break;
			}
			if (iter2->second->getViewState() == ViewObject::ViewState::INVALID) {
				++iter2;
				continue;
			}

			CRect intersectRect;
			intersectRect.IntersectRect(iter->second->getMainViewRectByPosOffset(), iter2->second->getMainViewRectByPosOffset());
			//test rect region first to avoid test two complicated part
			if (!intersectRect.IsRectEmpty()) {
				if (iter->second->getRegion()->isIntersectWith(*iter2->second->getRegion())) {
					iter->second->notifyEvent(OBJ_COLLISION, iter2->second);
					iter2->second->notifyEvent(OBJ_COLLISION, iter->second);
				}
			}
			++iter2;
		}
		++iter;
	}
}

void ViewObjectManager::invalidateObject(int objectId)
{
	auto iter = getInstance().objects.find(objectId);
	if (iter != getInstance().objects.end()) {
		iter->second->invalidate();
	}
}

ViewObject & ViewObjectManager::getOjbect(int id)
{
	return *(getInstance().objects[id]);
}

void addToCollisionSet(map<string, set<string>>& mp, string src, string dest) {
	auto iter = mp.find(src);
	mp[src].insert(dest);
}

void ViewObjectManager::setCollision(string objType1, string objType2)
{
	addToCollisionSet(getInstance().collisionSet, objType1, objType2);
	addToCollisionSet(getInstance().collisionSet, objType2, objType1);
}
