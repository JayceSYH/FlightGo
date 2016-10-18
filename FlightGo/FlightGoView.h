
// FlightGoView.h : CFlightGoView ��Ľӿ�
//

#pragma once
#include"FlightGoDoc.h"
#include"Canvas.h"
#include"Drawable.h"
#include"LayoutView.h"


class CFlightGoView : public CView
{
protected: // �������л�����
	CFlightGoView();
	DECLARE_DYNCREATE(CFlightGoView)

// ����
public:
	CFlightGoDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CFlightGoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

//��Ϸ���
protected:
	Canvas* getCanvas();
	LayoutView mainView;
public:
	LayoutView& getMainLayout();
	CSize getSize();


protected:

// ���ɵ���Ϣӳ�亯��
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

#ifndef _DEBUG  // FlightGoView.cpp �еĵ��԰汾
inline CFlightGoDoc* CFlightGoView::GetDocument() const
   { return reinterpret_cast<CFlightGoDoc*>(m_pDocument); }
#endif

