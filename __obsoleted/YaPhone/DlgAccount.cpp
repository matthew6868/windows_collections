// DlgAccount.cpp : implementation file
//

#include "stdafx.h"
#include "YaPhone.h"
#include "DlgAccount.h"


// CDlgAccount dialog

IMPLEMENT_DYNAMIC(CDlgAccount, CDialog)

CDlgAccount::CDlgAccount(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAccount::IDD, pParent)
	, m_szAccountName(_T(""))
	, m_szUserName(_T(""))
	, m_szPwd(_T(""))
	, m_szRegUrl(_T(""))
{

}

CDlgAccount::~CDlgAccount()
{
}

void CDlgAccount::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_PROTOCOL, m_cmbProtocol);
	DDX_Text(pDX, IDC_EDIT_NAME, m_szAccountName);
	DDX_Text(pDX, IDC_EDIT_USER, m_szUserName);
	DDX_Text(pDX, IDC_EDIT_PWD, m_szPwd);
	DDX_Text(pDX, IDC_EDIT_REG, m_szRegUrl);
}


BEGIN_MESSAGE_MAP(CDlgAccount, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgAccount::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgAccount message handlers

BOOL CDlgAccount::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cmbProtocol.AddString(_T("ITU H.323"));
	m_cmbProtocol.AddString(_T("IETF SIP"));
	m_cmbProtocol.SetCurSel(0);
	
	m_szAccountName = _T("66124");
	m_szUserName	= _T("yaooo");
	m_szRegUrl		= _T("172.16.66.91");

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAccount::OnBnClickedOk()
{
	UpdateData(TRUE);

	if (m_cmbProtocol.GetCurSel())
	{
		MessageBox(_T("Not support SIP protocol now!"), _T("Error"), MB_ICONWARNING);
		return;
	}
	if (m_szAccountName.IsEmpty() || strspn(m_szAccountName, "1234567890*#") != m_szAccountName.GetLength())
	{
		MessageBox(_T("Account Name invaild!!!"), _T("Error"), MB_ICONWARNING);
		return;
	}

	/*if (!m_szRegUrl.IsEmpty())
	{
		g_pManager->RegisterGatekeeper(m_szRegUrl.GetBuffer(0), m_szUserName.GetBuffer(0), m_szAccountName.GetBuffer(0), m_szPwd.GetBuffer(0));
	}*/
	
	OnOK();
}
