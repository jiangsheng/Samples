// IEAutomationView.cpp : implementation file
//

#include "stdafx.h"

#include <initguid.h> // Realizes CLSID definitions
#include "msscript.h" // defines CLSIDs for VBScript and JScript
//#include "StrSafe.h"
#include <Mshtmdid.h>
#include <exdispid.h>
#include <Exdisp.h>
#include <idispids.h>
#include "IEAutomation.h"
#include "IEAutomationDoc.h"
#include "IEAutomationView.h"
#include "ChildFrm.h"
#include "CmdTargetPlus.h"
#include "DlgInputText.h"
#include "DlgDocOutline.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIEAutomationView
//[ uuid(6627B8C6-C0B2-4438-B989-61EF9A97FCD8), version(1.0) ]
//library IEAutomation
const GUID CDECL BASED_CODE _tlid =
		{ 0x6627B8C6, 0xC0B2, 0x4438, { 0xb9, 0x89, 0x61, 0xef, 0x9a, 0x97, 0xfc, 0xd8 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;
const GUID	CGID_WebBrowser={
	0xED016940, 0xBD5B, 0x11cf, 0xBA,0x4E,0x00,0xC0,0x4F,0xD7,0x08,0x16};

IMPLEMENT_DYNCREATE(CIEAutomationView, CHtmlView)

CIEAutomationView::CIEAutomationView()
{
	EnableAutomation();
	//{{AFX_DATA_INIT(CIEAutomationView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
///////////////////////////////////////////////////
//Active Script Support
	m_axsp = NULL;
	m_axs  = NULL;
	m_bScriptIsRunning = FALSE;
///////////////////////////////////////////////////
// Hook up windows objects to their dispatch implementations
// so they can fire COM events when Windows events occur
	#pragma message(__LOC__"Need to do Windows Objects hook up here\r\n") 
// vice versa, so dispatch implementations can affect windows
///////////////////////////////////////////////////
//Offline Support
	m_bOffline=FALSE;
///////////////////////////////////////////////////
//Host Flags
	m_dwHostFlags=DOCHOSTUIFLAG_NO3DBORDER;
	m_domExternal.m_pView=this;
	ExternalAddRef();
}

CIEAutomationView::~CIEAutomationView()
{
///////////////////////////////////////////////////
//Active Script Support
	if (m_axsp)
		m_axsp->Release();
	if (m_axs){
		m_axs->Close();
		m_axs->Release();
	}
///////////////////////////////////////////////////
}

void CIEAutomationView::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CHtmlView::OnFinalRelease();
}

void CIEAutomationView::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIEAutomationView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIEAutomationView, CHtmlView)
	//{{AFX_MSG_MAP(CIEAutomationView)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_GO_BACK, OnGoBack)
	ON_COMMAND(ID_GO_FORWARD, OnGoForward)
	ON_COMMAND(ID_GO_SEARCH_THE_WEB, OnGoSearchTheWeb)
	ON_COMMAND(ID_GO_START_PAGE, OnGoStartPage)
	ON_COMMAND(ID_VIEW_STOP, OnViewStop)
	ON_COMMAND(ID_VIEW_REFRESH, OnViewRefresh)
	ON_COMMAND(ID_VIEW_FONTS_LARGEST, OnViewFontsLargest)
	ON_COMMAND(ID_VIEW_FONTS_LARGE, OnViewFontsLarge)
	ON_COMMAND(ID_VIEW_FONTS_MEDIUM, OnViewFontsMedium)
	ON_COMMAND(ID_VIEW_FONTS_SMALL, OnViewFontsSmall)
	ON_COMMAND(ID_VIEW_FONTS_SMALLEST, OnViewFontsSmallest)
	ON_COMMAND(ID_VIEW_SOURCE, OnViewSource)
	ON_WM_DESTROY()
	ON_COMMAND(ID_EDIT_FIND, OnEditFind)
	ON_COMMAND(ID_VIEW_OPTION, OnViewOption)
	ON_COMMAND(ID_FILE_OFFLINE, OnFileOffline)
	ON_UPDATE_COMMAND_UI(ID_FILE_OFFLINE, OnUpdateFileOffline)
	ON_COMMAND(ID_FAVORITES_MANAGE, OnFavoritesManage)
	ON_COMMAND(ID_FAVORITES_ADD, OnFavoritesAdd)
	ON_COMMAND(ID_SCRIPT_EDIT, OnScriptEdit)
	ON_UPDATE_COMMAND_UI(ID_SCRIPT_EDIT, OnUpdateScriptEdit)
	ON_COMMAND(ID_SCRIPT_RUN, OnScriptRun)
	ON_UPDATE_COMMAND_UI(ID_SCRIPT_RUN, OnUpdateScriptRun)
	//}}AFX_MSG_MAP
//Knowledge Base  
//PRB: WebBrowser Control Disappears When Script Calls window.close()
//new code
	ON_WM_PARENTNOTIFY() 
//end new code
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CIEAutomationView, CHtmlView)
	//{{AFX_DISPATCH_MAP(CIEAutomationView)
	DISP_PROPERTY_EX(CIEAutomationView, "WebBrowser", GetWebBrowser, SetNotSupported, VT_DISPATCH)
	DISP_FUNCTION(CIEAutomationView, "HostCreateObject", HostCreateObject, VT_DISPATCH, VTS_VARIANT)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

