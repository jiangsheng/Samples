// Automation.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Automation.h"
#include "AutomationDlg.h"
#include <initguid.h>
#include "Automation_i.c"
#include ".\automation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAutomationApp


class CAutomationModule :
	public CAtlMfcModule
{
public:
	DECLARE_LIBID(LIBID_AutomationLib);
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_AUTOMATION, "{75E41207-FD36-4221-82FC-6893F3C532C9}");};

CAutomationModule _AtlModule;

BEGIN_MESSAGE_MAP(CAutomationApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CAutomationApp construction

CAutomationApp::CAutomationApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CAutomationApp object

CAutomationApp theApp;


// CAutomationApp initialization

BOOL CAutomationApp::InitInstance()
{
	AfxOleInit();
	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();
	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// Register class factories via CoRegisterClassObject().
	if (FAILED(_AtlModule.RegisterClassObjects(CLSCTX_LOCAL_SERVER, REGCLS_MULTIPLEUSE)))
		return FALSE;
	// App was launched with /Embedding or /Automation switch.
	// Run app as automation server.
	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		// Don't show the main window
		return TRUE;
	}
	// App was launched with /Unregserver or /Unregister switch.
	if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppUnregister)
	{
		_AtlModule.UpdateRegistryAppId(FALSE);
		_AtlModule.UnregisterServer(TRUE);
		return FALSE;
	}
	// App was launched with /Register or /Regserver switch.
	if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppRegister)
	{
		_AtlModule.UpdateRegistryAppId(TRUE);
		_AtlModule.RegisterServer(TRUE);
		return FALSE;
	}

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	CAutomationDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

BOOL CAutomationApp::ExitInstance(void)
{
	_AtlModule.RevokeClassObjects();
	return CWinApp::ExitInstance();
}
