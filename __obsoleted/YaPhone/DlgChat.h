#pragma once
#include "afxcmn.h"


// CDlgChat dialog

class CDlgChat : public CDialog
{
	DECLARE_DYNAMIC(CDlgChat)

public:
	CDlgChat(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgChat();

// Dialog Data
	enum { IDD = IDD_DLG_CHAT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CRichEditCtrl	m_editMsg;
	CRichEditCtrl	m_editTalkMsg;
	CString			m_szTalkMsg;
	CWnd			*m_pParent;

protected:
	afx_msg void OutputMessage(PString userName, PString message, BOOL isIncoming = FALSE);
	afx_msg void OnBnClickedBtnSend();

	virtual void PostNcDestroy();

	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnCancel();
public:
	afx_msg void OnBnClickedOk();
};
