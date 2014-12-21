// ChatServer.h : main header file for the CHATSERVER application
//

#if !defined(AFX_CHATSERVER_H__E6A3D994_FFB7_404F_8FA5_1A9F0C259F08__INCLUDED_)
#define AFX_CHATSERVER_H__E6A3D994_FFB7_404F_8FA5_1A9F0C259F08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CChatServerApp:
// See ChatServer.cpp for the implementation of this class
//

class CChatServerApp : public CWinApp
{
public:
	CChatServerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChatServerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CChatServerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHATSERVER_H__E6A3D994_FFB7_404F_8FA5_1A9F0C259F08__INCLUDED_)