BEGIN_EVENTSINK_MAP(CIEAutomationView, CHtmlView)
	ON_EVENT(CHtmlView, AFX_IDW_PANE_FIRST, DISPID_DOCUMENTCOMPLETE, DocumentComplete, VTS_DISPATCH VTS_PVARIANT)
	ON_EVENT(CHtmlView, AFX_IDW_PANE_FIRST, DISPID_WINDOWCLOSING, WindowClosing,VTS_BOOL VTS_PBOOL)
	ON_EVENT(CHtmlView, AFX_IDW_PANE_FIRST, 250 /* BeforeNavigate2 */, BeforeNavigate2, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
END_EVENTSINK_MAP()

// Note: we add support for IID_IIEAutomationView to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {31BB0017-183E-4BB6-AF90-907EFE4CEA01}
static const IID IID_IIEAutomationView =
{ 0x31bb0017, 0x183e, 0x4bb6, { 0xaf, 0x90, 0x90, 0x7e, 0xfe, 0x4c, 0xea, 0x1 } };

// {31BB0017-183E-4BB6-AF90-907EFE4CEA01}
static const IID CLSID_IIEAutomationView =
{ 0x31bb0017, 0x183e, 0x4bb6, { 0xaf, 0x90, 0x90, 0x7e, 0xfe, 0x4c, 0xea, 0x1 } };

IMPLEMENT_OLETYPELIB(CIEAutomationView, _tlid, _wVerMajor, _wVerMinor)
BOOL CIEAutomationView::GetDispatchIID(IID* piid)
{
	*piid = IID_IIEAutomationView ;

	return TRUE;
}

BEGIN_INTERFACE_MAP(CIEAutomationView, CHtmlView)
	INTERFACE_PART(CIEAutomationView, IID_IActiveScriptSite, ActiveScriptSite)
	INTERFACE_PART(CIEAutomationView, IID_IActiveScriptSiteWindow, ActiveScriptSiteWindow)
	INTERFACE_PART(CIEAutomationView, IID_IIEAutomationView, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIEAutomationView diagnostics

#ifdef _DEBUG
void CIEAutomationView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CIEAutomationView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
CIEAutomationDoc* CIEAutomationView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIEAutomationDoc)));
	return (CIEAutomationDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// IActiveScriptSite Implementation

STDMETHODIMP_(ULONG)CIEAutomationView::XActiveScriptSite::AddRef()
{
    METHOD_PROLOGUE(CIEAutomationView, ActiveScriptSite)
    return pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG)CIEAutomationView::XActiveScriptSite::Release()
{
    METHOD_PROLOGUE(CIEAutomationView, ActiveScriptSite)
    return pThis->ExternalRelease();
}

STDMETHODIMP CIEAutomationView::XActiveScriptSite::QueryInterface(
    REFIID iid, void FAR* FAR* ppvObj)
{
    METHOD_PROLOGUE(CIEAutomationView, ActiveScriptSite)
    return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CIEAutomationView::XActiveScriptSite::GetLCID(LCID* plcid)
{
    METHOD_PROLOGUE(CIEAutomationView, ActiveScriptSite)

	return E_NOTIMPL;
}

STDMETHODIMP CIEAutomationView::XActiveScriptSite::GetItemInfo(
            /* [in] */ LPCOLESTR pstrName,
            /* [in] */ DWORD dwReturnMask,
            /* [out] */LPUNKNOWN* ppiunkItem,
            /* [out] */LPTYPEINFO* ppti)
{
	HRESULT hr = S_OK; 

    METHOD_PROLOGUE(CIEAutomationView, ActiveScriptSite)
	USES_CONVERSION;

	TRACE("GetItemInfo: Name = %s Mask = %x\n", OLE2CT(pstrName), dwReturnMask);

	if (dwReturnMask & SCRIPTINFO_ITYPEINFO)
	{
		if (!ppti)
			return E_INVALIDARG;
		*ppti = NULL;
	}

	if (dwReturnMask & SCRIPTINFO_IUNKNOWN)
	{
		if (!ppiunkItem)
			return E_INVALIDARG;
		*ppiunkItem = NULL;
	}

	// Global object
	if (!_wcsicmp(L"Scripter", pstrName))
	{
		if (dwReturnMask & SCRIPTINFO_ITYPEINFO)
		{
			hr=pThis->GetTypeInfoOfGuid(0 /* lcid unknown! */, CLSID_IIEAutomationView, ppti);
			if(SUCCEEDED(hr))
				(*ppti)->AddRef();      // because returning
		}
		
		if (dwReturnMask & SCRIPTINFO_IUNKNOWN)
		{
			*ppiunkItem = pThis->GetIDispatch(TRUE);
			(*ppiunkItem)->AddRef();    // because returning
		}
		return S_OK;
	}

	// Check map for named item
	CCmdTarget* pdispimpl = NULL;
	if (!(pThis->mapNamedItems.Lookup(OLE2CT(pstrName), (CObject*&) pdispimpl)))
	{
		return TYPE_E_ELEMENTNOTFOUND;
	}

	LPUNKNOWN pUnk = NULL;
	ASSERT(NULL != pdispimpl);
	if (pdispimpl->IsKindOf(RUNTIME_CLASS(CCmdTargetPlus)))
		// Automation class
		pUnk = pdispimpl->GetIDispatch(TRUE);
	else
		// Wrapper Class probably
		pUnk = ((CWnd*)pdispimpl)->GetControlUnknown();

	if (NULL == pUnk)
	{
		TRACE("No IUnknown for item!\n");
		// This shouldn't ever happen, since we're providing the items
		return E_FAIL;
	}

	if (dwReturnMask & SCRIPTINFO_ITYPEINFO)
	{
		// Use IProvideClassInfo to get ITypeInfo of coclass!
		IProvideClassInfo *pci = NULL;
		hr = pUnk->QueryInterface(IID_IProvideClassInfo, (void**)&pci);
		if (SUCCEEDED(hr) && pci)
			hr = pci->GetClassInfo(ppti);
		if (FAILED(hr))
			return E_FAIL;
	}
	
	if (dwReturnMask & SCRIPTINFO_IUNKNOWN)
	{
		*ppiunkItem = pUnk;
		(*ppiunkItem)->AddRef();    // because returning
	}

	return S_OK;
}

STDMETHODIMP CIEAutomationView::XActiveScriptSite::GetDocVersionString(LPBSTR pbstrVersion)
{
	METHOD_PROLOGUE(CIEAutomationView, ActiveScriptSite)

	return E_NOTIMPL;

}

STDMETHODIMP CIEAutomationView::XActiveScriptSite::OnScriptTerminate( 
            /* [in] */ const VARIANT* pvarResult,
            /* [in] */ const EXCEPINFO* pexcepinfo)
{
    METHOD_PROLOGUE(CIEAutomationView, ActiveScriptSite)

	return S_OK;

}

STDMETHODIMP CIEAutomationView::XActiveScriptSite::OnStateChange( 
            /* [in] */ SCRIPTSTATE ssScriptState)
{
    METHOD_PROLOGUE(CIEAutomationView, ActiveScriptSite)

	return S_OK;
}

STDMETHODIMP CIEAutomationView::XActiveScriptSite::OnScriptError( 
            /* [in] */ IActiveScriptError* pse)
{
	METHOD_PROLOGUE_EX(CIEAutomationView, ActiveScriptSite)

	EXCEPINFO ei;
	DWORD     dwSrcContext;
	ULONG     ulLine;
	LONG      ichError;
	BSTR      bstrLine = NULL;
	CString strError;

	pse->GetExceptionInfo(&ei);
	pse->GetSourcePosition(&dwSrcContext, &ulLine, &ichError);
	pse->GetSourceLineText(&bstrLine);
	
	CString desc;
	CString src;

	desc = (LPCWSTR)ei.bstrDescription;
	src = (LPCWSTR)ei.bstrSource;

	strError.Format("%s\nSrc: %s\nLine:%d Error:%d Scode:%x", desc, src, ulLine, (int)ei.wCode, ei.scode);
	AfxMessageBox(strError);

	TRACE(strError);
	TRACE("\n");

	return S_OK;
}

STDMETHODIMP CIEAutomationView::XActiveScriptSite::OnEnterScript()
{
    METHOD_PROLOGUE(CIEAutomationView, ActiveScriptSite)
	TRACE("OnEnterScript\n");

	return S_OK;
}

STDMETHODIMP CIEAutomationView::XActiveScriptSite::OnLeaveScript()
{
    METHOD_PROLOGUE(CIEAutomationView, ActiveScriptSite)
	TRACE("OnLeaveScript\n");

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// IActiveScriptSiteWindow Implementation

STDMETHODIMP_(ULONG) CIEAutomationView::XActiveScriptSiteWindow::AddRef()
{
    METHOD_PROLOGUE_EX_(CIEAutomationView, ActiveScriptSiteWindow)
    return pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) CIEAutomationView::XActiveScriptSiteWindow::Release()
{
    METHOD_PROLOGUE_EX_(CIEAutomationView, ActiveScriptSiteWindow)
    return pThis->ExternalRelease();
}

STDMETHODIMP CIEAutomationView::XActiveScriptSiteWindow::QueryInterface(REFIID iid, LPVOID* ppvObj)
{
    METHOD_PROLOGUE_EX_(CIEAutomationView, ActiveScriptSiteWindow)
    return pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CIEAutomationView::XActiveScriptSiteWindow::EnableModeless(BOOL fEnable)
{
    METHOD_PROLOGUE_EX_(CIEAutomationView, ActiveScriptSiteWindow)
	
	CWinApp* pApp = AfxGetApp();
	if (!pApp)
		return E_FAIL;

	pApp->EnableModeless(fEnable);
	return S_OK;
}

STDMETHODIMP CIEAutomationView::XActiveScriptSiteWindow::GetWindow(HWND* phWnd)
{
    METHOD_PROLOGUE_EX_(CIEAutomationView, ActiveScriptSiteWindow)

	if (!(pThis))
		return E_FAIL;

	*phWnd = pThis->GetSafeHwnd();
	if (*phWnd)
		return S_OK;
	else
		return E_FAIL;
}
/////////////////////////////////////////////////////////////////////////////
// CIEAutomationView message handlers

BOOL CIEAutomationView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	//Knowledge Base  
	//FIX: CHtmlView Does Not Repaint Correctly in an AppWizard-Generated MDI Application	cs.style |= WS_CLIPCHILDREN;
	cs.style |= WS_CLIPCHILDREN;
	return CHtmlView::PreCreateWindow(cs);
}

void CIEAutomationView::OnInitialUpdate() 
{
	//TODO: This code navigates to a popular spot on the web.
	//Change the code to go where you'd like.
	GoHome();
}
//////////////////////////////////////////////////
//Knowledge Base  
//BUG: CHtmlView Leaks Memory by Not Releasing BSTRs in Several Methods
// Implementation
//new code
CString CIEAutomationView::GetFullName() const
{
    ASSERT(m_pBrowserApp != NULL);

    BSTR bstr;
    m_pBrowserApp->get_FullName(&bstr);
    CString retVal(bstr);

    SysFreeString(bstr); // Added this line to prevent leak.
    return retVal;
}

CString CIEAutomationView::GetType() const
{
    ASSERT(m_pBrowserApp != NULL);

    BSTR bstr;
    m_pBrowserApp->get_Type(&bstr);

    CString retVal(bstr);

    SysFreeString(bstr); // Added this line to prevent leak.
    return retVal;
}

CString CIEAutomationView::GetLocationName() const
{
    ASSERT(m_pBrowserApp != NULL);

    BSTR bstr;
    m_pBrowserApp->get_LocationName(&bstr);
    CString retVal(bstr);

    SysFreeString(bstr); // Added this line to prevent leak.
    return retVal;
}
 
CString CIEAutomationView::GetLocationURL() const
{
    ASSERT(m_pBrowserApp != NULL);

    BSTR bstr;
    m_pBrowserApp->get_LocationURL(&bstr);
    CString retVal(bstr);

    SysFreeString(bstr); // Added this line to prevent leak.
    return retVal;
}

void CIEAutomationView::Navigate(LPCTSTR lpszURL, DWORD dwFlags /* = 0 */,
	LPCTSTR lpszTargetFrameName /* = NULL */ ,
	LPCTSTR lpszHeaders /* = NULL */, LPVOID lpvPostData /* = NULL */,
	DWORD dwPostDataLen /* = 0 */)
{
    CString strURL(lpszURL);
    BSTR bstrURL = strURL.AllocSysString();

    COleSafeArray vPostData;
    if (lpvPostData != NULL)
    {
       if (dwPostDataLen == 0)
          dwPostDataLen = lstrlen((LPCTSTR) lpvPostData);

       vPostData.CreateOneDim(VT_UI1, dwPostDataLen, lpvPostData);
    }

    m_pBrowserApp->Navigate(bstrURL, COleVariant((long) dwFlags, VT_I4), COleVariant(lpszTargetFrameName, VT_BSTR), 
                            vPostData, COleVariant(lpszHeaders, VT_BSTR));

    SysFreeString(bstrURL); // Added this line to prevent leak.
}


BOOL CIEAutomationView::LoadFromResource(LPCTSTR lpszResource)
{
    HINSTANCE hInstance = AfxGetResourceHandle();
    ASSERT(hInstance != NULL);

    CString strResourceURL;
    BOOL bRetVal = TRUE;
    LPTSTR lpszModule = new TCHAR[_MAX_PATH];

    if (GetModuleFileName(hInstance, lpszModule, _MAX_PATH))
    {
       strResourceURL.Format(_T("res://%s/%s"), lpszModule, lpszResource);
       Navigate(strResourceURL, 0, 0, 0);
    }
    else
       bRetVal = FALSE;

    delete [] lpszModule;
    return bRetVal;
}

BOOL CIEAutomationView::LoadFromResource(UINT nRes)
{
    HINSTANCE hInstance = AfxGetResourceHandle();
    ASSERT(hInstance != NULL);

    CString strResourceURL;
    BOOL bRetVal = TRUE;
    LPTSTR lpszModule = new TCHAR[_MAX_PATH];

    if (GetModuleFileName(hInstance, lpszModule, _MAX_PATH))
    {
       strResourceURL.Format(_T("res://%s/%d"), lpszModule, nRes);
       Navigate(strResourceURL, 0, 0, 0);
    }
    else
       bRetVal = FALSE;

    delete [] lpszModule;
    return bRetVal;
}
//end new code
//////////////////////////////////////////////////
//Knowledge Base  
//PRB: WebBrowser Control Disappears When Script Calls window.close()
//////////////////////////////
//new code
void CIEAutomationView::OnParentNotify(UINT message, LPARAM lParam )  
{ 	
	if ((LOWORD(message) == WM_DESTROY) && ((HWND)lParam == m_wndBrowser)) 
	{ 	
		// Close the parent frame window.
		GetParentFrame()->PostMessage(WM_CLOSE, 0, 0); 
	} 	
	else 		
		CHtmlView::OnParentNotify(message, lParam ); 
}
void CIEAutomationView::WindowClosing(VARIANT_BOOL IsChildWindow,
		VARIANT_BOOL *Cancel)
{
	if(!IsChildWindow){
		if(Cancel)
			*Cancel=VARIANT_TRUE;
		if(AfxMessageBox(IDS_WINDOW_CLOSING,MB_YESNO)==IDYES)
			GetParentFrame()->PostMessage(WM_CLOSE, 0, 0); 
	}
}
//end new code
//////////////////////////////
BOOL CIEAutomationView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	// create the view window itself
	m_pCreateContext = pContext;

	if (!CView::Create(lpszClassName, lpszWindowName,
				dwStyle, rect, pParentWnd,  nID, pContext))
	{
		return FALSE;
	}

	RECT rectClient;
	GetClientRect(&rectClient);

	// create the control window
	// AFX_IDW_PANE_FIRST is a safe but arbitrary ID
	if (!m_wndBrowser.CreateControl(CLSID_WebBrowser, lpszWindowName,
				WS_VISIBLE | WS_CHILD, rectClient, this, AFX_IDW_PANE_FIRST))
	{
		DestroyWindow();
		return FALSE;
	}

	LPUNKNOWN lpUnk = m_wndBrowser.GetControlUnknown();
	HRESULT hr = lpUnk->QueryInterface(IID_IWebBrowser2, (void**) &m_pBrowserApp);
	if (!SUCCEEDED(hr))
	{
		m_pBrowserApp = NULL;
		m_wndBrowser.DestroyWindow();
		DestroyWindow();
		return FALSE;
	}

	return TRUE;
	
	//return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
// * CImpIDocHostUIHandler::GetHostInfo
// *
// * Purpose: Called at initialization
// *
HRESULT FAR EXPORT  CIEAutomationView::GetHostInfo( DOCHOSTUIINFO* pInfo )
{
	pInfo->dwFlags = m_dwHostFlags;
	
	//pInfo->dwFlags = DOCHOSTUIFLAG_NO3DBORDER;
	pInfo->dwDoubleClick = DOCHOSTUIDBLCLK_DEFAULT;
    return S_OK;
}

// * CImpIDocHostUIHandler::ShowUI
// *
// * Purpose: Called when MSHTML.DLL shows its UI
// *
HRESULT FAR EXPORT  CIEAutomationView::ShowUI(
				DWORD dwID, 
				IOleInPlaceActiveObject * pActiveObject,
				IOleCommandTarget * pCommandTarget,
				IOleInPlaceFrame * pFrame,
				IOleInPlaceUIWindow * pDoc)
{

	
	// We've already got our own UI in place so just return S_OK
	return S_OK;
}

// * CImpIDocHostUIHandler::HideUI
// *
// * Purpose: Called when MSHTML.DLL hides its UI
// *
HRESULT FAR EXPORT  CIEAutomationView::HideUI(void)
{
	return S_OK;
}

// * CImpIDocHostUIHandler::UpdateUI
// *
// * Purpose: Called when MSHTML.DLL updates its UI
// *
HRESULT FAR EXPORT  CIEAutomationView::UpdateUI(void)
{
	
	// MFC is pretty good about updating it's UI in it's Idle loop so I don't do anything here
	return S_OK;
}

// * CImpIDocHostUIHandler::EnableModeless
// *
// * Purpose: Called from MSHTML.DLL's IOleInPlaceActiveObject::EnableModeless
// *
HRESULT FAR EXPORT  CIEAutomationView::EnableModeless(BOOL fEnable)
{
    return E_NOTIMPL;
}

// * CImpIDocHostUIHandler::OnDocWindowActivate
// *
// * Purpose: Called from MSHTML.DLL's IOleInPlaceActiveObject::OnDocWindowActivate
// *
HRESULT FAR EXPORT  CIEAutomationView::OnDocWindowActivate(BOOL fActivate)
{
    return E_NOTIMPL;
}

// * CImpIDocHostUIHandler::OnFrameWindowActivate
// *
// * Purpose: Called from MSHTML.DLL's IOleInPlaceActiveObject::OnFrameWindowActivate
// *
HRESULT FAR EXPORT  CIEAutomationView::OnFrameWindowActivate(BOOL fActivate)
{
    return E_NOTIMPL;
}

// * CImpIDocHostUIHandler::ResizeBorder
// *
// * Purpose: Called from MSHTML.DLL's IOleInPlaceActiveObject::ResizeBorder
// *
HRESULT FAR EXPORT  CIEAutomationView::ResizeBorder(
				LPCRECT prcBorder, 
				IOleInPlaceUIWindow* pUIWindow,
				BOOL fRameWindow)
{
    return E_NOTIMPL;
}

// * CImpIDocHostUIHandler::ShowContextMenu
// *
// * Purpose: Called when MSHTML.DLL would normally display its context menu
// *
HRESULT FAR EXPORT  CIEAutomationView::ShowContextMenu(
				DWORD dwID, 
				POINT* pptPosition,
				IUnknown* pCommandTarget,
				IDispatch* pDispatchObjectHit)
{
    //return S_OK; // We've shown our own context menu. MSHTML.DLL will no longer try to show its own.
	return E_NOTIMPL;
}

// * CImpIDocHostUIHandler::TranslateAccelerator
// *
// * Purpose: Called from MSHTML.DLL's TranslateAccelerator routines
// *
HRESULT FAR EXPORT  CIEAutomationView::TranslateAccelerator(LPMSG lpMsg,
            /* [in] */ const GUID __RPC_FAR *pguidCmdGroup,
            /* [in] */ DWORD nCmdID)
{
	
	return S_FALSE;
}

// * CImpIDocHostUIHandler::GetOptionKeyPath
// *
// * Purpose: Called by MSHTML.DLL to find where the host wishes to store 
// *	its options in the registry
// *
HRESULT FAR EXPORT  CIEAutomationView::GetOptionKeyPath(BSTR* pbstrKey, DWORD	dw)
{
	return E_NOTIMPL;
}

STDMETHODIMP CIEAutomationView::GetDropTarget( 
            /* [in] */ IDropTarget __RPC_FAR *pDropTarget,
            /* [out] */ IDropTarget __RPC_FAR *__RPC_FAR *ppDropTarget)
{
	return E_NOTIMPL;
}


STDMETHODIMP CIEAutomationView::GetExternal( 
            /* [out] */ IDispatch __RPC_FAR *__RPC_FAR *ppDispatch)
{
	// return the IDispatch we have for extending the object Model
	if(m_spExternalDisp.p){
		*ppDispatch=m_spExternalDisp.p;
		m_spExternalDisp.p->AddRef();
		return S_OK;
	}
	else{
		if(m_domExternal.m_pShellUIHelper){
			*ppDispatch=m_domExternal.GetIDispatch(TRUE);
			return S_OK;
		}
	}
	return E_NOTIMPL;
}
        
STDMETHODIMP CIEAutomationView::TranslateUrl( 
            /* [in] */ DWORD dwTranslate,
            /* [in] */ OLECHAR __RPC_FAR *pchURLIn,
            /* [out] */ OLECHAR __RPC_FAR *__RPC_FAR *ppchURLOut)
{
	
	return E_NOTIMPL;

}
        
STDMETHODIMP CIEAutomationView::FilterDataObject( 
            /* [in] */ IDataObject __RPC_FAR *pDO,
            /* [out] */ IDataObject __RPC_FAR *__RPC_FAR *ppDORet)
{
	return E_NOTIMPL;
}
STDMETHODIMP CIEAutomationView::GetOverrideKeyPath( 
					/* [out] */ LPOLESTR *pchKey,
					/* [in] */ DWORD dw)
{
	return E_NOTIMPL;
#pragma message(__LOC__"Uncomment this if u need override default IE Setting?\r\n") 
	/*
	HRESULT hr;
	#define CCHMAX 256
	size_t cchLength;
	if (pchKey){
		WCHAR* szMyKey = L"Software\\IEAutoMation\\IESettings";
		hr = StringCchLengthW(szMyKey, CCHMAX, &cchLength);
		if(FAILED(hr))return hr;
		//TODO: Hanlde errors here
		*pchKey = (LPOLESTR)CoTaskMemAlloc((cchLength + 1) * sizeof(WCHAR));
		if (*pchKey)
			hr = StringCchCopyW(*pchKey, cchLength + 1, szMyKey );
			//TODO: Hanlde errors here
		hr = (*pchKey) ? S_OK : E_OUTOFMEMORY;
		if(FAILED(hr)){
			if(*pchKey)
				CoTaskMemFree(*pchKey);
			return hr;
		}
	}
	else
		hr = E_INVALIDARG;
	return hr;*/
}

BOOL CIEAutomationView::CanAccessExternal()
{
	// if the dispatch we have is safe, 
	// we allow access
	if (IsExternalDispatchSafe())
		return TRUE;

	// the external dispatch is not safe, so we check
	// whether the current zone allows for scripting
	// of objects that are not safe for scripting
	if (m_spHtmlDoc == NULL)
		return FALSE;

	CComPtr<IInternetHostSecurityManager> spSecMan;
	m_spHtmlDoc->QueryInterface(IID_IInternetHostSecurityManager,
			(void **) &spSecMan);
	if (spSecMan == NULL)
		return FALSE;

	HRESULT hr = spSecMan->ProcessUrlAction(URLACTION_ACTIVEX_OVERRIDE_OBJECT_SAFETY,
		NULL, 0, NULL, 0, 0, PUAF_DEFAULT);
	if (hr == S_OK)
		return TRUE;
	return FALSE;
}

BOOL CIEAutomationView::IsExternalDispatchSafe()
{
	return FALSE;
}
void CIEAutomationView::SetExternalDispatch(IDispatch *pdispExternal)
{
	m_spExternalDisp = pdispExternal;
}
void CIEAutomationView::SetHostFlags(DWORD dwFlags)
{
	m_dwHostFlags = dwFlags;
}

//override to handle document complete for frames
//////////////////////////////////////////////
void CIEAutomationView::DocumentComplete(LPDISPATCH pDisp, VARIANT* URL)
{
	//UNUSED_ALWAYS(pDisp);
	ASSERT(V_VT(URL) == VT_BSTR);

	CString str(V_BSTR(URL));
	OnDocumentComplete(pDisp, str);
}
void CIEAutomationView::OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR lpszUrl)
{
	// make sure the main frame has the new URL.  This call also stops the animation
	((CChildFrame*)GetParentFrame())->SetAddress(lpszUrl);
	CString strURL(lpszUrl);	
	IUnknown* pUnkBrowser = NULL;
    IUnknown* pUnkDisp = NULL;
	TRACE(_T("Document %s Done\r\n"),lpszUrl);
	//is it top level document?
	HRESULT hr = m_pBrowserApp->QueryInterface( IID_IUnknown,  (void**)&pUnkBrowser);
	if ( SUCCEEDED(hr) ){
        hr = pDisp->QueryInterface( IID_IUnknown,  (void**)&pUnkDisp );
        if ( SUCCEEDED(hr) ){
            if ( pUnkBrowser == pUnkDisp ){
				//top level
				m_spHtmlDoc=NULL;
				hr=m_pBrowserApp->get_Document(&m_spHtmlDoc);
				TRACE(_T("Downloading Complete\r\n"),lpszUrl);
			}
			pUnkDisp->Release();
		}
		pUnkBrowser->Release();

	}
}

void CIEAutomationView::OnBeforeNavigate2(LPCTSTR /*lpszURL*/, DWORD /*nFlags*/,
	LPCTSTR /*lpszTargetFrameName*/, CByteArray& /*baPostedData*/,
	LPCTSTR /*lpszHeaders*/, BOOL* /*pbCancel*/)
{
	// start the animation so that is plays while the new page is being loaded
	((CChildFrame*)GetParentFrame())->StartAnimation();
	m_spHtmlDoc=NULL;
}
// This demonstrates how we can use the Navigate2() function to load local files
// including local HTML pages, GIFs, AIFF files, etc.
void CIEAutomationView::OnFileOpen() 
{
	// TODO: Add your command handler code here
	CString str;

	str.LoadString(IDS_FILETYPES);

	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, str);

	if(fileDlg.DoModal() == IDOK)
		Navigate2(fileDlg.GetPathName(), 0, NULL);
}

void CIEAutomationView::OnNewWindow2(LPDISPATCH* ppDisp, BOOL* Cancel) 
{
  // Get a pointer to the application object.
   CWinApp* pApp = AfxGetApp();

   // Get the correct document template.
   POSITION pos = pApp->GetFirstDocTemplatePosition();
   CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate( pos );

   // Create a new frame.
   CFrameWnd* pFrame = pDocTemplate->CreateNewFrame(
                                          GetDocument(),
                                          (CFrameWnd*)AfxGetMainWnd() );

   // Activate the frame.
   pDocTemplate->InitialUpdateFrame( pFrame, NULL );
   CIEAutomationView* pView = (CIEAutomationView*)pFrame->GetActiveView();

   // Pass pointer of WebBrowser object.
   pView->SetRegisterAsBrowser( TRUE );
   *ppDisp = pView->GetApplication();   
}
void CIEAutomationView::OnTitleChange(LPCTSTR lpszText)
{
	// this will change the main frame's title bar
	if (m_pDocument != NULL)
		m_pDocument->SetTitle(lpszText);
}
//////////////////////////////
// these are all simple one-liners to do simple controlling of the browser
void CIEAutomationView::OnGoBack()
{
	GoBack();
}

void CIEAutomationView::OnGoForward()
{
	GoForward();
}

void CIEAutomationView::OnGoSearchTheWeb()
{
	GoSearch();
}

void CIEAutomationView::OnGoStartPage()
{
	GoHome();
}

void CIEAutomationView::OnViewStop()
{
	Stop();
}

void CIEAutomationView::OnViewRefresh()
{
	Refresh();
}
// these functions control the font size.  There is no explicit command in the
// CHtmlView class to do this, but we can do it by using the ExecWB() function.
void CIEAutomationView::OnViewFontsLargest()
{
	COleVariant vaZoomFactor(4l);

	ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER,
		   &vaZoomFactor, NULL);
}

