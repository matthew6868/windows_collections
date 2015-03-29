/*********************************************************************

SAPrefsDialog

Copyright (C) 2002 Smaller Animals Software, Inc.

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.

3. This notice may not be removed or altered from any source distribution.

http://www.smalleranimals.com
smallest@smalleranimals.com

**********************************************************************/
#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "SAPrefsSubDlg.h"
#include "SAPrefsStatic.h"

#define WM_CHANGE_PAGE		(WM_APP + 100)
#define WM_SET_FOCUS_WND	(WM_APP + 101)

struct pageStruct
{
	CSAPrefsSubDlg *pDlg;
	UINT id;
	CSAPrefsSubDlg *pDlgParent;
	CString csCaption;
};

// CDlgPreferences dialog

class CDlgPreferences : public CDialog
{
	DECLARE_DYNAMIC(CDlgPreferences)

public:
	CDlgPreferences(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgPreferences();

	// dialog title
	void SetTitle(CString t)   {m_csTitle = t;}

	// used in the pretty shaded static control
	void SetConstantText(CString t)   {m_csConstantText = t;}

	// if you set this, UpdateData will be called on the current page before
	// switching to the new page. if UpdateData fails, the page is not changed.
	void SetValidateBeforeChangingPages(bool b)	{m_bValidateBeforeChangingPage = b;}

	// add a page (page, page title, optional parent)
	bool AddPage(CSAPrefsSubDlg &page, const char *pCaption, CSAPrefsSubDlg *pDlgParent = NULL);

	// show a page
	bool ShowPage(int iPage);
	bool ShowPage(CSAPrefsSubDlg * pPage);

	// end the dialog with a special return code
	void EndSpecial(UINT res, bool bOk = true);

	// set the first page
	void SetStartPage(CSAPrefsSubDlg *pPage = NULL) {m_pStartPage = pPage;}

	bool EndOK();
// Dialog Data
	enum { IDD = IDD_DLG_SETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// check to see if this dlg has already been added to the tree
	HTREEITEM FindHTREEItemForDlg(CSAPrefsSubDlg *pParent);

	DECLARE_MESSAGE_MAP()
private:
	virtual BOOL OnInitDialog();
//	afx_msg void OnNMClickTreeSetup(NMHDR *pNMHDR, LRESULT *pResult);

	CTreeCtrl		m_pageTree;
	CSAPrefsStatic	m_captionBar;
	CRect			m_frameRect;
	CStatic			m_boundingFrame;
	CPtrArray		m_pages;
	int				m_iCurPage;
	CString			m_csTitle, m_csConstantText;

	bool			m_bValidateBeforeChangingPage;

	CSAPrefsSubDlg	*m_pStartPage;

	// store info about *pDlgs that have been added to 
	// the tree - used for quick lookup of parent nodes
	// DWORDs are used because HTREEITEMs can't be... blame Microsoft
	CMap< CSAPrefsSubDlg *, CSAPrefsSubDlg *, DWORD, DWORD&  > m_dlgMap;
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnTvnSelchangedTreeSetup(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnTvnSelchangingTreeSetup(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnTvnGetdispinfoTreeSetup(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg long OnChangePage(UINT, LONG);
	afx_msg long OnSetFocusWnd(UINT, LONG);
protected:
	virtual void OnCancel();
	virtual void OnOK();
public:
	afx_msg void OnBnClickedBtnApply();
};
