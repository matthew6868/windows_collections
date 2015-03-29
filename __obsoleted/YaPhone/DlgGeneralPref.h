#pragma once
#include "SAPrefsSubDlg.h"

// CDlgGeneralPref dialog

class CDlgGeneralPref : public CSAPrefsSubDlg
{
	DECLARE_DYNAMIC(CDlgGeneralPref)

public:
	CDlgGeneralPref(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgGeneralPref();

// Dialog Data
	enum { IDD = IDD_FORM_GENERAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
	CString m_szUserName;
};
