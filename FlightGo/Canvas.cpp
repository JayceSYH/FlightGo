#include "stdafx.h"
#include"Canvas.h"

void Canvas::initCommon(CDC* pDC, int width, int height)
{
	this->height = height;
	this->width = width;

	this->bufferDC.CreateCompatibleDC(pDC);
	CreateBitmap(width, height);
	//CBitmap bitmap;
	//bitmap.CreateCompatibleBitmap(pDC, width, height);
	//bufferDC.SelectObject(&bitmap);
	this->bufferDC.SetStretchBltMode(HALFTONE);
	//CBrush *pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	//CBrush *pOldBrush = bufferDC.SelectObject(pBrush);
	//bufferDC.FillRect(CRect(0, 0, width, height), pBrush);
	//this->bufferDC.SetBkMode(TRANSPARENT);
}

Canvas::Canvas(int height, int width, CDC* pDC)
{
	initCommon(pDC, width, height);
}

Canvas::Canvas(Canvas & canvas)
{
	initCommon(&canvas.bufferDC, canvas.getWidth(), canvas.getHeight());
	this->bufferDC.BitBlt(0, 0, canvas.width, canvas.height, &canvas.bufferDC, 0, 0, SRCCOPY);
}

Canvas::Canvas(CImage& image, CDC* pDC)
{
	initCommon(pDC, image.GetWidth(), image.GetHeight());
	image.Draw(this->bufferDC.m_hDC, 0, 0, this->width, this->height);
}

Canvas::Canvas(Image& image, CDC * pDC)
{
	initCommon(pDC, image.GetWidth(), image.GetHeight());
	Graphics grahic(this->getDC().m_hDC);
	grahic.DrawImage(&image, 0, 0, image.GetWidth(), image.GetHeight());
}

Canvas::~Canvas()
{
	DeleteObject(m_hBmp);
	DeleteDC(bufferDC);
}

Canvas*  Canvas::subRect(int left, int top, int width, int height)
{
	Canvas *canvas = new Canvas(height, width, &this->bufferDC);
	canvas->bufferDC.BitBlt(0, 0, width, height, &this->bufferDC, left, top, SRCCOPY);
	return canvas;
}

Canvas*  Canvas::subRect(CRect rect)
{
	return subRect(rect.left, rect.top, rect.Width(), rect.Height());
}

void Canvas::drawOnSelf(CImage& image, int left, int top, int width, int height)
{
	image.Draw(this->bufferDC.m_hDC, left, top, width, height);
}

void Canvas::drawOnSelf(Canvas& canvas, int left, int top, int width, int height)
{
	this->bufferDC.BitBlt(left, top, width, height, &canvas.bufferDC, 0, 0, SRCCOPY);
}

void Canvas::drawOnSelf(Image & image, int left, int top, int width, int height)
{
	Graphics graphic(this->bufferDC.m_hDC);
	graphic.DrawImage(&image, left, top, width, height);
}

void Canvas::drawOnSelf(Drawable & drawer, int left, int top, int width, int height)
{
	CRect rect(left, top, left + width, top + height);
	drawer.draw(*this, &rect);
}

void Canvas::draw(Canvas& canvas, CRect* rect) {
	BLENDFUNCTION bf;
	bf.AlphaFormat = AC_SRC_ALPHA;
	bf.BlendFlags = 0;
	bf.BlendOp = AC_SRC_OVER;
	bf.SourceConstantAlpha = 255;

	if (rect == nullptr) {
		//canvas.bufferDC.TransparentBlt(0, 0, this->width, this->height, &this->bufferDC, 0, 0, this->width, this->height, RGB(0, 0, 0));
		BOOL bRet = canvas.bufferDC.AlphaBlend(0, 0, width, height, &bufferDC, 0, 0, width, height, bf);
		VERIFY(bRet);
	}
	else {
		//canvas.bufferDC.TransparentBlt(rect->left, rect->top, rect->Width(), rect->Height(), &this->bufferDC, 0, 0, this->width, this->height, RGB(0, 0, 0));
		BOOL bRet = canvas.bufferDC.AlphaBlend(rect->left, rect->top, rect->Width(), rect->Height(), &bufferDC, 0, 0, width, height, bf);
		VERIFY(bRet);
	}
}

int Canvas::getHeight()
{
	return this->height;
}

int Canvas::getWidth()
{
	return this->width;
}

CDC& Canvas::getDC()
{
	return this->bufferDC;
}

void Canvas::drawOnDC(CDC & DC, int width, int height, bool if_stretch)
{
	DC.SetStretchBltMode(COLORONCOLOR);
	DC.StretchBlt(0, 0, width, height, &this->bufferDC, 0, 0, this->width, this->height, SRCCOPY);
}

void Canvas::drawLines(CPoint * points, int pointNum)
{
	CBrush brush(RGB(255, 255, 255));
	CBrush *old = this->bufferDC.SelectObject(&brush);
	this->bufferDC.FillRect(CRect(0, 0, this->width, this->height), &brush);
	
	CBrush brush2(RGB(255, 0, 0));
	this->bufferDC.SelectObject(&brush2);

	this->bufferDC.SelectObject(&brush);
	for (int i = 0; i < pointNum; ++i) {
		if (i == 0) {
			this->bufferDC.MoveTo(points[0]);
		}
		else {
			this->bufferDC.LineTo(points[i]);
		}
	}

	this->bufferDC.SelectObject(old);
	
}

void Canvas::drawRegion(CRgn * rgn)
{
	CBrush brush;
	brush.CreateSolidBrush(RGB(255, 0, 0));
	this->getDC().FillRgn(rgn, &brush);
}

void Canvas::drawRect(CRect & rect, int R, int G, int B  )
{
	CBrush brush(RGB(255, 255, 255));
	this->bufferDC.FillRect(rect, &brush);
}

Canvas * Canvas::clone()
{
	return new Canvas(*this);
}



void Canvas::CreateBitmap(int nWidth, int nHeight)
{
	BITMAPINFO bi;
	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bi.bmiHeader.biBitCount = 32;
	bi.bmiHeader.biHeight = nHeight;
	bi.bmiHeader.biWidth = nWidth;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biCompression = BI_RGB;
	bi.bmiHeader.biXPelsPerMeter = 0;
	bi.bmiHeader.biYPelsPerMeter = 0;
	bi.bmiHeader.biClrUsed = 0;
	bi.bmiHeader.biSizeImage = 0;
	bi.bmiHeader.biSizeImage = nWidth * nHeight * bi.bmiHeader.biBitCount / 8;

	if (m_hBmp)
		::DeleteObject(m_hBmp);
	m_hBmp = ::CreateDIBSection(bufferDC, &bi, 0, NULL, 0, 0);//创建32位位图  

	bufferDC.SelectObject(m_hBmp);
}
