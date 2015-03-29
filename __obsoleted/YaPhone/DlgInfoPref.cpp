// DlgInfoPref.cpp : implementation file
//

#include "stdafx.h"
#include "YaPhone.h"
#include "DlgInfoPref.h"


// CDlgInfoPref dialog

IMPLEMENT_DYNAMIC(CDlgInfoPref, CSAPrefsSubDlg)

CDlgInfoPref::CDlgInfoPref(CWnd* pParent /*=NULL*/)
	: CSAPrefsSubDlg(CDlgInfoPref::IDD, pParent)
{

}

CDlgInfoPref::~CDlgInfoPref()
{
}

void CDlgInfoPref::DoDataExchange(CDataExchange* pDX)
{
	CSAPrefsSubDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgInfoPref, CSAPrefsSubDlg)
END_MESSAGE_MAP()


// CDlgInfoPref message handlers
