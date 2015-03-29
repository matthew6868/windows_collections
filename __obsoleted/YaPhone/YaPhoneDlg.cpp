// YaPhoneDlg.cpp : implementation file
//

#include "stdafx.h"
#include "YaPhone.h"
#include "YaPhoneDlg.h"
#include "DlgAccount.h"
#include "DlgAnswer.h"

#include "DlgInfoPref.h"
#include "DlgSIPPref.h"
#include "DlgDataPref.h"
#include "DlgH323Pref.h"
#include "DlgAudioPref.h"
#include "DlgPreferences.h"
#include "DlgGeneralPref.h"
#include "DlgCallStatsPref.h"
#include "DlgMediaStatsPref.h"
#include "DlgCallSettingsPref.h"
#include "DlgVideoSourcesPref.h"
#include "DlgVideoProcotolPref.h"
#include "DlgNetConnectionPref.h"
#include "DlgNetPortsPref.h"
#include "DlgNetBandwidthPref.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const CHAR  *_COMPILER__ = "Build " __DATE__ " " __TIME__" ";

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
private:
	CString m_strBuildVer;
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
, m_strBuildVer(_COMPILER__)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_VERSION, m_strBuildVer);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CYaPhoneDlg dialog




CYaPhoneDlg::CYaPhoneDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CYaPhoneDlg::IDD, pParent)
	, m_szAddr(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// for RichEdit to work
	AfxInitRichEdit();
	AfxEnableControlContainer();

	m_pDlgChat = NULL;
	m_pDlgVideo= NULL;
}

void CYaPhoneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_AUDIO, m_btnAudio);
	DDX_Control(pDX, IDC_BTN_MIC, m_btnMic);
	DDX_Control(pDX, IDC_CMB_ADDRESS, m_cmbAddr);
	DDX_CBString(pDX, IDC_CMB_ADDRESS, m_szAddr);
	DDX_Control(pDX, IDC_SLIDER_VOL, m_sliderPlayVol);
	DDX_Control(pDX, IDC_SLIDER_MIC, m_sliderMicVol);
	DDX_Control(pDX, IDC_PROGRESS_VOL, m_progressPlayVol);
	DDX_Control(pDX, IDC_PROGRESS_MIC, m_progressMicVol);
	DDX_Control(pDX, IDC_RICHEDIT_LOG, m_ctrlLog);
}

BEGIN_MESSAGE_MAP(CYaPhoneDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_INITMENUPOPUP()
	ON_MESSAGE(WM_NOTIFYICON, &CYaPhoneDlg::OnTrayNotify)
	ON_COMMAND(ID_HELP_ABOUTYAPHONE, &CYaPhoneDlg::OnHelpAbout)
	ON_COMMAND(ID_TOOLS_CHATWITH, &CYaPhoneDlg::OnToolsChat)
	ON_COMMAND(ID_EDIT_ACCOUNT, &CYaPhoneDlg::OnEditAccount)
	ON_BN_CLICKED(IDC_BTN_CALL, &CYaPhoneDlg::OnBnClickedBtnCall)
	ON_COMMAND(ID_EDIT_PREFERENCES, &CYaPhoneDlg::OnEditPreferences)
END_MESSAGE_MAP()


// CYaPhoneDlg message handlers

BOOL CYaPhoneDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	m_btnAudio.SetIcon(::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_AUDIO_ON)));
	m_btnMic.SetIcon(::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_MIC_ON)));

	m_sliderPlayVol.SetRange(0, 100);
	m_sliderMicVol.SetRange(0, 100);
	m_sliderPlayVol.SetPos(m_sliderPlayVol.GetRangeMax());
	m_sliderMicVol.SetPos(m_sliderMicVol.GetRangeMax());

	m_progressMicVol.SetRange(0, 0xff);
	m_progressPlayVol.SetRange(0, 0xff);

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	m_cmbAddr.AddString(_T("172.16.66.124"));
	m_cmbAddr.AddString(_T("192.168.0.88"));
	m_cmbAddr.AddString(_T("172.16.66.125"));
	m_cmbAddr.AddString(_T("h323.voxgratia.org"));
	m_cmbAddr.SetCurSel(0);
	m_szAddr = _T("172.16.66.124");

	g_pManager->SetLogHandler(CYaPhoneDlg::StackLogCb);
	g_pManager->Initialize(this);
	g_pManager->SetAllMediaFormats();
	g_pManager->SetUserNameAndAlias("YaPhone", "66125");

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CYaPhoneDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CYaPhoneDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CYaPhoneDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CYaPhoneDlg::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	CDialog::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

	// TODO: Add your message handler code here
}

