#pragma once


// CDlgSIPPref dialog

class CDlgSIPPref : public CDialog
{
	DECLARE_DYNAMIC(CDlgSIPPref)

public:
	CDlgSIPPref(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSIPPref();

// Dialog Data
	enum { IDD = IDD_FORM_SIP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
