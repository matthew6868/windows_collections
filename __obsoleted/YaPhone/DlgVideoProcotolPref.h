#pragma once

#include "SAPrefsSubDlg.h"

// CDlgVideoProcotolPref dialog

class CDlgVideoProcotolPref : public CSAPrefsSubDlg
{
	DECLARE_DYNAMIC(CDlgVideoProcotolPref)

public:
	CDlgVideoProcotolPref(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgVideoProcotolPref();

// Dialog Data
	enum { IDD = IDD_FORM_VIDEO_PROTOCOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
