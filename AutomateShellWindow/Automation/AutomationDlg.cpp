// AutomationDlg.cpp : implementation file
// by Sheng Jiang
// Oct. 18, 2005


#include "stdafx.h"
#include <exdisp.h> //For IWebBrowser2* and others
#include <exdispid.h>
#include "Automation.h"
#include "AutomationDlg.h"
#include <Mshtml.h>
#include <Mshtmdid.h>
#include <shobjidl.h>
#include <Afxctl.h>
#include ".\automationdlg.h"
#include "shlobj.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
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

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CAutomationDlg dialog



CAutomationDlg::CAutomationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAutomationDlg::IDD, pParent)
	, m_strFileToFind(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_dwCookie = 0;
	EnableAutomation();
	m_bOwnIE=FALSE;
	m_pidlToNavigate=NULL;
}

void CAutomationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FILE_TO_FIND, m_strFileToFind);
}

BEGIN_MESSAGE_MAP(CAutomationDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_NEWIE, OnBnClickedButtonNewie)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_CURRENT_IE, OnBnClickedButtonCurrentIe)
	ON_BN_CLICKED(IDC_BUTTON_NEW_EXPLORER, OnBnClickedButtonNewExplorer)
	ON_BN_CLICKED(IDC_BUTTON_CURRENT_EXPLORER, OnBnClickedButtonCurrentExplorer)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CAutomationDlg, CDialog)
	DISP_FUNCTION_ID(CAutomationDlg, "OnQuit",DISPID_ONQUIT,OnQuit,VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CAutomationDlg, "DocumentComplete",DISPID_DOCUMENTCOMPLETE,DocumentComplete,
					 VT_EMPTY, VTS_DISPATCH VTS_PVARIANT)
	DISP_FUNCTION_ID(CAutomationDlg, "WindowRegistered",0x000000c8,WindowRegistered,VT_EMPTY,VTS_I4)
	//DISP_FUNCTION_ID(CAutomationDlg, "WindowRevoked",0x000000c9,WindowRevoked,VT_EMPTY,VTS_I4)
END_DISPATCH_MAP()

// CAutomationDlg message handlers

BOOL CAutomationDlg::OnInitDialog()
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
	SHAutoComplete(::GetDlgItem(m_hWnd,IDC_EDIT_FILE_TO_FIND),SHACF_FILESYSTEM);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAutomationDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CAutomationDlg::OnPaint() 
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
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAutomationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CAutomationDlg::OnDestroy()
{
	//close the IE window created by this program before exit
	if(m_pWebBrowser2)
	{
		if(m_bOwnIE)
		{
			m_pWebBrowser2->Quit();
			m_bOwnIE=FALSE;
		}
		UnadvisesinkIE();
		m_pWebBrowser2=(LPUNKNOWN)NULL;
	}
	CDialog::OnDestroy();
}
void CAutomationDlg::OnQuit()
{
	//detach from the closing shell window 
	if(m_pWebBrowser2)
	{
		UnadvisesinkIE();
		m_pWebBrowser2=(LPUNKNOWN)NULL;
	}
}
void	CAutomationDlg::AdviseSinkIE()
{
	if(m_pWebBrowser2)
	{
		LPUNKNOWN pUnkSink = GetIDispatch(FALSE);
		AfxConnectionAdvise((LPUNKNOWN)m_pWebBrowser2, 
		DIID_DWebBrowserEvents2,pUnkSink,FALSE,&m_dwCookie); 
	}
}
void	CAutomationDlg::UnadvisesinkIE()
{
	if(m_dwCookie != 0&&m_pWebBrowser2!=NULL)
	{
		LPUNKNOWN pUnkSink = GetIDispatch(FALSE);
		AfxConnectionUnadvise((LPUNKNOWN)m_pWebBrowser2, DIID_DWebBrowserEvents2, pUnkSink, FALSE, m_dwCookie);
		m_dwCookie = 0;
	}
}

void CAutomationDlg::OnBnClickedButtonNewie()
{
	CreateNewShellWindow(TRUE);	
}

void CAutomationDlg::OnBnClickedButtonCurrentIe()
{
	AttachShellWindow(TRUE);
}

void CAutomationDlg::OnBnClickedButtonNewExplorer()
{
	CreateNewShellWindow(FALSE);	
}
void CAutomationDlg::OnBnClickedButtonCurrentExplorer()
{
	AttachShellWindow(FALSE);
}
#ifndef WM_GETISHELLBROWSER
   #define WM_GETISHELLBROWSER (WM_USER+7)
