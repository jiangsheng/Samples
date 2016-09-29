
// HtmlCaretTest.h : main header file for the HtmlCaretTest application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CHtmlCaretTestApp:
// See HtmlCaretTest.cpp for the implementation of this class
//

class CHtmlCaretTestApp : public CWinApp
{
public:
	CHtmlCaretTestApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CHtmlCaretTestApp theApp;
