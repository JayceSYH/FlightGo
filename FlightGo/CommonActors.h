#pragma once
#include "Actable.h"
#include <set>
#include "CommonStructs.h"

class KeyboardControlActor:virtual public Actor
{
private:
	static const string actorName;
private:
	int id;
public:
	KeyboardControlActor(ViewObject& object);
	virtual string getActorName();
	virtual void onDraw();
	virtual KeyboardControlActor* clone();
};

class FlightCollisionActor:virtual public Actor
{
private:
	static const string actorName;
private:
	int id;
	ActionListener listener;
	bool allCollision;
	set<string> collisionSet;
	set<string> excludeSet;
public:
	FlightCollisionActor(ViewObject& object, bool allCollision = true);
	~FlightCollisionActor();
	FlightCollisionActor& operator=(const FlightCollisionActor& otherActor);
	virtual string getActorName();
	virtual void onDraw();
	virtual FlightCollisionActor* clone();
	virtual void disable();
	virtual void enable();
	void notifyDestroy(ViewObject* otherObj);
	bool isAllCollision();
	bool isValidCollision(string objType);
	void addCollisionType(string objType);
	void addExcludeType(string objType);
};


class AutoMovementActor :virtual public Actor{
private:
	static const string actorName;
	Speed speed;
private:
	int id;
public:
	AutoMovementActor(ViewObject& obj, int xspeed = 0, int yspeed = 0);
	virtual string getActorName();
	virtual void onDraw();
	virtual AutoMovementActor* clone();
	virtual void disable();
	virtual void enable();
	void moveViewObj();
	void setSpeed(int xspeed, int yspeed);
	Speed getSpeed();
};


#define __DEFAULT_INIT_POS__ 0xfffffff
#define __DEFAULT_COOLDOWN_TIME_MULTIPLE__ 1.5
class BulletObject;
class BulletShooterActor: virtual public Actor
{
private:
	static const string actorName;
	BulletObject* bullet;
private:
	int id;
	int initx;
	int inity;
	int cooldownTime;
	int leftCooldownTime;
	int getLeastCooldownTime();
	int shootKey;
public:
	~BulletShooterActor();
	BulletShooterActor(ViewObject& obj, BulletObject* bullet, bool downWard = false, int initx = __DEFAULT_INIT_POS__, int inity = __DEFAULT_INIT_POS__);
	BulletShooterActor(const BulletShooterActor& actor);
	BulletShooterActor& operator=(const BulletShooterActor& actor);
	virtual string getActorName();
	virtual void onDraw();
	virtual BulletShooterActor* clone();
	virtual void disable();
	virtual void enable();
	void shootBullet();
	void setInitPos(int initx, int inity);
	void setCooldownTime(int millisecond);
	void setBulletSpeed(int xspeed, int yspeed);
	void bindKey(unsigned key);
};