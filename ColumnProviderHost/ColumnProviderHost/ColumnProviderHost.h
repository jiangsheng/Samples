// ColumnProviderHost.h : main header file for the ColumnProviderHost application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CColumnProviderHostApp:
// See ColumnProviderHost.cpp for the implementation of this class
//

class CColumnProviderHostApp : public CWinAppEx
{
public:
	CColumnProviderHostApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CColumnProviderHostApp theApp;
