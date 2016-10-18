#pragma once
#include "LayoutView.h"
#include "Actable.h"
#include "Region.h"
#include "CommonStructs.h"


class ViewObject : virtual public LayoutView, virtual public Actable {
public:
	enum ViewState { SHOW, HIDE, INVALID };
protected:
	CPoint pos;
	CRect limitRect;
	SimpleRegion* region;
private:
	int id;
	int getObjectId();
	int moveSpeed;
	ViewState viewState;
public:
	ViewObject(ListenableDrawer& drawer, CSize size, CRect LimitRect, CPoint pos = CPoint(0, 0));
	ViewObject(const ViewObject& object, bool ifNew = false);
	ViewObject& operator=(const ViewObject& object);
	void addToLayout(LayoutView& layout);
	int getId();
	virtual void move(int hori, int vert);
	virtual void move(Speed speed);
	void notifyEvent(string event, ViewObject* otherObj);
	void setSpeed(int speed);
	int getSpeed();
	ViewState getViewState() { return this->viewState;  };
	void hide();
	void show();
	void invalidate();
	SimpleRegion* getRegion();
	void setAbsoluteRegion(SimpleRegion& region);
	void setRegionWithPosOffset(SimpleRegion& region);
	void setRegionAsDefaultRect();
	void setRegionAsDefaultCircle();
	virtual ~ViewObject();
	virtual string getObjectType() = 0;
	virtual void onDraw();
	virtual void draw(Canvas& canvas, CRect* rect);
	virtual ViewObject* clone() = 0;
	virtual CSize getSize();
	virtual CPoint getPos();
	virtual void setPos(CPoint point);
	virtual CRect getMainViewRectByPosOffset();
};
