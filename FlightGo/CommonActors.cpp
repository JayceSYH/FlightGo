#include "stdafx.h"
#include <time.h>
#include "CommonActors.h"
#include "StringUtils.h"
#include"ViewObject.h"
#include "KeyboardManager.h"
#include "TimerManager.h"
#include "ViewObjectManager.h"
#include "CommonViewObjects.h"

const string KeyboardControlActor::actorName = "KeyboadrControaller";
const string FlightCollisionActor::actorName = "FlightCollision";
const string AutoMovementActor::actorName = "AutoMovement";
const string BulletShooterActor::actorName = "BullteShooter";

KeyboardControlActor::KeyboardControlActor(ViewObject & object):Actor(object)
{
	this->id = generateId();
	this->viewObject.ActionListenable::addListener(ActionListener([this](string event, ViewObject* obj)->void {
		this->disable();
	}), OBJ_DESTROY);
	this->enable();
}

string KeyboardControlActor::getActorName()
{	
	return actorName + StringUtils::i2s(this->id);
}

void KeyboardControlActor::onDraw()
{
	int xoffset = 0 , yoffset = 0;

	if (KeyboardManager::isPressed('W')) {
		yoffset -= this->viewObject.getSpeed();
	}
	if (KeyboardManager::isPressed('S')) {
		yoffset += this->viewObject.getSpeed();
	}
	if (KeyboardManager::isPressed('A')) {
		xoffset -= this->viewObject.getSpeed();
	}
	if (KeyboardManager::isPressed('D')) {
		xoffset += this->viewObject.getSpeed();
	}

	this->viewObject.move(xoffset, yoffset);
}

KeyboardControlActor * KeyboardControlActor::clone()
{
	return new KeyboardControlActor(*this);
}

FlightCollisionActor::FlightCollisionActor(ViewObject & object, bool allCollision):Actor(object)
{
	this->allCollision = allCollision;
	this->id = generateId();
	this->listener = [this](string event, ViewObject* obj)->void {
		if (this->isValidCollision(obj->getObjectType())) {
			ViewObjectManager::invalidateObject(this->viewObject.getId());
			this->disable();
		}
	};

	if (!this->isActive()) {
		this->enable();
	}
}

FlightCollisionActor::~FlightCollisionActor()
{
	this->disable();
}

FlightCollisionActor & FlightCollisionActor::operator=(const FlightCollisionActor & otherActor)
{
	this->allCollision = otherActor.allCollision;
	this->collisionSet = otherActor.collisionSet;
	this->excludeSet = otherActor.excludeSet;

	return *this;
}

string FlightCollisionActor::getActorName()
{
	return actorName + StringUtils::i2s(this->id);
}

void FlightCollisionActor::onDraw()
{
}

FlightCollisionActor * FlightCollisionActor::clone()
{
	return new FlightCollisionActor(*this);
}

void FlightCollisionActor::disable()
{
	Actor::disable();
	this->viewObject.ActionListenable::removeListener(listener, OBJ_COLLISION);
}

void FlightCollisionActor::enable()
{
	Actor::enable();
	this->viewObject.ActionListenable::addListener(listener, OBJ_COLLISION);
}

void FlightCollisionActor::notifyDestroy(ViewObject* otherObj)
{
	this->viewObject.notifyEvent(OBJ_DESTROY, otherObj);
}

bool FlightCollisionActor::isAllCollision()
{
	return allCollision && (collisionSet.begin() == collisionSet.end());
}

bool FlightCollisionActor::isValidCollision(string objType)
{
	return (excludeSet.find(objType) == excludeSet.end()) && (isAllCollision() || collisionSet.find(objType) != collisionSet.end());
}

void FlightCollisionActor::addCollisionType(string objType)
{
	this->collisionSet.insert(objType);
}

void FlightCollisionActor::addExcludeType(string objType)
{
	this->excludeSet.insert(objType);
}

AutoMovementActor::AutoMovementActor(ViewObject & obj, int xspeed, int yspeed):Actor(obj), speed(xspeed, yspeed)
{
	this->id = generateId();
	this->enable();
}

string AutoMovementActor::getActorName()
{
	return actorName + StringUtils::i2s(this->id);
}

void AutoMovementActor::onDraw()
{
	this->moveViewObj();
}

AutoMovementActor * AutoMovementActor::clone()
{
	return new AutoMovementActor(*this);
}

