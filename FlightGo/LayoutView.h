#pragma once
#include<list>
#include<map>
#include "Drawable.h"
#include "Canvas.h"
#include"Events.h"
#include "TemplateTools.h"
using namespace std;

template <class T>
class DrawableView: virtual public ListenableDrawer
{
protected:
	T* content;
public:
	DrawableView(T& content);
	virtual ~DrawableView();
	virtual DrawableView<T>* clone();
	virtual void draw(Canvas& canvas, CRect* rect);
};

template <class T>
class StaticDrawableView : virtual public ListenableDrawer
{
private:
	StaticDrawableView() { content = nullptr; };
protected:
	T* content;
public:
	StaticDrawableView(T& content);
	virtual ~StaticDrawableView();
	virtual StaticDrawableView<T>* clone() { return new StaticDrawableView(*this->content); }
	virtual void draw(Canvas& canvas, CRect* rect) { this->content->draw(canvas, rect);  }
};


/***********************Layout View****************************************/

class SubLayout {
private:
	ListenableDrawer* drawer;
	CRect rect;
	string id;
public:
	//SubLayout(string id) :rect(), id(id), drawer(*(ListenableDrawer*)0) {};
	SubLayout(const SubLayout& subLayout);
	SubLayout(ListenableDrawer* drawer, string id) :rect(), drawer(drawer),id(id) {}
	SubLayout(ListenableDrawer* drawer, string id, int left, int top, int width, int height) :rect(left, top, left + width, top + height), drawer(drawer),id(id) {}
	SubLayout(ListenableDrawer* drawer, string id, CRect rect):rect(rect), drawer(drawer), id(id){}
	~SubLayout() { 
		delete drawer; 
	}
	template <class... Params>
	void notifyListenable(string event, Params... params);
	void draw(Canvas& canvas, CRect* rect);
	bool operator==(const SubLayout& layout) { return this->id == layout.id ;}
	bool operator!=(const SubLayout& layout) { return ! (*this == layout); }
	SubLayout& operator=(const SubLayout& layout);
	ListenableDrawer& getDrawer();
	string getId();
	CRect getRect();
	void setRect(CRect rect);
};


class LayoutView: virtual public ListenableDrawer
{
private:
	list<SubLayout> subLayoutList;
protected:
	CSize size;
	template<class... Params>
	void __notifyListeners(string event, Params... params);
public:
	LayoutView() {};
	void setSize(CSize size) { this->size = size; }
	void setSize( int width, int height) { this->size = CSize(width, height); }
	virtual void draw(Canvas& canvas, CRect* rect = nullptr);
	virtual void notifyListeners(string event);
	virtual void notifyListeners(string event, CPoint point);
	virtual void notifyListeners(string event, unsigned key);
	void clearView();
	void addView(ListenableDrawer& view, string id, int left, int top, int width, int height);
	void addView(ListenableDrawer& view, string id, CRect rect);
	void removeView(string id);
	ListenableDrawer& getView(string id);
	void moveSubViewByOffset(string id, int hori, int vert);
	void moveSubView(string id, int l, int t);
	void moveToTop(string id);
	void moveToBottom(string id);
	void moveUp(string id, int step = 1);
	void moveDown(string id, int step = 1);
	void copyFrom(LayoutView& layout);
	CSize& getSize() { return size; };
	list<SubLayout>& getSubLayoutList();
	virtual LayoutView* clone();
};


template<class ...Params>
inline void LayoutView::__notifyListeners(string event, Params ...params)
{
	Listenable<Params...>::notifyListeners(event, unpack(params)...);
	
	//default invoke
	if (!false) {
		for (auto subLayout : subLayoutList) {
			subLayout.getDrawer().Listenable<Params...>::notifyListeners(event, unpack(params)...);
		}
	}
	//other invocation
}

template<>
inline void LayoutView::__notifyListeners(string event, CPoint point) {
	if (event == FG_CLICK) {
		for (auto subLayout : subLayoutList) {
			CRect rect = subLayout.getRect();
			if (rect.left <= point.x && rect.right >= point.x &&
				rect.top <= point.y && rect.bottom >= point.y) {
				subLayout.notifyListenable(event, CPoint(point.x - rect.left, point.y - rect.top));
			}
		}
	}
}

template<class ...Params>
inline void SubLayout::notifyListenable(string event, Params ...params)
{
	this->drawer->Listenable<Params...>::notifyListeners(event, unpack(params)...);
}

template<class T>
inline DrawableView<T>::DrawableView(T & content)
{
	this->content = new T(content);
}

template<class T>
inline DrawableView<T>::~DrawableView()
{
	delete this->content;
}

template<class T>
inline DrawableView<T>* DrawableView<T>::clone()
{
	return new DrawableView<T>(*this->content);
}

template<class T>
StaticDrawableView<T>::StaticDrawableView(T& content) {
	this->content = &content;
}

template<class T>
inline StaticDrawableView<T>::~StaticDrawableView()
{
}


#include "CommonView.h"