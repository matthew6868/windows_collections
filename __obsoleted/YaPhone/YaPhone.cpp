// YaPhone.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "YaPhone.h"
#include "YaPhoneDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const TCHAR *strApp = _T("{A99DF0BD-EEAC-40d8-9DB4-A8E7D852327D}");
// CYaPhoneApp

BEGIN_MESSAGE_MAP(CYaPhoneApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CYaPhoneApp construction

CYaPhoneApp::CYaPhoneApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	g_pManager = new CYaManager;
	if (!g_pManager)
		ASSERT(0);
}


// The one and only CYaPhoneApp object

CYaPhoneApp		theApp;
CYaManager		*g_pManager = NULL;

// CYaPhoneApp initialization

BOOL CYaPhoneApp::InitInstance()
{
	/////////////////////////////////////////////////////////////////////////////
	// make sure only one instance run whenever, because it can't listen 1720 TCP port more than once at same time. 
	HANDLE handle = ::CreateMutex(NULL,TRUE,strApp);
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CWnd* pPrevWnd = CWnd::GetDesktopWindow()->GetWindow(GW_CHILD);
		while(pPrevWnd)
		{
			if(::GetProp(pPrevWnd->GetSafeHwnd(),strApp))
			{
				if(pPrevWnd->IsIconic())	
					pPrevWnd->ShowWindow(SW_RESTORE);
				pPrevWnd->SetForegroundWindow();	
				pPrevWnd->GetLastActivePopup()->SetForegroundWindow();
				ReleaseMutex(handle);

				//
				AfxMessageBox(_T("YaPhone:One of Instance is already running..."));
				//

				return FALSE;
			}
			pPrevWnd = pPrevWnd->GetWindow(GW_HWNDNEXT);
		}
		TRACE0("Could not find previous instance main window!\n");
		ReleaseMutex(handle);

		//
		AfxMessageBox(_T("YaPhone:One of instance is already running...\t"));
		//

		return FALSE;
	}
	ReleaseMutex(handle);

//TODO: call AfxInitRichEdit2() to initialize richedit2 library.
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	CYaPhoneDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CYaPhoneApp::ExitInstance()
{
	if (g_pManager)
	{
		delete g_pManager;
		g_pManager = NULL;
	}

	return __super::ExitInstance();
}