void CIEAutomationView::OnViewFontsLarge()
{
	COleVariant vaZoomFactor(3l);

	ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER,
		   &vaZoomFactor, NULL);
}

void CIEAutomationView::OnViewFontsMedium()
{
	COleVariant vaZoomFactor(2l);

	ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER,
		   &vaZoomFactor, NULL);
}

void CIEAutomationView::OnViewFontsSmall()
{
	COleVariant vaZoomFactor(1l);

	ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER,
		   &vaZoomFactor, NULL);
}

void CIEAutomationView::OnViewFontsSmallest()
{
	COleVariant vaZoomFactor(0l);

	ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER,
		   &vaZoomFactor, NULL);
}


void CIEAutomationView::OnDestroy() 
{
	m_spHtmlDoc=NULL;
	CHtmlView::OnDestroy();
}

BOOL CIEAutomationView::OnAmbientProperty(COleControlSite* pSite, DISPID dispid, VARIANT* pvar) 
{
	USES_CONVERSION;
//Knowledge Base  
//PRB: Session ID Is Lost When You Close a Browser Window
	if (dispid == DISPID_AMBIENT_OFFLINEIFNOTCONNECTED)
	{
		pvar->vt = VT_BOOL;
		pvar->boolVal = VARIANT_TRUE;

		return TRUE;
	}
//offline support
	if (dispid == DISPID_AMBIENT_DLCONTROL)
	{
		pvar->vt = VT_I4;
		if(m_bOffline)
			pvar->lVal |= DLCTL_FORCEOFFLINE;
		else
			pvar->lVal &= ~DLCTL_FORCEOFFLINE;
		return TRUE;
	} 
	return CHtmlView::OnAmbientProperty(pSite, dispid, pvar);
}
void	CIEAutomationView::ExecWBCommand(
	const GUID *pguidCmdGroup,  // Pointer to command group
	DWORD nCmdID,               // Identifier of command to execute
	DWORD nCmdExecOpt,          // Options for executing the command
	VARIANTARG *pvaIn,          // Pointer to input arguments
	VARIANTARG *pvaOut          // Pointer to command output
)
{
	if(m_pBrowserApp==NULL)return;
	CComQIPtr<IWebBrowser2>	pBrowserApp;
	pBrowserApp.p=m_pBrowserApp;
	m_pBrowserApp->AddRef();
	CComQIPtr<IOleCommandTarget> pCT(pBrowserApp);
	if(pCT){
		pCT->Exec(
			pguidCmdGroup,  // Pointer to command group
			nCmdID,               // Identifier of command to execute
			nCmdExecOpt,          // Options for executing the command
			pvaIn,          // Pointer to input arguments
			pvaOut          // Pointer to command output
		);
	}
}

