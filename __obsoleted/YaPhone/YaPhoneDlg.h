// YaPhoneDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "DlgChat.h"
#include "DlgVideo.h"

// CYaPhoneDlg dialog
class CYaPhoneDlg : public CDialog
{
// Construction
public:
	CYaPhoneDlg(CWnd* pParent = NULL);	// standard constructor

	void AppendLog(const char *szLog, COLORREF color = RGB(0, 0, 255));

	static void StackLogCb(int lvl, char *szLog);

	void SetCallState(CYaManager::CallState eCallState);

	void SetCallID(YaInt nCallID){m_nCallID = nCallID;};

	CYaManager::CallState GetCallState(void) const{return m_callState;}

	void SetRemotePartyInfo(PString token, PString remoteName, PString remoteApp, PString remoteUrl);

	void GetRemotePartyInfo(PString &token, PString &remoteName, PString &remoteApp, PString &remoteUrl);

	PString GetRemotePartyName(void) const {return m_remoteName;}

	PString GetRemotePartyApp(void) const {return m_remoteApp;}

	PString GetRemotePartyUrl(void) const {return m_remoteUrl;}

	IncomingCallMode GetIncomingCallMode(void) const {return m_icm;}
	void	SetIncomingCallMode(IncomingCallMode icm){m_icm = icm;}

// Dialog Data
	enum { IDD = IDD_YAPHONE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

public:
	CDlgChat	*m_pDlgChat;
	CDlgVideo	*m_pDlgVideo;
// Implementation
protected:
	HICON		m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
public:
	afx_msg void OnHelpAbout();

private:
	CButton			m_btnAudio;
	CButton			m_btnMic;
	CComboBox		m_cmbAddr;
	CString			m_szAddr;
	CSliderCtrl		m_sliderPlayVol;
	CSliderCtrl		m_sliderMicVol;
	CProgressCtrl	m_progressPlayVol;
	CProgressCtrl	m_progressMicVol;
	CRichEditCtrl	m_ctrlLog;
	
	IncomingCallMode		m_icm;
	PString					m_token;
	YaInt					m_nCallID;
	PString					m_remoteApp;
	PString					m_remoteName;
	PString					m_remoteUrl;
	CYaManager::CallState	m_callState;

public:
	afx_msg void OnToolsChat();
	afx_msg void OutputChatMessage(PString userName, PString message);
	afx_msg void UpdateVolumeSlider(unsigned volume, BOOL bMic);
	afx_msg void UpdateVolumeIndicate(unsigned volume, BOOL bMic);

protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	afx_msg void OnEditAccount(void);

	afx_msg void AddIconTray(void);
	afx_msg void RemoveIconTray(void);
	afx_msg void ChangeIconTray(CString strMsg);

	afx_msg LRESULT OnTrayNotify(WPARAM wParam, LPARAM lParam);

	afx_msg void OnBnClickedBtnCall();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	afx_msg void OnEditPreferences();
};
