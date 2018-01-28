
// ProxyTestMainFrm.cpp : CProxyTestMainFrame 类的实现
//

#include "stdafx.h"
#include "ProxyTest.h"

#include "ProxyTestMainFrm.h"
#include "Territoriality.h"
#include "ProxyDelay.h"
#include "SettingDialog.h"

#define DATALIST_TIME 0x01 

//#define THREAD_MAX 100

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

vector<PROXYDATA> g_ProxyData;
vector<PROXYLISTDATA> g_ProxyListData;


// CProxyTestMainFrame

IMPLEMENT_DYNAMIC(CProxyTestMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CProxyTestMainFrame, CFrameWnd)
	ON_WM_CREATE()

	ON_COMMAND(ID_FILE_READ, &CProxyTestMainFrame::OnFileRead)
	ON_COMMAND(ID_OPERATION_TEST_BEGIN, &CProxyTestMainFrame::OnOperationTestBegin)
	ON_UPDATE_COMMAND_UI(ID_OPERATION_TEST_BEGIN, &CProxyTestMainFrame::OnUpdateOperationTestBegin)
	ON_COMMAND(ID_OPERATION_TEST_END, &CProxyTestMainFrame::OnOperationTestEnd)
	ON_UPDATE_COMMAND_UI(ID_OPERATION_TEST_END, &CProxyTestMainFrame::OnUpdateOperationTestEnd)
	ON_WM_TIMER()
	ON_COMMAND(ID_OPEN, &CProxyTestMainFrame::OnOpen)
	ON_COMMAND(ID_SAVE, &CProxyTestMainFrame::OnSave)
	ON_COMMAND(ID_SCREEN_TIMEOUT, &CProxyTestMainFrame::OnScreenTimeOut)
	ON_COMMAND(ID_SETTING, &CProxyTestMainFrame::OnSetting)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CProxyTestMainFrame 构造/析构

CProxyTestMainFrame::CProxyTestMainFrame()
{
	m_bRun = FALSE;
	InitializeCriticalSection(&m_Section);
	InitializeCriticalSection(&m_SectionDataUpdate);
}

CProxyTestMainFrame::~CProxyTestMainFrame()
{
	DeleteCriticalSection(&m_Section);
	DeleteCriticalSection(&m_SectionDataUpdate);
}

int CProxyTestMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_wndList.Create(WS_CHILD | WS_VISIBLE | LVS_REPORT, CRect(0, 0, 100, 100), this, AFX_IDW_PANE_FIRST);

	m_wndList.InsertColumn(0, _T("IP地址"), LVCFMT_LEFT, 100);
	m_wndList.InsertColumn(1, _T("端口号"), LVCFMT_LEFT, 100);
	m_wndList.InsertColumn(2, _T("国家"), LVCFMT_LEFT, 100);
	m_wndList.InsertColumn(3, _T("省份"), LVCFMT_LEFT, 100);
	m_wndList.InsertColumn(4, _T("城市"), LVCFMT_LEFT, 100);
	//m_wndList.InsertColumn(5, _T("运营商"), LVCFMT_LEFT, 100); 
	m_wndList.InsertColumn(6, _T("延迟"), LVCFMT_LEFT, 100);

	m_wndList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}

	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));

	return 0;
}

BOOL CProxyTestMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CFrameWnd::PreCreateWindow(cs))
		return FALSE;


	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CProxyTestMainFrame 诊断

#ifdef _DEBUG
void CProxyTestMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CProxyTestMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG




void CProxyTestMainFrame::OnFileRead()
{

}

// 获取代理
BOOL CProxyTestMainFrame::GetProxy(LPPROXYDATA pProxyData)
{
	BOOL bRef = FALSE;
	// 加锁
	EnterCriticalSection(&m_Section);
	if (g_ProxyData.empty() != true && m_bThreadRun == TRUE)
	{
		*pProxyData = g_ProxyData.back();
		g_ProxyData.pop_back();
		bRef = TRUE;
	}
	// 减锁
	LeaveCriticalSection(&m_Section);
	return bRef;
}

