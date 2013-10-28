// Automation.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "Automation_i.h"


// CAutomationApp:
// See Automation.cpp for the implementation of this class
//

class CAutomationApp : public CWinApp
{
public:
	CAutomationApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
	BOOL ExitInstance(void);
};

extern CAutomationApp theApp;