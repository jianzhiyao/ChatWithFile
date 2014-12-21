#if !defined(AFX_FILECLIENTSOCKET_H__61D291BB_C375_460C_B55E_5EDFC50064F2__INCLUDED_)
#define AFX_FILECLIENTSOCKET_H__61D291BB_C375_460C_B55E_5EDFC50064F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileClientSocket.h : header file
//
#include "ChatServerDlg.h"
#include "Message.h"


/////////////////////////////////////////////////////////////////////////////
// CFileClientSocket command target

class CFileClientSocket : public CSocket
{
// Attributes
public:

// Operations
public:
	CFileClientSocket();
	virtual ~CFileClientSocket();

// Overrides
public:
	CSocketFile* m_pFile;
	CArchive* m_pArchiveIn;
	CArchive* m_pArchiveOut;
	
	void Init();
	void Abort();

	BOOL SendMsg(CMessage* pMsg);
	void ReceiveMsg(CMessage* pMsg);
	
	CFileClientSocket(CChatServerDlg* pdlgMain);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileClientSocket)
	public:
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CFileClientSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
	CChatServerDlg* m_pdlgMain;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILECLIENTSOCKET_H__61D291BB_C375_460C_B55E_5EDFC50064F2__INCLUDED_)
