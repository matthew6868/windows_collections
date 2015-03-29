// DlgNetConnectionPref.cpp : implementation file
//

#include "stdafx.h"
#include "YaPhone.h"
#include "DlgNetConnectionPref.h"


// CDlgNetConnectionPref dialog

IMPLEMENT_DYNAMIC(CDlgNetConnectionPref, CSAPrefsSubDlg)

CDlgNetConnectionPref::CDlgNetConnectionPref(CWnd* pParent /*=NULL*/)
	: CSAPrefsSubDlg(CDlgNetConnectionPref::IDD, pParent)
	, m_szStunServer(_T(""))
	, m_nNetType(0)
{

}

CDlgNetConnectionPref::~CDlgNetConnectionPref()
{
}

void CDlgNetConnectionPref::DoDataExchange(CDataExchange* pDX)
{
	CSAPrefsSubDlg::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_STUN, m_szStunServer);
	DDX_Radio(pDX, IDC_RADIO_LAN, m_nNetType);
}


BEGIN_MESSAGE_MAP(CDlgNetConnectionPref, CSAPrefsSubDlg)
	ON_BN_CLICKED(IDC_RADIO_NAT, &CDlgNetConnectionPref::OnBnClickedRadioNat)
	ON_BN_CLICKED(IDC_RADIO_LAN, &CDlgNetConnectionPref::OnBnClickedRadioLan)
END_MESSAGE_MAP()


// CDlgNetConnectionPref message handlers

void CDlgNetConnectionPref::OnBnClickedRadioNat()
{
	m_nNetType = 1;
	GetDlgItem(IDC_EDIT_STUN)->EnableWindow(TRUE);
}

void CDlgNetConnectionPref::OnBnClickedRadioLan()
{
	m_nNetType = 0;
	GetDlgItem(IDC_EDIT_STUN)->EnableWindow(FALSE);
}

void CDlgNetConnectionPref::OnOK()
{
	CString szPrevStunServer = m_szStunServer;

	UpdateData(TRUE);

	if (1 == m_nNetType)
	{
		if (!m_szStunServer.IsEmpty() && m_szStunServer.Compare(szPrevStunServer))
		{
			PSTUNClient::NatTypes natType;
			PString	natTypeStr;

			natType = g_pManager->SetSTUNServer(m_szStunServer.GetBuffer(0));
			natTypeStr = PSTUNClient::GetNatTypeString(natType);
			g_pManager->Logger("the Nat type is %s", (const char*)natTypeStr);
		}
	}
	if (0 == m_nNetType)
	{
		PSTUNClient *stun = g_pManager->GetSTUN();
		if (stun)
		{
			g_pManager->SetSTUNServer(PString());
		}
	}

	//CSAPrefsSubDlg::OnOK();
}

BOOL CDlgNetConnectionPref::OnInitDialog()
{
	CSAPrefsSubDlg::OnInitDialog();
	
	PSTUNClient *stun = g_pManager->GetSTUN();
	if (stun)
	{
		m_szStunServer.Format("%s", (const char*)stun->GetServer());
		m_nNetType = 1;
		GetDlgItem(IDC_EDIT_STUN)->EnableWindow(TRUE);
	}
	
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