/*
DWORD WINAPI CoreThreadProc(_In_ LPVOID lpParameter)
{
	CProxyTestMainFrame* pProxyTestMainFrame = (CProxyTestMainFrame*)lpParameter;

	// 定义获取地址对象
	CTerritoriality Territoriality;
	CProxyDelay ProxyDelay;

	// 循环创建线程
	PROXYDATA data = { 0 };
	while (pProxyTestMainFrame->GetProxy(&data))
	{
		PROXYLISTDATA ListData = { 0 };
		ListData.nItem = data.nCurrentOperation;

		// 获取城市名
		Territoriality.GetCityName(data.strProxyIP, &ListData);

		// 获取延迟时间
		DWORD dwTime = ProxyDelay.GetProxyDelay(data.strProxyIP, data.strProxyPort);
		if (dwTime == DWORD_MAX)
			ListData.strState = _T("响应超时");
		else
			ListData.strState.Format(_T("%d ms"), dwTime);

		// 加锁
		EnterCriticalSection(&pProxyTestMainFrame->m_SectionDataUpdate);
		g_ProxyListData.push_back(ListData);
		// 减锁
		LeaveCriticalSection(&pProxyTestMainFrame->m_SectionDataUpdate);
	}
	return 0;
}
*/
VOID CALLBACK WorkCallback(PTP_CALLBACK_INSTANCE Instance, PVOID Context, PTP_WORK Work)
{
	CProxyTestMainFrame* pProxyTestMainFrame = (CProxyTestMainFrame*)Context;

	// 定义获取地址对象
	CTerritoriality Territoriality;
	CProxyDelay ProxyDelay;

	// 循环创建线程
	PROXYDATA data = { 0 };
	while (pProxyTestMainFrame->GetProxy(&data))
	{
		PROXYLISTDATA ListData = { 0 };
		ListData.nItem = data.nCurrentOperation;

		// 获取城市名
		Territoriality.GetCityName(data.strProxyIP, &ListData);

		// 获取延迟时间
		DWORD dwTime = ProxyDelay.GetProxyDelay(data.strProxyIP, data.strProxyPort);
		if (dwTime == DWORD_MAX)
			ListData.strState = _T("响应超时");
		else
			ListData.strState.Format(_T("%d ms"), dwTime);

		// 加锁
		EnterCriticalSection(&pProxyTestMainFrame->m_SectionDataUpdate);
		g_ProxyListData.push_back(ListData);
		// 减锁
		LeaveCriticalSection(&pProxyTestMainFrame->m_SectionDataUpdate);
	}
}

DWORD WINAPI CreateThreadProc(_In_ LPVOID lpParameter)
{
	CProxyTestMainFrame* pProxyTestMainFrame = (CProxyTestMainFrame*)lpParameter;

	// 初始化
	pProxyTestMainFrame->m_nCurrentOperation = 0;
	g_ProxyData.clear();
	// 读取List
	for (int i = pProxyTestMainFrame->m_wndList.GetItemCount(); i >= 0; --i)
	{
		PROXYDATA ProxyData = { 0 };
		ProxyData.strProxyIP = pProxyTestMainFrame->m_wndList.GetItemText(i, 0);
		ProxyData.strProxyPort = pProxyTestMainFrame->m_wndList.GetItemText(i, 1);
		ProxyData.nCurrentOperation = i;

		g_ProxyData.push_back(ProxyData);
	}

	// 创建线程
	//HANDLE hThread[THREAD_MAX];
	//for (int i = 0; i < THREAD_MAX; i++)
	//{
	//	hThread[i] = CreateThread(NULL, 0, CoreThreadProc, lpParameter, 0, NULL);
	//}
	//
	//// 等待线程结束
	//WaitForMultipleObjects(THREAD_MAX, hThread, TRUE, INFINITE);

	//// 销毁句柄
	//for (int i = 0; i < THREAD_MAX; i++)
	//{
	//	CloseHandle(hThread[i]);
	//}

	//////////////////////////////////////////////////////////////////////////
	// 线程池
	//////////////////////////////////////////////////////////////////////////
	// 环境块
	TP_CALLBACK_ENVIRON CallBackEnviron;
	// 定制线程池
	PTP_POOL pool = NULL;
	// 清理资源
	PTP_CLEANUP_GROUP cleanupgroup = NULL;
	// 普通线程池
	PTP_WORK work = NULL;

	//初始化环境块
	InitializeThreadpoolEnvironment(&CallBackEnviron);
	//创建线程池
	pool = CreateThreadpool(NULL);
	//设置最大线程数
	SetThreadpoolThreadMaximum(pool, 200);
	//设置最小线程数
	SetThreadpoolThreadMinimum(pool, 100);
	//创建一个资源清理器
	cleanupgroup = CreateThreadpoolCleanupGroup();
	//设置环境块与线程池关联
	SetThreadpoolCallbackPool(&CallBackEnviron, pool);
	//设置清理器与环境块关联
	SetThreadpoolCallbackCleanupGroup(&CallBackEnviron, cleanupgroup, NULL);
	//创建线程池需要的回调函数,这里是普通的工作项,也就是一个简单的回调函数
	work = CreateThreadpoolWork(WorkCallback, lpParameter, &CallBackEnviron);

	for (int i = 0; i < 100; i++)
	{
		//确认提交工作项
		SubmitThreadpoolWork(work);

	}

	//当所有的线程池回调函数都被执行后,关闭清理器
	CloseThreadpoolCleanupGroupMembers(cleanupgroup, FALSE, NULL);

	//关闭清理器
	CloseThreadpoolCleanupGroup(cleanupgroup);
	
	//关闭线程池
	CloseThreadpool(pool);

	// 结束定时器(手动调用)
	//pProxyTestMainFrame->OnTimer(DATALIST_TIME);
	pProxyTestMainFrame->SendMessage(WM_TIMER, DATALIST_TIME);
	pProxyTestMainFrame->KillTimer(DATALIST_TIME);
	// 菜单项状态
	pProxyTestMainFrame->m_bRun = FALSE;

	return 0;
}
void CProxyTestMainFrame::OnOperationTestBegin()
{
	if (m_wndList.GetItemCount() < 1)
		return;

	// 菜单项状态
	m_bRun = TRUE;

	// 设置线程运行状态
	m_bThreadRun = TRUE;

	// 设置定时器
	SetTimer(DATALIST_TIME, 1000, NULL);

	// 创建线程
	CreateThread(NULL, 0, CreateThreadProc, this, 0, NULL);

}

