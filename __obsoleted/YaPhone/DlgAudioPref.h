#pragma once


// CDlgAudioPref dialog

class CDlgAudioPref : public CDialog
{
	DECLARE_DYNAMIC(CDlgAudioPref)

public:
	CDlgAudioPref(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgAudioPref();

// Dialog Data
	enum { IDD = IDD_FORM_VIDEO_PROTOCOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
