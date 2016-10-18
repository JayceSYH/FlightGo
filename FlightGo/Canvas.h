#pragma once
#include<afxwin.h>
#include<atlimage.h>
#include"Drawable.h"

class Canvas : virtual public Drawable
{
private:
	int height;
	int width;
	CDC bufferDC;
	HBITMAP m_hBmp;

private:
	void initCommon(CDC* pDC, int width = 200, int height = 200);
public:
	Canvas(int height, int width, CDC* pDC);
	Canvas(Canvas& canvas);
	Canvas(CImage& image, CDC* pDC);
	Canvas(Image& image, CDC* pDC);
	virtual ~Canvas();
	Canvas* subRect(int left, int top, int witdth, int height);
	Canvas* subRect(CRect rect);
	void drawOnSelf(CImage& image, int left, int top, int width, int height);
	void drawOnSelf(Canvas& canvas, int left, int top, int width, int height);
	void drawOnSelf(Image& image, int left, int top, int width, int height);
	void drawOnSelf(Drawable& drawer, int left, int top, int width, int height);
	void draw(Canvas& canvs, CRect* rect);
	int getHeight();
	int getWidth();
	CDC& getDC();
	void drawOnDC(CDC& DC, int width, int height, bool if_stretch = true);
	void drawLines(CPoint *points, int pointNum);
	void drawRegion(CRgn* rgn);
	void drawRect(CRect& rect, int R = 255, int G = 255, int B = 255);
	virtual Canvas* clone();
	void CreateBitmap(int nWidth, int nHeight);
};