#include "stdafx.h"
#include "Region.h"
#include "exceptions.h"
#include <math.h>

bool isEqual(double d1, double d2) {
	double delta = d1 - d2;
	return (delta > 0 ? delta : -delta) < 0.000000001;
}

static bool intersectCircle_Polygon(CPoint points[], int len, CPoint center, int radius) {
	for (int i = 0; i < len; ++i) {
		if (i == len) {
			Line line(points[i], points[0]);
			if (line.getDistanceFromPoint(center) < radius) {
				return true;
			}
		}
		else {
			Line line(points[i], points[i + 1]);
			if (line.getDistanceFromPoint(center) < radius) {
				return true;
			}
		}
	}

	return false;
}

PolygonRegion::PolygonRegion(CPoint points[], int length)
{
	if (length < 3) {
		throw exception("Too few points to create a polygon region" + length);
	}
	this->pointNum = length;
	this->points = new CPoint[length];
	for (int i = 0; i < length; ++i) {
		this->points[i] = points[i];
	}
}

PolygonRegion::PolygonRegion(const PolygonRegion & region)
{
	this->pointNum = region.pointNum;
	this->points = new CPoint[region.pointNum];
	for (int i = 0; i < pointNum; ++i) {
		this->points[i] = region.points[i];
	}
}

PolygonRegion & PolygonRegion::operator=(const PolygonRegion & region)
{
	this->pointNum = region.pointNum;
	this->points = new CPoint[region.pointNum];
	for (int i = 0; i < pointNum; ++i) {
		this->points[i] = region.points[i];
	}

	return *this;
}

PolygonRegion::~PolygonRegion()
{
	delete[] points;
}

bool PolygonRegion::isIntersectWith(PolygonRegion & otherRegion)
{
	for (int i = 0; i < this->pointNum; ++i) {
		int inext = (i == (this->pointNum - 1)) ? 0 : (i + 1);
		for (int j = 0; j < otherRegion.pointNum; ++j) {
			int jnext = (j == (otherRegion.pointNum - 1)) ? 0 : (j+ 1);
			if (Line(this->points[i], this->points[inext]).isCrossed(Line(otherRegion.points[j], otherRegion.points[jnext]))) {
				return true;
			}
		}
	}

	return false;
}

bool PolygonRegion::isIntersectWith(CircleRegion & otherRegion)
{
	return intersectCircle_Polygon(this->points, this->pointNum, otherRegion.getCenter(), otherRegion.getRadius());
}

bool PolygonRegion::isIntersectWith(RectRegion & otherRegion)
{
	return otherRegion.isIntersectWith(*this);
}

bool PolygonRegion::isInRegion(CPoint& point)
{
	CRgn region;
	region.CreatePolygonRgn(this->points, this->pointNum, WINDING);
	return (bool)PtInRegion(region, point.x, point.y);
}

int PolygonRegion::getPointNum()
{
	return this->pointNum;
}

CPoint * PolygonRegion::getPoints()
{
	return this->points;
}

void PolygonRegion::move(int hori, int vert)
{
	for (int i = 0; i < this->pointNum; ++i) {
		this->points[i].Offset(hori, vert);
	}
}

RectRegion::RectRegion(int left, int top, int width, int height)
{
	CPoint temp[4] = {
		CPoint(left, top),
		CPoint(left + width, top),
		CPoint(left + width, top + height),
		CPoint(left, top + height)
	};
	this->PolygonRegion::PolygonRegion(temp, 4);
	this->rect = CRect(left, top, left + width, top + height);
}

bool RectRegion::isIntersectWith(RectRegion & otherRegion)
{
	CRect subrect;
	subrect.IntersectRect(this->rect, otherRegion.rect);

	return !subrect.IsRectEmpty();
}

bool RectRegion::isIntersectWith(CircleRegion & otherRegion)
{
	return PolygonRegion::isIntersectWith(otherRegion);
}

bool RectRegion::isIntersectWith(PolygonRegion & otherRegion)
{
	return PolygonRegion::isIntersectWith(otherRegion);
}

bool RectRegion::isInRegion(CPoint & point)
{
	return (point.x <= this->rect.right && point.x >= this->rect.left &&
		point.y >= this->rect.top && point.y <= this->rect.bottom);
}

void RectRegion::move(int hori, int vert)
{
	PolygonRegion::move(hori, vert);
	this->rect.OffsetRect(hori, vert);
}

CircleRegion::CircleRegion(int centx, int centy, int radius)
{
	this->center = CPoint(centx, centy);
	this->radius = radius;
}

bool CircleRegion::isIntersectWith(CircleRegion & otherRegion)
{
	return pow(this->center.x - otherRegion.center.x, 2) + pow(this->center.y - otherRegion.center.y, 2) <= pow(this->radius + otherRegion.radius, 2);
}

