
// FlightGoView.h : CFlightGoView 类的接口
//

#pragma once
#include"FlightGoDoc.h"
#include"Canvas.h"
#include"Drawable.h"
#include"LayoutView.h"


class CFlightGoView : public CView
{
protected: // 仅从序列化创建
	CFlightGoView();
	DECLARE_DYNCREATE(CFlightGoView)

// 特性
public:
	CFlightGoDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CFlightGoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

//游戏相关
protected:
	Canvas* getCanvas();
	LayoutView mainView;
public:
	LayoutView& getMainLayout();
	CSize getSize();


protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnInitialUpdate();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // FlightGoView.cpp 中的调试版本
inline CFlightGoDoc* CFlightGoView::GetDocument() const
   { return reinterpret_cast<CFlightGoDoc*>(m_pDocument); }
#endif

