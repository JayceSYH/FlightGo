
#include"stdafx.h"
#include "ViewObject.h"

Actable::~Actable()
{
	auto iter = actors.begin();
	while (iter != actors.end()) {
		delete (iter->second);
		++iter;
	}
}

void Actable::onDraw()
{
	auto iter = actors.begin();
	while (iter != actors.end()) {
		if (iter->second->isActive()) {
			iter->second->onDraw();
		}
		++iter;
	}
}

Actable::Actable(const Actable & actable)
{
	for (auto iter : actable.actors) {
		this->actors[iter.first] = iter.second->clone();
	}
}

void Actable::addActor(Actor * actor, string id)
{
	if (id == "") {
		actors[actor->getActorName()] = actor;
	}
	else {
		actors[id] = actor;
	}
}

void Actable::enableActor(string id)
{
	actors[id]->enable();
}

void Actable::disableActor(string id)
{
	actors[id]->disable();
}

void Actable::removeActor(string id)
{
	auto iter = actors.find(id);
	if (iter != actors.end()) {
		delete iter->second;
	}
	actors.erase(iter);
}

void Actable::removeActor(Actor * actor)
{
	auto iter = actors.begin();
	while (iter != actors.end()) {
		if (iter->second == actor) {
			delete actor;
			actors.erase(iter);
			break;
		}
		++iter;
	}
}

int Actor::generateId()
{
	static int count = 0;
	return count++;
}

Actor::Actor(ViewObject & object):viewObject(object)
{
	Actor::disable();
}

Actor::~Actor()
{
}

void Actor::enable()
{
	this->active = true;
}

void Actor::disable()
{
	this->active = false;
}

bool Actor::isActive()
{
	return this->active;
}

