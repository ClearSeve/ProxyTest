#pragma once


// CSettingDialog 对话框

class CSettingDialog : public CDialog
{
	DECLARE_DYNAMIC(CSettingDialog)

public:
	CSettingDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSettingDialog();

// 对话框数据
	enum { IDD = IDD_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void Read();
	void Write();
public:
	// 线程数量
	UINT m_nThreadNumber;
	// 超时时间
	UINT m_nTimeOut;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
