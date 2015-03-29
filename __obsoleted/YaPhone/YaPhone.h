// YaPhone.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "YaManager.h"

//USER Defined Message
#define WM_INCOMING_CALL			WM_USER + 0x1
#define WM_NOTIFYICON				WM_USER + 0x2
#define WM_CLOSE_CHAT				WM_USER + 0x3
#define WM_CHAT_STRING				WM_USER + 0x4
#define WM_CHAT_CREATE				WM_USER + 0x5
#define WM_UPDATE_VOLUME_SLIDER		WM_USER + 0x6
#define WM_UPDATE_VOLUME_INDICATE	WM_USER + 0x7

// CYaPhoneApp:
// See YaPhone.cpp for the implementation of this class
//

class CYaPhoneApp : public CWinApp, public PProcess
{
	PCLASSINFO(CYaPhoneApp, PProcess);
public:
	CYaPhoneApp();
	
	void Main(){};
// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
public:
	virtual int ExitInstance();
};

extern CYaPhoneApp		theApp;
extern CYaManager		*g_pManager;