
// ProxyTest.h : ProxyTest 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号


// CProxyTestApp:
// 有关此类的实现，请参阅 ProxyTest.cpp
//

class CProxyTestApp : public CWinApp
{
public:
	CProxyTestApp();


// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
public:
	void Read();
public:
	// 线程数量
	UINT m_nThreadNumber;
	// 超时时间
	UINT m_nTimeOut;
public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CProxyTestApp theApp;

