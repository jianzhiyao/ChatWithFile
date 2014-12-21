// ChatServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChatServer.h"
#include "ChatServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChatServerDlg dialog
#define WM_ACCEPT_TRANSFERS WM_USER + 100

CWinThread* pThreadListen;		//监听线程 --> _ListenThread
CWinThread* pThreadSend;        //连接线程 --> _ConnectThread
UINT _SendThread(LPVOID lparam);
UINT _ListenThread(LPVOID lparam);
CChatServerDlg::CChatServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChatServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChatServerDlg)
	m_iServerPort = 0;
	m_sMsg = _T("");
	m_strFileName = _T("");
	m_strFileSize = _T("");
	m_nServerType = -1;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_psockServer = NULL;
	m_psockClient = NULL;

	m_strPath = _T("");
	m_dwFileSize = 0;

	m_bIsStop = FALSE;
	m_bIsWait = FALSE;
	m_bIsTransmitting = FALSE;
}

void CChatServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChatServerDlg)
	DDX_Control(pDX, IDC_PROGRESS, m_ctrlProgress);
	DDX_Control(pDX, IDC_MSGBOX, m_cMsgBox);
	DDX_Control(pDX, IDC_SERVERIP, m_cServerIP);
	DDX_Text(pDX, IDC_SERVERPORT, m_iServerPort);
	DDV_MinMaxUInt(pDX, m_iServerPort, 1024, 60000);
	DDX_Text(pDX, IDC_MSG, m_sMsg);
	DDX_Text(pDX, IDC_FILE_NAME, m_strFileName);
	DDX_Text(pDX, IDC_FILE_SIZE, m_strFileSize);
	DDX_Radio(pDX, IDC_RADIOSERVER, m_nServerType);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CChatServerDlg, CDialog)
	//{{AFX_MSG_MAP(CChatServerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_LISTEN, OnListen)
	ON_BN_CLICKED(IDC_END, OnEnd)
	ON_BN_CLICKED(IDC_SEND, OnSend)
	ON_BN_CLICKED(IDC_RADIOCLIENT, OnRadioclient)
	ON_BN_CLICKED(IDC_RADIOSERVER, OnRadioserver)
	ON_BN_CLICKED(IDC_STOP_TRANSFERS, OnStopTransfers)
	ON_BN_CLICKED(IDC_SELECT_FILE, OnSelectFile)
	ON_MESSAGE(WM_ACCEPT_TRANSFERS, OnAcceptTransfers)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_MSG, OnChangeMsg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChatServerDlg message handlers