#endif
/*
//oops, this does not work. the new process just notifies the previous instance
//and quits if there is another explorer.exe process running.
//For more information of getting the main window of a process, visit
//http://msdn.microsoft.com/msdnmag/issues/02/07/CQA/
BOOL CALLBACK CAutomationDlg::EnumWindowsProc(HWND hwnd,LPARAM lParam)
{
	CAutomationDlg* pdlg=(CAutomationDlg*)lParam;
	DWORD pidwin;
	GetWindowThreadProcessId(hwnd, &pidwin);
	if (pidwin==pdlg->m_hExplorerProcess)
	{
		IShellBrowser* psb=(IShellBrowser*)::SendMessage(hwnd,WM_USER+7,0,0);
		CComQIPtr<IWebBrowser2> pwb(psb);
		return FALSE;
	}
	return TRUE;
}*/
void	CAutomationDlg::CreateNewShellWindow(BOOL bIE)
{
	if(!UpdateData())return;
	//Detach the previous instance, if any
	if(m_pWebBrowser2)
	{
		if(m_bOwnIE)
		{
			m_pWebBrowser2->Quit();
			m_bOwnIE=FALSE;
		}
		UnadvisesinkIE();
		m_pWebBrowser2=(LPUNKNOWN)NULL;
	}
	if(bIE)
	{
		// create a new IE instance and show it 
		m_pWebBrowser2.CoCreateInstance(CLSID_InternetExplorer);
		m_bOwnIE=TRUE;
		HRESULT hr;
		hr = m_pWebBrowser2->put_StatusBar(VARIANT_TRUE);
		hr = m_pWebBrowser2->put_ToolBar(VARIANT_TRUE);
		hr = m_pWebBrowser2->put_MenuBar(VARIANT_TRUE);
		hr = m_pWebBrowser2->put_Visible(VARIANT_TRUE);
		//sink for the Quit and DocumentComplete events
		AdviseSinkIE();
		NavigateToSamplePage(bIE);
	}
	else
	{
		//there's no CLSID to create a Windows Explorer object
		//so I have to create one with CreateProcess
		//and compare the old and new shell window list 
		//to determin which window should I attatch to

		//sink DShellWindowsEvents events
		LPUNKNOWN pUnkSink = GetIDispatch(FALSE);
		if(m_dwCookieShellWindows!= 0&&m_pShellWindows!=NULL)
		{
			AfxConnectionUnadvise((LPUNKNOWN)m_pShellWindows, 
				DIID_DShellWindowsEvents, pUnkSink, FALSE, m_dwCookieShellWindows);
			m_dwCookieShellWindows= 0;
			m_pShellWindows=(LPUNKNOWN)NULL;
		}
		m_pShellWindows.CoCreateInstance(CLSID_ShellWindows);
		if(m_pShellWindows)
		{
			//get the list of running IE windows
			//using the ShellWindows collection
			//For more information, please visit 
			//http://support.microsoft.com/kb/176792
			long lCount=0;
			m_pShellWindows->get_Count(&lCount);
			for(long i=0;i<lCount;i++)
			{
				CComPtr<IDispatch> pdispShellWindow;
				m_pShellWindows->Item(COleVariant(i),&pdispShellWindow);
				if(pdispShellWindow)
				{
					m_listShellWindows.AddTail(new CComQIPtrIDispatch(pdispShellWindow));
				}
			}
			AfxConnectionAdvise((LPUNKNOWN)m_pShellWindows, 
				DIID_DShellWindowsEvents,pUnkSink,FALSE,&m_dwCookieShellWindows);

		}
		//start the new process
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		ZeroMemory( &si, sizeof(si) );
		si.cb = sizeof(si);
		ZeroMemory( &pi, sizeof(pi) );

		// Start the child process. 
		if( !CreateProcess( NULL, // No module name (use command line). 
			_T("explorer.exe"), // Command line. 
			NULL,             // Process handle not inheritable. 
			NULL,             // Thread handle not inheritable. 
			FALSE,            // Set handle inheritance to FALSE. 
			0,                // No creation flags. 
			NULL,             // Use parent's environment block. 
			NULL,             // Use parent's starting directory. 
			&si,              // Pointer to STARTUPINFO structure.
			&pi )             // Pointer to PROCESS_INFORMATION structure.
		) 
		//wait a gracefully time 
		//so the window is created and is ready to answer messages.
		::WaitForInputIdle(pi.hProcess,1000);
		//m_hExplorerProcess=(DWORD)pi.hProcess;

		//oops, this won't work
		//if another explorer.exe instance exists
		//the new process quits before EnumWindows
		//::EnumWindows(EnumWindowsProc,(LPARAM)this);

		// Close process and thread handles. 
		CloseHandle( pi.hProcess);
		CloseHandle( pi.hThread );
	}
}
void CAutomationDlg::WindowRegistered(long lCookie) 
{
	//ok, a new shell window is created
	if(m_pShellWindows)
	{
		//enumerate through the new shell window list
		long lCount=0;
		m_pShellWindows->get_Count(&lCount);
		for(long i=0;i<lCount;i++)
		{
			//search the new window
			//using the ShellWindows collection
			//For more information, please visit 
			//http://support.microsoft.com/kb/176792
			BOOL bFound=FALSE;
			CComPtr<IDispatch> pdispShellWindow;
			m_pShellWindows->Item(COleVariant(i),&pdispShellWindow);
			//search it in the old shell window list
			POSITION pos=m_listShellWindows.GetHeadPosition();
			while(pos)
			{
				CComQIPtrIDispatch* pDispatch=m_listShellWindows.GetNext(pos);
				if(pDispatch&&pdispShellWindow.p==pDispatch->p)
				{
					bFound=TRUE;break;	
				}
			}
			if(!bFound)//new window 
			{
				//attach to it
				m_pWebBrowser2=pdispShellWindow;
				m_bOwnIE=TRUE;
				//sink for the Quit and DocumentComplete events
				AdviseSinkIE();
				NavigateToSamplePage(FALSE);
			}
		}
		//cleanup
		if(m_dwCookieShellWindows!= 0)
		{
			LPUNKNOWN pUnkSink = GetIDispatch(FALSE);
			AfxConnectionUnadvise((LPUNKNOWN)m_pShellWindows, 
				DIID_DShellWindowsEvents, pUnkSink, FALSE, m_dwCookieShellWindows);
			m_dwCookieShellWindows= 0;
		}
		POSITION pos=m_listShellWindows.GetHeadPosition();
		while(pos)
		{
			CComQIPtrIDispatch* pDispatch=m_listShellWindows.GetNext(pos);
			delete	pDispatch;
		}
		m_listShellWindows.RemoveAll();
		m_pShellWindows=(LPUNKNOWN)NULL;
	}
}
/*
void CAutomationDlg::WindowRevoked(long lCookie) {
}*/

