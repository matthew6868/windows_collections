// DlgGeneralPref.cpp : implementation file
//

#include "stdafx.h"
#include "YaPhone.h"
#include "DlgGeneralPref.h"


// CDlgGeneralPref dialog

IMPLEMENT_DYNAMIC(CDlgGeneralPref, CSAPrefsSubDlg)

CDlgGeneralPref::CDlgGeneralPref(CWnd* pParent /*=NULL*/)
	: CSAPrefsSubDlg(CDlgGeneralPref::IDD, pParent)
	, m_szUserName(_T(""))
{

}

CDlgGeneralPref::~CDlgGeneralPref()
{
}

void CDlgGeneralPref::DoDataExchange(CDataExchange* pDX)
{
	CSAPrefsSubDlg::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_szUserName);
}


BEGIN_MESSAGE_MAP(CDlgGeneralPref, CSAPrefsSubDlg)
END_MESSAGE_MAP()


// CDlgGeneralPref message handlers

void CDlgGeneralPref::OnOK()
{
	//CSAPrefsSubDlg::OnOK();
	if (strcmp(m_szUserName.GetBuffer(0), g_pManager->GetDefaultUserName()))
	{
		g_pManager->SetDefaultUserName(m_szUserName.GetBuffer(0));
	}
}

BOOL CDlgGeneralPref::OnInitDialog()
{
	CSAPrefsSubDlg::OnInitDialog();

	m_szUserName.Format("%s", (const char*)g_pManager->GetDefaultUserName());

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
