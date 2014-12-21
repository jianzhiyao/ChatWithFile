// FileListenSocket.cpp : implementation file
//

#include "stdafx.h"
#include "ChatServer.h"
#include "FileListenSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileListenSocket

CFileListenSocket::CFileListenSocket(CChatServerDlg* pdlgMain)
{
	m_pdlgMain = pdlgMain;
}

CFileListenSocket::CFileListenSocket()
{
	m_pdlgMain = NULL;
}

CFileListenSocket::~CFileListenSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CFileListenSocket, CSocket)
	//{{AFX_MSG_MAP(CFileListenSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CFileListenSocket member functions
void CFileListenSocket::OnAccept(int nErrorCode) 
{	
	m_pdlgMain->ProcessAccept();
	CSocket::OnAccept(nErrorCode);
}