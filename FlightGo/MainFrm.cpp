
// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "FlightGo.h"

#include "MainFrm.h"
#include "ConfigLoader.h"
#include "StringUtils.h"
#include <sstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	string name = "MyApp";
	ConfigLoader::getAttribute(ConfigLoader::loadConfig("res//config.ini", "[AppConfig]"), "name", &name);
	SetWindowText(StringUtils::s2ws(name).c_str());

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	map<string, string> config = ConfigLoader::loadConfig("res\\config.ini", "[AppConfig]");
	stringstream ss;
	int width = cs.cx, height = cs.cy;
	ConfigLoader::getAttribute(config, "width", &width);
	ConfigLoader::getAttribute(config, "height", &height);

	//去掉标题的-
	cs.style &= ~FWS_ADDTOTITLE;
	//去掉菜单
	cs.hMenu = NULL;
	//设置窗口大小
	cs.cx = width;
	cs.cy = height;

	int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);

	//设置初始位置
	cs.x = (screenWidth - width) / 2;
	cs.y = (screenHeight - height) / 2;

	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