BOOL CChatServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	this->m_iServerPort=2333;
	this->OnRadioserver();
	UpdateData(FALSE);
	((CButton*)GetDlgItem(IDC_RADIOSERVER))->SetCheck(true);
	GetDlgItem(IDC_SERVERIP)->EnableWindow(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CChatServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CChatServerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CChatServerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CChatServerDlg::OnListen() 
{
	GetDlgItem(IDC_END)->EnableWindow(TRUE);
	GetDlgItem(IDC_LISTEN)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIOCLIENT)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIOSERVER)->EnableWindow(FALSE);
	UpdateData(TRUE);
	BYTE nFild[4];
	CString sIP;
	CString sP;
	if(this->isServer){
		//文字部分socket
		m_cServerIP.GetAddress(nFild[0],nFild[1],nFild[2],nFild[3]);
		sIP.Format("%d.%d.%d.%d",nFild[0],nFild[1],nFild[2],nFild[3]);
		sP.Format("%d",m_iServerPort);
		m_ListenSocket.Create(m_iServerPort,1,FD_ACCEPT,sIP);
		if(m_ListenSocket.Listen()){
			m_cMsgBox.AddString("Start to listen ip:"+sIP+",port:"+sP);
			m_cMsgBox.SetTopIndex(
				m_cMsgBox.GetCount()-1
			);
		}

		//文件部分socket
		//创建服务器套接字
		m_psockServer = new CFileListenSocket(this);
		if(!m_psockServer->Create(m_iServerPort+1))
		{
			delete m_psockServer;
			m_psockServer = NULL;
			
			MessageBox(GetError(GetLastError()), _T("错误"), MB_ICONHAND);
			
			return ;
		}
		//监听客户端连接
		if(!m_psockServer->Listen())
		{
			delete m_psockServer;
			m_psockServer = NULL;
			MessageBox(GetError(GetLastError()), _T("错误"), MB_ICONHAND);
			return ;
		}
		
	}
	else{
		//文字部分socket

		m_cMsgBox.AddString("Try to connet server");
		m_cMsgBox.SetTopIndex(
				m_cMsgBox.GetCount()-1
			);
		m_cServerIP.GetAddress(nFild[0],nFild[1],nFild[2],nFild[3]);
		sIP.Format("%d.%d.%d.%d",nFild[0],nFild[1],nFild[2],nFild[3]);
		m_ClientSocket.Create();
		if(m_ClientSocket.Connect(sIP,m_iServerPort))
		{
			m_cMsgBox.AddString("Start to connect ip:"+sIP+",port:"+sP);
			m_cMsgBox.SetTopIndex(
				m_cMsgBox.GetCount()-1
			);
		}
		

		//当程序作为客户端
		if(((CIPAddressCtrl*)GetDlgItem(IDC_SERVERIP))->IsBlank())
		{
			MessageBox("IP 地址不能为空", "错误", MB_ICONHAND);
			return ;
		}
		
		//创建客户端套接字
		m_psockClient = new CFileClientSocket(this);
		if(!m_psockClient->Create())
		{
			delete m_psockClient;
			m_psockClient = NULL;
			
			MessageBox(GetError(GetLastError()), _T("错误"), MB_ICONHAND);
			
			return ;
		}
		
		//与服务器建立连接
		CString strIPAddress;
		GetDlgItem(IDC_SERVERIP)->GetWindowText(strIPAddress);
		if(!m_psockClient->Connect(strIPAddress, m_iServerPort+1))
		{
			delete m_psockClient;
			m_psockClient = NULL;
			
			MessageBox(GetError(GetLastError()), _T("错误"), MB_ICONHAND);
			
			return ;
		}
		
		//初始化套接字
		m_psockClient->Init();
		
		GetDlgItem(IDC_SERVERIP)->EnableWindow(FALSE);
	}
	
	
	
}

void CChatServerDlg::TransfersFailed()
{
	GetDlgItem(IDC_END)->EnableWindow(TRUE);
	GetDlgItem(IDC_SELECT_FILE)->EnableWindow(TRUE);
	GetDlgItem(IDC_STOP_TRANSFERS)->EnableWindow(FALSE);
}

void CChatServerDlg::OnEnd() 
{
	if(this->isServer){
		// TODO: Add your control notification handler code here
		m_ListenSocket.Close();
		m_cMsgBox.AddString("Server is closed");
		delete m_psockServer;
		m_psockServer = NULL;
	}
	else{
		m_ClientSocket.Close();
		m_cMsgBox.AddString("Disconnect from server");
		//告诉对方发送等待被取消
		CMessage* pMsg = new CMessage(DISCONNECT);
		m_psockClient->SendMsg(pMsg);
		
		delete m_psockClient;
		m_psockClient = NULL;
	}
	GetDlgItem(IDC_END)->EnableWindow(!TRUE);
	GetDlgItem(IDC_LISTEN)->EnableWindow(!FALSE);
	GetDlgItem(IDC_RADIOCLIENT)->EnableWindow(!FALSE);
	GetDlgItem(IDC_RADIOSERVER)->EnableWindow(!FALSE);
}

void CChatServerDlg::OnSend() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(this->isServer)
	{
		m_ServerSocket.Send(m_sMsg,m_sMsg.GetLength());
		m_cMsgBox.AddString("send:"+m_sMsg);
		m_cMsgBox.SetTopIndex(m_cMsgBox.GetCount()-1);
		m_sMsg="";
	}
	else{
		m_ClientSocket.Send(m_sMsg,m_sMsg.GetLength());
		m_cMsgBox.AddString("Send to server:\n"+m_sMsg);
		m_cMsgBox.SetTopIndex(m_cMsgBox.GetCount()-1);
		m_sMsg="";
	}
	UpdateData(0);
}

