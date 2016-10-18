#include"stdafx.h"
#include "ViewObject.h"
#include "LayoutView.h"

int ViewObject::getObjectId()
{
	static int count = 0;
	return count++;
}

ViewObject::ViewObject(ListenableDrawer& drawer, CSize size, CRect limitRect, CPoint pos)
{
	this->size = size;
	this->addView(drawer, "main", 0, 0, size.cx, size.cy);
	this->limitRect = limitRect;
	this->pos = pos;
	this->id = getObjectId();
	this->setSpeed(0);
	this->region = new EmptyRegion();
	this->viewState = ViewState::SHOW;
}

ViewObject::ViewObject(const ViewObject & object, bool ifNew)
{
	if (ifNew) {
		this->id = getObjectId();
	}
	else {
		this->id = object.id;
	}

	this->moveSpeed = object.moveSpeed;
	this->viewState = object.viewState;
	this->size = object.size;
	this->pos = object.pos;
	this->limitRect = object.limitRect;
	this->region = object.region->clone();
	this->viewState = ViewState::SHOW;
}

ViewObject & ViewObject::operator=(const ViewObject & object)
{
	this->id = object.id;
	this->moveSpeed = object.moveSpeed;
	this->viewState = object.viewState;
	this->size = object.size;
	this->pos = object.pos;
	this->limitRect = object.limitRect;
	this->region = object.region->clone();

	return *this;
}

void ViewObject::addToLayout(LayoutView & layout)
{
	for (auto sub : getSubLayoutList()) {
		CRect originRect = sub.getRect();
		int left = originRect.left + pos.x;
		int top = originRect.top + pos.y;
		layout.addView(sub.getDrawer(), sub.getId(), CRect(left, top, left + originRect.Width(), top + originRect.Height()));
	}
	//this->draw(canvas);
	//layout.addView(CanvasView(canvas), StringUtils::i2s(this->id), pos.x, pos.y, size.cx, size.cy);
}

ViewObject::~ViewObject()
{
	if (region != nullptr) {
		delete this->region;
		this->region = nullptr;
	}
}

int ViewObject::getId()
{
	return this->id;
}

void ViewObject::move(int hori, int vert)
{

	int offsetx = 0, offsety = 0;
	//hori
	if (hori > 0) {
		if (limitRect.right < hori + pos.x + size.cx) {
			offsetx = limitRect.right - size.cx - pos.x;
		}
		else {
			offsetx = hori;
		}
	}
	else {
		if (pos.x + hori < limitRect.left) {
			offsetx = limitRect.left - pos.x;
		}
		else {
			offsetx = hori;
		}
	}

	//vert
	if (vert > 0) {
		if (limitRect.bottom < vert + pos.y + size.cy) {
			offsety =  limitRect.bottom - size.cy - pos.y;
		}
		else {
			offsety = vert;
		}
	}
	else {
		if (pos.y + vert < limitRect.top) {
			offsety = limitRect.top - pos.y;
		}
		else {
			offsety =  vert;
		}
	}

	pos.x += offsetx;
	pos.y += offsety;

	//move region
	this->region->move(offsetx, offsety);
}

void ViewObject::move(Speed speed)
{
	move(speed.xs, speed.ys);
}

void ViewObject::notifyEvent(string event, ViewObject* otherObj)
{
	this->ActionListenable::notifyListeners(event, otherObj);
}

void ViewObject::setSpeed(int speed)
{
	this->moveSpeed = speed;
}

int ViewObject::getSpeed()
{
	return this->moveSpeed;
}

void ViewObject::hide()
{
	if (this->viewState != ViewState::INVALID) {
		this->viewState = ViewState::HIDE;
	}
}

void ViewObject::show()
{
	if (this->viewState != ViewState::INVALID) {
		this->viewState = ViewState::SHOW;
	}
}

void ViewObject::invalidate()
{
	this->viewState = ViewState::INVALID;
}

void ViewObject::onDraw()
{
	Actable::onDraw();
}

void ViewObject::draw(Canvas & canvas, CRect* rect)
{
	if (this->viewState == ViewState::SHOW) {
		LayoutView::draw(canvas, rect);
	}
}

CSize ViewObject::getSize()
{
	return this->size;
}

CPoint ViewObject::getPos()
{
	return this->pos;
}

void ViewObject::setPos(CPoint point)
{
	this->region->move(point.x - this->pos.x, point.y - this->pos.y);
	this->pos = point;
}

CRect ViewObject::getMainViewRectByPosOffset()
{
	return CRect(pos.x, pos.y, pos.x + size.cx, pos.y + size.cy);
}

SimpleRegion * ViewObject::getRegion()
{
	return this->region;
}

void ViewObject::setAbsoluteRegion(SimpleRegion & region)
{
	delete this->region;
	this->region = region.clone();
}

void ViewObject::setRegionWithPosOffset(SimpleRegion & region)
{
	this->region = region.clone();
	this->region->move(pos.x, pos.y);
}

void ViewObject::setRegionAsDefaultRect()
{
	setAbsoluteRegion(RectRegion(pos.x, pos.y, size.cx, size.cy));
}

void ViewObject::setRegionAsDefaultCircle()
{
	int centx = pos.x + (size.cx / 2);
	int centy = pos.y + (size.cy / 2);

	int radius = min(size.cx, size.cy) / 2;
	setAbsoluteRegion(CircleRegion(centx, centy, radius));
}



