#if !defined(AFX_IEAUTOMATIONVIEW_H__AC86F771_D61C_4B65_9971_817758F36A7D__INCLUDED_)
#define AFX_IEAUTOMATIONVIEW_H__AC86F771_D61C_4B65_9971_817758F36A7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IEAutomationView.h : header file
//
#include <mshtml.h>
#include <mshtmhst.h>
//
//
//
#include <activscp.h> // Active Scripting Interfaces

/////////////////////////////////////////////////////////////////////////////
// CIEAutomationView html view

class	CIEAutomationDoc;
#include "DOMExternal.h"

class CIEAutomationView : public CHtmlView
{
protected:
	CIEAutomationView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CIEAutomationView)

// html Data
public:
	//{{AFX_DATA(CIEAutomationView)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	CIEAutomationDoc* GetDocument();
//offline support
	BOOL	m_bOffline;
//CGID_WebBrowser Command IDs
	enum	enumHTMLID{
		HTMLID_FIND       = 1
		,HTMLID_VIEWSOURCE = 2
		,HTMLID_OPTIONS    = 3
	};

// Operations
public:
	//from MFC7
	HRESULT ExecFormsCommand(DWORD dwCommandID,
		VARIANT* pVarIn, VARIANT* pVarOut);
	HRESULT QueryFormsCommand(DWORD dwCommandID,
		BOOL* pbSupported, BOOL* pbEnabled, BOOL* pbChecked);
	void	ExecWBCommand(
		const GUID *pguidCmdGroup,  // Pointer to command group
		DWORD nCmdID,               // Identifier of command to execute
		DWORD nCmdExecOpt,          // Options for executing the command
		VARIANTARG *pvaIn,          // Pointer to input arguments
		VARIANTARG *pvaOut          // Pointer to command output
	);
	//call script method from HTML page
	void CallScriptDirect(IHTMLWindow2* pWindow2
		,BSTR bstrMethod);
	void CallScript(IHTMLWindow2* pWindow2
		,BSTR bstrCode
		,BSTR bstrLanguage);

	void RunScript() ;
	//to save the autocomplete information
	void	AutoCompleteSaveForm(LPDISPATCH pDisp);
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIEAutomationView)
	public:
	virtual void OnFinalRelease();
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void OnNewWindow2(LPDISPATCH* ppDisp, BOOL* Cancel);
	virtual BOOL OnAmbientProperty(COleControlSite* pSite, DISPID dispid, VARIANT* pvar);
	//}}AFX_VIRTUAL
	//from MFCIE sample
	void OnTitleChange(LPCTSTR lpszText);
	void DocumentComplete(LPDISPATCH pDisp, VARIANT* URL);
	void OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR lpszUrl);
	void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags,
		LPCTSTR lpszTargetFrameName, CByteArray& baPostedData,
		LPCTSTR lpszHeaders, BOOL* pbCancel);	
	//override to redirect BeforeNavigate2 event
	virtual void BeforeNavigate2(LPDISPATCH pDisp, VARIANT* URL,
		VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData,
		VARIANT* Headers,   BOOL* Cancel);

// Implementation
protected:
///////////////////////////////////////////////////
//Type Library Support
	DECLARE_OLETYPELIB(CIEAutomationView)
	GetDispatchIID(IID*);
///////////////////////////////////////////////////
//	Q168214 SAMPLE: MFCAxs.exe Implements an ActiveX Script Host Using MFC
///////////////////////////////////////////////////
//Active Script Support
	BEGIN_INTERFACE_PART(ActiveScriptSite, IActiveScriptSite)
		STDMETHOD(GetLCID)(LCID*);
		STDMETHOD(GetItemInfo)(LPCOLESTR, DWORD, LPUNKNOWN*, LPTYPEINFO*);
        STDMETHOD(GetDocVersionString)(BSTR*);
        STDMETHOD(OnScriptTerminate)(const VARIANT*, const EXCEPINFO*);
        STDMETHOD(OnStateChange)(SCRIPTSTATE);
        STDMETHOD(OnScriptError)(IActiveScriptError*);
        STDMETHOD(OnEnterScript)();
        STDMETHOD(OnLeaveScript)();
	END_INTERFACE_PART(ActiveScriptSite)

	BEGIN_INTERFACE_PART(ActiveScriptSiteWindow, IActiveScriptSiteWindow)
		STDMETHOD(EnableModeless)(BOOL);
		STDMETHOD(GetWindow)(HWND*);
	END_INTERFACE_PART(ActiveScriptSiteWindow)