void CChatServerDlg::OnRadioclient() 
{
	((CButton*)GetDlgItem(IDC_RADIOSERVER))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIOCLIENT))->SetCheck(true);
	this->isServer=false;
	CString strHostName,strIPAddress;
	if(GetLocalHostInfo(strHostName, strIPAddress))
		return ;
	GetDlgItem(IDC_SERVERIP)->SetWindowText(strIPAddress);
	GetDlgItem(IDC_SERVERIP)->EnableWindow(TRUE);
	GetDlgItem(IDC_LISTEN)->SetWindowText(_T("连  接(&B)"));
	GetDlgItem(IDC_END)->SetWindowText(_T("断  开(&D)"));
}

void CChatServerDlg::OnRadioserver() 
{
	((CButton*)GetDlgItem(IDC_RADIOCLIENT))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIOSERVER))->SetCheck(true);
	this->isServer=true;
	CString strHostName,strIPAddress;
	if(GetLocalHostInfo(strHostName, strIPAddress))
		return ;
	GetDlgItem(IDC_SERVERIP)->SetWindowText(strIPAddress);
	GetDlgItem(IDC_SERVERIP)->EnableWindow(FALSE);
	GetDlgItem(IDC_LISTEN)->SetWindowText(_T("启  动(&B)"));
	GetDlgItem(IDC_END)->SetWindowText(_T("关  闭(&D)"));
	GetDlgItem(IDC_TRANSFERS_TIP)->SetWindowText(_T("已发送:"));
	
}

void CChatServerDlg::ProcessAccept()
{
	CFileClientSocket* pSocket = new CFileClientSocket(this);
	//将请求接收下来，得到一个新的套接字pSocket
	if(m_psockServer->Accept(*pSocket))
	{
		//初始化套接字pSocket
		pSocket->Init();
		
		CMessage* pMsg;
		//如果m_psockClient套接字为空，则表示还没有和任何客户端建立连接
		if(m_psockClient == NULL)
		{
			//向客户端发送一个消息，表示连接被接受
			pMsg = new CMessage(CONNECT_BE_ACCEPT);
			pSocket->SendMsg(pMsg);
			m_psockClient = pSocket;
			
			GetDlgItem(IDC_SELECT_FILE)->EnableWindow(TRUE);
		}
		else
		{
			//否则向客户端发一个信息，服务器已经存在连接
			pMsg = new CMessage(CONNECT_BE_REFUSE);
			pSocket->SendMsg(pMsg);
		}
	}
}

