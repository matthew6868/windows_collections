// DlgNetPortsPref.cpp : implementation file
//

#include "stdafx.h"
#include "YaPhone.h"
#include "DlgNetPortsPref.h"


// CDlgNetPortsPref dialog

IMPLEMENT_DYNAMIC(CDlgNetPortsPref, CSAPrefsSubDlg)

CDlgNetPortsPref::CDlgNetPortsPref(CWnd* pParent /*=NULL*/)
	: CSAPrefsSubDlg(CDlgNetPortsPref::IDD, pParent)
{

}

CDlgNetPortsPref::~CDlgNetPortsPref()
{
}

void CDlgNetPortsPref::DoDataExchange(CDataExchange* pDX)
{
	CSAPrefsSubDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgNetPortsPref, CSAPrefsSubDlg)
END_MESSAGE_MAP()


// CDlgNetPortsPref message handlers