void CProxyTestMainFrame::OnUpdateOperationTestBegin(CCmdUI *pCmdUI)
{
	if (m_bRun == TRUE)
	{
		pCmdUI->Enable(FALSE);
	}

}


void CProxyTestMainFrame::OnOperationTestEnd()
{
	m_bThreadRun = FALSE;
}


void CProxyTestMainFrame::OnUpdateOperationTestEnd(CCmdUI *pCmdUI)
{
	if (m_bRun == FALSE)
	{
		pCmdUI->Enable(FALSE);
	}
}


void CProxyTestMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == DATALIST_TIME)
	{
		// 加锁
		EnterCriticalSection(&m_SectionDataUpdate);

		if (g_ProxyListData.size() > 0)
		{
			// 遍历
			vector<PROXYLISTDATA>::iterator ProxyListData = g_ProxyListData.begin();
			while (ProxyListData != g_ProxyListData.end() && g_ProxyListData.size() != 0)
			{
				m_wndList.SetItemText(ProxyListData->nItem, 2, ProxyListData->strCountry);
				m_wndList.SetItemText(ProxyListData->nItem, 3, ProxyListData->strRegion);
				m_wndList.SetItemText(ProxyListData->nItem, 4, ProxyListData->strCity);
				//m_wndList.SetItemText(ProxyListData->nItem, 5, ProxyListData->strISP);
				m_wndList.SetItemText(ProxyListData->nItem, 5, ProxyListData->strState);
				ProxyListData = g_ProxyListData.erase(ProxyListData);
				m_nCurrentOperation++;
			}
		}
		// 判断扫描的进度

		double dItemCount = (double)m_wndList.GetItemCount();
		dItemCount += 1.0;
		int nProcess = (m_nCurrentOperation / dItemCount) * 100;
		if (nProcess == 100)
		{
			m_wndStatusBar.SetPaneText(0, _T("测试完成"));
		}
		else
		{
			CString strStatusBar;
			strStatusBar.Format(_T("测试进度:%d%%"), nProcess);
			m_wndStatusBar.SetPaneText(0, strStatusBar);
		}



		// 减锁
		LeaveCriticalSection(&m_SectionDataUpdate);


	}
	CFrameWnd::OnTimer(nIDEvent);
}


void CProxyTestMainFrame::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code

		// 遍历List
		int nItemCount = m_wndList.GetItemCount();
		ar << nItemCount;
		for (int i = 0; i < nItemCount; ++i)
		{
			CString strText[6];
			strText[0] = m_wndList.GetItemText(i, 0);
			strText[1] = m_wndList.GetItemText(i, 1);
			strText[2] = m_wndList.GetItemText(i, 2);
			strText[3] = m_wndList.GetItemText(i, 3);
			strText[4] = m_wndList.GetItemText(i, 4);
			strText[5] = m_wndList.GetItemText(i, 5);
			ar << strText[0] << strText[1] << strText[2] << strText[3] << strText[4] << strText[5];
		}
	}
	else
	{	// loading code
		// 读取长度
		int nItemCount = 0;
		ar >> nItemCount;

		for (int i = 0; i < nItemCount; ++i)
		{
			CString strText[6];
			ar >> strText[0] >> strText[1] >> strText[2] >> strText[3] >> strText[4] >> strText[5];
			m_wndList.InsertItem(i, strText[0]);
			m_wndList.SetItemText(i, 1, strText[1]);
			m_wndList.SetItemText(i, 2, strText[2]);
			m_wndList.SetItemText(i, 3, strText[3]);
			m_wndList.SetItemText(i, 4, strText[4]);
			m_wndList.SetItemText(i, 5, strText[5]);
		}

	}
}