/////////////////////////////////
//IDocUIHandler2 Methods from MFC7
public:
	STDMETHOD(ShowContextMenu)(/* [in] */ DWORD dwID,
            /* [in] */ POINT __RPC_FAR *ppt,
            /* [in] */ IUnknown __RPC_FAR *pcmdtReserved,
            /* [in] */ IDispatch __RPC_FAR *pdispReserved);
	STDMETHOD(GetHostInfo)( 
            /* [out][in] */ DOCHOSTUIINFO __RPC_FAR *pInfo);
	STDMETHOD(ShowUI)( 
            /* [in] */ DWORD dwID,
            /* [in] */ IOleInPlaceActiveObject __RPC_FAR *pActiveObject,
            /* [in] */ IOleCommandTarget __RPC_FAR *pCommandTarget,
            /* [in] */ IOleInPlaceFrame __RPC_FAR *pFrame,
            /* [in] */ IOleInPlaceUIWindow __RPC_FAR *pDoc);
	STDMETHOD(HideUI)(void);
	STDMETHOD(UpdateUI)(void);
	STDMETHOD(EnableModeless)(/* [in] */ BOOL fEnable);
	STDMETHOD(OnDocWindowActivate)(/* [in] */ BOOL fEnable);
	STDMETHOD(OnFrameWindowActivate)(/* [in] */ BOOL fEnable);
	STDMETHOD(ResizeBorder)( 
            /* [in] */ LPCRECT prcBorder,
            /* [in] */ IOleInPlaceUIWindow __RPC_FAR *pUIWindow,
            /* [in] */ BOOL fRameWindow);
	STDMETHOD(TranslateAccelerator)( 
            /* [in] */ LPMSG lpMsg,
            /* [in] */ const GUID __RPC_FAR *pguidCmdGroup,
            /* [in] */ DWORD nCmdID);
	STDMETHOD(GetOptionKeyPath)( 
            /* [out] */ LPOLESTR __RPC_FAR *pchKey,
            /* [in] */ DWORD dw);
	STDMETHOD(GetDropTarget)(
            /* [in] */ IDropTarget __RPC_FAR *pDropTarget,
            /* [out] */ IDropTarget __RPC_FAR *__RPC_FAR *ppDropTarget);
    STDMETHOD(GetExternal)( 
            /* [out] */ IDispatch __RPC_FAR *__RPC_FAR *ppDispatch);
    STDMETHOD(TranslateUrl)( 
            /* [in] */ DWORD dwTranslate,
            /* [in] */ OLECHAR __RPC_FAR *pchURLIn,
            /* [out] */ OLECHAR __RPC_FAR *__RPC_FAR *ppchURLOut);
    STDMETHOD(FilterDataObject)( 
            /* [in] */ IDataObject __RPC_FAR *pDO,
            /* [out] */ IDataObject __RPC_FAR *__RPC_FAR *ppDORet);
	STDMETHOD(GetOverrideKeyPath)( 
					/* [out] */ LPOLESTR *pchKey,
					/* [in] */ DWORD dw) ;
//end IDocUIHandler2 Methods
//////////////////////////////////
//Q168214 SAMPLE: MFCAxs.exe Implements an ActiveX Script Host Using MFC
	BOOL CreateEngine();
	CMapStringToOb mapNamedItems;
	BOOL m_bScriptIsRunning;
	IActiveScriptParse* m_axsp;
	IActiveScript* m_axs;
///////////////////////////////////////////////////
// host handler from MFC7
	DWORD m_dwHostFlags;
	void SetHostFlags(DWORD dwFlags);
	CComPtr<IDispatch> m_spExternalDisp;
	void SetExternalDispatch(IDispatch *pdispExternal);
	virtual BOOL IsExternalDispatchSafe();
	virtual BOOL CanAccessExternal();
	CComPtr<IDispatch> m_spHtmlDoc;
//////////////////////////////////////////////////
//Shell Helper document object model extension
	CDOMExternal	m_domExternal;
	virtual ~CIEAutomationView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CIEAutomationView)
	afx_msg void OnFileOpen();
	afx_msg void OnGoBack();
	afx_msg void OnGoForward();
	afx_msg void OnGoSearchTheWeb();
	afx_msg void OnGoStartPage();
	afx_msg void OnViewStop();
	afx_msg void OnViewRefresh();
	afx_msg void OnViewFontsLargest();
	afx_msg void OnViewFontsLarge();
	afx_msg void OnViewFontsMedium();
	afx_msg void OnViewFontsSmall();
	afx_msg void OnViewFontsSmallest();
	afx_msg void OnViewSource();
	afx_msg void OnDestroy();
	afx_msg void OnEditFind();
	afx_msg void OnViewOption();
	afx_msg void OnFileOffline();
	afx_msg void OnUpdateFileOffline(CCmdUI* pCmdUI);
	afx_msg void OnScriptEdit();
	afx_msg void OnUpdateScriptEdit(CCmdUI* pCmdUI);
	afx_msg void OnScriptRun();
	afx_msg void OnUpdateScriptRun(CCmdUI* pCmdUI);
	afx_msg void OnFavoritesAdd();
	afx_msg void OnFavoritesManage();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CIEAutomationView)
	afx_msg LPDISPATCH GetWebBrowser();
	afx_msg LPDISPATCH HostCreateObject(const VARIANT FAR& bstrProgID);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_EVENTSINK_MAP()
//Knowledge Base  
//BUG: CHtmlView Leaks Memory by Not Releasing BSTRs in Several Methods
// Implementation
//////////////////////////////////////////////////
//new code
// Operations
public:
    CString GetFullName() const;
    CString GetType() const;
    CString GetLocationName() const;
    CString GetLocationURL() const;

    void Navigate(LPCTSTR lpszURL, DWORD dwFlags = 0 ,  
                  LPCTSTR lpszTargetFrameName = NULL ,  
                  LPCTSTR lpszHeaders = NULL, LPVOID lpvPostData = NULL,  
                  DWORD dwPostDataLen = 0);

//Add the following functions. This action prevents the leak because the 
//CHtmlView versions of LoadFromResource() call CHtmlView::Navigate().
//These need to be defined because CHtmlView::Navigate() is not virtual.
    BOOL LoadFromResource(LPCTSTR lpszResource);
    BOOL LoadFromResource(UINT nRes);
//end new code
//////////////////////////////////////////////////
//Knowledge Base  
//PRB: WebBrowser Control Disappears When Script Calls window.close()
//////////////////////////////
//new code
	 afx_msg void OnParentNotify( UINT message, LPARAM lParam ); 
	void	WindowClosing(
		VARIANT_BOOL IsChildWindow,
		VARIANT_BOOL *Cancel
	);
//end new code
//////////////////////////////
};

extern const GUID CDECL BASED_CODE _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;
/////////////////////////////////////////////////////////////////////////////
#ifndef _DEBUG  // debug version in IEAutomationView.cpp
inline CIEAutomationDoc* CIEAutomationView::GetDocument()
   { return (CIEAutomationDoc*)m_pDocument; }
#endif
/////////////////////////////////////////////////////////////////////////////
extern	const GUID	CGID_WebBrowser;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IEAUTOMATIONVIEW_H__AC86F771_D61C_4B65_9971_817758F36A7D__INCLUDED_)
