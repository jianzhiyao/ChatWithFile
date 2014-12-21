#if !defined(AFX_FILELISTENSOCKET_H__09FF37E1_4F2D_48D5_9856_29032AF30093__INCLUDED_)
#define AFX_FILELISTENSOCKET_H__09FF37E1_4F2D_48D5_9856_29032AF30093__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileListenSocket.h : header file
//

#include "ChatServerDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CFileListenSocket command target

class CFileListenSocket : public CSocket
{
// Attributes
public:

// Operations
public:
	CFileListenSocket();
	virtual ~CFileListenSocket();

// Overrides
public:
	CFileListenSocket(CChatServerDlg* pdlgMain);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileListenSocket)
	public:
	virtual void OnAccept(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CFileListenSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
	CChatServerDlg* m_pdlgMain;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILELISTENSOCKET_H__09FF37E1_4F2D_48D5_9856_29032AF30093__INCLUDED_)
