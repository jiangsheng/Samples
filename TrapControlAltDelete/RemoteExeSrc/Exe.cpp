// Exe.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Exe.h"
#include "ExeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExeApp

BEGIN_MESSAGE_MAP(CExeApp, CWinApp)
	//{{AFX_MSG_MAP(CExeApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExeApp construction

CExeApp::CExeApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CExeApp object

CExeApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CExeApp initialization

BOOL CExeApp::InitInstance()
{
	// Standard initialization

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CExeDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