//shell windows object will list both IE and Explorer windows
//use their window class names to identify them.
CString	CAutomationDlg::GetWindowClassName(IWebBrowser2* pwb)
{
	TCHAR szClassName[_MAX_PATH];
	ZeroMemory(szClassName,_MAX_PATH*sizeof(TCHAR));
	HWND hwnd=NULL;
	if(pwb)
	{
		LONG_PTR lwnd=NULL;
		pwb->get_HWND(&lwnd);
		hwnd=reinterpret_cast<HWND>(lwnd);
		::GetClassName(hwnd,szClassName,_MAX_PATH);
	}
	return szClassName;
}
//attach to an existing shell window
void	CAutomationDlg::AttachShellWindow(BOOL bIE)
{
	//SHGetInstanceExplorer succeeds only if it is called from 
	//within an Explorer.exe or Iexplorer.exe process. 

	if(!UpdateData())return;
	//Detach the previous instance, if any
	if(m_pWebBrowser2)
	{
		if(m_bOwnIE)
		{
			m_pWebBrowser2->Quit();
			m_bOwnIE=FALSE;
		}
		UnadvisesinkIE();
	}

	//get the list of running IE windows
	//using the ShellWindows collection
	//For more information, please visit 
	//http://support.microsoft.com/kb/176792

	CComPtr<IShellWindows> psw;
	psw.CoCreateInstance(CLSID_ShellWindows);
	if(psw)
	{
		//array to storage IE window handles and interfaces
		CDWordArray	arHWNDShellWindows;
		arHWNDShellWindows.SetSize(0,10);	//grow by 10 
		CTypedPtrArray<CPtrArray,CComQIPtrIWebBrowser2*> arShellWindows;
		arShellWindows.SetSize(0,10);//grow by 10 
		//enumerate the ShellWindow collection 
		long lShellWindowCount=0;
		psw->get_Count(&lShellWindowCount);
		for(long i=0;i<lShellWindowCount;i++)
		{
			CComPtr<IDispatch> pdispShellWindow;
			psw->Item(COleVariant(i),&pdispShellWindow);
			CComQIPtr<IWebBrowser2> pIE(pdispShellWindow);
			if(pIE)//is it a Shell window?
			{
				//is it the right type?
				CString strWindowClass=GetWindowClassName(pIE);
				if(bIE?	strWindowClass==_T("IEFrame")
					:strWindowClass==_T("ExploreWClass"))
				{
					HWND hWndID=NULL;
					pIE->get_HWND((long*)&hWndID);
					//store its information
					arHWNDShellWindows.Add((DWORD)hWndID);
					arShellWindows.Add(new CComQIPtrIWebBrowser2(pIE));
				}
			}	
		}
		if(arHWNDShellWindows.GetSize()>0)//at least one shell window found
			//, get the top one in z order
		{
			BOOL bFound=FALSE;
			//the first top-level window in zorder
			HWND hwndTest=::GetWindow((HWND)arHWNDShellWindows[0],GW_HWNDFIRST);
			while( hwndTest&& !bFound)
			{
				for(int i=0;i<arHWNDShellWindows.GetSize();i++)
				{
					if(hwndTest==(HWND)arHWNDShellWindows[i])
					{
						//got it, attach to it
						m_pWebBrowser2=*arShellWindows[i];
						AdviseSinkIE();
						NavigateToSamplePage(bIE);
						bFound=TRUE;
						m_bOwnIE=FALSE;
						break;
					}
				}
				hwndTest = ::GetWindow(hwndTest, GW_HWNDNEXT);
			}
		}
		//cleanup
		for(int i=0;i<arShellWindows.GetSize();i++)
		{
			delete arShellWindows[i];
		}
	}
}
//demostrate some common operations
void	CAutomationDlg::NavigateToSamplePage(BOOL bIE)
{
	if(bIE)
	{
		if(!::PathIsURL(m_strFileToFind))
			m_strFileToFind=_T("http://blog.joycode.com/jiangsheng");
		COleVariant vaURL((LPCTSTR)m_strFileToFind);
		m_pWebBrowser2->Navigate2(
			&vaURL,COleVariant((long) 0, VT_I4),
			COleVariant((LPCTSTR)NULL, VT_BSTR),
			COleSafeArray(),
			COleVariant((LPCTSTR)NULL, VT_BSTR)
			);
	}
	else
	{
		//show the folder bar
		// for a list of available bands, visit
		//http://msdn.microsoft.com/library/default.asp?url=/library/en-us/shellcc/platform/shell/reference/objects/ishelldispatch2/showbrowserbar.asp
		COleVariant	clsIDFolderBar(_T("{EFA24E64-B078-11d0-89E4-00C04FC9E26E}"));
		COleVariant	FolderBarShow(VARIANT_TRUE,VT_BOOL);
		COleVariant	dummy;
		if(m_pWebBrowser2)
			m_pWebBrowser2->ShowBrowserBar(&clsIDFolderBar,&FolderBarShow,&dummy);
		//browse to a given folder
		CComQIPtr<IServiceProvider> psp(m_pWebBrowser2);
		CComPtr<IShellBrowser>	psb;		
		if(psp)
			psp->QueryService(SID_STopLevelBrowser,IID_IShellBrowser,(LPVOID*)&psb);
		if(psb)
		{
			USES_CONVERSION;
			LPITEMIDLIST  pidl=NULL;
			SFGAOF sfgao;
			SHParseDisplayName (T2OLE(m_strFileToFind),NULL,&pidl,0, &sfgao);
			if(pidl==NULL)
				::SHGetSpecialFolderLocation(m_hWnd,CSIDL_DRIVES,&pidl);
			m_pidlToNavigate=NULL;
			if(pidl)
			{
				//if the start address is a folder, then browse it.
				//otherwise browse to its parent folder, and select it in the folder view.
				LPCITEMIDLIST  pidlChild=NULL;
				CComPtr<IShellFolder> psf;
				HRESULT	hr = SHBindToParent(pidl, IID_IShellFolder, (LPVOID*)&psf, &pidlChild);
				if (SUCCEEDED(hr)){
					SFGAOF rgfInOut=SFGAO_FOLDER;
					hr=psf->GetAttributesOf(1,&pidlChild,&rgfInOut);
					if (SUCCEEDED(hr)){
						m_pidlToNavigate=ILClone(pidl);
						if(rgfInOut&SFGAO_FOLDER){//this is a folder
							psb->BrowseObject(pidl,SBSP_SAMEBROWSER);	
						}
						else
						{
							//this is a file, browse to the parent folder
							LPITEMIDLIST  pidlParent=ILClone(pidl);
							::ILRemoveLastID(pidlParent);
							psb->BrowseObject(pidlParent,SBSP_SAMEBROWSER);
							ILFree(pidlParent);
						}
					}
				}
				//cleanup
				ILFree(pidl);
			}
		}
	}
}
void CAutomationDlg::DocumentComplete(IDispatch *pDisp,VARIANT *URL)
{
	//HTML DOM is available AFTER the DocumentComplete event is fired.
	//For more information, please visit KB article 
	//"How To Determine When a Page Is Done Loading in WebBrowser Control"
	//http://support.microsoft.com/kb/q180366/
	
	CComQIPtr<IUnknown,&IID_IUnknown> pWBUK(m_pWebBrowser2);
	CComQIPtr<IUnknown,&IID_IUnknown> pSenderUK(pDisp);
	USES_CONVERSION;
	TRACE(_T("Page downloading complete:\r\n"));
	CComBSTR	bstrName;
	m_pWebBrowser2->get_LocationName(&bstrName);
	CComBSTR	bstrURL;
	m_pWebBrowser2->get_LocationURL(&bstrURL);
	TRACE(_T("Name:[ %s ]\r\nURL: [ %s ]\r\n"),
		OLE2T(bstrName),
		OLE2T(bstrURL));
	if (pWBUK== pSenderUK)
	{
		CComQIPtr<IDispatch> pHTMLDocDisp;
		m_pWebBrowser2->get_Document(&pHTMLDocDisp);
		CComQIPtr<IHTMLDocument2> pHTMLDoc(pHTMLDocDisp);
		CComQIPtr<IHTMLElementCollection> ecAll;
		CComPtr<IDispatch> pTagLineDisp;
		if(pHTMLDoc)
		{
			CComBSTR	bstrNewTitle(_T("Sheng Jiang's Automation Test"));
			pHTMLDoc->put_title(bstrNewTitle);
			pHTMLDoc->get_all(&ecAll);
		}
		if(ecAll)
		{
			ecAll->item(COleVariant(_T("tagline")),COleVariant((long)0),&pTagLineDisp);
		}
		CComQIPtr<IHTMLElement> eTagLine(pTagLineDisp);
		if(eTagLine)
		{
			eTagLine->put_innerText(CComBSTR(_T("Command what is yours, conquer what is not. --Kane")));
		}
	}
	else if(m_pidlToNavigate)
	{
		//If the start address is a file, browse to the parent folder
		//and then select it
		CComQIPtr<IServiceProvider> psp(m_pWebBrowser2);
		CComPtr<IShellBrowser>	psb;
		CComPtr<IShellView>	psv;
		if(psp)
			psp->QueryService(SID_STopLevelBrowser,IID_IShellBrowser,(LPVOID*)&psb);
		if(psb)
			psb->QueryActiveShellView(&psv);
		if(psv)
		{
			LPCITEMIDLIST  pidlChild=NULL;
			CComPtr<IShellFolder> psf;
			SFGAOF rgfInOut=SHCIDS_ALLFIELDS;
			HRESULT	hr = SHBindToParent(m_pidlToNavigate, IID_IShellFolder, (LPVOID*)&psf, &pidlChild);
			if (SUCCEEDED(hr)){
				hr=psf->GetAttributesOf(1,&pidlChild,&rgfInOut);
				if (SUCCEEDED(hr)){
					if((rgfInOut&SFGAO_FOLDER)==0){
						//a file, select it
						hr=psv->SelectItem(ILFindLastID(m_pidlToNavigate),SVSI_SELECT|SVSI_ENSUREVISIBLE|SVSI_FOCUSED|SVSI_POSITIONITEM);
					}
				}
			}
		}
		//cleanup
		ILFree(m_pidlToNavigate);
		m_pidlToNavigate=NULL;
	}
}