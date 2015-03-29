// DlgCallSettingsPref.cpp : implementation file
//

#include "stdafx.h"
#include "YaPhone.h"
#include "YaPhoneDlg.h"
#include "DlgCallSettingsPref.h"


// CDlgCallSettingsPref dialog

IMPLEMENT_DYNAMIC(CDlgCallSettingsPref, CSAPrefsSubDlg)

CDlgCallSettingsPref::CDlgCallSettingsPref(CWnd* pParent /*=NULL*/)
	: CSAPrefsSubDlg(CDlgCallSettingsPref::IDD, pParent)
{
}

CDlgCallSettingsPref::~CDlgCallSettingsPref()
{
}

void CDlgCallSettingsPref::DoDataExchange(CDataExchange* pDX)
{
	CSAPrefsSubDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_ANSWER, m_btnAnswer);
}


BEGIN_MESSAGE_MAP(CDlgCallSettingsPref, CSAPrefsSubDlg)
	ON_BN_CLICKED(IDC_CHECK_ANSWER, &CDlgCallSettingsPref::OnBnClickedCheckAnswer)
END_MESSAGE_MAP()


// CDlgCallSettingsPref message handlers

void CDlgCallSettingsPref::OnBnClickedCheckAnswer()
{
	if (m_btnAnswer.GetCheck())
	{
		g_pManager->GetMainDlg()->SetIncomingCallMode(AUTO_ANSWER);
	}
	else
	{
		g_pManager->GetMainDlg()->SetIncomingCallMode(AVAILABLE);
	}
}

BOOL CDlgCallSettingsPref::OnInitDialog()
{
	CSAPrefsSubDlg::OnInitDialog();

	if (AUTO_ANSWER == g_pManager->GetMainDlg()->GetIncomingCallMode())
	{
		m_btnAnswer.SetCheck(1);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCallSettingsPref::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	//CSAPrefsSubDlg::OnOK();
}