void CIEAutomationView::OnEditFind() 
{
	ExecWBCommand(&CGID_WebBrowser,HTMLID_FIND,0,NULL,NULL);	
}

void CIEAutomationView::OnViewSource() 
{
	//ExecWBCommand(&CGID_WebBrowser,HTMLID_VIEWSOURCE,0,NULL,NULL);	
	if(m_spHtmlDoc==NULL)return;
	CComQIPtr<IHTMLDocument2>	pHtmlDoc(m_spHtmlDoc);
	CDlgDocOutline	DlgDocOutline;
	DlgDocOutline.m_pHtmlDoc=pHtmlDoc;
	DlgDocOutline.DoModal();
}

void CIEAutomationView::OnViewOption() 
{
	ExecWBCommand(&CGID_WebBrowser,HTMLID_OPTIONS,0,NULL,NULL);
/*
	typedef BOOL (WINAPI *LAUNCHCPL) (HWND);
	HMODULE hInetcpl;
	HWND hParent;
	LAUNCHCPL cpl;

	hInetcpl = LoadLibrary("inetcpl.cpl");

	if (hInetcpl == NULL)
	{
	   // Error loading module -- fail as securely as possible
	   return;
	}

	cpl = (LAUNCHCPL)GetProcAddress(hInetcpl, "LaunchInternetControlPanel");
	if(cpl)
	{
	  cpl(hParent);
	}
	FreeLibrary(hInetcpl);	*/
}
void CIEAutomationView::CallScriptDirect(IHTMLWindow2* pWindow2
		,BSTR bstrMethod)
{
	DISPID dispidScriptObject, dispidMethodToInvoke;
	IDispatch *pDispScriptObject;
	DISPPARAMS dispparamsNoArgs = { NULL, NULL, 0, 0};
	HRESULT hr;
	OLECHAR *szScript = L"script";
	OLECHAR *szFuncName = bstrMethod;
	
	IHTMLDocument2* pDocument2 = NULL;
	hr = pWindow2->get_document(&pDocument2);
	ASSERT(SUCCEEDED(hr) && pDocument2);

	IDispatch* pDocDisp = NULL;

	hr = pDocument2->QueryInterface(IID_IDispatch, (void**)&pDocDisp);
	ASSERT(SUCCEEDED(hr) && pDocDisp);

	// Get the DISPID of the document's Script property.
	hr = pDocDisp->GetIDsOfNames(IID_NULL, &szScript,
		1, LOCALE_USER_DEFAULT, &dispidScriptObject);
	ASSERT(SUCCEEDED(hr));

	COleVariant varResult;
	
	// Get the document's Script property.
	hr = pDocDisp->Invoke(dispidScriptObject,
		IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET,
		&dispparamsNoArgs, &varResult, NULL, NULL);
	ASSERT(SUCCEEDED(hr) && varResult.pdispVal);

	pDispScriptObject = varResult.pdispVal;

	varResult.Clear();	
	// Get the DISPID of the script method to invoke.
	hr = pDispScriptObject->GetIDsOfNames(IID_NULL, &szFuncName, 1, 
		LOCALE_USER_DEFAULT, &dispidMethodToInvoke);
	ASSERT(SUCCEEDED(hr));

	// Invoke the script method.
	hr = pDispScriptObject->Invoke(dispidMethodToInvoke, IID_NULL,
		LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparamsNoArgs, 
		&varResult, NULL, NULL);
	ASSERT(SUCCEEDED(hr));

	pDocDisp->Release();
	pDocument2->Release();

}
void CIEAutomationView::CallScript(IHTMLWindow2* pWindow2
		,BSTR bstrCode
		,BSTR bstrLanguage)
{
	ASSERT(pWindow2);
	HRESULT hr = S_OK;
	COleVariant ret;
	hr = pWindow2->execScript(bstrCode, bstrLanguage, &ret);
}

