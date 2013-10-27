// MFCWinFormHybrid.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxpriv.h"
#include "MFCWinFormHybrid.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "MFCWinFormHybridDoc.h"
#include "MFCWinFormHybridView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCWinFormHybridApp

BEGIN_MESSAGE_MAP(CMFCWinFormHybridApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CMFCWinFormHybridApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// CMFCWinFormHybridApp construction

CMFCWinFormHybridApp::CMFCWinFormHybridApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CMFCWinFormHybridApp object

CMFCWinFormHybridApp theApp;


// CMFCWinFormHybridApp initialization

BOOL CMFCWinFormHybridApp::InitInstance()
{
#ifdef HYBRID_MODE	
	ManagedInit();
#endif
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_MFCWinFormTYPE,
		RUNTIME_CLASS(CMFCWinFormHybridDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CMFCWinFormHybridView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// call DragAcceptFiles only if there's a suffix
	//  In an MDI app, this should occur immediately after setting m_pMainWnd


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}



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

// App command to run the dialog
void CMFCWinFormHybridApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CMFCWinFormHybridApp message handlers
#pragma managed
bool MFCMessageFilter(HWND hwnd,UINT message,LONG x,LONG y,DWORD time,
WPARAM wparam, LPARAM lparam)
{
	_AFX_THREAD_STATE *pState = AfxGetThreadState();

	pState->m_msgCur.hwnd=hwnd;
	pState->m_msgCur.message=message;
	pState->m_msgCur.pt.x=x;
	pState->m_msgCur.pt.y=y;
	pState->m_msgCur.time=time;
	pState->m_msgCur.wParam=wparam;
	pState->m_msgCur.lParam=lparam;
	if(message==WM_KICKIDLE)
	return true;
	bool mfcPretranslate=(bool)AfxPreTranslateMessage(&(pState->m_msgCur));
	return mfcPretranslate;
} 
ref class CMFCMessageFilter:public System::Windows::Forms::IMessageFilter
{
public:

	[System::Security::Permissions::SecurityPermission(System::Security::Permissions::SecurityAction::LinkDemand,
	Flags=System::Security::Permissions::SecurityPermissionFlag::UnmanagedCode)]
	 virtual bool PreFilterMessage(System::Windows::Forms::Message % m)
	 {
		 System::Runtime::InteropServices::HandleRef handle=System::Runtime::InteropServices::HandleRef (m.HWnd,m.HWnd);
		return
		MFCMessageFilter((HWND)m.HWnd.ToPointer(),m.Msg,0,0,0,(WPARAM)m.WParam.ToPointer(),(LPARAM)m.LParam.ToPointer());
	 }
}; 
#ifdef HYBRID_MODE
void CMFCWinFormHybridApp::ManagedInit(void)
{
	System::Windows::Forms::Application::AddMessageFilter(gcnew CMFCMessageFilter()); 
}
#endif
void ManagedRun()
{
 System::Windows::Forms::Application::Run();

} 

#pragma unmanaged 
#ifdef HYBRID_MODE
int CMFCWinFormHybridApp::Run()
{
 if (m_pMainWnd == NULL && AfxOleGetUserCtrl())
 {
  // Not launched /Embedding or /Automation, but has no main window!
  TRACE(traceAppMsg, 0, "Warning: m_pMainWnd is NULL in CWinApp::Run -quitting application.\n");
  AfxPostQuitMessage(0);
 }
 ManagedRun();
 return ExitInstance();
 //return CWinApp::Run();
}

#endif