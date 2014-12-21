; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CChatServerDlg
LastTemplate=CSocket
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ChatServer.h"

ClassCount=7
Class1=CChatServerApp
Class2=CChatServerDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class5=CListenSocket
Class4=CServerSocket
Class6=CFileListenSocket
Class7=CFileClientSocket
Resource3=IDD_CHATSERVER_DIALOG

[CLS:CChatServerApp]
Type=0
HeaderFile=ChatServer.h
ImplementationFile=ChatServer.cpp
Filter=N

[CLS:CChatServerDlg]
Type=0
HeaderFile=ChatServerDlg.h
ImplementationFile=ChatServerDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_MSG

[CLS:CAboutDlg]
Type=0
HeaderFile=ChatServerDlg.h
ImplementationFile=ChatServerDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_CHATSERVER_DIALOG]
Type=1
Class=CChatServerDlg
ControlCount=19
Control1=IDCANCEL,button,1342242816
Control2=IDC_LISTEN,button,1342242816
Control3=IDC_SERVERIP,SysIPAddress32,1342242816
Control4=IDC_SERVERPORT,edit,1350631552
Control5=IDC_END,button,1476460544
Control6=IDC_MSGBOX,listbox,1352728835
Control7=IDC_MSG,edit,1350762624
Control8=IDC_SEND,button,1342373889
Control9=IDC_RADIOCLIENT,button,1342308361
Control10=IDC_RADIOSERVER,button,1342308361
Control11=IDC_FILE_NAME,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_FILE_SIZE,static,1342308352
Control14=IDC_PROGRESS,msctls_progress32,1350565888
Control15=IDC_SELECT_FILE,button,1476460544
Control16=IDC_STOP_TRANSFERS,button,1476460544
Control17=IDC_STATIC,static,1342308352
Control18=IDC_RECEIVE_SIZE,static,1342308352
Control19=IDC_TRANSFERS_TIP,static,1342308352

[CLS:CServerSocket]
Type=0
HeaderFile=ServerSocket.h
ImplementationFile=ServerSocket.cpp
BaseClass=CAsyncSocket
Filter=N
VirtualFilter=q

[CLS:CListenSocket]
Type=0
HeaderFile=ListenSocket.h
ImplementationFile=ListenSocket.cpp
BaseClass=CAsyncSocket
Filter=N
VirtualFilter=q

[CLS:CFileListenSocket]
Type=0
HeaderFile=FileListenSocket.h
ImplementationFile=FileListenSocket.cpp
BaseClass=CSocket
Filter=N
VirtualFilter=uq

[CLS:CFileClientSocket]
Type=0
HeaderFile=FileClientSocket.h
ImplementationFile=FileClientSocket.cpp
BaseClass=CSocket
Filter=N
VirtualFilter=uq

