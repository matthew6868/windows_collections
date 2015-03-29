#pragma once

#include "SAPrefsSubDlg.h"

// CDlgNetPortsPref dialog

class CDlgNetPortsPref : public CSAPrefsSubDlg
{
	DECLARE_DYNAMIC(CDlgNetPortsPref)

public:
	CDlgNetPortsPref(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgNetPortsPref();

// Dialog Data
	enum { IDD = IDD_FORM_NET_PORTS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