void CYaPhoneDlg::OnHelpAbout()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void CYaPhoneDlg::OnToolsChat()
{
	if (!m_pDlgChat)
	{
		m_pDlgChat = new CDlgChat(this);
		m_pDlgChat->Create(CDlgChat::IDD);	
		//m_pDlgChat->ShowWindow(SW_SHOW);
	}
}

void CYaPhoneDlg::OutputChatMessage(PString userName, PString message)
{
	if (!m_pDlgChat)
	{
		this->SendMessage(WM_CHAT_CREATE, 0, 0);//It must be send the message to main thread to create this child dialog.
	}

	if (m_pDlgChat)//check again in order to avoid NULL pointer access.
	{
		std::vector<PString> vecContext(2);
		vecContext[0] = m_remoteName;
		vecContext[1] = message;
		m_pDlgChat->SendMessage(WM_CHAT_STRING, (WPARAM)&vecContext, 0);
	}
}

void CYaPhoneDlg::UpdateVolumeSlider(unsigned volume, BOOL bMic)
{
	PostMessage(WM_UPDATE_VOLUME_SLIDER, bMic, volume);
}

void CYaPhoneDlg::UpdateVolumeIndicate(unsigned int volume, BOOL bMic)
{
	SendMessage(WM_UPDATE_VOLUME_INDICATE, bMic, volume);
}

LRESULT CYaPhoneDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INCOMING_CALL:
		{
			char		szIncomingName[64] = {0};
			char		*pszTitle = "You have an incoming call from:\r\n ";
			CDlgAnswer	dlgAnswer;

			dlgAnswer.SetDlgTitle("\tYaPhone");
			dlgAnswer.m_szRemoteInfo = pszTitle;
			dlgAnswer.m_szRemoteInfo += GetRemotePartyName();
			dlgAnswer.m_szRemoteInfo += _T("\t[");
			dlgAnswer.m_szRemoteInfo += GetRemotePartyUrl();
			dlgAnswer.m_szRemoteInfo += _T("]");
			dlgAnswer.m_szRemoteInfo += "\r\nRemote Application: ";
			dlgAnswer.m_szRemoteInfo += GetRemotePartyApp();

			dlgAnswer.DoModal();
			int nResponse = dlgAnswer.GetUserAction();		
			if (IDC_BTN_ANSWER == nResponse)
			{
				g_pManager->CallAnswer(m_nCallID);
			}
			else
			{
				g_pManager->CallReject(m_nCallID);
			}
		}
		break;
	case WM_CHAT_CREATE:
		{
			OnToolsChat();
		}
		break;
	case WM_UPDATE_VOLUME_SLIDER:
		{
			if ((BOOL)wParam)
			{
				if (!lParam)lParam = 10;
				m_sliderMicVol.SetPos((int)lParam);
			}
			else
			{
				m_sliderPlayVol.SetPos((int)lParam);
			}
		}
		break;
	case WM_UPDATE_VOLUME_INDICATE:
		{
			static int t = 0;
			if ((BOOL)wParam)
			{

				if (!(t++ % 10)) lParam = 10;
				else if (!(t & 30))lParam = 33;

				m_progressMicVol.SetPos((int)lParam);
			}
			else
			{
				if (!(t++ % 10)) lParam = 15;
				else if (!(t % 30))lParam = 67;

				m_progressPlayVol.SetPos((int)lParam);
			}
		}
		break;
	default:
		break;
	}

	return CDialog::DefWindowProc(message, wParam, lParam);
}

void CYaPhoneDlg::OnEditAccount()
{
	CDlgAccount dlgAccount;
	if (IDOK == dlgAccount.DoModal())
	{
		g_pManager->RegisterGatekeeper(dlgAccount.GetRegUrl().GetBuffer(0), dlgAccount.GetUserName().GetBuffer(0),
									dlgAccount.GetAccountName().GetBuffer(0), dlgAccount.GetPassword().GetBuffer(0));
	}
}

