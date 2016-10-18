#pragma once
#include <afxwin.h>
#include "LayoutView.h"
#include"ViewObject.h"
#include "Region.h"


class FlightObject:virtual public ViewObject{
public:
	enum FlightState { Normal, KILLED };
private:
	string objType;
	FlightState state;
public:
	FlightObject(ListenableDrawer& drawer, CSize size, CRect LimitRect, CPoint pos = CPoint(0, 0));
	//FlightObject(const FlightObject& obj);
	FlightState getFlightState() { return this->state;  };
	virtual string getObjectType();
	void setObjectType(string objType);
	virtual FlightObject* clone();
};


class HeroFlight : virtual public FlightObject {
public:
	HeroFlight(ListenableDrawer& drawer, CSize size, CRect LimitRect, CPoint pos = CPoint(0, 0));
};


class EnemyFlight : virtual public FlightObject {
private:
	bool initLambda;
	function<void(EnemyFlight&)> lambda;
public:
	EnemyFlight(ListenableDrawer& drawer, CSize size, CRect LimitRect, CPoint pos = CPoint(0, 0));
	~EnemyFlight();
	void init(EnemyFlight& flight);
	void setInitFunction(function<void(EnemyFlight& flight)> lambda);
	EnemyFlight* divide();
	virtual void move(int xs, int ys);
	virtual void move(Speed speed);
};