#pragma once

#include "SAPrefsSubDlg.h"
// CDlgInfoPref dialog

class CDlgInfoPref : public CSAPrefsSubDlg
{
	DECLARE_DYNAMIC(CDlgInfoPref)

public:
	CDlgInfoPref(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgInfoPref();

// Dialog Data
	enum { IDD = IDD_FORM_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
