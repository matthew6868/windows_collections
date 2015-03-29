// DlgVideoProcotolPref.cpp : implementation file
//

#include "stdafx.h"
#include "YaPhone.h"
#include "DlgVideoProcotolPref.h"


// CDlgVideoProcotolPref dialog

IMPLEMENT_DYNAMIC(CDlgVideoProcotolPref, CSAPrefsSubDlg)

CDlgVideoProcotolPref::CDlgVideoProcotolPref(CWnd* pParent /*=NULL*/)
	: CSAPrefsSubDlg(CDlgVideoProcotolPref::IDD, pParent)
{

}

CDlgVideoProcotolPref::~CDlgVideoProcotolPref()
{
}

void CDlgVideoProcotolPref::DoDataExchange(CDataExchange* pDX)
{
	CSAPrefsSubDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgVideoProcotolPref, CSAPrefsSubDlg)
END_MESSAGE_MAP()


// CDlgVideoProcotolPref message handlers
