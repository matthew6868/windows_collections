#pragma once


// CDlgVideo dialog

class CDlgVideo : public CDialog
{
	DECLARE_DYNAMIC(CDlgVideo)

public:
	CDlgVideo(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgVideo();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void OnCancel();
	virtual void PostNcDestroy();
public:
	afx_msg void ShowYUVImage(unsigned char *pBuf, DWORD dwWidth, DWORD dwHeight);

	afx_msg void OnBnClickedOk();

private:
	CWnd			*m_pParent;
	CDC				*m_pDCShow;
	CRect			m_rectShow;

	virtual BOOL OnInitDialog();
};
