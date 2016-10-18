#include "stdafx.h"
#include "FlightObject.h"

FlightObject::FlightObject(ListenableDrawer & drawer, CSize size, CRect LimitRect, CPoint point):ViewObject(drawer, size, LimitRect, point)
{
	this->state = FlightState::Normal;
}

string FlightObject::getObjectType()
{
	if (objType == "") {
		return "flight";
	}
	else {
		return objType;
	}
}

void FlightObject::setObjectType(string objType)
{
	this->objType = objType;
}

FlightObject * FlightObject::clone()
{
	return new FlightObject(*this);
}

HeroFlight::HeroFlight(ListenableDrawer & drawer, CSize size, CRect LimitRect, CPoint pos):FlightObject(drawer, size, LimitRect, pos), 
ViewObject(drawer, size, LimitRect, pos)
{
}

EnemyFlight::EnemyFlight(ListenableDrawer & drawer, CSize size, CRect LimitRect, CPoint pos):FlightObject(drawer, size, LimitRect, pos),
	ViewObject(drawer, size, LimitRect, pos)
{
	initLambda = false;
}

EnemyFlight::~EnemyFlight()
{
}

void EnemyFlight::init(EnemyFlight & flight)
{
}

void EnemyFlight::setInitFunction(function<void(EnemyFlight& flight)> lambda)
{
	this->lambda = lambda;
	initLambda = true;
}

EnemyFlight * EnemyFlight::divide()
{
	EnemyFlight* enemy = new EnemyFlight(getView("main"), size, limitRect);
	init(*enemy);
	if (initLambda) {
		lambda(*enemy);
	}

	return enemy;
}

void EnemyFlight::move(int xs, int ys)
{
	move(Speed(xs, ys));
}

void EnemyFlight::move(Speed speed)
{
	this->pos.Offset(speed.xs, speed.ys);
	if (pos.x + size.cx <= 0 || pos.x >= limitRect.right ||
		pos.y + size.cy <= 0 || pos.y >= limitRect.bottom) {
		this->invalidate();
	}

	this->region->move(speed.xs, speed.ys);
}