void CIEAutomationView::OnFileOffline() 
{
	m_bOffline=!m_bOffline;	
}

void CIEAutomationView::OnUpdateFileOffline(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bOffline);
	
}
// If a script is currently running, stop it
// else, start a script running based on the text in the edit control
void CIEAutomationView::RunScript() 
{
	CString strScriptText;
	EXCEPINFO    ei;
	HRESULT hr;

	if (m_bScriptIsRunning){
		// Close Script
		hr = m_axs->Close();
		do{
			if (FAILED(hr))	{
				TRACE("Attempt to Close AXS failed\n");
				break;
			}
			// NOTE: Handle OLESCRIPT_S_PENDING
			// The documentation reports that if you get this return value,
			// you'll need to wait until IActiveScriptSite::OnStateChange before
			// finishing the Close. However, no current script engine implementation
			// will do this and this return value is not defined in any headers currently
			// (This is true as of IE 3.01)

			if (m_axsp)	{
				m_axsp->Release();
				m_axsp = NULL;
			}
			if (m_axs)	{
				m_axs->Close();
				m_axs->Release();
				m_axs = NULL;
			}

			m_bScriptIsRunning = FALSE;
			return;
		}
		while(FALSE);
#ifndef DEBUG
		AfxMessageBox("Attempt to stop running script failed!\n");
#endif
	}
	else
	{
		if (!CreateEngine())
		{
#ifndef DEBUG
			AfxMessageBox("Failed to create script engine. Cannot execute script.");
#endif
			TRACE("Failed to create script engine. Cannot execute script.");
			return;
		}

		// Get text from edit control and PARSE IT DUDE!!
		strScriptText=GetDocument()->m_strScript;

		BSTR bstrParseMe = strScriptText.AllocSysString();

		m_axsp->ParseScriptText(bstrParseMe, L"Scripter", NULL, NULL, 0, 0, 0L, NULL, &ei);

		// Start the script running...
		hr = m_axs->SetScriptState(SCRIPTSTATE_CONNECTED);
		if (FAILED(hr))
		{
			TRACE("SetScriptState to CONNECTED failed\n");
		}

		// Change Button Text to "&Stop Script"
		//m_wndRunScript.SetWindowText("&Stop Script");
		m_bScriptIsRunning = TRUE;
	}
}
/////////////////////////////////////////////////////////////////////////////
// CIEAutomationView Faked CreateObject implementation

