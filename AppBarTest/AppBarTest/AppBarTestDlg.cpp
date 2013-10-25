
// AppBarTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AppBarTest.h"
#include "AppBarTestDlg.h"
#include "afxdialogex.h"
#include "RestartManager.h"
#include <psapi.h>
#include <vector>
#include <string>
#include <algorithm>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CAppBarTestDlg dialog




CAppBarTestDlg::CAppBarTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAppBarTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAppBarTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAppBarTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CAppBarTestDlg message handlers
UINT GetEdge(RECT rc)
{
	UINT uEdge = -1;

	if (rc.top == rc.left && rc.bottom > rc.right)
	{
		uEdge = ABE_LEFT;
	}
	else if (rc.top == rc.left && rc.bottom < rc.right)
	{
		uEdge = ABE_TOP;
	}
	else if (rc.top > rc.left )
	{
		uEdge = ABE_BOTTOM;
	}
	else
	{
		uEdge = ABE_RIGHT;
	}

    return uEdge;
}
RM_UNIQUE_PROCESS GetExplorerApplication()
{
	RM_UNIQUE_PROCESS  result={0};
	DWORD bytesReturned=0;
	DWORD processIdSize=4096;
	std::vector<DWORD> processIds;
	processIds.resize(1024);
	EnumProcesses(processIds.data(),processIdSize,&bytesReturned);
	while(bytesReturned==processIdSize)
	{
		processIdSize+=processIdSize;
		processIds.resize(processIdSize/4);
		EnumProcesses(processIds.data(),processIdSize,&bytesReturned);
	}

	std::for_each(processIds.begin(), processIds.end(), [&result] (DWORD processId) {
		 HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,
                                   FALSE, processId);
		 if (hProcess) {
			std::wstring imageName;
			imageName.resize(4096);
			if(GetProcessImageFileName (hProcess,(LPWSTR)imageName.data(),4096)>0)
			{
				if(wcscmp(L"explorer.exe",PathFindFileName(imageName.data()))==0)
				{
					//this is assmuing the user is not running elevated and won't see explorer processes in other sessions
					FILETIME ftCreate, ftExit, ftKernel, ftUser;
					if (GetProcessTimes(hProcess, &ftCreate, &ftExit,&ftKernel, &ftUser))
					{
						if(result.dwProcessId==0)
						{
							result.dwProcessId=processId;
							result.ProcessStartTime=ftCreate;
						}
						else if(CompareFileTime(&result.ProcessStartTime,&ftCreate)>0)
						{
							result.dwProcessId=processId;
							result.ProcessStartTime=ftCreate;
						}
					}
				}
			}
			CloseHandle(hProcess);
		 }
	});
	return result;
}

