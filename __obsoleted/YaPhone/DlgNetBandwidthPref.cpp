// DlgNetBandwidthPref.cpp : implementation file
//

#include "stdafx.h"
#include "YaPhone.h"
#include "DlgNetBandwidthPref.h"


// CDlgNetBandwidthPref dialog

IMPLEMENT_DYNAMIC(CDlgNetBandwidthPref, CSAPrefsSubDlg)

CDlgNetBandwidthPref::CDlgNetBandwidthPref(CWnd* pParent /*=NULL*/)
	: CSAPrefsSubDlg(CDlgNetBandwidthPref::IDD, pParent)
{

}

CDlgNetBandwidthPref::~CDlgNetBandwidthPref()
{
}

void CDlgNetBandwidthPref::DoDataExchange(CDataExchange* pDX)
{
	CSAPrefsSubDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgNetBandwidthPref, CSAPrefsSubDlg)
END_MESSAGE_MAP()


// CDlgNetBandwidthPref message handlers
