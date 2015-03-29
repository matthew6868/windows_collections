#pragma once

#include "SAPrefsSubDlg.h"
#include "afxwin.h"
// CDlgH323Pref dialog

class CDlgH323Pref : public CSAPrefsSubDlg
{
	DECLARE_DYNAMIC(CDlgH323Pref)

public:
	CDlgH323Pref(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgH323Pref();

// Dialog Data
	enum { IDD = IDD_FORM_H323 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CString m_szGKAddress;
private:
	CString m_szH323ID;
private:
	CString m_szE164;
	afx_msg void OnBnClickedRadioSpec();
	virtual BOOL OnInitDialog();
private:
	//CButton m_GKRegType;
	//int			m_nRegType;
	afx_msg void OnBnClickedRadioSearch();
	afx_msg void OnBnClickedRadioOff();
private:
	int m_nRegType;
protected:
	virtual void OnOK();
};
