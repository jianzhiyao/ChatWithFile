// ChatServerDlg.h : header file
//

#include "FileClientSocket.h"
#include "FileListenSocket.h"

#if !defined(AFX_CHATSERVERDLG_H__E0596E36_E1D2_4032_AB51_3855F345D459__INCLUDED_)
#define AFX_CHATSERVERDLG_H__E0596E36_E1D2_4032_AB51_3855F345D459__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ListenSocket.h"
#include "ServerSocket.h"
#include "ClientSocket.h"
#include "FileClientSocket.h"
#include "FileListenSocket.h"

/////////////////////////////////////////////////////////////////////////////
// CChatServerDlg dialog

#define PORT 1024
#define BLOCKSIZE 1024         //»º³åÇø´óÐ¡

#define SERVER 0
#define CLIENT 1

#define CONNECT_BE_ACCEPT    0x00
#define CONNECT_BE_REFUSE    0x01
#define DISCONNECT           0x02
#define REQUEST              0x03
#define ACCEPT               0x04
#define REFUSE               0x05
#define CANCEL               0x06

class CFileClientSocket;
class CFileListenSocket;
class CChatServerDlg : public CDialog
{
// Construction
public:
	void SendFile(CSocket &senSo);
	void ReceiveFile(CSocket &recSo);
	void ProcessReceive(CFileClientSocket* pSocket);
	void ProcessAccept();
	CServerSocket m_ServerSocket;
	CListenSocket m_ListenSocket;
	CClientSocket m_ClientSocket;
	bool isServer;
	
	CFileListenSocket* m_psockServer;
	CFileClientSocket* m_psockClient;

	CString m_strPath;
	DWORD m_dwFileSize;

	int m_nTimer;
	BOOL m_bIsClient;
	BOOL m_bIsStop, m_bIsWait, m_bIsTransmitting;
	
	void TransfersFailed();
	CString GetError(DWORD error);
	int GetLocalHostInfo(CString &strHostName, CString &strIPAddress);
	CChatServerDlg(CWnd* pParent = NULL);	// standard constructor
	//add
	
// Dialog Data
	//{{AFX_DATA(CChatServerDlg)
	enum { IDD = IDD_CHATSERVER_DIALOG };
	CProgressCtrl	m_ctrlProgress;
	CListBox	m_cMsgBox;
	CIPAddressCtrl	m_cServerIP;
	UINT	m_iServerPort;
	CString	m_sMsg;
	CString	m_strFileName;
	CString	m_strFileSize;
	int		m_nServerType;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChatServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CChatServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnListen();
	afx_msg void OnEnd();
	afx_msg void OnSend();
	afx_msg void OnRadioclient();
	afx_msg void OnRadioserver();
	afx_msg void OnStopTransfers();
	afx_msg void OnSelectFile();
	virtual void OnCancel();
	afx_msg LRESULT OnAcceptTransfers(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnChangeMsg();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHATSERVERDLG_H__E0596E36_E1D2_4032_AB51_3855F345D459__INCLUDED_)
