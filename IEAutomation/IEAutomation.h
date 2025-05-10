// IEAutomation.h : main header file for the IEAUTOMATION application
//

#if !defined(AFX_IEAUTOMATION_H__B1ED802B_2424_4DE6_B970_FE1ED2650E88__INCLUDED_)
#define AFX_IEAUTOMATION_H__B1ED802B_2424_4DE6_B970_FE1ED2650E88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CIEAutomationApp:
// See IEAutomation.cpp for the implementation of this class
//

class CIEAutomationApp : public CWinApp
{
public:
	CIEAutomationApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIEAutomationApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL
	CString m_strAuthenticateUsername ;
	CString m_strAuthenticatePassword ;
	//to update Favorites Menu for all mdi child frame 
	void	UpdateFavoritesMenu();
// Implementation
	COleTemplateServer m_server;
		// Server object for document creation
	//{{AFX_MSG(CIEAutomationApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern	CIEAutomationApp	theApp;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IEAUTOMATION_H__B1ED802B_2424_4DE6_B970_FE1ED2650E88__INCLUDED_)
