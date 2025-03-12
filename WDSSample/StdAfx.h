// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__4730CE06_A543_409A_B242_797B91359C64__INCLUDED_)
#define AFX_STDAFX_H__4730CE06_A543_409A_B242_797B91359C64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxcview.h>
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


	#define _ATL_APARTMENT_THREADED
#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
class CWDSSampleModule : public CComModule
{
public:
	LONG Unlock();
	LONG Lock();
	LPCTSTR FindOneOf(LPCTSTR p1, LPCTSTR p2);
	DWORD dwThreadID;
};
extern CWDSSampleModule _Module;
#include <atlcom.h>
#import "c:\Program Files\Common Files\system\ado\Msado15.dll" rename("EOF","adoEOF") rename("DataTypeEnum","adoDataTypeEnum") 
using namespace ADODB;
#import "wdsQuery.tlb" rename("EOF","adoEOF") rename("DataTypeEnum","adoDataTypeEnum") 
using namespace WDSQuery;
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__4730CE06_A543_409A_B242_797B91359C64__INCLUDED_)
