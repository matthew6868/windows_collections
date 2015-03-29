// DlgH323Pref.cpp : implementation file
//

#include "stdafx.h"
#include "YaPhone.h"
#include "DlgH323Pref.h"


// CDlgH323Pref dialog

static BOOL IsE164(const PString & str);

IMPLEMENT_DYNAMIC(CDlgH323Pref, CSAPrefsSubDlg)

CDlgH323Pref::CDlgH323Pref(CWnd* pParent /*=NULL*/)
	: CSAPrefsSubDlg(CDlgH323Pref::IDD, pParent)
	, m_szGKAddress(_T(""))
{
	if (g_pManager->IsRegisteredWithGatekeeper())
	{
		m_nRegType = 1;
	}
	else
	{
		m_nRegType = 0;

	}
	
	m_szH323ID.Format("%s", (const char*)g_pManager->GetH323EndPoint()->GetLocalUserName());

	PStringList aliasNames;
	aliasNames = g_pManager->GetH323EndPoint()->GetAliasNames();
	for (int i = 0; i < aliasNames.GetSize(); ++i)
	{
		if (IsE164(aliasNames[i]))
		{
			m_szE164.Format("%s", (const char*)aliasNames[i]);
			break;
		}
	}
}

CDlgH323Pref::~CDlgH323Pref()
{
}

void CDlgH323Pref::DoDataExchange(CDataExchange* pDX)
{
	CSAPrefsSubDlg::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_GK_ADDRESS, m_szGKAddress);
	DDX_Text(pDX, IDC_EDIT_H323ID, m_szH323ID);
	DDX_Text(pDX, IDC_EDIT_E164, m_szE164);
	DDX_Radio(pDX, IDC_RADIO_OFF, m_nRegType);
}


BEGIN_MESSAGE_MAP(CDlgH323Pref, CSAPrefsSubDlg)
	ON_BN_CLICKED(IDC_RADIO_SPEC, &CDlgH323Pref::OnBnClickedRadioSpec)
	ON_BN_CLICKED(IDC_RADIO_SEARCH, &CDlgH323Pref::OnBnClickedRadioSearch)
	ON_BN_CLICKED(IDC_RADIO_OFF, &CDlgH323Pref::OnBnClickedRadioOff)
END_MESSAGE_MAP()


// CDlgH323Pref message handlers

void CDlgH323Pref::OnBnClickedRadioSpec()
{
	GetDlgItem(IDC_EDIT_GK_ADDRESS)->EnableWindow(TRUE);
}

BOOL CDlgH323Pref::OnInitDialog()
{
	CSAPrefsSubDlg::OnInitDialog();
	
	if (g_pManager->IsRegisteredWithGatekeeper())
	{
		GetDlgItem(IDC_EDIT_GK_ADDRESS)->EnableWindow(TRUE);
		m_szGKAddress.Format("%s", (const char*)g_pManager->GetH323EndPoint()->GetGatekeeperAddress());
	}
	else
	{
		GetDlgItem(IDC_EDIT_GK_ADDRESS)->EnableWindow(FALSE);
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgH323Pref::OnBnClickedRadioSearch()
{
	GetDlgItem(IDC_EDIT_GK_ADDRESS)->EnableWindow(FALSE);
}

void CDlgH323Pref::OnBnClickedRadioOff()
{
	GetDlgItem(IDC_EDIT_GK_ADDRESS)->EnableWindow(FALSE);
}

void CDlgH323Pref::OnOK()
{
	PStringList aliasNames;
	CString		szPrevE164		= m_szE164;
	CString		szPrevH323ID	= m_szH323ID;

	UpdateData(TRUE);
	
	if (m_szH323ID.Compare(szPrevH323ID))
	{
		g_pManager->GetH323EndPoint()->RemoveAliasName(szPrevH323ID.GetBuffer(0));
		g_pManager->GetH323EndPoint()->AddAliasName(m_szH323ID.GetBuffer(0));
	}

	if (m_szE164.Compare(szPrevE164))
	{
		aliasNames = g_pManager->GetH323EndPoint()->GetAliasNames();
		for (int i = 0; i < aliasNames.GetSize(); ++i)
		{
			if (IsE164(aliasNames[i]) && IsE164(m_szE164.GetBuffer(0)))
			{
				g_pManager->GetH323EndPoint()->RemoveAliasName(aliasNames[i]);
				g_pManager->GetH323EndPoint()->AddAliasName(m_szE164.GetBuffer(0));
				break;
			}
		}
	}

	if (0 == m_nRegType)
	{
		if (g_pManager->IsRegisteredWithGatekeeper())
		{
			g_pManager->UnregisterGatekeeper();
		}
	}
	else if (1 == m_nRegType)
	{
		GetDlgItemText(IDC_EDIT_GK_ADDRESS, m_szGKAddress);
		g_pManager->RegisterGatekeeper(m_szGKAddress.GetBuffer(0), m_szH323ID.GetBuffer(0), m_szE164.GetBuffer(0));
	}
	else if (2 == m_nRegType)
	{
		MessageBox(_T("Not Support Discover Gatekepper Mode now"), _T("Warning!!!"), MB_ICONWARNING);
	}
	
	//CSAPrefsSubDlg::OnOK();
}

static BOOL IsE164(const PString & str)
{
	return !str && strspn(str, "1234567890*#") == strlen(str);
}