// DlgChat.cpp : implementation file
//

#include "stdafx.h"
#include "YaPhone.h"
#include "DlgChat.h"
#include "YaPhoneDlg.h"

// CDlgChat dialog

IMPLEMENT_DYNAMIC(CDlgChat, CDialog)

CDlgChat::CDlgChat(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgChat::IDD, pParent)
	, m_szTalkMsg(_T(""))
{
	m_pParent = pParent;
}

CDlgChat::~CDlgChat()
{
}

void CDlgChat::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT_MSG, m_editMsg);
	DDX_Control(pDX, IDC_RICHEDIT_SMSG, m_editTalkMsg);
	DDX_Text(pDX, IDC_RICHEDIT_SMSG, m_szTalkMsg);
}


BEGIN_MESSAGE_MAP(CDlgChat, CDialog)
	ON_BN_CLICKED(IDC_BTN_SEND, &CDlgChat::OnBnClickedBtnSend)
	ON_BN_CLICKED(IDOK, &CDlgChat::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgChat message handlers

void CDlgChat::OnBnClickedBtnSend()
{
	UpdateData(TRUE);

	if (m_szTalkMsg.IsEmpty())
	{
		GetDlgItem(IDC_RICHEDIT_SMSG)->SetFocus();
		return;
	}

	g_pManager->SendTextMessage(g_pManager->GetCurrentCallToken(), (PString)m_szTalkMsg);

	OutputMessage(PString("You "), (PString)m_szTalkMsg, FALSE);

	m_szTalkMsg.Empty();
	UpdateData(FALSE);
	GetDlgItem(IDC_RICHEDIT_SMSG)->SetFocus();
}

void CDlgChat::PostNcDestroy()
{
	CDialog::PostNcDestroy();

	if (m_pParent)
	{
		((CYaPhoneDlg*)m_pParent)->m_pDlgChat = NULL;
	}

	delete this;
}

LRESULT CDlgChat::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CHAT_STRING:
		{
			//I'm not familiar with the STL
			std::vector<PString> *pContext = (std::vector<PString>*)wParam;
			std::vector<PString> vecContext;
			for (std::vector<PString>::iterator it = pContext->begin(); it != pContext->end(); ++it)
			{
				vecContext.push_back(*it);
			}

			OutputMessage((PString)vecContext[0], (PString)vecContext[1], TRUE);
		}
		break;
	default:
		break;
	}

	return CDialog::DefWindowProc(message, wParam, lParam);
}

void CDlgChat::OutputMessage(PString userName, PString message, BOOL isIncoming/* = FALSE*/)
{
	// formatting
	int nOldLines = 0, nNewLines = 0, nScroll = 0;
	CHARFORMAT cf;
	//CHARFORMAT oldcf;

	m_editMsg.GetDefaultCharFormat(cf);
	//m_editTalkMsg.GetDefaultCharFormat(oldcf);
	//m_editTalkMsg.GetSelectionCharFormat(cf);

	if(!isIncoming) // Outgoing chat message
	{
		cf.dwMask		= CFM_COLOR | CFM_BOLD | CFM_FACE | CFM_SIZE;
		cf.crTextColor	= RGB(0,153,0);
		cf.dwEffects	= CFE_DISABLED;
	}
	if(isIncoming) // Incoming chat message
	{
		cf.dwMask		= CFM_COLOR | CFM_BOLD | CFM_FACE | CFM_SIZE;
		cf.crTextColor	= RGB(0,0,153);
		cf.dwEffects	= CFE_DISABLED;// To disable CFE_AUTOCOLOR
	}

	CString strNew, strOld;
	PString MSG = userName + " says :\r\n" + message;
	strNew = CString((LPCTSTR)MSG) + CString("\r\n");;

	// Save number of lines before insertion of new text
	nOldLines = m_editMsg.GetLineCount();

	/*m_editMsg.GetWindowText(strOld);
	m_editMsg.SetSel(strOld.GetLength(), strOld.GetLength());*/

	// Set insertion point to end of text
	int nInsertionPoint = m_editMsg.GetWindowTextLength();
	m_editMsg.SetSel(nInsertionPoint, -1);

	// Set the character format
	m_editMsg.SetSelectionCharFormat(cf);
	m_editMsg.Invalidate();

	m_editMsg.ReplaceSel((LPCTSTR) strNew.GetBuffer(0));
	//m_editMsg.SetSel( strNew.GetLength()+strOld.GetLength(), strNew.GetLength()+strOld.GetLength());
	//m_editMsg.SetSelectionCharFormat(oldcf);

	// Get new line count
	nNewLines = m_editMsg.GetLineCount();
	// Scroll by the number of lines just inserted
	nScroll = nNewLines - nOldLines;
	m_editMsg.LineScroll(nScroll);

	UpdateData(FALSE);
}

BOOL CDlgChat::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYDOWN == pMsg->message && VK_RETURN == pMsg->wParam)
	{
		GetDlgItem(IDC_BTN_SEND)->SetFocus();
		OnBnClickedBtnSend();
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgChat::OnCancel()
{
	DestroyWindow();
}

void CDlgChat::OnBnClickedOk()
{
	DestroyWindow();
}
