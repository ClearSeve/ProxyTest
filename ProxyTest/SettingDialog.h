#pragma once


// CSettingDialog �Ի���

class CSettingDialog : public CDialog
{
	DECLARE_DYNAMIC(CSettingDialog)

public:
	CSettingDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSettingDialog();

// �Ի�������
	enum { IDD = IDD_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	void Read();
	void Write();
public:
	// �߳�����
	UINT m_nThreadNumber;
	// ��ʱʱ��
	UINT m_nTimeOut;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
