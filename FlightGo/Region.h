#pragma once

#include<afxwin.h>
#include <string>
#include"exceptions.h"

#define TYPE_POLYGON "polygon"
#define TYPE_RECT "rect"
#define TYPE_CIRCLE "circle"
#define TYPE_EMPTY "empty"

class Line
{
public:
	Line(CPoint& start, CPoint& end);
	CPoint start;
	CPoint end;
	double getDistanceFromPoint(CPoint& point);
	bool isCrossed(Line& line);
	bool isInLine(CPoint& point);
	double getLength();
	double operator*(Line& line);
	Line operator*(double val);
	Line operator+(Line& line);
	CPoint operator+(CPoint& point);
	Line operator-();
};

class CircleRegion;
class RectRegion;
class PolygonRegion;
class EmptyRegion;

class SimpleRegion {
protected:
public:
	virtual ~SimpleRegion() {};
	virtual string getType() = 0;
	virtual bool isInRegion(CPoint& point) = 0;
	virtual bool isIntersectWith(SimpleRegion& otherRegion);
	virtual bool isIntersectWith(PolygonRegion& otherRegion) = 0;
	virtual bool isIntersectWith(CircleRegion& otherRegion) = 0;
	virtual bool isIntersectWith(RectRegion& otherRegion) = 0;
	virtual bool isIntersectWith(EmptyRegion& otherRegion) { return false; }
	virtual SimpleRegion* clone() = 0;
	virtual void move(int hori, int vert) = 0;
};


class EmptyRegion: virtual public SimpleRegion {
	virtual ~EmptyRegion() {}
	virtual string getType() { return TYPE_EMPTY; }
	virtual bool isInRegion(CPoint& point) { return false; }
	virtual bool isIntersectWith(PolygonRegion& otherRegion) { return false; }
	virtual bool isIntersectWith(CircleRegion& otherRegion) { return false; }
	virtual bool isIntersectWith(RectRegion& otherRegion) { return false; }
	virtual EmptyRegion* clone() { return new EmptyRegion(*this); };
	virtual void move(int hori, int vert);
};

class PolygonRegion: virtual public SimpleRegion
{
protected:
	CPoint *points;
	int pointNum;
	PolygonRegion() {};
public:
	PolygonRegion(CPoint points[], int length);
	PolygonRegion(const PolygonRegion& region);
	PolygonRegion& operator=(const PolygonRegion& region);
	virtual ~PolygonRegion();
	virtual string getType() { return TYPE_POLYGON; }
	virtual bool isIntersectWith(PolygonRegion& otherRegion);
	virtual bool isIntersectWith(CircleRegion& otherRegion);
	virtual bool isIntersectWith(RectRegion& otherRegion);
	virtual bool isInRegion(CPoint& point);
	int getPointNum();
	CPoint* getPoints();
	virtual PolygonRegion* clone() { return new PolygonRegion(*this); }
	virtual void move(int hori, int vert);
};

class RectRegion : virtual public PolygonRegion
{
private:
	CRect rect;
public:
	RectRegion(int left, int top, int width, int height);
	virtual ~RectRegion() {}
	virtual string getType() { return TYPE_RECT; }
	virtual bool isIntersectWith(RectRegion& otherRegion);
	virtual bool isIntersectWith(CircleRegion& otherRegion);
	virtual bool isIntersectWith(PolygonRegion& otherRegion);
	virtual bool isInRegion(CPoint& point);
	virtual RectRegion* clone() { return new RectRegion(*this); }
	virtual void move(int hori, int vert);
};

class CircleRegion : virtual public SimpleRegion
{
private:
	CPoint center;
	int radius;
public:
	CircleRegion(int centx, int centy, int radius);
	virtual ~CircleRegion(){}
	virtual string getType() { return TYPE_CIRCLE; }
	virtual bool isIntersectWith(CircleRegion& otherRegion);
	virtual bool isIntersectWith(PolygonRegion& otherRegion);
	virtual bool isIntersectWith(RectRegion& otherRegion);
	virtual bool isInRegion(CPoint& point);
	CPoint getCenter();
	int getRadius();
	virtual CircleRegion* clone() { return new CircleRegion(*this); }
	virtual void move(int hori, int vert);
};
