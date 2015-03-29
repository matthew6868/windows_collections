#pragma once


// CDlgAnswer dialog

class CDlgAnswer : public CDialog
{
	DECLARE_DYNAMIC(CDlgAnswer)

public:
	CDlgAnswer(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgAnswer();

	void SetDlgTitle(const char *szTitle){m_pszTitle = (char*)szTitle;};
// Dialog Data
	enum { IDD = IDD_DLG_ANSWER };

	int		m_nUserAction;
	char*	m_pszTitle;
	CString m_szRemoteInfo;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedBtnAnswer();
public:
	afx_msg void OnBnClickedBtnReject();
	afx_msg int GetUserAction(void);
	
public:
	virtual BOOL OnInitDialog();
};
