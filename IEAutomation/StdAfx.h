// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__84038409_40B0_4AE1_B97C_CD7AFE4B5ADB__INCLUDED_)
#define AFX_STDAFX_H__84038409_40B0_4AE1_B97C_CD7AFE4B5ADB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxhtml.h>			// MFC HTML view support

#include <afxctl.h>			// For Event Map things
#include <afxole.h>

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>
#include <atlwin.h>

#include <Shlwapi.h>
#include "StrSafe.h"
#include <afxpriv.h>		// For OLE conversion macros
#define __STR2__(x) #x
#define __STR1__(x) __STR2__(x)
#define __LOC__ __FILE__ "("__STR1__(__LINE__)") :  "
#ifndef	INTERNET_MAX_PATH_LENGTH	
	#define	INTERNET_MAX_PATH_LENGTH	2048
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__84038409_40B0_4AE1_B97C_CD7AFE4B5ADB__INCLUDED_)