LPDISPATCH CIEAutomationView::HostCreateObject(const VARIANT FAR& bstrProgID) 
{
	USES_CONVERSION;

	HRESULT hr = S_OK;
	LPDISPATCH retval = NULL;
	TRACE( "Calling HostCreateObject()\n" );

	if (VT_BSTR != bstrProgID.vt)
	{
		AfxThrowOleException(E_INVALIDARG);
	}

	CLSID clsidObject;
	hr = CLSIDFromProgID( (LPCOLESTR)bstrProgID.pbstrVal, &clsidObject );
	if( SUCCEEDED( hr ) )
	{
		hr = CoCreateInstance( clsidObject, NULL, CLSCTX_LOCAL_SERVER, IID_IDispatch, (void**)&retval );
		if (FAILED(hr))
		{
			CString strProgID;
			strProgID = (BSTR)bstrProgID.pbstrVal;
			TRACE("Failed to create object '%s'.\nReturn = 0x%x\n",strProgID,hr);
			AfxThrowOleException(E_FAIL);
		}
	}
	else
	{
		CString strProgID;
		strProgID = (BSTR)bstrProgID.pbstrVal;
		TRACE("Invalid ProgID '%s'\n",strProgID);
		AfxThrowOleException(E_INVALIDARG);
	}

	return retval;
}

