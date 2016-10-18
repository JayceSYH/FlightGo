#include "stdafx.h"
#include "CommonViewObjects.h"
#include "CommonActors.h"
#include "CommonStructs.h"

BulletObject * BulletObject::clone()
{
	return new BulletObject(*this);
}

void BulletObject::move(Speed speed)
{
	this->pos.Offset(speed.xs, speed.ys);
	if (pos.x + size.cx <= 0 || pos.x >= limitRect.right ||
		pos.y + size.cy <= 0 || pos.y >= limitRect.bottom) {
		this->invalidate();
	}

	this->region->move(speed.xs, speed.ys);
}

void BulletObject::move(int xs, int ys)
{
	move(Speed(xs, ys));
}

BulletObject * BulletObject::divide()
{
	BulletObject* ret =  new BulletObject(this->getView("main"), this->size, this->limitRect, this->pos);
	ret->setAbsoluteRegion(*this->getRegion());
	ret->setSpeed(this->autoMoveActor->getSpeed());
	*ret->collisionActor = *this->collisionActor;
	ret->setObjectType(this->getObjectType());

	return ret;
}

void BulletObject::setSpeed(int xspeed, int yspeed)
{
	this->autoMoveActor->setSpeed(xspeed, yspeed);
}

void BulletObject::setSpeed(Speed & speed)
{
	this->setSpeed(speed.xs, speed.ys);
}

void BulletObject::setObjectType(const string objType)
{
	this->objType = objType;
}

Speed BulletObject::getSpeed()
{
	return this->autoMoveActor->getSpeed();
}

void BulletObject::addTargetType(string objType)
{
	this->collisionActor->addCollisionType(objType);
}

void BulletObject::addExcludeType(string objType)
{
	this->collisionActor->addExcludeType(objType);
}

BulletObject::BulletObject(ListenableDrawer & drawer, CSize size, CRect LimitRect, CPoint pos):ViewObject(drawer, size, LimitRect, pos)
{
	this->collisionActor = new FlightCollisionActor(*this);
	this->addActor(this->collisionActor);
	this->autoMoveActor = new AutoMovementActor(*this);
	this->addActor(this->autoMoveActor);
}

BulletObject::~BulletObject()
{
}

string BulletObject::getObjectType()
{
	if (objType == "") {
		return "bullet";
	}
	else {
		return objType;
	}
}