void CProxyTestMainFrame::OnOpen()
{
	static TCHAR BASED_CODE szFilter[] = _T("代理文件 (*.pt)|*.pt|文本文件 (*.txt)|*.txt||");

	// 打开文件
	CFileDialog FileDialog(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	if (FileDialog.DoModal() != IDOK)
		return;
	

	if (StrStrI(FileDialog.GetFileExt(), _T("txt")))
	{
		// 读取文件
		CStdioFile StdioFile;
		if (StdioFile.Open(FileDialog.GetPathName(), CFile::modeRead | CFile::typeText) == FALSE)
			return;

		// 关闭自动更新
		m_wndList.SetRedraw(FALSE);
		// 循环读取
		CString strText;
		while (StdioFile.ReadString(strText))
		{
			// 间隔规则	
			CString strIP = strText.Left(strText.Find(_T(':')));
			CString strPort = strText.Right(strText.GetLength() - (strText.Find(_T(':')) + 1));

			int nItem = m_wndList.InsertItem(m_wndList.GetItemCount(), strIP);
			m_wndList.SetItemText(nItem, 1, strPort);
		}
		// 开启自动更新
		m_wndList.SetRedraw(TRUE);

		StdioFile.Close();
	}
	else if (StrStrI(FileDialog.GetFileExt(), _T("pt")))
	{
		CFile File;
		if (File.Open(FileDialog.GetPathName(), CFile::modeRead) == TRUE)
		{
			// 序列化
			CArchive Archive(&File, CArchive::load);
			// 关闭自动更新
			m_wndList.SetRedraw(FALSE);
			Serialize(Archive);
			// 开启自动更新
			m_wndList.SetRedraw(TRUE);
			Archive.Close();
		}
		File.Close();
	}

	

}


void CProxyTestMainFrame::OnSave()
{
	static TCHAR BASED_CODE szFilter[] = _T("代理文件 (*.pt)|*.pt|文本文件 (*.txt)|*.txt||");
	CFileDialog FileDialog(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	if (FileDialog.DoModal() != IDOK)
		return;
	if (StrStrI(FileDialog.GetFileExt(), _T("txt")))
	{
		// 读取文件
		CStdioFile StdioFile;
		if (StdioFile.Open(FileDialog.GetPathName(), CFile::modeCreate | CFile::modeWrite | CFile::typeText) == FALSE)
			return;

		// 遍历List
		int nItemCount = m_wndList.GetItemCount();
		for (int i = 0; i < nItemCount; ++i)
		{
			CString strText[2];
			strText[0] = m_wndList.GetItemText(i, 0);
			strText[1] = m_wndList.GetItemText(i, 1);
			CString strLineText;
			strLineText.Format(_T("%s:%s\r\n"), strText[0], strText[1]);
			StdioFile.WriteString(strLineText);
		}

		StdioFile.Close();
	}
	else if (StrStrI(FileDialog.GetFileExt(), _T("pt")))
	{
		CFile File;
		if (File.Open(FileDialog.GetPathName(), CFile::modeCreate | CFile::modeWrite) == TRUE)
		{
			// 序列化
			CArchive Archive(&File, CArchive::store);
			Serialize(Archive);
			Archive.Close();
		}
		File.Close();
	}
}


void CProxyTestMainFrame::OnScreenTimeOut()
{
	m_wndList.SetRedraw(FALSE);
	int nItemCount = m_wndList.GetItemCount();
	for (int i = 0; i < nItemCount; ++i)
	{
		if (m_wndList.GetItemText(i, 5) == _T("响应超时"))
		{
			m_wndList.DeleteItem(i);
			nItemCount--;
			i--;
		}
	}
	m_wndList.SetRedraw(TRUE);
}


void CProxyTestMainFrame::OnSetting()
{
	CSettingDialog SettingDialog;
	SettingDialog.DoModal();
}


BOOL CProxyTestMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F1)
	{
		// 打开主页
		ShellExecute(NULL, _T("OPEN"), _T("http://www.clearseve.com/Software/248.html"), NULL, NULL, SW_SHOWNORMAL);
		return TRUE;
	}


	return CFrameWnd::PreTranslateMessage(pMsg);
}