void CChatServerDlg::ProcessReceive(CFileClientSocket *pSocket)
{
	//获取信息
	CMessage* pMsg = new CMessage();
	pSocket->ReceiveMsg(pMsg);
	
	//当消息类型为连接被接受时执行该if语句里面的内容
	if(pMsg->m_nType == CONNECT_BE_ACCEPT)
	{
		GetDlgItem(IDC_SELECT_FILE)->EnableWindow(TRUE);
		return;
	}

	//当消息类型为连接被拒绝时执行该if语句里面的内容
	if(pMsg->m_nType == CONNECT_BE_REFUSE)
	{
		MessageBox(_T("服务器已经和另外的客户端建立连接，请等一下再连接。"), _T("错误"), MB_ICONHAND);
		
		delete m_psockClient;
		m_psockClient = NULL;
		
		GetDlgItem(IDC_RADIOSERVER)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIOCLIENT)->EnableWindow(TRUE);
		GetDlgItem(IDC_SERVERIP)->EnableWindow(TRUE);
		GetDlgItem(IDC_SERVERPORT)->EnableWindow(TRUE);
		GetDlgItem(IDC_LISTEN)->EnableWindow(TRUE);
		GetDlgItem(IDC_END)->EnableWindow(FALSE);
		return ;
	}
	
	//当消息类型为连接被断开时执行该if语句里面的内容
	if(pMsg->m_nType == DISCONNECT)
	{
		MessageBox(_T("对方已经关闭"), _T("警告"), MB_ICONHAND);
		
		if(m_psockClient != NULL)
		{
			delete m_psockClient;
			m_psockClient = NULL;
		}
		
		if(m_nServerType == CLIENT)
		{	
			GetDlgItem(IDC_RADIOSERVER)->EnableWindow(TRUE);
			GetDlgItem(IDC_RADIOCLIENT)->EnableWindow(TRUE);
			GetDlgItem(IDC_SERVERPORT)->EnableWindow(TRUE);
			GetDlgItem(IDC_LISTEN)->EnableWindow(TRUE);
			GetDlgItem(IDC_END)->EnableWindow(FALSE);
			GetDlgItem(IDC_SELECT_FILE)->EnableWindow(FALSE);
			GetDlgItem(IDC_SERVERIP)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_SELECT_FILE)->EnableWindow(FALSE);
		}
		return ;
	}

	//当收到传输文件请求时执行该if语句里面的内容
	if(pMsg->m_nType == REQUEST)
	{
		m_bIsWait = TRUE;
		
		m_strFileName = pMsg->m_strFileName;
		m_dwFileSize = pMsg->m_dwFileSize;
		
		CFileDialog dlg(FALSE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "所有文件 (*.*)|*.*||", this);
		dlg.m_ofn.lpstrTitle = _T("另存为");
		strcpy(dlg.m_ofn.lpstrFile, m_strFileName.GetBuffer(m_strFileName.GetLength()));
		if(dlg.DoModal() == IDOK)
		{
			if(m_bIsWait == FALSE)
			{
				MessageBox(_T("对方已经取消文件发送"), _T("警告"), MB_ICONEXCLAMATION);
				return ;
			}

			m_bIsClient = FALSE;
			m_strPath = dlg.GetPathName();
			
			GetDlgItem(IDC_END)->EnableWindow(FALSE);
			GetDlgItem(IDC_SELECT_FILE)->EnableWindow(FALSE);
			GetDlgItem(IDC_STOP_TRANSFERS)->EnableWindow(TRUE);
			
			m_strFileSize.Format("%ld 字节", m_dwFileSize);
			GetDlgItem(IDC_FILE_NAME)->SetWindowText(dlg.GetFileName());
			GetDlgItem(IDC_FILE_SIZE)->SetWindowText(m_strFileSize);
			GetDlgItem(IDC_TRANSFERS_TIP)->SetWindowText(_T("已收到:"));
			
			//启动接收文件的线程
			pThreadListen = ::AfxBeginThread(_ListenThread, this);
			return ;
		}
		
		if(m_bIsWait == TRUE)
		{		
			//告诉对方文件发送请求被拒绝
			CMessage* pMsg = new CMessage(REFUSE);
			m_psockClient->SendMsg(pMsg);
		}
		
		m_bIsWait = FALSE;
		return ;
	}

	//当对方同意且准备好接收文件时执行该if语句里面的内容
	if(pMsg->m_nType == ACCEPT)
	{	
		KillTimer(1);
		m_bIsWait = FALSE;
		//启动文件发送线程
		pThreadSend = ::AfxBeginThread(_SendThread, this);
		return ;
	}
	
	//当发送文件请求被拒绝时执行该if语句里面的内容
	if(pMsg->m_nType == REFUSE)
	{	
		m_bIsWait = FALSE;
		
		MessageBox(_T("请求被拒绝"), _T("警告"), MB_ICONEXCLAMATION);
		
		GetDlgItem(IDC_END)->EnableWindow(TRUE);
		GetDlgItem(IDC_SELECT_FILE)->EnableWindow(TRUE);
		GetDlgItem(IDC_STOP_TRANSFERS)->EnableWindow(FALSE);
		return ;
	}

	//当对方取消文件传输时执行该if语句里面的内容
	if(pMsg->m_nType == CANCEL)
	{
		m_bIsWait = FALSE;
		return ;
	}
	
	return ;
}

