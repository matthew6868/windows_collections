#pragma once


// CDlgCallStatsPref dialog

class CDlgCallStatsPref : public CDialog
{
	DECLARE_DYNAMIC(CDlgCallStatsPref)

public:
	CDlgCallStatsPref(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgCallStatsPref();

// Dialog Data
	enum { IDD = IDD_FORM_STATS_CALL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
