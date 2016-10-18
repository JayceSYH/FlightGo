
// FlightGoView.cpp : CFlightGoView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_KEYUP()
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CFlightGoView 构造/析构

CFlightGoView::CFlightGoView()
{
	// TODO: 在此处添加构造代码
}

CFlightGoView::~CFlightGoView()
{

}

BOOL CFlightGoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CFlightGoView 绘制

void CFlightGoView::OnDraw(CDC* pDC)
{
	CFlightGoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
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


// CFlightGoView 打印

BOOL CFlightGoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CFlightGoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CFlightGoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CFlightGoView 诊断

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

CFlightGoDoc* CFlightGoView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFlightGoDoc)));
	return (CFlightGoDoc*)m_pDocument;
}
#endif //_DEBUG


// CFlightGoView 消息处理程序


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

	// TODO: 在此添加专用代码和/或调用基类
}


BOOL CFlightGoView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return true;
	//return CView::OnEraseBkgnd(pDC);
}
