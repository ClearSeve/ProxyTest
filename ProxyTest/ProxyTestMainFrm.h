
// ProxyTestMainFrm.h : CProxyTestMainFrame 类的接口
//

#pragma once
#include <vector>
using namespace std;



class CProxyTestMainFrame : public CFrameWnd
{
	
public:
	CProxyTestMainFrame();
protected: 
	DECLARE_DYNAMIC(CProxyTestMainFrame)

// 特性
public:

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CProxyTestMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	BOOL m_bRun;
	BOOL m_bThreadRun;
	// 获取代理
	BOOL GetProxy(LPPROXYDATA pProxyData);
	LONG m_nCurrentOperation;
	CRITICAL_SECTION m_Section;
	CRITICAL_SECTION m_SectionDataUpdate;
public:  // 控件条嵌入成员
	CListCtrl m_wndList;
	CStatusBar m_wndStatusBar;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
	
public:
	afx_msg void OnFileRead();
	afx_msg void OnOperationTestBegin();
	afx_msg void OnUpdateOperationTestBegin(CCmdUI *pCmdUI);
	afx_msg void OnOperationTestEnd();
	afx_msg void OnUpdateOperationTestEnd(CCmdUI *pCmdUI);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void Serialize(CArchive& ar);
	afx_msg void OnOpen();
	afx_msg void OnSave();
	afx_msg void OnScreenTimeOut();
	afx_msg void OnSetting();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


