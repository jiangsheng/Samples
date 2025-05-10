// PicView.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "PicView.h"

#include "MainFrm.h"
#include "PicViewDoc.h"
#include "PicViewView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPicViewApp

BEGIN_MESSAGE_MAP(CPicViewApp, CWinApp)
	//{{AFX_MSG_MAP(CPicViewApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	// Standard file based document commands
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPicViewApp construction

CPicViewApp::CPicViewApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CPicViewApp object

CPicViewApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CPicViewApp initialization

BOOL CPicViewApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CPicViewDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CPicViewView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}


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
		// No message handlers
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

// App command to run the dialog
void CPicViewApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CPicViewApp message handlers


int CPicViewApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	return CWinApp::ExitInstance();
}

void CPicViewApp::OnFileOpen() 
{
	TCHAR tszDisplayName[_MAX_PATH];
	TCHAR tszPathSelected[_MAX_PATH];
	LPITEMIDLIST pidlSelected=PidlBrowse(m_pMainWnd->GetSafeHwnd(),0,tszDisplayName);
	if(pidlSelected){
		if(SHGetPathFromIDList(pidlSelected,tszPathSelected)){
			CDocument* pDocument=OpenDocumentFile(tszPathSelected);
			pDocument->SetTitle(tszDisplayName);
			ILFree(pidlSelected);
		}
	}
}
LPITEMIDLIST CPicViewApp::PidlBrowse(HWND hwnd, int nCSIDL, LPSTR pszDisplayName)
{
    LPITEMIDLIST pidlRoot = NULL;
    LPITEMIDLIST pidlSelected = NULL;
    BROWSEINFO bi = {0};
    LPMALLOC pMalloc = NULL;
    SHGetMalloc(&pMalloc);
    if(nCSIDL){
        SHGetFolderLocation(hwnd, nCSIDL, NULL, NULL, &pidlRoot);
    }
    else{
        pidlRoot = NULL;
    }
    bi.hwndOwner = hwnd;
    bi.pidlRoot = pidlRoot;
    bi.pszDisplayName = pszDisplayName;
    bi.lpszTitle = "Choose a folder";
    bi.ulFlags = BIF_RETURNONLYFSDIRS|BIF_USENEWUI;
    bi.lpfn = NULL;
    bi.lParam = 0;

    pidlSelected = SHBrowseForFolder(&bi);

    if(pidlRoot){
        pMalloc->Free(pidlRoot);
    }
    pMalloc->Release();
    return pidlSelected;
}	
HRESULT CPicViewApp::SHGetDisplayNameOf(IShellFolder *psfParent,LPCITEMIDLIST pidlRelative,LPTSTR pszDisplayName)
{
	STRRET strDispName;
	HRESULT	hr=S_OK;
    hr = psfParent->GetDisplayNameOf(pidlRelative, SHGDN_NORMAL, &strDispName);
    hr = StrRetToBuf(&strDispName, pidlRelative, pszDisplayName, _MAX_PATH*sizeof(TCHAR));
	return hr;
}
HRESULT CPicViewApp::SHGetDisplayNameOf(LPCITEMIDLIST pidl,LPTSTR pszDisplayName)
{
	LPMALLOC pMalloc = NULL;
    IShellFolder *psfParent = NULL;
    LPCITEMIDLIST pidlRelative = NULL;    
    HRESULT hr;
    hr = SHGetMalloc(&pMalloc);
    hr = SHBindToParent(pidl, IID_IShellFolder, (void **) &psfParent, &pidlRelative);
    if(SUCCEEDED(hr)){
		hr=SHGetDisplayNameOf(psfParent, pidlRelative,pszDisplayName);
    }
    psfParent->Release();
    pMalloc->Release();
	return hr;
}
// Macros for interface casts
#ifdef __cplusplus
#define IID_PPV_ARG(IType, ppType) IID_##IType, reinterpret_cast(static_cast(ppType))
#else
#define IID_PPV_ARG(IType, ppType) &IID_##IType, (void**)(ppType)
#endif

// Retrieves the UIObject interface for the specified full PIDL
HRESULT	CPicViewApp::SHGetUIObjectFromFullPIDL(LPCITEMIDLIST pidl, HWND hwnd, REFIID riid, void **ppv)
{
    LPCITEMIDLIST pidlChild;
    IShellFolder* psf;

    *ppv = NULL;

    HRESULT hr = SHBindToParent(pidl, IID_IShellFolder, (LPVOID*)&psf, &pidlChild);
    if (SUCCEEDED(hr))
    {
        hr = psf->GetUIObjectOf(hwnd, 1, &pidlChild, riid, NULL, ppv);
        psf->Release();
    }
    return hr;
}
// Get the target PIDL for a folder PIDL. This deals with cases where a folder
// is an alias to a real folder, folder shortcuts, etc.
HRESULT	CPicViewApp::SHGetTargetFolderIDList(LPCITEMIDLIST pidlFolder, LPITEMIDLIST *ppidl)
{
    IShellLink *psl;
	
    *ppidl = NULL;
    
    HRESULT hr = SHGetUIObjectFromFullPIDL(pidlFolder, NULL, IID_IShellLink, (LPVOID*)&psl);
    
    if (SUCCEEDED(hr))
    {
        hr = psl->GetIDList(ppidl);
        psl->Release();
    }
    
    // It's not a folder shortcut so get the PIDL normally.
    if (FAILED(hr))
        *ppidl=ILClone(pidlFolder);
    
    return hr;
}

// Get the target folder for a folder PIDL. This deals with cases where a folder
// is an alias to a real folder, folder shortcuts, the My Documents folder, etc.
HRESULT	CPicViewApp::SHGetTargetFolderPath(LPCITEMIDLIST pidlFolder, LPWSTR pszPath, UINT cchPath)
{
    LPITEMIDLIST pidlTarget;
	
    *pszPath = 0;

    HRESULT hr = SHGetTargetFolderIDList(pidlFolder, &pidlTarget);
    
    if (SUCCEEDED(hr))
    {
        SHGetPathFromIDListW(pidlTarget, pszPath);   // Make sure it is a path
        CoTaskMemFree(pidlTarget);
    }
    
    return *pszPath ? S_OK : E_FAIL;
}
BOOL	CPicViewApp::ILIsFolder(LPCITEMIDLIST pidl)
{
	BOOL bRet=FALSE;
	LPCITEMIDLIST  pidlChild=NULL;
	IShellFolder* psf;
	HRESULT	hr = SHBindToParent(pidl, IID_IShellFolder, (LPVOID*)&psf, &pidlChild);
	if (SUCCEEDED(hr)){
		SFGAOF rgfInOut=SFGAO_FOLDER;
		hr=psf->GetAttributesOf(1,&pidlChild,&rgfInOut);
		if (SUCCEEDED(hr)){
			if(rgfInOut&SFGAO_FOLDER){
				bRet=TRUE;	
			}
		}
		psf->Release();
	}
	return bRet;
}