BOOL CIEAutomationView::CreateEngine()
{
	HRESULT hr;

	// XX ActiveX Scripting XX 
	hr = CoCreateInstance(CLSID_VBScript, NULL, CLSCTX_INPROC_SERVER, IID_IActiveScript, (void **)&m_axs);
	if (FAILED(hr))
		// If this happens, the scripting engine is probably not properly registered
		return FALSE;
	// Script Engine must support IActiveScriptParse for us to use it
	hr = m_axs->QueryInterface(IID_IActiveScriptParse, (void **)&m_axsp);
	if (FAILED(hr))
		goto Failure;
	hr = m_axs->SetScriptSite(&m_xActiveScriptSite); // Our MFC-OLE interface implementation
	if (FAILED(hr))
		goto Failure;
	// InitNew the object:
	hr = m_axsp->InitNew();
	if (FAILED(hr))
		goto Failure;

	mapNamedItems[_T("Scripter")] = this;
	// Add Top-level Global Named Item
	hr = m_axs->AddNamedItem(L"Scripter", SCRIPTITEM_ISVISIBLE | SCRIPTITEM_ISSOURCE | SCRIPTITEM_GLOBALMEMBERS);
	if (FAILED(hr))
		TRACE("Couldn't add named item Scripter\n");

	// NOTE: Here's a tricky point in the scripting object model stuff
	// Technically, all a scripting host needs to do is expose the top level items
	// in its hierarchy. In our case, thats the Scripter object, since all of the following
	// are available as properties in the Scripter IDispatch interface.
	// We do not need to add these as named items for script code such as
	// 'LBox.AddString "Hi There"'
	// to work without doing 'Scripter.LBox.AddString "Hi There"'
	// However! script engines will only set up sinks for Named Items. So if you
	// want an item to be able to support event-handling such as
	// "Sub LBox_OnMouseOver"
	// you must add that item as a Named Item. In this case, everything on the dialog
	// generates events that we might want to script, so we add all items as named
	// items.
	// Web Browser Control
	mapNamedItems[_T("WebBrowser")] = &m_wndBrowser;
	hr = m_axs->AddNamedItem(L"WebBrowser", SCRIPTITEM_ISVISIBLE | SCRIPTITEM_ISSOURCE);
	if (FAILED(hr))
		TRACE("Couldn't add named item BroCon\n"); 
	//External
	mapNamedItems[_T("External")] = &m_domExternal;
	hr = m_axs->AddNamedItem(L"External", SCRIPTITEM_ISVISIBLE | SCRIPTITEM_ISSOURCE);
	if (FAILED(hr))
		TRACE("Couldn't add named item External\n");

	return TRUE;

Failure:
	if (m_axs)
		m_axs->Release();
	if (m_axsp)
		m_axsp->Release();

	// XX ActiveX Scripting XX
	return FALSE;
}

