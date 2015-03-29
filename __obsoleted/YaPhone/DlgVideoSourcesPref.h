#pragma once
#include "SAPrefsSubDlg.h"
#include "afxwin.h"

// CDlgVideoSourcesPref dialog

class CDlgVideoSourcesPref : public CSAPrefsSubDlg
{
	DECLARE_DYNAMIC(CDlgVideoSourcesPref)

public:
	CDlgVideoSourcesPref(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgVideoSourcesPref();

// Dialog Data
	enum { IDD = IDD_FORM_VIDEO_SOURCE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CComboBox m_cmbVideoDevice;
	virtual BOOL OnInitDialog();
};
