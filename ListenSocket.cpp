// ListenSocket.cpp : implementation file
//

#include "stdafx.h"
#include "ChatServer.h"
#include "ListenSocket.h"
#include "ChatServerDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListenSocket

CListenSocket::CListenSocket()
{
}

CListenSocket::~CListenSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CListenSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CListenSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CListenSocket member functions

void CListenSocket::OnAccept(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	Accept(((CChatServerDlg*)(AfxGetApp()->m_pMainWnd))->m_ServerSocket);
	((CChatServerDlg*)(AfxGetApp()->m_pMainWnd))->m_ServerSocket.AsyncSelect(FD_READ|FD_CLOSE);

	((CChatServerDlg*)(AfxGetApp()->m_pMainWnd))->m_cMsgBox.AddString("receive a new connection!");

	((CChatServerDlg*)(AfxGetApp()->m_pMainWnd))->m_cMsgBox.SetTopIndex(
		((CChatServerDlg*)(AfxGetApp()->m_pMainWnd))->m_cMsgBox.GetCount()-1
		);
	Close();
	CAsyncSocket::OnAccept(nErrorCode);
}