LPDISPATCH CIEAutomationView::GetWebBrowser() 
{
	// no dispatch pointer yet; find it now
	LPDISPATCH pDispatch = NULL;
	LPUNKNOWN pUnkControl = m_wndBrowser.GetControlUnknown();

	if ((pUnkControl != NULL) &&
		SUCCEEDED(pUnkControl->QueryInterface(IID_IDispatch, (LPVOID*)&pDispatch)))
	{
		ASSERT(pDispatch != NULL);
	}
	else
	{
		TRACE("Failed to obtain dispatch for Web Browser Control");
	}

	return pDispatch;
}


void CIEAutomationView::OnScriptEdit() 
{
	// TODO: Add your command handler code here
	CDlgInputText	dlg;
	dlg.m_strText=GetDocument()->m_strScript;

	if(dlg.DoModal()!=IDOK)return;

	GetDocument()->m_strScript=dlg.m_strText;
}

void CIEAutomationView::OnUpdateScriptEdit(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_bScriptIsRunning);	
}

void CIEAutomationView::OnScriptRun() 
{
	RunScript();	
}

void CIEAutomationView::OnUpdateScriptRun(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bScriptIsRunning);
	
}

void CIEAutomationView::OnFavoritesAdd() 
{
	CComQIPtr<IHTMLDocument2>	pHtmlDoc(m_spHtmlDoc);
	if(pHtmlDoc==NULL)
		return;
	CComBSTR bstrTitle,bstrURL;
	if(FAILED(pHtmlDoc->get_title(&bstrTitle)))return;
	if(FAILED(pHtmlDoc->get_URL(&bstrURL)))return;
	COleVariant	varTitle(CString(bstrTitle.m_str));
	m_domExternal.AddFavorite(CString(bstrURL),varTitle);
}

void CIEAutomationView::OnFavoritesManage() 
{
	COleVariant vMissing(DISP_E_PARAMNOTFOUND,VT_ERROR);
	m_domExternal.ShowBrowserUI(_T("OrganizeFavorites"),vMissing);	
}
//override to redirect BeforeNavigate2 event
void CIEAutomationView::BeforeNavigate2(LPDISPATCH  pDisp , VARIANT* URL,
		VARIANT* Flags, VARIANT* TargetFrameName,
		VARIANT* PostData, VARIANT* Headers, BOOL* Cancel)
{
	m_domExternal.FireBeforeNavigate2(pDisp , URL,
		Flags, TargetFrameName,
		PostData, Headers, Cancel);
	ASSERT(V_VT(URL) == VT_BSTR);
	ASSERT(V_VT(TargetFrameName) == VT_BSTR);
	ASSERT(V_VT(PostData) == (VT_VARIANT | VT_BYREF));
	ASSERT(V_VT(Headers) == VT_BSTR);
	ASSERT(Cancel != NULL);

	USES_CONVERSION;

	VARIANT* vtPostedData = V_VARIANTREF(PostData);
	CByteArray array;
	if (V_VT(vtPostedData) & VT_ARRAY)
	{
		// must be a vector of bytes
		ASSERT(vtPostedData->parray->cDims == 1 && vtPostedData->parray->cbElements == 1);

		vtPostedData->vt |= VT_UI1;
		COleSafeArray safe(vtPostedData);

		DWORD dwSize = safe.GetOneDimSize();
		LPVOID pVoid;
		safe.AccessData(&pVoid);

		array.SetSize(dwSize);
		LPBYTE lpByte = array.GetData();

		memcpy(lpByte, pVoid, dwSize);
		safe.UnaccessData();
		AutoCompleteSaveForm(pDisp);
	}
	// make real parameters out of the notification

	CString strTargetFrameName(V_BSTR(TargetFrameName));
	CString strURL = V_BSTR(URL);
	CString strHeaders = V_BSTR(Headers);
	DWORD nFlags = V_I4(Flags);

	// notify the user's class
	OnBeforeNavigate2(strURL, nFlags, strTargetFrameName,
		array, strHeaders, Cancel);
}
void	CIEAutomationView::AutoCompleteSaveForm(LPDISPATCH pDisp)
{
	#pragma message(__LOC__"not working,why?Need m_pShellUIHelper->AutoCompleteAttatch?\r\n") 
	CComQIPtr<IWebBrowser2>	pWebBrowser2(pDisp);
	CComPtr<IDispatch>	spHtmlDoc;
	if(FAILED(pWebBrowser2->get_Document(&spHtmlDoc)))	return;
	CComQIPtr<IHTMLDocument2>	pHtmlDoc(spHtmlDoc);
	CComPtr<IHTMLElement>	pElement;
	if(FAILED(pHtmlDoc->get_activeElement(&pElement))||pElement==NULL)	return;
	while(pElement){
		CComQIPtr<IHTMLFormElement>	pForm(pElement);
		if(pForm){
			CComQIPtr<IDispatch>	spForm(pForm);
			COleVariant var;
			var.vt=VT_DISPATCH;
			var.pdispVal=spForm;
			m_domExternal.AutoCompleteSaveForm(var);
			break;
		}
		CComPtr<IHTMLElement>	pParentElement;
		if(FAILED(pElement->get_parentElement(&pParentElement))||pParentElement==NULL)
			break;
		pElement=pParentElement;
	}
	
}
