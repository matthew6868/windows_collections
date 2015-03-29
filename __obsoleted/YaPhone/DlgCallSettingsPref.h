#pragma once

#include "SAPrefsSubDlg.h"
#include "afxwin.h"

// CDlgCallSettingsPref dialog

class CDlgCallSettingsPref : public CSAPrefsSubDlg
{
	DECLARE_DYNAMIC(CDlgCallSettingsPref)

public:
	CDlgCallSettingsPref(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgCallSettingsPref();

// Dialog Data
	enum { IDD = IDD_FORM_CALL_SETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CButton m_btnAnswer;
	afx_msg void OnBnClickedCheckAnswer();
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
