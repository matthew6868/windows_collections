#pragma once

#include "SAPrefsSubDlg.h"

// CDlgNetBandwidthPref dialog

class CDlgNetBandwidthPref : public CSAPrefsSubDlg
{
	DECLARE_DYNAMIC(CDlgNetBandwidthPref)

public:
	CDlgNetBandwidthPref(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgNetBandwidthPref();

// Dialog Data
	enum { IDD = IDD_FORM_NET_BANDWIDTH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