bool CircleRegion::isIntersectWith(PolygonRegion & otherRegion)
{
	return intersectCircle_Polygon(otherRegion.getPoints(), otherRegion.getPointNum(), this->getCenter(), this->getRadius());
}

bool CircleRegion::isIntersectWith(RectRegion & otherRegion)
{
	return otherRegion.isIntersectWith(*this);
}

bool CircleRegion::isInRegion(CPoint& point)
{
	return pow(this->center.x - point.x, 2) + pow(this->center.y - point.y, 2) < pow(this->radius, 2);
}

CPoint CircleRegion::getCenter()
{
	return this->center;
}

int CircleRegion::getRadius()
{
	return this->radius;
}

void CircleRegion::move(int hori, int vert)
{
	this->center.Offset(hori, vert);
}

Line::Line(CPoint& start, CPoint& end)
{
	this->start = start;
	this->end = end;
}

double Line::getDistanceFromPoint(CPoint & point)
{
	if (isInLine(point)) {
		return 0;
	}

	Line S2E(CPoint(0, 0), CPoint(this->end.x - this->start.x, this->end.y - this->start.y));
	Line S2P(CPoint(0, 0), CPoint(point.x - this->start.x, point.y - this->start.y));
	double length = getLength();

	Line ret = S2E * ((S2P * S2E) / (length * length));
	CPoint Dest = ret + this->start;
	if (isInLine(Dest)) {
		return Line(Dest, point).getLength();
	}
	else {
		return min(Line(this->start, point).getLength(), Line(this->end, point).getLength());
	}
}

bool intersect(CPoint line1_s, CPoint line1_e, CPoint line2_s, CPoint line2_e);
bool Line::isCrossed(Line & line)
{
	return intersect(this->start, this->end, line.start, line.end);
}



bool Line::isInLine(CPoint & point)
{
	if (point == this->start || point == this->end) {
		return true;
	}

	if (isEqual(-((point.y - this->start.y) / (point.x - this->start.x)), ((point.y - this->end.y) / (point.x - this->end.x)))) {
		return true;
	}

	return false;
}

double Line::getLength()
{
#define pow2(x) ((x)*(x))
	return sqrt(pow2(this->start.x - this->end.x) + pow2(this->start.y - this->end.y));
}

double Line::operator*(Line & line)
{
	return this->start.x * line.start.x + this->end.y * line.end.y;
}

Line Line::operator*(double val)
{
	return Line(CPoint(this->start.x * val, this->start.y * val), CPoint(this->end.x * val, this->end.y * val));
}

Line Line::operator+(Line & line)
{
	return Line(CPoint(this->start.x + line.start.x, this->start.y + line.start.y), 
		CPoint(this->end.x + line.end.x, this->end.y + line.end.y));
}

CPoint Line::operator+(CPoint & point)
{
	return CPoint(point.x + this->end.x - this->start.x, point.y + this->end.y - this->start.y);
}

Line Line::operator-()
{
	return Line(this->end, this->start);
}






double mult(CPoint a, CPoint b, CPoint c)
{
	return (a.x - c.x)*(b.y - c.y) - (b.x - c.x)*(a.y - c.y);
}

//aa, bb为一条线段两端点 cc, dd为另一条线段的两端点 相交返回true, 不相交返回false  
bool intersect(CPoint aa, CPoint bb, CPoint cc, CPoint dd)
{
	if (max(aa.x, bb.x)<min(cc.x, dd.x))
	{
		return false;
	}
	if (max(aa.y, bb.y)<min(cc.y, dd.y))
	{
		return false;
	}
	if (max(cc.x, dd.x)<min(aa.x, bb.x))
	{
		return false;
	}
	if (max(cc.y, dd.y)<min(aa.y, bb.y))
	{
		return false;
	}
	if (mult(cc, bb, aa)*mult(bb, dd, aa)<0)
	{
		return false;
	}
	if (mult(aa, dd, cc)*mult(dd, bb, cc)<0)
	{
		return false;
	}
	return true;
}

bool SimpleRegion::isIntersectWith(SimpleRegion & otherRegion)
{
	if (otherRegion.getType() == TYPE_CIRCLE) {
		return isIntersectWith(dynamic_cast<CircleRegion&>(otherRegion));
	}
	else if (otherRegion.getType() == TYPE_RECT) {
		return isIntersectWith(dynamic_cast<RectRegion&>(otherRegion));
	}
	else if (otherRegion.getType() == TYPE_POLYGON) {
		return isIntersectWith(dynamic_cast<PolygonRegion&>(otherRegion));
	}
	else if (otherRegion.getType() == TYPE_EMPTY) {
		return isIntersectWith(dynamic_cast<EmptyRegion&>(otherRegion));
	}

	return false;
}

void EmptyRegion::move(int hori, int vert)
{
}
