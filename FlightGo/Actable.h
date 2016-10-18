#pragma once
#include "Listen.h"
#include "CommonStructs.h"
class ViewObject;


typedef Listenable<ViewObject*> ActionListenable;
typedef Listener<ViewObject*> ActionListener;

class Actor  {
protected:
	ViewObject& viewObject;
	int generateId();
	bool active;
public:
	Actor(ViewObject& object);
	virtual void onDraw() = 0;
	virtual string getActorName() = 0;
	virtual ~Actor();
	virtual Actor*clone() = 0;
	virtual void enable();
	virtual void disable();
	virtual bool isActive();
};


class Actable : virtual public Listenable<ViewObject*>
{
private:
	map<string, Actor*> actors;
public:
	virtual ~Actable();
	virtual void onDraw();
	Actable() {};
	Actable(const Actable& actable);
	void addActor(Actor* actor, string id = "");
	void enableActor(string id);
	void disableActor(string id);
	void removeActor(string id);
	void removeActor(Actor* actor);
};