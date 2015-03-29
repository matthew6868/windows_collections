// DlgVideo.cpp : implementation file
//

#include "stdafx.h"
#include "YaPhone.h"
#include "DlgVideo.h"
#include "YaPhoneDlg.h"

// CDlgVideo dialog

IMPLEMENT_DYNAMIC(CDlgVideo, CDialog)

CDlgVideo::CDlgVideo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgVideo::IDD, pParent)
{
	m_pParent = pParent;

	m_pDCShow = NULL;
}

CDlgVideo::~CDlgVideo()
{
}

void CDlgVideo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgVideo, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgVideo::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgVideo message handlers

void CDlgVideo::OnCancel()
{
	DestroyWindow();
	//CDialog::OnCancel();
}

void CDlgVideo::PostNcDestroy()
{	
	CDialog::PostNcDestroy();

	if (m_pParent)
	{
		((CYaPhoneDlg*)m_pParent)->m_pDlgVideo = NULL;
	}

	delete this;
}

void CDlgVideo::OnBnClickedOk()
{
	DestroyWindow();
	//OnOK();
}

BOOL CDlgVideo::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pDCShow = GetDlgItem(IDC_SHOW)->GetDC();
	GetDlgItem(IDC_SHOW)->GetClientRect(&m_rectShow);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgVideo::ShowYUVImage(unsigned char *pBuf, DWORD dwWidth, DWORD dwHeight)
{
	if (!pBuf)
		return;

	unsigned char* pBmp = NULL;

	BITMAPINFO m_BmpInfo;
	m_BmpInfo.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	m_BmpInfo.bmiHeader.biWidth			= dwWidth;
	m_BmpInfo.bmiHeader.biHeight		= -dwHeight;
	m_BmpInfo.bmiHeader.biCompression	= BI_RGB;
	m_BmpInfo.bmiHeader.biPlanes		= 1;
	m_BmpInfo.bmiHeader.biBitCount		= 24;
	m_BmpInfo.bmiHeader.biSizeImage		= dwHeight*dwWidth*3;    

	HBITMAP hBmp;
	CDC MemDC;

	hBmp = CreateDIBitmap(
		m_pDCShow->m_hDC,					// handle to device context
		(BITMAPINFOHEADER*)(&m_BmpInfo),   //	pointer to bitmap size and format data
		CBM_INIT,	// initialization flag
		pBmp,	// pointer to initialization data
		&m_BmpInfo,	// pointer to bitmap color-format data
		DIB_RGB_COLORS	// color-data usage
		);

	if(hBmp == NULL)
	{
		m_pDCShow->DeleteDC();
		return;
	}

	MemDC.CreateCompatibleDC(m_pDCShow);

	SelectObject(MemDC.m_hDC, hBmp);

	BOOL ret = m_pDCShow->BitBlt(0, 0, m_rectShow.Width(), m_rectShow.Height(), &MemDC, 0, 0, SRCCOPY);
	
	DeleteObject(hBmp);
	MemDC.DeleteDC();
	//////////////////////////////////////////////////////////////////////////
}