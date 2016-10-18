#include"stdafx.h"
#include "LayoutView.h"
#include "Canvas.h"
#include <algorithm>
#include <iostream>

void LayoutView::draw(Canvas& canvas, CRect* rect)
{
	for (auto riter = subLayoutList.rbegin(); riter != subLayoutList.rend(); ++riter) {
		CRect subrect;
		CRect subLayout = riter->getRect();
		subrect.IntersectRect(subLayout, CRect(0, 0, size.cx, size.cy));
		if (!subrect.IsRectEmpty()) {
			if (rect == nullptr) {
				riter->draw(canvas, nullptr);
			}
			else {
				double xrate = rect->Width() / size.cx;
				double yrate = rect->Height() / size.cy;
				CRect realRect((int)(subLayout.left / xrate), (int)(subLayout.top / yrate), (int)(subLayout.right / xrate), (int)(subLayout.bottom / yrate));
				subLayout.OffsetRect(rect->left, rect->right);
				riter->draw(canvas, &realRect);
			}
		}
	}
}

void LayoutView::notifyListeners(string event)
{
	__notifyListeners(event);
}

void LayoutView::notifyListeners(string event, CPoint point)
{
	__notifyListeners(event, point);
}

void LayoutView::notifyListeners(string event, unsigned key)
{
	__notifyListeners(event, key);
}

void LayoutView::clearView()
{
	subLayoutList.clear();
}

void LayoutView::addView(ListenableDrawer & view, string id, int left, int top, int width, int height)
{
	SubLayout sub(view.clone(), id, left, top, width, height);
	this->subLayoutList.push_front(sub);
}

void LayoutView::addView(ListenableDrawer & view, string id, CRect rect)
{
	addView(view, id, rect.left, rect.top, rect.Width(), rect.Height());
}

void LayoutView::removeView(string id)
{
	for (auto drawable : subLayoutList) {
		if (drawable.getId() == id) {
			subLayoutList.remove(drawable);
			break;
		}
	}
}

ListenableDrawer & LayoutView::getView(string id)
{
	return find_if(subLayoutList.begin(), subLayoutList.end(), [id](auto& drawer)->bool {return drawer.getId() == id; })->getDrawer();
}

void LayoutView::moveSubViewByOffset(string id, int hori, int vert)
{
	auto iter = subLayoutList.begin();
	while (iter != subLayoutList.end()) {
		if (iter->getId() == id) {
			CRect rec = iter->getRect();
			rec.MoveToXY(rec.left + hori, rec.top + vert);
			iter->setRect(rec);
		}
	}
}

void LayoutView::moveSubView(string id, int l, int t)
{
	auto iter = subLayoutList.begin();
	while (iter != subLayoutList.end()) {
		if (iter->getId() == id) {
			CRect rec = iter->getRect();
			rec.MoveToXY(l, t);
			iter->setRect(rec);
		}
	}
}

void LayoutView::moveToTop(string id)
{
	for (auto drawable : subLayoutList) {
		if (drawable.getId() == id) {
			auto backup = drawable;
			subLayoutList.remove(drawable);
			subLayoutList.push_front(backup);
			break;
		}
	}
}

void LayoutView::moveToBottom(string id)
{
	for (auto drawable : subLayoutList) {
		if (drawable.getId() == id) {
			auto backup = drawable;
			subLayoutList.remove(drawable);
			subLayoutList.push_back(backup);
			break;
		}
	}
}

void LayoutView::moveUp(string id, int step)
{
	auto iter = subLayoutList.rbegin();
	while (iter != subLayoutList.rend() && (*iter).getId() != id) { ++iter; }


	while (iter != subLayoutList.rend() && step-- > 0) {
		auto& target = *iter;
		++iter; 
		auto temp = *iter; 
		*iter = target;
		--iter; 
		*iter = temp;
		++++iter;
	}
}

void LayoutView::moveDown(string id, int step)
{
	auto iter = subLayoutList.begin();
	while (iter != subLayoutList.end() && (*iter).getId() != id) { ++iter; }

	while (iter != subLayoutList.end() && step-- > 0) {
		auto& target = *iter;
		++iter;
		auto temp = *iter;
		*iter = target;
		--iter;
		*iter = temp;
		++++iter;
	}
}

void LayoutView::copyFrom(LayoutView & layout)
{
	for (auto sub : layout.subLayoutList) {
		this->addView(sub.getDrawer(), sub.getId(), sub.getRect());
	}
}

list<SubLayout>& LayoutView::getSubLayoutList()
{
	return this->subLayoutList;
}

LayoutView * LayoutView::clone()
{
	return new LayoutView(*this);
}

SubLayout::SubLayout(const SubLayout & subLayout)
{
	this->drawer = subLayout.drawer->clone();
	this->id = subLayout.id;
	this->rect = subLayout.rect;
}

void SubLayout::draw(Canvas& canvas, CRect* rect)
{
	if (rect == nullptr) {
		this->drawer->draw(canvas, &this->rect);
	}
	else {
		this->drawer->draw(canvas, rect);
	}
}

SubLayout& SubLayout::operator=(const SubLayout& layout) {
	this->id = layout.id;
	this->rect = layout.rect;
	this->drawer = layout.drawer->clone();
	return *this;
}

ListenableDrawer & SubLayout::getDrawer()
{
	return *this->drawer;
}

string SubLayout::getId()
{
	return this->id;
}

CRect SubLayout::getRect()
{
	return this->rect;
}

void SubLayout::setRect(CRect rect)
{
	this->rect = rect;
}
