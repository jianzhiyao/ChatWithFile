// ClientSocket.cpp : implementation file
//

#include "stdafx.h"
#include "ChatServer.h"
#include "ClientSocket.h"
#include "ChatServerDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClientSocket

CClientSocket::CClientSocket()
{
}

CClientSocket::~CClientSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CClientSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CClientSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CClientSocket member functions

void CClientSocket::OnConnect(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(nErrorCode)
	{
		((CChatServerDlg*)(AfxGetApp()->m_pMainWnd))->m_cMsgBox.AddString("false!please try again");
		return;
	}
	((CChatServerDlg*)(AfxGetApp()->m_pMainWnd))->m_cMsgBox.AddString("successful connection!");
	((CChatServerDlg*)(AfxGetApp()->m_pMainWnd))->m_cMsgBox.SetTopIndex(
		((CChatServerDlg*)(AfxGetApp()->m_pMainWnd))->m_cMsgBox.GetCount()-1
		);
	CAsyncSocket::OnConnect(nErrorCode);
}

void CClientSocket::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	((CChatServerDlg*)(AfxGetApp()->m_pMainWnd))->m_cMsgBox.AddString("server disconneted");
	((CChatServerDlg*)(AfxGetApp()->m_pMainWnd))->m_cMsgBox.SetTopIndex(
		((CChatServerDlg*)(AfxGetApp()->m_pMainWnd))->m_cMsgBox.GetCount()-1
	);
	CAsyncSocket::OnClose(nErrorCode);
}

void CClientSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	char buf[200];
	int n=Receive(buf,200);
	buf[n]='\0';
	CString message;
	message.Format("received:%s",buf);
	((CChatServerDlg*)(AfxGetApp()->m_pMainWnd))->m_cMsgBox.AddString(message);
	((CChatServerDlg*)(AfxGetApp()->m_pMainWnd))->m_cMsgBox.SetTopIndex(
		((CChatServerDlg*)(AfxGetApp()->m_pMainWnd))->m_cMsgBox.GetCount()-1
		);
	CAsyncSocket::OnReceive(nErrorCode);
}
