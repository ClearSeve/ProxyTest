#pragma once

//#import "c:\\windows\system32\\winhttpcom.dll" no_namespace 


class CTerritoriality
{
public:
	CTerritoriality();
	~CTerritoriality();
public:
	BOOL GetCityName(CString strIP, LPPROXYLISTDATA pProxyListData);
private:
//	IWinHttpRequestPtr m_pHttpReq;
	BOOL m_bInit;
};