void CChatServerDlg::OnStopTransfers() 
{
	if(m_bIsWait)
	{
		if(MessageBox(_T("真的要停止等待吗？"), _T("警告"), MB_ICONEXCLAMATION|MB_YESNO) == IDYES)
		{
			m_bIsWait = FALSE;
			if(this->isServer)
			{
				//停止ID为2的计时器
				if(KillTimer(2))
				{
					//结束监听
					CSocket sockClient;
					sockClient.Create();
					sockClient.Connect(_T("127.0.0.1"), m_iServerPort+1 + PORT);
					sockClient.Close();
				}
			}
			else
			{
				//停止ID为1的计时器
				if(KillTimer(1))
				{					
					//告诉对方发送等待被取消
					CMessage* pMsg = new CMessage(CANCEL);
					m_psockClient->SendMsg(pMsg);
					
					GetDlgItem(IDC_END)->EnableWindow(TRUE);
					GetDlgItem(IDC_SELECT_FILE)->EnableWindow(TRUE);
					GetDlgItem(IDC_STOP_TRANSFERS)->EnableWindow(FALSE);
				}
			}
		}
		return ;
	}
	
	if(MessageBox(_T("真的要停止文件传输吗？"), _T("警告"), MB_ICONEXCLAMATION|MB_YESNO) == IDYES)
	{
		m_bIsStop = TRUE;
		return ;
	}
}

void CChatServerDlg::OnSelectFile() 
{
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "所有文件 (*.*)|*.*||", this);
	dlg.m_ofn.lpstrTitle = _T("打开");
	if(dlg.DoModal() == IDOK)
	{
		m_bIsWait = TRUE;
		this->isServer = !TRUE;
		m_strPath = dlg.GetPathName();
		m_strFileName = dlg.GetFileName();
		
		//打开文件
		CFile file(m_strPath, CFile::modeRead);
		//获取文件大小
		m_dwFileSize = file.GetLength();
		m_strFileSize.Format("%ld 字节", m_dwFileSize);
		//关闭文件
		file.Close();
		UpdateData(FALSE);
		
		//发出文件发送请求
		CMessage* pMsg = new CMessage(REQUEST, m_strFileName, m_dwFileSize);
		m_psockClient->SendMsg(pMsg);
		
		GetDlgItem(IDC_END)->EnableWindow(FALSE);
		GetDlgItem(IDC_SELECT_FILE)->EnableWindow(FALSE);
		GetDlgItem(IDC_STOP_TRANSFERS)->EnableWindow(TRUE);
		GetDlgItem(IDC_TRANSFERS_TIP)->SetWindowText(_T("已发送:"));
		
		//设置等待超时定时器
		m_nTimer = SetTimer(1, 50000, NULL);
	}
}
int CChatServerDlg::GetLocalHostInfo(CString &strHostName, CString &strIPAddress)
{
	char szHostName[256];
	
	if(gethostname(szHostName, sizeof(szHostName)))
	{
		strHostName = _T("");
		MessageBox(GetError(GetLastError()), _T("错误"), MB_ICONHAND|MB_OK);
		return -1;
	}
	
	PHOSTENT hostinfo;
	if((hostinfo = gethostbyname(szHostName)) == NULL)
		return GetLastError();
	
	LPCSTR ip = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list);
	
	strIPAddress = ip;
	strHostName = szHostName;
	
	return 0;
}
CString CChatServerDlg::GetError(DWORD error)
{
	CString strError;
	switch(error)
	{
	case WSANOTINITIALISED:
		strError="初始化错误";
		break;
	case WSAENOTCONN:
		strError="对方没有启动";
		break;
	case WSAEWOULDBLOCK :
		strError="对方已经关闭";
		break;
	case WSAECONNREFUSED:
		strError="连接的尝试被拒绝";
		break;
	case WSAENOTSOCK:
		strError="在一个非套接字上尝试了一个操作";
		break;
	case WSAEADDRINUSE:
		strError="特定的地址已在使用中";
		break;
	case WSAECONNRESET:
		strError="与主机的连接被关闭";
		break;
	default:
		strError="一般性错误";	
	}
	return strError;
}

void CChatServerDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	if(m_bIsWait)
	{
		MessageBox(_T("等待中，请先停止传送后再退出"), _T("警告"), MB_ICONEXCLAMATION);
		return ;
	}
	
	if(m_bIsTransmitting)
	{
		MessageBox(_T("文件传输中，请先停止传送后再退出"), _T("警告"), MB_ICONEXCLAMATION);
		return ;
	}
	
	OnEnd();
	CDialog::OnCancel();
}