void CYaPhoneDlg::AppendLog(const char *szLog, COLORREF color)
{
	/*WCHAR	wszLog[256] = {0};
	INT		wSize = sizeof(wszLog)/sizeof(wszLog[0]);

	MultiByteToWideChar(CP_ACP, 0, szLog, strlen(szLog)+1, wszLog, wSize);*/

	// formatting
	int nOldLines = 0, nNewLines = 0, nScroll = 0;
	CHARFORMAT cf;

	m_ctrlLog.GetDefaultCharFormat(cf);

	cf.dwMask		= CFM_COLOR|CFM_BOLD;
	cf.crTextColor	= color;
	cf.dwEffects	= CFE_DISABLED;

	CString strNew = (CString)szLog + (CString)"\r\n";

	// Save number of lines before insertion of new text
	nOldLines = m_ctrlLog.GetLineCount();

	// Set insertion point to end of text
	int nInsertionPoint = m_ctrlLog.GetWindowTextLength();
	m_ctrlLog.SetSel(nInsertionPoint, -1);

	// Set the character format
	m_ctrlLog.SetSelectionCharFormat(cf);

	m_ctrlLog.ReplaceSel((LPCTSTR) strNew.GetBuffer(0));

	// Get new line count
	nNewLines = m_ctrlLog.GetLineCount();
	// Scroll by the number of lines just inserted
	nScroll = nNewLines - nOldLines;
	m_ctrlLog.LineScroll(nScroll);
}

void CYaPhoneDlg::StackLogCb(int lvl, char *szLog)
{
	CYaPhoneDlg* pDlg = (CYaPhoneDlg*)theApp.m_pMainWnd;
	if (pDlg)
		pDlg->AppendLog(szLog);
}

void CYaPhoneDlg::SetCallState(CYaManager::CallState eCallState)
{
	m_callState = eCallState;

	if (!::IsWindow(m_hWnd))//before change button text,we should check it's window availability.
		return;

	switch (m_callState)
	{
	case CYaManager::Connected:
		{
			SetDlgItemText(IDC_BTN_CALL, _T("Hangup"));
			GetDlgItem(IDC_BTN_CALL)->EnableWindow(TRUE);
		}
		break;
	case CYaManager::Released:
		{
			SetDlgItemText(IDC_BTN_CALL, _T("Call"));
			GetDlgItem(IDC_BTN_CALL)->EnableWindow(TRUE);

			m_sliderPlayVol.SetPos(0);
			m_sliderMicVol.SetPos(0);
			m_progressPlayVol.SetPos(0);
			m_progressMicVol.SetPos(0);
		}
		break;
	case CYaManager::Calling:
		{
			GetDlgItem(IDC_BTN_CALL)->EnableWindow(FALSE);
		}
		break;
	case CYaManager::StandBy:
		{
			GetDlgItem(IDC_BTN_CALL)->EnableWindow(TRUE);
		}
		break;
	default:
		{
			SetDlgItemText(IDC_BTN_CALL, _T("Call"));
			GetDlgItem(IDC_BTN_CALL)->EnableWindow(TRUE);
		}
		break;
	}
}

void CYaPhoneDlg::SetRemotePartyInfo(PString token, PString remoteName, PString remoteApp, PString remoteUrl)
{
	m_token			= token;
	m_remoteName	= remoteName;
	m_remoteApp		= remoteApp;
	m_remoteUrl		= remoteUrl;
}

void CYaPhoneDlg::GetRemotePartyInfo(PString &token, PString &remoteName, PString &remoteApp, PString &remoteUrl)
{
	token		= m_token;
	remoteName	= m_remoteName;
	remoteApp	= m_remoteApp;
	remoteUrl	= m_remoteUrl;
}

void CYaPhoneDlg::AddIconTray(void)
{
	//set system tray
	NOTIFYICONDATA iconTray;

	iconTray.cbSize = sizeof(NOTIFYICONDATA);
	iconTray.hWnd	= this->m_hWnd;
	iconTray.uID	= IDR_MAINFRAME;
	iconTray.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	iconTray.uCallbackMessage = WM_NOTIFYICON;
	iconTray.hIcon	= m_hIcon;
	strcpy(iconTray.szTip, _T("YaPhone"));

	Shell_NotifyIcon(NIM_ADD, &iconTray);
}

void CYaPhoneDlg::RemoveIconTray(void)
{
	NOTIFYICONDATA iconTray;

	//remove system tray
	iconTray.cbSize = sizeof(NOTIFYICONDATA);
	iconTray.uID	= IDR_MAINFRAME;
	iconTray.hWnd	= this->m_hWnd;

	Shell_NotifyIcon(NIM_DELETE, &iconTray);
}

