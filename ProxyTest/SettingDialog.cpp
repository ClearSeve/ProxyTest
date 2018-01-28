// SettingDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ProxyTest.h"
#include "SettingDialog.h"
#include "afxdialogex.h"


// CSettingDialog �Ի���

IMPLEMENT_DYNAMIC(CSettingDialog, CDialog)

CSettingDialog::CSettingDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSettingDialog::IDD, pParent)
	, m_nThreadNumber(0)
	, m_nTimeOut(0)
{

}

CSettingDialog::~CSettingDialog()
{
}

void CSettingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_THREADNUMBER, m_nThreadNumber);
	DDV_MinMaxUInt(pDX, m_nThreadNumber, 1, 1000);
	DDX_Text(pDX, IDC_EDIT_TIMEOUT, m_nTimeOut);
	DDV_MinMaxUInt(pDX, m_nTimeOut, 1, 60);
}


BEGIN_MESSAGE_MAP(CSettingDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CSettingDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CSettingDialog ��Ϣ�������


BOOL CSettingDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��ע����ȡ
	Read();

	// ���µ��Ի���
	UpdateData(FALSE);


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CSettingDialog::OnBnClickedOk()
{
	// �ӶԻ����ȡ
	if (UpdateData() == FALSE)
		return;

	// д�뵽ע���
	
	Write();


	CDialog::OnOK();
}

void CSettingDialog::Read()
{
	m_nThreadNumber = theApp.GetProfileInt(_T("SETTING"), _T("ThreadNumber"), 10);
	m_nTimeOut = theApp.GetProfileInt(_T("SETTING"), _T("TimeOut"), 3);
}

void CSettingDialog::Write()
{
	theApp.WriteProfileInt(_T("SETTING"), _T("ThreadNumber"), m_nThreadNumber);
	theApp.WriteProfileInt(_T("SETTING"), _T("TimeOut"), m_nTimeOut);
}