void AutoMovementActor::disable()
{
	Actor::disable();
}

void AutoMovementActor::enable()
{
	Actor::enable();
}

void AutoMovementActor::moveViewObj()
{
	this->viewObject.move(this->speed);
}

void AutoMovementActor::setSpeed(int xspeed, int yspeed)
{
	this->speed.setSpeed(xspeed, yspeed);
}

Speed AutoMovementActor::getSpeed()
{
	return this->speed;
}

BulletShooterActor::BulletShooterActor(ViewObject & obj, BulletObject* bullet, bool downward, int initx, int inity):Actor(obj)
{
	this->id = generateId();
	if (initx == __DEFAULT_INIT_POS__ || inity == __DEFAULT_INIT_POS__) {
		this->initx = obj.getSize().cx / 2 - (bullet->getSize().cx / 2);
		if (!downward) {
			this->inity = -bullet->getSize().cy;
		}
		else {
			this->inity = obj.getSize().cy;
		}
	}
	else {
		this->initx = initx;
		this->inity = inity;
	}
	this->bullet = bullet;

	this->cooldownTime = getLeastCooldownTime();
	this->leftCooldownTime = 0;
	this->shootKey = -1;

	this->enable();
}

BulletShooterActor::BulletShooterActor(const BulletShooterActor & actor):Actor(actor)
{
	this->bullet = actor.bullet->divide();
	this->id = actor.id;
	this->initx = actor.initx;
	this->inity = actor.inity;
	this->cooldownTime = actor.cooldownTime;
	this->leftCooldownTime = actor.leftCooldownTime;
	this->shootKey = actor.shootKey;
}

BulletShooterActor & BulletShooterActor::operator=(const BulletShooterActor & actor)
{
	this->bullet = actor.bullet->divide();
	this->id = actor.id;
	this->initx = actor.initx;
	this->inity = actor.inity;
	this->cooldownTime = actor.cooldownTime;
	this->leftCooldownTime = actor.leftCooldownTime;
	this->shootKey = actor.shootKey;

	return *this;
}

int BulletShooterActor::getLeastCooldownTime()
{
#define __LONG_TIME__ 0xfffffff
	Speed bulletSpeed = this->bullet->getSpeed();
	CSize size = this->bullet->getSize();
	int xtime, ytime;
	if (bulletSpeed.xs != 0) {
		xtime = abs(size.cx / bulletSpeed.xs);
	}
	else {
		xtime = __LONG_TIME__;
	}

	if (bulletSpeed.ys != 0) {
		ytime = abs(size.cy / bulletSpeed.ys);
	}
	else {
		ytime = __LONG_TIME__;
	}
	
	return (int)(__DEFAULT_COOLDOWN_TIME_MULTIPLE__ * min(xtime, ytime));
}

BulletShooterActor::~BulletShooterActor()
{
	if (this->bullet != nullptr) {
		delete bullet;
		bullet = nullptr;
	}
}

string BulletShooterActor::getActorName()
{
	return this->actorName + StringUtils::i2s(this->id);
}

void BulletShooterActor::onDraw()
{
	shootBullet();
}

BulletShooterActor * BulletShooterActor::clone()
{
	return new BulletShooterActor(*this);
}

void BulletShooterActor::disable()
{
	Actor::disable();
}

void BulletShooterActor::enable()
{
	Actor::enable();
}

void BulletShooterActor::shootBullet()
{
	if (--leftCooldownTime <= 0 && (shootKey == -1 || KeyboardManager::isPressed(shootKey))) {
		CPoint& pos = viewObject.getPos();
		this->bullet->setPos(CPoint(initx + pos.x, inity + pos.y));
		ViewObjectManager::addViewObject(bullet->divide());
		leftCooldownTime = cooldownTime;
	}
}

void BulletShooterActor::setInitPos(int initx, int inity)
{
	this->initx = initx;
	this->inity = inity;
}

void BulletShooterActor::setCooldownTime(int millisecond)
{
	int leastTime = getLeastCooldownTime();
	this->cooldownTime = millisecond > leastTime ? millisecond : leastTime;
}

void BulletShooterActor::setBulletSpeed(int xspeed, int yspeed)
{
	this->bullet->setSpeed(xspeed, yspeed);
	this->cooldownTime = getLeastCooldownTime();
}

void BulletShooterActor::bindKey(unsigned key)
{
	this->shootKey = key;
}
