// PicView.h : main header file for the PICVIEW application
//

#if !defined(AFX_PICVIEW_H__FAEF4470_5B76_4C5C_89F2_504C6A9F3AC8__INCLUDED_)
#define AFX_PICVIEW_H__FAEF4470_5B76_4C5C_89F2_504C6A9F3AC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CPicViewApp:
// See PicView.cpp for the implementation of this class
//

class CPicViewApp : public CWinApp
{
public:
	CPicViewApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPicViewApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL
	LPITEMIDLIST PidlBrowse(HWND hwnd, int nCSIDL, LPTSTR pszDisplayName);
	HRESULT SHGetDisplayNameOf(IShellFolder *psfParent,LPCITEMIDLIST pidlRelative,LPTSTR pszDisplayName);
	HRESULT SHGetDisplayNameOf(LPCITEMIDLIST pidl,LPSTR pszDisplayName);
	HRESULT SHGetUIObjectFromFullPIDL(LPCITEMIDLIST pidl, HWND hwnd, REFIID riid, void **ppv);
	HRESULT SHGetTargetFolderIDList(LPCITEMIDLIST pidlFolder, LPITEMIDLIST *ppidl);
	HRESULT SHGetTargetFolderPath(LPCITEMIDLIST pidlFolder, LPWSTR pszPath, UINT cchPath);
	BOOL	ILIsFolder(LPCITEMIDLIST pidl);
// Implementation
	//{{AFX_MSG(CPicViewApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern	CPicViewApp theApp;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PICVIEW_H__FAEF4470_5B76_4C5C_89F2_504C6A9F3AC8__INCLUDED_)
