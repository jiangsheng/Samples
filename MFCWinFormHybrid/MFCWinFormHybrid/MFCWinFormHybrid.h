// MFCWinFormHybrid.h : main header file for the MFCWinFormHybrid application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMFCWinFormHybridApp:
// See MFCWinFormHybrid.cpp for the implementation of this class
//
#define HYBRID_MODE
class CMFCWinFormHybridApp : public CWinApp
{
public:
	CMFCWinFormHybridApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
private:
#ifdef HYBRID_MODE
	void ManagedInit(void);
public:
	virtual int Run();
#endif
};

extern CMFCWinFormHybridApp theApp;