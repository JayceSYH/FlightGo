
// FlightGoView.cpp : CFlightGoView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "FlightGo.h"
#endif

#include "FlightGoDoc.h"
#include "FlightGoView.h"
#include "Events.h"
#include "AppManager.h"
#include "KeyboardManager.h"
#include "MainApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFlightGoView

IMPLEMENT_DYNCREATE(CFlightGoView, CView)

BEGIN_MESSAGE_MAP(CFlightGoView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_KEYUP()
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CFlightGoView ����/����

CFlightGoView::CFlightGoView()
{
	// TODO: �ڴ˴���ӹ������
}

CFlightGoView::~CFlightGoView()
{

}

BOOL CFlightGoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CFlightGoView ����

void CFlightGoView::OnDraw(CDC* pDC)
{
	CFlightGoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
	if (AppManager::getInstance().isDrawerInited()) {
		
		AppManager::getInstance().updateView();
		Canvas* canvas = getCanvas();
		canvas->drawRect(CRect(0, 0, getSize().cx, getSize().cy));
		getMainLayout().draw(*canvas);
		canvas->drawOnDC(*pDC, getSize().cx, getSize().cy);
		delete canvas;
	}
	else {
		getMainLayout().setSize(getSize());
		AppManager::getInstance().registerView(this);
		AppManager::getInstance().registerApp(new MainApp());
		TimerManager::onTimer(this);
	}
}


// CFlightGoView ��ӡ

BOOL CFlightGoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CFlightGoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CFlightGoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CFlightGoView ���

#ifdef _DEBUG
void CFlightGoView::AssertValid() const
{
	CView::AssertValid();
}

void CFlightGoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Canvas* CFlightGoView::getCanvas()
{
	CSize size = getSize();
	CDC* cdc = GetDC();
	Canvas* canvas =   new Canvas(size.cy, size.cx, cdc);
	ReleaseDC(cdc);
	return canvas;
}

LayoutView & CFlightGoView::getMainLayout()
{
	return this->mainView;
}

CSize CFlightGoView::getSize()
{
	CRect rt;
	GetClientRect(&rt);
	CSize size;
	size.cx = rt.right - rt.left;
	size.cy = rt.bottom - rt.top;
	return size;
}

CFlightGoDoc* CFlightGoView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFlightGoDoc)));
	return (CFlightGoDoc*)m_pDocument;
}
#endif //_DEBUG


// CFlightGoView ��Ϣ�������


void CFlightGoView::OnLButtonDown(UINT nFlags, CPoint point)
{
	getMainLayout().notifyListeners(FG_TIMER, point);

	CView::OnLButtonDown(nFlags, point);
}


void CFlightGoView::OnTimer(UINT_PTR nIDEvent)
{
	TimerManager::onTimer(this);

	CView::OnTimer(nIDEvent);
}


void CFlightGoView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	KeyboardManager::onKey(FG_KEYUP, nChar);

	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CFlightGoView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	KeyboardManager::onKey(FG_KEYDOWN, nChar);

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CFlightGoView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
}


BOOL CFlightGoView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return true;
	//return CView::OnEraseBkgnd(pDC);
}
