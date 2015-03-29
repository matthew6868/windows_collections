#pragma once


// CDlgMediaStatsPref dialog

class CDlgMediaStatsPref : public CDialog
{
	DECLARE_DYNAMIC(CDlgMediaStatsPref)

public:
	CDlgMediaStatsPref(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgMediaStatsPref();

// Dialog Data
	enum { IDD = IDD_FORM_STATS_MEDIA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
