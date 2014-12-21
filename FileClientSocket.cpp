// FileClientSocket.cpp : implementation file
//

#include "stdafx.h"
#include "ChatServer.h"
#include "FileClientSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileClientSocket

CFileClientSocket::CFileClientSocket(CChatServerDlg* pdlgMain)
{
	m_pdlgMain = pdlgMain;

	m_pFile = NULL;
	m_pArchiveIn = NULL;
	m_pArchiveOut = NULL;
}

CFileClientSocket::CFileClientSocket()
{
	m_pdlgMain = NULL;

	m_pFile = NULL;
	m_pArchiveIn = NULL;
	m_pArchiveOut = NULL;
}

CFileClientSocket::~CFileClientSocket()
{
}



// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CFileClientSocket, CSocket)
	//{{AFX_MSG_MAP(CFileClientSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CFileClientSocket member functions
void CFileClientSocket::Init()
{
	m_pFile = new CSocketFile(this);
	m_pArchiveIn = new CArchive(m_pFile,CArchive::load);
	m_pArchiveOut = new CArchive(m_pFile,CArchive::store);
}

void CFileClientSocket::Abort()
{
	if (m_pArchiveOut != NULL)
	{
		m_pArchiveOut->Abort();
		delete m_pArchiveOut;
		m_pArchiveOut = NULL;
	}
}

BOOL CFileClientSocket::SendMsg(CMessage *pMsg)
{
	if (m_pArchiveOut != NULL)
	{
		TRY
		{
			pMsg->Serialize(*m_pArchiveOut);
			m_pArchiveOut->Flush();

			return TRUE;
		}
		CATCH(CFileException, e)
		{
			m_pArchiveOut->Abort();
			delete m_pArchiveOut;
			m_pArchiveOut = NULL;
		}
		END_CATCH
	}
	
	return FALSE;
}

void CFileClientSocket::ReceiveMsg(CMessage *pMsg)
{
	pMsg->Serialize(*m_pArchiveIn);
}

void CFileClientSocket::OnReceive(int nErrorCode) 
{
	m_pdlgMain->ProcessReceive(this);
	CSocket::OnReceive(nErrorCode);
}
