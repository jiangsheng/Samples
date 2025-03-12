// WDSSample.h : main header file for the WDSSAMPLE application
//

#if !defined(AFX_WDSSAMPLE_H__44375E04_3B9B_41B7_9E01_239A2D1C1180__INCLUDED_)
#define AFX_WDSSAMPLE_H__44375E04_3B9B_41B7_9E01_239A2D1C1180__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "WDSSample_i.h"

/////////////////////////////////////////////////////////////////////////////
// CWDSSampleApp:
// See WDSSample.cpp for the implementation of this class
//

class CWDSSampleApp : public CWinApp
{
public:
	CWDSSampleApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWDSSampleApp)
	public:
	virtual BOOL InitInstance();
		virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CWDSSampleApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bATLInited;
private:
	BOOL InitATL();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WDSSAMPLE_H__44375E04_3B9B_41B7_9E01_239A2D1C1180__INCLUDED_)
