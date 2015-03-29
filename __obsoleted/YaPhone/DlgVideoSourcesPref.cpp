// DlgVideoSourcesPref.cpp : implementation file
//

#include "stdafx.h"
#include "YaPhone.h"
#include "DlgVideoSourcesPref.h"


// CDlgVideoSourcesPref dialog

IMPLEMENT_DYNAMIC(CDlgVideoSourcesPref, CSAPrefsSubDlg)

CDlgVideoSourcesPref::CDlgVideoSourcesPref(CWnd* pParent /*=NULL*/)
	: CSAPrefsSubDlg(CDlgVideoSourcesPref::IDD, pParent)
{

}

CDlgVideoSourcesPref::~CDlgVideoSourcesPref()
{
}

void CDlgVideoSourcesPref::DoDataExchange(CDataExchange* pDX)
{
	CSAPrefsSubDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_VIDEO_DEVICE, m_cmbVideoDevice);
}


BEGIN_MESSAGE_MAP(CDlgVideoSourcesPref, CSAPrefsSubDlg)
END_MESSAGE_MAP()


// CDlgVideoSourcesPref message handlers

BOOL CDlgVideoSourcesPref::OnInitDialog()
{
	CSAPrefsSubDlg::OnInitDialog();

	//PVideoDevice::OpenArgs videoDevice = g_pManager->GetVideoPreviewDevice();
	PStringArray videoManager;

	videoManager = PVideoInputDevice::GetDriverNames();
	/*for (PINDEX i = 0; i < videoManager.GetSize(); ++i)
	{
		m_cmbVideoDevice.AddString((const char*)videoManager[i]);
	}*/
	if (videoManager.GetValuesIndex(PString("FakeVideo")) != P_MAX_INDEX)
		videoManager.RemoveAt(videoManager.GetValuesIndex(PString("FakeVideo")));

 	videoManager = PVideoInputDevice::GetDriversDeviceNames(videoManager[0]);
	for (PINDEX i = 0; i < videoManager.GetSize(); ++i)
	{
		m_cmbVideoDevice.AddString((const char*)videoManager[i]);
	}

	m_cmbVideoDevice.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
