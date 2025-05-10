// IEAutomation.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"

#include "IEAutomation.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "IEAutomationDoc.h"
#include "IEAutomationView.h"
#include <Transact.h>
#undef AFX_DATA
#define AFX_DATA AFX_DATA_IMPORT
// NOTE: This line is a hardcoded reference to an MFC header file
//  this path may need to be changed to refer to the location of VC5 install
//  for successful compilation.
#include <..\src\occimpl.h>
#undef AFX_DATA
#define AFX_DATA AFX_DATA_EXPORT

#include "CustomOccManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIEAutomationApp

BEGIN_MESSAGE_MAP(CIEAutomationApp, CWinApp)
	//{{AFX_MSG_MAP(CIEAutomationApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIEAutomationApp construction

CIEAutomationApp::CIEAutomationApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CIEAutomationApp object

CIEAutomationApp theApp;

// This identifier was generated to be statistically unique for your app.
// You may change it if you prefer to choose a specific identifier.

// {8A483A03-99C1-4073-8B75-967DE3C39503}
static const CLSID clsid =
{ 0x8a483a03, 0x99c1, 0x4073, { 0x8b, 0x75, 0x96, 0x7d, 0xe3, 0xc3, 0x95, 0x3 } };

/////////////////////////////////////////////////////////////////////////////
// CIEAutomationApp initialization

BOOL CIEAutomationApp::InitInstance()
{
	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	CCustomOccManager *pMgr = new CCustomOccManager;

	// Create an IDispatch class for extending the Dynamic HTML Object Model 
//	m_pDispOM = new CImpIDispatch;

	// Set our control containment up but using our control container 
	// management class instead of MFC's default
	AfxEnableControlContainer(pMgr);

	//AfxEnableControlContainer();

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

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_IEAUTOTYPE,
		RUNTIME_CLASS(CIEAutomationDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CIEAutomationView));
	AddDocTemplate(pDocTemplate);

	// Connect the COleTemplateServer to the document template.
	//  The COleTemplateServer creates new documents on behalf
	//  of requesting OLE containers by using information
	//  specified in the document template.
	m_server.ConnectTemplate(clsid, pDocTemplate, FALSE);

	// Register all OLE server factories as running.  This enables the
	//  OLE libraries to create objects from other applications.
	COleTemplateServer::RegisterAll();
		// Note: MDI applications register all server objects without regard
		//  to the /Embedding or /Automation on the command line.

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Check to see if launched as OLE server
	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		// Application was run with /Embedding or /Automation.  Don't show the
		//  main window in this case.
		return TRUE;
	}

	// When a server application is launched stand-alone, it is a good idea
	//  to update the system registry in case it has been damaged.
	m_server.UpdateRegistry(OAT_DISPATCH_OBJECT);
	COleObjectFactory::UpdateRegistryAll();
	AfxOleRegisterTypeLib(
		AfxGetInstanceHandle(),
		_tlid);
	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow|SW_SHOWMAXIMIZED);
	pMainFrame->UpdateWindow();

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
void CIEAutomationApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CIEAutomationApp message handlers

void	CIEAutomationApp::UpdateFavoritesMenu()
{
	POSITION	posDocTemplate=GetFirstDocTemplatePosition();
	while(posDocTemplate){
		CDocTemplate*	pDocTemplate=GetNextDocTemplate(posDocTemplate);
		POSITION	posDoc=pDocTemplate->GetFirstDocPosition();
		while(posDoc){
			CDocument* pDoc=pDocTemplate->GetNextDoc(posDoc);
			POSITION	posView=pDoc->GetFirstViewPosition();
			if(posView){
				CView* pView=pDoc->GetNextView(posView);
				CFrameWnd* pFrame=pView->GetParentFrame();
				if(pFrame->IsKindOf(RUNTIME_CLASS(CChildFrame))){
					CChildFrame* pChildFrame=(CChildFrame*)pFrame;
					pChildFrame->BuildFavoritesMenu();
				}
			}
		}
	}
}