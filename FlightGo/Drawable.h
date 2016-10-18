#pragma once
#include "Listen.h"

class Canvas;

class Drawable 
{
public:
	virtual void draw(Canvas& canvas, CRect* rect = nullptr) = 0;
	virtual Drawable* clone() = 0;
	virtual ~Drawable() {};
};


class ListenableDrawer: 
	virtual public Drawable, virtual public Listenable<CPoint>, virtual public Listenable<>,
	virtual public Listenable<UINT>
{
public:
	virtual ListenableDrawer* clone() = 0;
	virtual bool operator==(ListenableDrawer& listenableDrawer);
	virtual bool operator!=(ListenableDrawer& listenableDrawer);
	template<class... Params>
	void addListener(Listener<Params...> listener, string event);
};

template<class ...Params>
inline void ListenableDrawer::addListener(Listener<Params...> listener, string event)
{
	this->Listenable<Params...>::addListener(listener, event);
}
