
// FlightGo.h : FlightGo Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CFlightGoApp:
// �йش����ʵ�֣������ FlightGo.cpp
//

class CFlightGoApp : public CWinApp
{
protected:
	ULONG_PTR m_gdiplusToken;
public:
	CFlightGoApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CFlightGoApp theApp;
