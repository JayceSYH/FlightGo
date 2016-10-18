#pragma once
#include "StringUtils.h"
#include "GdiPlus.h"
using namespace Gdiplus;

/**************Common  DrawableView*************/
//CImage content view
typedef StaticDrawableView<CImage> ImageView;

template<>
inline void ImageView::draw(Canvas& canvas, CRect* rect) {
	this->content->Draw(canvas.getDC().m_hDC, rect->left, rect->top, rect->Width(), rect->Height());
}

//Graphic content view
typedef StaticDrawableView<Image> GraphicImageView;

template<>
inline void GraphicImageView::draw(Canvas& canvas, CRect* rect) {
	Graphics graphic(canvas.getDC().m_hDC);
	graphic.DrawImage(this->content, rect->left, rect->top, rect->Width(), rect->Height());
}

//Canvas content view
typedef StaticDrawableView<Canvas> StaticCanvasView;

template<>
inline void StaticCanvasView::draw(Canvas& canvas, CRect* rect) {
	this->content->draw(canvas, rect);
}

//Text content view
typedef DrawableView<string> TextView;

template<>
inline void TextView::draw(Canvas& canvas, CRect* rect) {
	wstring ws = StringUtils::s2ws(*this->content);
	canvas.getDC().TextOutW(rect->left, rect->top, ws.c_str(), ws.length());
}