void CChatServerDlg::OnTimer(UINT nIDEvent) 
{
		switch(nIDEvent)
	{
	//ID为1的计时器
	case 1:
		{
			//结束ID为1的计时器
			KillTimer(1);
			m_bIsWait = FALSE;
			
			//告诉对方发送等待被取消
			CMessage* pMsg = new CMessage(CANCEL);
			m_psockClient->SendMsg(pMsg);
			
			MessageBox(_T("等待超时"), _T(" 警告"), MB_ICONEXCLAMATION);
			
			GetDlgItem(IDC_END)->EnableWindow(TRUE);
			GetDlgItem(IDC_SELECT_FILE)->EnableWindow(TRUE);
			GetDlgItem(IDC_STOP_TRANSFERS)->EnableWindow(FALSE);
			break;
		}
	//ID为2的计时器
	case 2:
		{
			//结束ID为2的计时器
			KillTimer(2);
			//结束监听
			CSocket sockClient;
			sockClient.Create();
			sockClient.Connect(_T("127.0.0.1"), (m_iServerPort+1) + PORT);
			sockClient.Close();
			break;
		}
	}
	
	CDialog::OnTimer(nIDEvent);
}

/* ---------------------------------- < 线程 > ---------------------------------- */

//接收文件线程
UINT _ListenThread(LPVOID lparam)
{
	CChatServerDlg* pDlg = (CChatServerDlg*)lparam;
	
	//创建套接字
	CSocket sockSrvr;
	if(!sockSrvr.Create((pDlg->m_iServerPort+1) + PORT))
	{
		pDlg->TransfersFailed();
		::MessageBox((HWND)lparam, pDlg->GetError(GetLastError()), _T("错误"), MB_ICONHAND|MB_OK);
		return -1;
	}
	
	//开始监听
	if(!sockSrvr.Listen())
	{
		pDlg->TransfersFailed();
		::MessageBox((HWND)lparam, pDlg->GetError(GetLastError()), _T("错误"), MB_ICONHAND|MB_OK);
		return -1;
	}
	
	pDlg->SendMessage(WM_ACCEPT_TRANSFERS);
	
	//接受连接
	CSocket recSo;
	if(!sockSrvr.Accept(recSo))
	{
		::MessageBox((HWND)lparam, pDlg->GetError(GetLastError()), _T("错误"), MB_ICONHAND|MB_OK);
		return -1;
	}
	
	sockSrvr.Close();

	pDlg->ReceiveFile(recSo);

	return 0;
}

//发送文件线程
UINT _SendThread(LPVOID lparam)
{
	CChatServerDlg* pDlg = (CChatServerDlg*) lparam;
	
	CSocket sockClient;
	if(!sockClient.Create())
	{
		pDlg->TransfersFailed();
		::MessageBox((HWND)lparam, pDlg->GetError(GetLastError()), _T("错误"), MB_ICONHAND|MB_OK);
		return -1;
	}
	
	CString strIPAddress;
	UINT nPort;
	pDlg->m_psockClient->GetPeerName(strIPAddress, nPort);
	if(!sockClient.Connect(strIPAddress, (pDlg->m_iServerPort+1) + PORT))
	{
		pDlg->TransfersFailed();
		::MessageBox((HWND)lparam, pDlg->GetError(GetLastError()), _T("错误"), MB_ICONHAND|MB_OK);
		return -1;
	}
	
	pDlg->SendFile(sockClient);
	
	return 0;
}

