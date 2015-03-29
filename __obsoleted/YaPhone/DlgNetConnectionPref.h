#pragma once

#include "SAPrefsSubDlg.h"

// CDlgNetConnectionPref dialog

class CDlgNetConnectionPref : public CSAPrefsSubDlg
{
	DECLARE_DYNAMIC(CDlgNetConnectionPref)

public:
	CDlgNetConnectionPref(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgNetConnectionPref();

// Dialog Data
	enum { IDD = IDD_FORM_NET_CONNECTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CString m_szStunServer;
private:
	INT m_nNetType;
	afx_msg void OnBnClickedRadioNat();
	afx_msg void OnBnClickedRadioLan();
protected:
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
};
