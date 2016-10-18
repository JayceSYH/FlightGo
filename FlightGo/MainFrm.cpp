
// MainFrm.cpp : CMainFrame ���ʵ��
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
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
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
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
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
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	map<string, string> config = ConfigLoader::loadConfig("res\\config.ini", "[AppConfig]");
	stringstream ss;
	int width = cs.cx, height = cs.cy;
	ConfigLoader::getAttribute(config, "width", &width);
	ConfigLoader::getAttribute(config, "height", &height);

	//ȥ�������-
	cs.style &= ~FWS_ADDTOTITLE;
	//ȥ���˵�
	cs.hMenu = NULL;
	//���ô��ڴ�С
	cs.cx = width;
	cs.cy = height;

	int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);

	//���ó�ʼλ��
	cs.x = (screenWidth - width) / 2;
	cs.y = (screenHeight - height) / 2;

	return TRUE;
}

// CMainFrame ���

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


// CMainFrame ��Ϣ�������

