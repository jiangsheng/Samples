#if !defined(AFX_DOMEXTERNAL_H__A7E7F294_7887_4B37_851E_EF9EDCBA2EBF__INCLUDED_)
#define AFX_DOMEXTERNAL_H__A7E7F294_7887_4B37_851E_EF9EDCBA2EBF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DOMExternal.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CDOMExternal command target
//custom dom extension and browse event redirector
#include "CmdTargetPlus.h"
class	CIEAutomationView;
class CDOMExternal : public CCmdTargetPlus
{
	DECLARE_DYNCREATE(CDOMExternal)

	CDOMExternal();           // protected constructor used by dynamic creation

// Attributes
public:
	//for IShellUIHelper method
	CComPtr<IShellUIHelper>	m_pShellUIHelper;
	//for window.external.menuargument support
	CIEAutomationView*		m_pView;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDOMExternal)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL
	virtual ~CDOMExternal();
// Implementation
protected:
	

	// Generated message map functions
	//{{AFX_MSG(CDOMExternal)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CDOMExternal)
	afx_msg LPDISPATCH GetMenuArguments();
	afx_msg void SetNotSupported(LPDISPATCH newValue);
	afx_msg void AddChannel(LPCTSTR URL);
	afx_msg void AddDesktopComponent(LPCTSTR URL, LPCTSTR Type, const VARIANT FAR& Left, const VARIANT FAR& Top, const VARIANT FAR& Width, const VARIANT FAR& Height);
	afx_msg void AutoCompleteSaveForm(const VARIANT FAR& pForm);
	afx_msg void AutoScan(LPCTSTR strSearch, LPCTSTR strFailureUrl, const VARIANT FAR& pvarTargetFrame);
	afx_msg void ImportExportFavorites(BOOL fImport, LPCTSTR strImpExpPath);
	afx_msg BOOL IsSubscribed(LPCTSTR URL);
	afx_msg void NavigateAndFind(LPCTSTR URL, LPCTSTR strQuery, const VARIANT FAR& varTargetFrame);
	afx_msg VARIANT	ShowBrowserUI(LPCTSTR bstrName, const VARIANT FAR& varNULL);
	afx_msg void AddFavorite(LPCTSTR URL, const VARIANT FAR& Title);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_EVENT_MAP()
	// Event maps
public:
	//{{AFX_EVENT(CDOMExternal)
	void FireBeforeNavigate2(LPDISPATCH pDisp, VARIANT FAR* URL, VARIANT FAR* Flags, VARIANT FAR* TargetFrameName, VARIANT FAR* PostData, VARIANT FAR* Headers, BOOL FAR* Cancel);
		//{FireEvent(eventidBeforeNavigate2,EVENT_PARAM(VTS_DISPATCH  VTS_PVARIANT  VTS_PVARIANT  VTS_PVARIANT  VTS_PVARIANT  VTS_PVARIANT  VTS_PBOOL), pDisp, URL, Flags, TargetFrameName, PostData, Headers, Cancel);}
	//}}AFX_EVENT
	HRESULT GetClassID(LPCLSID pclsid);
	DECLARE_OLETYPELIB(CDOMExternal)
	DECLARE_OLECREATE(CDOMExternal)

	enum {
	//{{AFX_DISP_ID(CDOMExternal)
	dispidMenuArguments = 1L,
	dispidAddChannel = 3L,
	dispidAddDesktopComponent = 4L,
	dispidIsSubscribed = 8L,
	dispidNavigateAndFind = 9L,
	dispidImportExportFavorites = 7L,
	dispidAutoCompleteSaveForm = 5L,
	dispidAutoScan = 6L,
	dispidShowBrowserUI = 10L,
	dispidAddFavorite = 11L,
	eventidBeforeNavigate2 = 1L,
	//}}AFX_DISP_ID
	};
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOMEXTERNAL_H__A7E7F294_7887_4B37_851E_EF9EDCBA2EBF__INCLUDED_)
