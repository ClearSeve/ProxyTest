#include "stdafx.h"
#include "Territoriality.h"
#include "Lib/cJSON/cJSON.h"
#include "Lib/ClearSeve/ClearSeveLib.h"
#include <afxinet.h>
CTerritoriality::CTerritoriality() :m_bInit(TRUE)
{
	//HRESULT hRef = m_pHttpReq.CreateInstance(__uuidof(WinHttpRequest));
	//if (FAILED(hRef))
	//{
	//	m_bInit = FALSE;
	//	return;
	//}

}


CTerritoriality::~CTerritoriality()
{
//	m_pHttpReq.Release();
}

BOOL CTerritoriality::GetCityName(CString strIP, LPPROXYLISTDATA pProxyListData)
{
	CString strURL;
	strURL.Format(_T("http://int.dpool.sina.com.cn/iplookup/iplookup.php?format=json&ip=%s"), strIP);
	//try
	//{
	//	HRESULT hRef = m_pHttpReq->Open(_T("GET"), strURL.AllocSysString());
	//	if (FAILED(hRef))
	//		return FALSE;

	//	hRef = m_pHttpReq->Send();
	//	if (FAILED(hRef))
	//		return FALSE;
	//}
	//catch (_com_error& e)
	//{
	//	return FALSE;
	//}

	//// 解析字符串
	//CString strRef = m_pHttpReq->ResponseText;

	//// 转换为ASCII
	//char* pstrRef = WToM(strRef);
	//cJSON* pJson = cJSON_Parse(pstrRef);
	//free(pstrRef);

	CInternetSession Session;
	CHttpFile* pHttpFile = NULL;
	try
	{
		pHttpFile = (CHttpFile*)Session.OpenURL(strURL);
	}
	catch (CInternetException* e)
	{
		
		Session.Close();
		return FALSE;
	}
	
	char* pszText = new char[pHttpFile->GetLength() + 1];
	ZeroMemory(pszText, pHttpFile->GetLength() + 1);
	pHttpFile->Read(pszText, pHttpFile->GetLength());
	cJSON* pJson = cJSON_Parse(pszText);
	delete pszText;

	
	pHttpFile->Close();
	Session.Close();


	// 开始解析
	if (pJson != NULL)
	{
		cJSON* pCountry = cJSON_GetObjectItem(pJson, "country");
		if (pCountry != NULL)
		{
			wchar_t* pwstrCountry = MToW(pCountry->valuestring, CP_UTF8);
			pProxyListData->strCountry = pwstrCountry;
			free(pwstrCountry);

		}
		cJSON* pProvince = cJSON_GetObjectItem(pJson, "province");
		if (pCountry != NULL)
		{
			wchar_t* pwstrProvince = MToW(pProvince->valuestring, CP_UTF8);
			pProxyListData->strRegion = pwstrProvince;
			free(pwstrProvince);

		}
		cJSON* pCity = cJSON_GetObjectItem(pJson, "city");
		if (pCity != NULL)
		{
			wchar_t* pwstrCity = MToW(pCity->valuestring, CP_UTF8);
			pProxyListData->strCity = pwstrCity;
			free(pwstrCity);
		}

		cJSON* pISP = cJSON_GetObjectItem(pJson, "isp");
		if (pISP != NULL)
		{
			wchar_t* pwstrISP = MToW(pISP->valuestring, CP_UTF8);
			pProxyListData->strISP = pwstrISP;
			free(pwstrISP);
		}

		cJSON_Delete(pJson);
	}

	return TRUE;
}
