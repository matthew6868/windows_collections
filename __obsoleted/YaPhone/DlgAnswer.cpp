// DlgAnswer.cpp : implementation file
//

#include "stdafx.h"
#include "YaPhone.h"
#include "DlgAnswer.h"


// CDlgAnswer dialog

IMPLEMENT_DYNAMIC(CDlgAnswer, CDialog)

CDlgAnswer::CDlgAnswer(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAnswer::IDD, pParent)
	, m_szRemoteInfo(_T(""))
{

}

CDlgAnswer::~CDlgAnswer()
{
}

void CDlgAnswer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_INFO, m_szRemoteInfo);
}


BEGIN_MESSAGE_MAP(CDlgAnswer, CDialog)
	ON_BN_CLICKED(IDC_BTN_ANSWER, &CDlgAnswer::OnBnClickedBtnAnswer)
	ON_BN_CLICKED(IDC_BTN_REJECT, &CDlgAnswer::OnBnClickedBtnReject)
END_MESSAGE_MAP()


// CDlgAnswer message handlers

void CDlgAnswer::OnBnClickedBtnAnswer()
{
	m_nUserAction = IDC_BTN_ANSWER;

	CDialog::OnOK();
}

void CDlgAnswer::OnBnClickedBtnReject()
{
	m_nUserAction = IDC_BTN_REJECT;

	CDialog::EndDialog(IDOK);
}

int CDlgAnswer::GetUserAction(void)
{
	return m_nUserAction;
}

BOOL CDlgAnswer::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(m_pszTitle);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