void DockTaskBar(HWND hwnd, int edge, int x, int y,OSVERSIONINFO osvi)
{
	int screenX=GetSystemMetrics(SM_CXSCREEN);
	int screenY=GetSystemMetrics(SM_CYSCREEN);
	APPBARDATA abd;
	ZeroMemory(&abd, sizeof(APPBARDATA));
	abd.cbSize = sizeof(APPBARDATA);
	abd.hWnd=hwnd;
	abd.uEdge=edge;
	switch(edge)
	{
	case ABE_LEFT:
	case ABE_RIGHT:
		abd.rc.top=0;
		abd.rc.bottom=screenY;
		switch(edge)
		{
		case ABE_LEFT:
			abd.rc.right=x;break;
		case ABE_RIGHT:
			abd.rc.right=screenX;abd.rc.left=screenX-x;break;
		}
		break;
	case ABE_TOP:
	case ABE_BOTTOM:
		abd.rc.left=0;
		abd.rc.right=screenX;
		switch(edge)
		{
		case ABE_TOP:
			abd.rc.bottom=y;break;
		case ABE_BOTTOM:
			abd.rc.bottom=screenY;abd.rc.top=screenY-y;break;
		}
		break;
	}
	//ask to dock
	BOOL result=SHAppBarMessage(ABM_QUERYPOS,&abd);
	//approved edge, adjust location
	switch(edge)
	{
	case ABE_LEFT:
		abd.rc.right=abd.rc.left+x;break;
	case ABE_RIGHT:
		abd.rc.left=abd.rc.right-x;break;
	case ABE_TOP:
		abd.rc.bottom=abd.rc.top+y;break;
	case ABE_BOTTOM:
		abd.rc.top=abd.rc.bottom-y;break;
	}
	//ask to resize
	result=SHAppBarMessage(ABM_SETPOS,&abd);
	//resize the taskbar to fit the appbar
	::MoveWindow(abd.hWnd,abd.rc.left,abd.rc.top,abd.rc.right-abd.rc.left,abd.rc.bottom-abd.rc.top,TRUE);
	result=SHAppBarMessage(ABM_WINDOWPOSCHANGED,&abd);

	if(osvi.dwMajorVersion<6)
	{
		////we fake a PostQuitMessage to explorer
		////note this use of lparam value is not documented and could change anytime
		//HWND hWndProgMan = FindWindow(_T("Progman"), NULL);
  //      PostMessage(hWndProgMan, WM_QUIT, 0, TRUE); 

  //      HWND hWndTray = FindWindow(_T("Shell_TrayWnd"), NULL);
  //      PostMessage(hWndTray, WM_QUIT, 0, 0); 
	}
	else
	{
		DWORD dwSession=0;
		WCHAR szSessionKey[CCH_RM_SESSION_KEY+1] = { 0 };
		DWORD dwError = RmStartSession(&dwSession, 0, szSessionKey);
		if (dwError == ERROR_SUCCESS) {
			RM_UNIQUE_PROCESS rgApplications[1]={GetExplorerApplication()};
			dwError=RmRegisterResources(
				dwSession,0,NULL,1,rgApplications,0,NULL);
			DWORD dwReason;
			UINT nProcInfoNeeded;
			UINT nProcInfo = 10;
			RM_PROCESS_INFO rgpi[10];
			dwError = RmGetList(dwSession, &nProcInfoNeeded,
						   &nProcInfo, rgpi, &dwReason);
			if(dwReason==RmRebootReasonNone)
			{
				RmShutdown(dwSession,RmForceShutdown,NULL);//important, if we change the registry before shutting down explorer will override our change
				//using undocumented setting structure, could break any time
				//edge setting is stored at HKCU\Software\Microsoft\Windows\CurrentVersion\Explorer\StuckRects2!Settings
				HKEY hKey={0};
				DWORD result=0;
				result=::RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\StuckRects2"),
						0, KEY_READ|KEY_WRITE, &hKey) ;
				if (result== ERROR_SUCCESS)
				{
					std::vector<BYTE> data;
					data.resize(256);
					TCHAR settingValue[]= _T("Settings");
					DWORD dwKeyDataType=0;
					DWORD dwDataBufSize=data.size();
					result=::RegQueryValueEx(hKey,settingValue, NULL, &dwKeyDataType,
						(LPBYTE) data.data(), &dwDataBufSize);
					while(ERROR_MORE_DATA==result)
					{
						data.resize(256+data.size());
						dwDataBufSize=data.size();
						result=::RegQueryValueEx(hKey,settingValue, NULL, &dwKeyDataType, 
							(LPBYTE) data.data(), &dwDataBufSize);
					}
					data.resize(dwDataBufSize);
					if(result==ERROR_SUCCESS)
					{
						switch ( dwKeyDataType )
						{
							case REG_BINARY:
								if(data.size()>=40)
								{
									BYTE taskbarPosition=data[12];
									taskbarPosition=edge;
									data[12]=taskbarPosition;
									RECT* taskbarRect=(RECT*)&data[24];
									CopyRect (taskbarRect,&abd.rc);
									result=::RegSetValueEx(hKey,settingValue,0,REG_BINARY,(LPBYTE) data.data(), dwDataBufSize);
								}
								break;
						}
					}
					::RegCloseKey( hKey );
				}
				RmRestart (dwSession,0,NULL);
			}
		}
		RmEndSession(dwSession);		
	}
}
BOOL CAppBarTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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

	OSVERSIONINFO osvi={0};
    BOOL bIsWindowsVistaPorLater;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osvi);
	bIsWindowsVistaPorLater=osvi.dwMajorVersion>=6;
	// TODO: Add extra initialization here
	HWND hWnd = ::FindWindow(_T("Shell_TrayWnd"), NULL);
	APPBARDATA abd;
	ZeroMemory(&abd, sizeof(APPBARDATA));
	abd.cbSize = sizeof(APPBARDATA);
	BOOL result=SHAppBarMessage(ABM_GETTASKBARPOS, &abd);
	UINT uEdge = GetEdge(abd.rc);

    switch(uEdge)
    {
		case ABE_LEFT:
			DockTaskBar(hWnd,ABE_BOTTOM,50,50,osvi);
		break;
		case ABE_RIGHT:
		break;
		case ABE_TOP:
		break;
		case ABE_BOTTOM:
			DockTaskBar(hWnd,ABE_LEFT,50,50,osvi);
		break;
		default:
		TRACE("Edge not found");
					break;
    }
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAppBarTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAppBarTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAppBarTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

