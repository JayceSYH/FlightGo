#pragma once
#include "ViewObject.h"
#include "CommonStructs.h"

class FlightCollisionActor;
class AutoMovementActor;

class BulletObject: virtual public ViewObject 
{
private:
	FlightCollisionActor* collisionActor;
	AutoMovementActor* autoMoveActor;
	string objType;
public:
	BulletObject(ListenableDrawer& drawer, CSize size, CRect LimitRect, CPoint pos = CPoint(0, 0));
	~BulletObject();
	virtual string getObjectType();
	virtual BulletObject* clone();
	virtual void move(Speed speed);
	virtual void move(int xs, int ys);
	void addTargetType(string objType);
	void addExcludeType(string objType);
	BulletObject* divide();
	void setSpeed(int xspeed, int yspeed);
	void setSpeed(Speed& speed);
	void setObjectType(string objType);
	Speed getSpeed();
};