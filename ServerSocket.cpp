// ServerSocket.cpp : implementation file
//

#include "stdafx.h"
#include "ChatServer.h"
#include "ServerSocket.h"
#include "ChatServerDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerSocket

CServerSocket::CServerSocket()
{
}

CServerSocket::~CServerSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CServerSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CServerSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CServerSocket member functions

void CServerSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	char buf[200];
	int n=Receive(buf,200);
	buf[n]='\0';
	CString message;
	message.Format("Received:%s",buf);
	((CChatServerDlg*)(AfxGetApp()->m_pMainWnd))->m_cMsgBox.AddString(message);
	((CChatServerDlg*)(AfxGetApp()->m_pMainWnd))->m_cMsgBox.SetTopIndex(
		((CChatServerDlg*)(AfxGetApp()->m_pMainWnd))->m_cMsgBox.GetCount()-1
		);
	CAsyncSocket::OnReceive(nErrorCode);
}

void CServerSocket::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	((CChatServerDlg*)(AfxGetApp()->m_pMainWnd))->m_cMsgBox.AddString("client is disconnected");
	((CChatServerDlg*)(AfxGetApp()->m_pMainWnd))->m_cMsgBox.SetTopIndex(
		((CChatServerDlg*)(AfxGetApp()->m_pMainWnd))->m_cMsgBox.GetCount()-1
		);
	CAsyncSocket::OnClose(nErrorCode);
}
