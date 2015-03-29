#pragma once


// CDlgDataPref dialog

class CDlgDataPref : public CDialog
{
	DECLARE_DYNAMIC(CDlgDataPref)

public:
	CDlgDataPref(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgDataPref();

// Dialog Data
	enum { IDD = IDD_FORM_DATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
