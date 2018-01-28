#pragma once
#include "Lib/WebService/soapCoreServicePortBindingProxy.h"
class CProxyDelay
{
public:
	CProxyDelay();
	~CProxyDelay();
public:
	DWORD GetProxyDelay(CString ProxyIP, CString ProxyPort);
private:
	
};