void CYaPhoneDlg::ChangeIconTray(CString strMsg)
{
	WINDOWPLACEMENT wPos;

	GetWindowPlacement(&wPos);
	if (!(wPos.showCmd & SW_MINIMIZE))//If it is not in minimize mode currently,we shall ignore it.
		return;

	NOTIFYICONDATA iconTray;
	iconTray.cbSize = sizeof(NOTIFYICONDATA);
	iconTray.hWnd	= this->GetSafeHwnd();
	iconTray.uID	= IDR_MAINFRAME;
	iconTray.hIcon	= m_hIcon;
	iconTray.uFlags = strMsg.IsEmpty() ? NIF_ICON : NIF_ICON | NIF_TIP;
	if (!strMsg.IsEmpty())
	{
		strcpy(iconTray.szInfoTitle, _T("YaPhone"));
		strcpy(iconTray.szInfo, (LPCSTR)strMsg);
		iconTray.dwInfoFlags = NIIF_INFO;
		iconTray.uTimeout	= 1000; //in milliseconds
	}

	Shell_NotifyIcon(NIM_MODIFY, &iconTray);
}

LRESULT CYaPhoneDlg::OnTrayNotify(WPARAM wParam, LPARAM lParam)
{
	if (IDR_MAINFRAME == wParam)
	{
		switch (lParam)
		{
		case WM_LBUTTONDBLCLK:
			{
				ShowWindow(SW_SHOW);
				SetForegroundWindow();
			}
			break;
		case WM_LBUTTONUP:
			{
				/*WINDOWPLACEMENT wPos;

				GetWindowPlacement(&wPos);
				if (wPos.showCmd && SW_MINIMIZE)//if minimized
				{
				PostMessage(WM_SYSCOMMAND, SW_RESTORE);
				SetForegroundWindow();
				}
				else
				{
				PostMessage(WM_SYSCOMMAND, SW_MINIMIZE);
				}*/
			}
			break;
		default:
			break;
		}
	}

	return 0L;
}

void CYaPhoneDlg::OnBnClickedBtnCall()
{
	UpdateData(TRUE);

	CString strBtnText;

	GetDlgItemText(IDC_BTN_CALL, strBtnText);
	if (!strBtnText.Compare("Call"))
	{
		PString calleeAddr = "h323:";
		if (m_szAddr.IsEmpty())
			return;

		calleeAddr += m_szAddr.GetBuffer(0);

		g_pManager->CallMake((const char*)calleeAddr, m_nCallID);
	}
	else
	{
		g_pManager->CallDrop(m_nCallID);
	}
}

BOOL CYaPhoneDlg::PreTranslateMessage(MSG* pMsg)
{
	//mask enter key
	if (WM_KEYDOWN == pMsg->message && VK_RETURN == pMsg->wParam)
	{
		GetDlgItem(IDC_CMB_ADDRESS)->SetFocus();
		OnBnClickedBtnCall();
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CYaPhoneDlg::OnEditPreferences()
{
	CDlgPreferences			dlgPrefences;
	CDlgInfoPref			dlgGeneralInfo;
	CDlgInfoPref			dlgVideoInfo;
	CDlgInfoPref			dlgNetInfo;

	CDlgGeneralPref			dlgGeneral;
	CDlgCallSettingsPref	dlgCallSetting;
	dlgPrefences.AddPage(dlgGeneralInfo, "General");
	dlgPrefences.AddPage(dlgGeneral, "General", &dlgGeneralInfo);
	dlgPrefences.AddPage(dlgCallSetting, "Call Setting", &dlgGeneralInfo);

	/*CDlgVideoSourcesPref	dlgVideoSources;
	CDlgVideoProcotolPref	dlgVideoProtocol;
	dlgPrefences.AddPage(dlgVideoInfo, "Video");
	dlgPrefences.AddPage(dlgVideoSources, "Video Source", &dlgVideoInfo);
	dlgPrefences.AddPage(dlgVideoProtocol, "Video Protocol", &dlgVideoInfo);*/
	
	CDlgH323Pref	dlgH323;
	dlgPrefences.AddPage(dlgH323, "H.323");

	CDlgNetPortsPref		dlgNetPorts;
	CDlgNetBandwidthPref	dlgNetBandwidth;
	CDlgNetConnectionPref	dlgNetConnction;
	dlgPrefences.AddPage(dlgNetInfo, "Network");
	dlgPrefences.AddPage(dlgNetConnction, "Connection", &dlgNetInfo);
	dlgPrefences.AddPage(dlgNetPorts, "Ports", &dlgNetInfo);
	dlgPrefences.AddPage(dlgNetBandwidth, "Bandwidth", &dlgNetInfo);

	CDlgVideoSourcesPref	dlgVideoSource;
	dlgPrefences.AddPage(dlgVideoSource, "Video Source");

	dlgPrefences.SetTitle(_T("Preferences"));	
	dlgPrefences.SetConstantText(_T("YaPhone"));
	dlgPrefences.SetValidateBeforeChangingPages(true);

	dlgPrefences.DoModal();
}
