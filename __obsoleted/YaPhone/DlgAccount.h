#pragma once
#include "afxwin.h"


// CDlgAccount dialog

class CDlgAccount : public CDialog
{
	DECLARE_DYNAMIC(CDlgAccount)

public:
	CDlgAccount(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgAccount();

	CString GetAccountName(void)const {return m_szAccountName;}
	CString GetRegUrl(void)const {return m_szRegUrl;}
	CString GetUserName(void)const {return m_szUserName;}
	CString GetPassword(void)const {return m_szPwd;}

// Dialog Data
	enum { IDD = IDD_DLG_ACCOUNT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CComboBox m_cmbProtocol;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
private:
	// e.g: E.164 form number
	CString m_szAccountName;

	CString m_szUserName;

	CString m_szPwd;

	CString m_szRegUrl;
};
