#include "stdafx.h"
#include "ProxyDelay.h"
#include "Lib/ClearSeve/ClearSeveLib.h"
#include <afxinet.h>

CProxyDelay::CProxyDelay()
{

}


CProxyDelay::~CProxyDelay()
{

}

DWORD CProxyDelay::GetProxyDelay(CString ProxyIP, CString ProxyPort)
{
	CString strProxy;
	strProxy.Format(_T("%s:%s"), ProxyIP, ProxyPort);



	/*INTERNET_PROXY_INFO ProxyInfo = { 0 };
	ProxyInfo.dwAccessType = INTERNET_OPEN_TYPE_PROXY;
	CString strProxy;
	strProxy.Format(_T("%s:%s"), ProxyIP, ProxyPort);
	ProxyInfo.lpszProxy = strProxy;
	ProxyInfo.lpszProxyBypass = NULL;
	Session.SetOption(INTERNET_OPTION_PROXY, &ProxyInfo, sizeof(INTERNET_PROXY_INFO));*/

	//Session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 5000);      // 5秒的连接超时  
	//Session.SetOption(INTERNET_OPTION_SEND_TIMEOUT, 1000);           // 1秒的发送超时  
	//Session.SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 7000);        // 7秒的接收超时  
	//Session.SetOption(INTERNET_OPTION_DATA_SEND_TIMEOUT, 1000);     // 1秒的发送超时  
	//Session.SetOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, 7000);       // 7秒的接收超时  
	//
	////CHttpConnection* pServer = NULL;
	//pServer = comm.GetHttpConnection(strServerName, nPort);

	// 获取时间
	DWORD dwTime = DWORD_MAX;
	DWORD dwBeginTime = 0;
	DWORD dwEndTime = 0;

	CHttpFile* pHttpFile = NULL;

	try
	{
		CInternetSession Session(AfxGetAppName(), 1, INTERNET_OPEN_TYPE_PROXY, strProxy);
		Session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, theApp.m_nTimeOut);
		Session.SetOption(INTERNET_OPTION_SEND_TIMEOUT, theApp.m_nTimeOut);
		Session.SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, theApp.m_nTimeOut);
		Session.SetOption(INTERNET_OPTION_DATA_SEND_TIMEOUT, theApp.m_nTimeOut);
		Session.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1);

		dwBeginTime = GetTickCount();
		pHttpFile = (CHttpFile*)Session.OpenURL(_T("http://123.207.12.239/ProxyText.html"));
		dwEndTime = GetTickCount();
	}

	catch (...)
	{
		pHttpFile = NULL;
		return dwTime;
	}
	if (pHttpFile != NULL)
	{
		dwTime = dwEndTime - dwBeginTime;
		dwTime = dwTime / 2;

	}
	pHttpFile->Close();
	//	Session.Close();
	return dwTime;
}