void CChatServerDlg::ReceiveFile(CSocket &recSo)
{
	//停止等待超时计时器
	KillTimer(2);
	m_bIsWait = FALSE;
	m_bIsTransmitting = TRUE;
	
	m_ctrlProgress.SetRange32(0, m_dwFileSize);
	GetDlgItem(IDC_END)->EnableWindow(FALSE);
	GetDlgItem(IDC_SELECT_FILE)->EnableWindow(FALSE);
	GetDlgItem(IDC_STOP_TRANSFERS)->EnableWindow(TRUE);
	
	int nSize = 0;
	DWORD dwCount = 0;
	char buf[BLOCKSIZE] = {0};	
	//创建一个文件
	CFile file(m_strPath, CFile::modeCreate|CFile::modeWrite);
	//开始接收文件
	for(;;)
	{
		//每次接收BLOCKSIZE大小的文件内容
		nSize = recSo.Receive(buf, BLOCKSIZE);
		if(nSize == 0)
			break;
		
		//将接收到的文件写到新建的文件中去
		file.Write(buf, nSize);
		dwCount += nSize;
		m_ctrlProgress.SetPos(dwCount);
		CString strTransfersSize;
		strTransfersSize.Format("%ld 字节", dwCount);
		GetDlgItem(IDC_RECEIVE_SIZE)->SetWindowText(strTransfersSize);
		
		//用户是否要停止接收
		if(m_bIsStop)
		{
			m_bIsStop = FALSE;
			break;
		}
	}
	//关闭文件
	file.Close();
	//关闭套接字
	recSo.Close();
	
	if(m_dwFileSize == dwCount)
		AfxMessageBox(_T("文件接收成功"));
	else
		AfxMessageBox(_T("文件接收失败"));
	
	m_ctrlProgress.SetPos(0);
	GetDlgItem(IDC_END)->EnableWindow(TRUE);
	GetDlgItem(IDC_SELECT_FILE)->EnableWindow(TRUE);
	GetDlgItem(IDC_STOP_TRANSFERS)->EnableWindow(FALSE);
	
	m_bIsTransmitting = FALSE;
}

void CChatServerDlg::SendFile(CSocket &senSo)
{
	m_bIsTransmitting = TRUE;

	//打开要发送的文件
	CFile file;
	if(!file.Open(m_strPath, CFile::modeRead | CFile::typeBinary))
	{
		AfxMessageBox(_T("文件打开失败"));
		
		GetDlgItem(IDC_END)->EnableWindow(TRUE);
		GetDlgItem(IDC_SELECT_FILE)->EnableWindow(TRUE);
		GetDlgItem(IDC_STOP_TRANSFERS)->EnableWindow(FALSE);
		
		senSo.Close();
		
		return ;
	}
	
	m_ctrlProgress.SetRange32(0, m_dwFileSize);
	
	int nSize = 0, nLen = 0;
	DWORD dwCount = 0;
	char buf[BLOCKSIZE] = {0};
	file.Seek(0, CFile::begin);
	//开始传送文件
	for(;;)
	{
		//一次读取BLOCKSIZE大小的文件内容
		nLen = file.Read(buf, BLOCKSIZE);
		if(nLen == 0)
			break;
		
		//发送文件内容
		nSize = senSo.Send(buf, nLen);
		
		dwCount += nSize;
		m_ctrlProgress.SetPos(dwCount);
		CString strTransfersSize;
		strTransfersSize.Format("%ld 字节", dwCount);
		GetDlgItem(IDC_RECEIVE_SIZE)->SetWindowText(strTransfersSize);
		
		if(m_bIsStop)
		{
			m_bIsStop = FALSE;
			break;
		}
		
		if(nSize == SOCKET_ERROR)
			break;
	}
	//关闭文件
	file.Close();
	//关闭套接字
	senSo.Close();
	
	if(m_dwFileSize == dwCount)
		AfxMessageBox(_T("文件发送成功"));
	else
		AfxMessageBox(_T("文件发送失败"));
	
	m_ctrlProgress.SetPos(0);
	GetDlgItem(IDC_END)->EnableWindow(TRUE);
	GetDlgItem(IDC_SELECT_FILE)->EnableWindow(TRUE);
	GetDlgItem(IDC_STOP_TRANSFERS)->EnableWindow(FALSE);

	m_bIsTransmitting = FALSE;	
}
LRESULT CChatServerDlg::OnAcceptTransfers(WPARAM wParam, LPARAM lParam)
{	
	//告诉对方文件请求被接受且准备好接收	
	CMessage* pMsg = new CMessage(ACCEPT);
	m_psockClient->SendMsg(pMsg);
	//设置一个ID为2的超时几时器
	m_nTimer = SetTimer(2, 5000, NULL);
	return 0;
}

void CChatServerDlg::OnChangeMsg() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	//this->OnSend();
	// TODO: Add your control notification handler code here
	
}
