// CustomControlSite.cpp: implementation of the CCustomControlSite class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IEAutomation.h"
#include "IEAutomationView.h"
#include "DlgAuthenticate.h"

#include <Transact.h>
#undef AFX_DATA
#define AFX_DATA AFX_DATA_IMPORT
// NOTE: This line is a hardcoded reference to an MFC header file
//  this path may need to be changed to refer to the location of VC5 install
//  for successful compilation.
#include <..\src\occimpl.h>
#undef AFX_DATA
#define AFX_DATA AFX_DATA_EXPORT
#include "CustomControlSite.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
BEGIN_INTERFACE_MAP(CCustomControlSite, COleControlSite)
	INTERFACE_PART(CCustomControlSite, IID_IDocHostUIHandler2, DocHostUIHandler2)
	INTERFACE_PART(CCustomControlSite, IID_IDocHostUIHandler, DocHostUIHandler2)
	INTERFACE_PART(CCustomControlSite, IID_IAuthenticate, Authenticate)
	INTERFACE_PART(CCustomControlSite, IID_IServiceProvider, ServiceProvider)
END_INTERFACE_MAP()

CCustomControlSite::CCustomControlSite(COleControlContainer *pCnt)
:COleControlSite(pCnt)
{
	m_pCustomImpl=NULL;
	//since only pCnt->m_pWnd is valid,
	//and can not derive a class from COleControlContainer in MFC6
	//use dynamic casting for pCnt->m_pWnd here
	CWnd* pWnd=pCnt->m_pWnd;
	if(pWnd){
		if(pWnd->IsKindOf(RUNTIME_CLASS(CIEAutomationView))){
			CIEAutomationView* pView=(CIEAutomationView*)pWnd;
			m_pCustomImpl=pView;
		}
	}
	
}

CCustomControlSite::~CCustomControlSite()
{

}
ULONG FAR EXPORT  CCustomControlSite::XDocHostUIHandler2::AddRef()
{
	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler2)
	return pThis->ExternalAddRef();
}


ULONG FAR EXPORT  CCustomControlSite::XDocHostUIHandler2::Release()
{                            
    METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler2)
	return pThis->ExternalRelease();
}

HRESULT FAR EXPORT  CCustomControlSite::XDocHostUIHandler2::QueryInterface(REFIID riid, void **ppvObj)
{
	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler2)
    HRESULT hr = (HRESULT)pThis->ExternalQueryInterface(&riid, ppvObj);
	return hr;
}

// * CImpIDocHostUIHandler::GetHostInfo
// *
// * Purpose: Called at initialization
// *
HRESULT FAR EXPORT  CCustomControlSite::XDocHostUIHandler2::GetHostInfo( DOCHOSTUIINFO* pInfo )
{

	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler2)
//	pInfo->dwFlags = DOCHOSTUIFLAG_NO3DBORDER;
//  pInfo->dwDoubleClick = DOCHOSTUIDBLCLK_DEFAULT;
	if(pThis->m_pCustomImpl){
		return pThis->m_pCustomImpl->GetHostInfo(pInfo );
	}
    return S_OK;
}

// * CImpIDocHostUIHandler::ShowUI
// *
// * Purpose: Called when MSHTML.DLL shows its UI
// *
HRESULT FAR EXPORT  CCustomControlSite::XDocHostUIHandler2::ShowUI(
				DWORD dwID, 
				IOleInPlaceActiveObject * pActiveObject,
				IOleCommandTarget * pCommandTarget,
				IOleInPlaceFrame * pFrame,
				IOleInPlaceUIWindow * pDoc)
{

	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler2)
	// We've already got our own UI in place so just return S_OK
	if(pThis->m_pCustomImpl){
		return pThis->m_pCustomImpl->ShowUI(
				 dwID, 
				 pActiveObject,
				 pCommandTarget,
				 pFrame,
				 pDoc);
	}
    return E_NOTIMPL;
  //return S_OK;
}

// * CImpIDocHostUIHandler::HideUI
// *
// * Purpose: Called when MSHTML.DLL hides its UI
// *
HRESULT FAR EXPORT  CCustomControlSite::XDocHostUIHandler2::HideUI(void)
{
	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler2)
	if(pThis->m_pCustomImpl){
		return pThis->m_pCustomImpl->HideUI( );
	}
	return E_NOTIMPL;
     //return S_OK;
}

// * CImpIDocHostUIHandler::UpdateUI
// *
// * Purpose: Called when MSHTML.DLL updates its UI
// *
HRESULT FAR EXPORT  CCustomControlSite::XDocHostUIHandler2::UpdateUI(void)
{
	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler2)
	// MFC is pretty good about updating it's UI in it's Idle loop so I don't do anything here
	if(pThis->m_pCustomImpl){
		return pThis->m_pCustomImpl->UpdateUI( );
	}
	return E_NOTIMPL;
	//return S_OK;
}

// * CImpIDocHostUIHandler::EnableModeless
// *
// * Purpose: Called from MSHTML.DLL's IOleInPlaceActiveObject::EnableModeless
// *
HRESULT FAR EXPORT  CCustomControlSite::XDocHostUIHandler2::EnableModeless(BOOL fEnable)
{
	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler2)
	if(pThis->m_pCustomImpl){
		return pThis->m_pCustomImpl->EnableModeless(fEnable);
	}
    return E_NOTIMPL;
}

// * CImpIDocHostUIHandler::OnDocWindowActivate
// *
// * Purpose: Called from MSHTML.DLL's IOleInPlaceActiveObject::OnDocWindowActivate
// *
HRESULT FAR EXPORT  CCustomControlSite::XDocHostUIHandler2::OnDocWindowActivate(BOOL fActivate)
{
	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler2)
	if(pThis->m_pCustomImpl){
		return pThis->m_pCustomImpl->OnDocWindowActivate(fActivate);
	}
    return E_NOTIMPL;
}

// * CImpIDocHostUIHandler::OnFrameWindowActivate
// *
// * Purpose: Called from MSHTML.DLL's IOleInPlaceActiveObject::OnFrameWindowActivate
// *
HRESULT FAR EXPORT  CCustomControlSite::XDocHostUIHandler2::OnFrameWindowActivate(BOOL fActivate)
{
	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler2)
	if(pThis->m_pCustomImpl){
		return pThis->m_pCustomImpl->OnFrameWindowActivate(fActivate);
	}
    return E_NOTIMPL;
}

// * CImpIDocHostUIHandler::ResizeBorder
// *
// * Purpose: Called from MSHTML.DLL's IOleInPlaceActiveObject::ResizeBorder
// *
HRESULT FAR EXPORT  CCustomControlSite::XDocHostUIHandler2::ResizeBorder(
				LPCRECT prcBorder, 
				IOleInPlaceUIWindow* pUIWindow,
				BOOL fRameWindow)
{
	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler2)
	if(pThis->m_pCustomImpl){
		return pThis->m_pCustomImpl->ResizeBorder(
				prcBorder, 
				pUIWindow,
				fRameWindow);
	}
    return E_NOTIMPL;
}

// * CImpIDocHostUIHandler::ShowContextMenu
// *
// * Purpose: Called when MSHTML.DLL would normally display its context menu
// *
HRESULT FAR EXPORT  CCustomControlSite::XDocHostUIHandler2::ShowContextMenu(
				DWORD dwID, 
				POINT* pptPosition,
				IUnknown* pCommandTarget,
				IDispatch* pDispatchObjectHit)
{
	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler2)
	if(pThis->m_pCustomImpl){
		return pThis->m_pCustomImpl->ShowContextMenu(
				dwID, 
				pptPosition,
				pCommandTarget,
				pDispatchObjectHit);
	}
	return E_NOTIMPL;
    //return S_OK; // We've shown our own context menu. MSHTML.DLL will no longer try to show its own.
}

// * CImpIDocHostUIHandler::TranslateAccelerator
// *
// * Purpose: Called from MSHTML.DLL's TranslateAccelerator routines
// *
HRESULT FAR EXPORT  CCustomControlSite::XDocHostUIHandler2::TranslateAccelerator(LPMSG lpMsg,
            /* [in] */ const GUID __RPC_FAR *pguidCmdGroup,
            /* [in] */ DWORD nCmdID)
{
	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler2)
 	if(pThis->m_pCustomImpl){
		return pThis->m_pCustomImpl->TranslateAccelerator(lpMsg,
            /* [in] */ pguidCmdGroup,
            /* [in] */ nCmdID);
	}
    return E_NOTIMPL;   
	//return S_FALSE;
}

// * CImpIDocHostUIHandler::GetOptionKeyPath
// *
// * Purpose: Called by MSHTML.DLL to find where the host wishes to store 
// *	its options in the registry
// *
HRESULT FAR EXPORT  CCustomControlSite::XDocHostUIHandler2::GetOptionKeyPath(BSTR* pbstrKey, DWORD	dw)
{

	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler2)
	if(pThis->m_pCustomImpl){
		return pThis->m_pCustomImpl->GetOptionKeyPath(pbstrKey, dw);
	}
	return E_NOTIMPL;
}

STDMETHODIMP CCustomControlSite::XDocHostUIHandler2::GetDropTarget( 
            /* [in] */ IDropTarget __RPC_FAR *pDropTarget,
            /* [out] */ IDropTarget __RPC_FAR *__RPC_FAR *ppDropTarget)
{
	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler2)
	if(pThis->m_pCustomImpl){
		return pThis->m_pCustomImpl->GetDropTarget( 
            /* [in] */ pDropTarget,
            /* [out] */ ppDropTarget);
	}
	return E_NOTIMPL;
}


STDMETHODIMP CCustomControlSite::XDocHostUIHandler2::GetExternal( 
            /* [out] */ IDispatch __RPC_FAR *__RPC_FAR *ppDispatch)
{
	// return the IDispatch we have for extending the object Model
	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler2)
	if(pThis->m_pCustomImpl){
		return pThis->m_pCustomImpl->GetExternal( 
            /* [out] */ ppDispatch);
	}
	return E_NOTIMPL;
//	IDispatch* pDisp = (IDispatch*)theApp.m_pDispOM;
//	pDisp->AddRef();
//	*ppDispatch = pDisp;
  //  return S_OK;
}
        
STDMETHODIMP CCustomControlSite::XDocHostUIHandler2::TranslateUrl( 
            /* [in] */ DWORD dwTranslate,
            /* [in] */ OLECHAR __RPC_FAR *pchURLIn,
            /* [out] */ OLECHAR __RPC_FAR *__RPC_FAR *ppchURLOut)
{
	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler2)
	if(pThis->m_pCustomImpl){
		return pThis->m_pCustomImpl->TranslateUrl( 
            /* [in] */ dwTranslate,
            /* [in] */ pchURLIn,
            /* [out] */ ppchURLOut);
	}
	return E_NOTIMPL;

}
        
STDMETHODIMP CCustomControlSite::XDocHostUIHandler2::FilterDataObject( 
            /* [in] */ IDataObject __RPC_FAR *pDO,
            /* [out] */ IDataObject __RPC_FAR *__RPC_FAR *ppDORet)
{
	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler2)
	if(pThis->m_pCustomImpl){
		return pThis->m_pCustomImpl->FilterDataObject( 
            /* [in] */ pDO,
            /* [out] */ ppDORet);
	}
	return E_NOTIMPL;
}

HRESULT FAR EXPORT  CCustomControlSite::XDocHostUIHandler2::GetOverrideKeyPath(BSTR* pbstrKey, DWORD	dw)
{

	METHOD_PROLOGUE(CCustomControlSite, DocHostUIHandler2)
	if(pThis->m_pCustomImpl){
		return pThis->m_pCustomImpl->GetOverrideKeyPath(pbstrKey, dw);
	}
	return E_NOTIMPL;
}


///////////////////////////////////////////////////////////////////////////// 
// CCustomControlSite: IAthenticate::IUnknown methods

ULONG CCustomControlSite::XAuthenticate::AddRef()
{
	METHOD_PROLOGUE(CCustomControlSite, Authenticate)
	return pThis->ExternalAddRef();
}

ULONG CCustomControlSite::XAuthenticate::Release()
{                            
	METHOD_PROLOGUE(CCustomControlSite, Authenticate)
	return pThis->ExternalRelease();
}

HRESULT CCustomControlSite::XAuthenticate::QueryInterface(REFIID riid, void **ppvObj)
{
	METHOD_PROLOGUE(CCustomControlSite, Authenticate)
	HRESULT hr = (HRESULT)pThis->ExternalQueryInterface(&riid, ppvObj);
	return hr;
}


///////////////////////////////////////////////////////////////////////////// 
// CCustomControlSite: Authenticate methods
//prompt for username and password
STDMETHODIMP CCustomControlSite::XAuthenticate::Authenticate( 
    HWND __RPC_FAR *phwnd,
    LPWSTR __RPC_FAR *pszUsername,
    LPWSTR __RPC_FAR *pszPassword)
{
	METHOD_PROLOGUE(CCustomControlSite, Authenticate)
	CDlgAuthenticate	DlgAuthenticate;
	DlgAuthenticate.m_strUserName=theApp.m_strAuthenticateUsername;
	DlgAuthenticate.m_strPassword=theApp.m_strAuthenticatePassword;
	if(DlgAuthenticate.DoModal()==IDOK){
		theApp.m_strAuthenticateUsername=DlgAuthenticate.m_strUserName;
		theApp.m_strAuthenticatePassword=DlgAuthenticate.m_strPassword;
		WCHAR *wszDlgUser = (WCHAR *) CoTaskMemAlloc(255 * sizeof(WCHAR));
		WCHAR *wszDlgPassword = (WCHAR *) CoTaskMemAlloc(255 * sizeof(WCHAR));
		MultiByteToWideChar(GetACP(), 0, (const char*)theApp.m_strAuthenticateUsername, -1, wszDlgUser, 255);
		MultiByteToWideChar(GetACP(), 0, (const char*)theApp.m_strAuthenticatePassword, -1, wszDlgPassword, 255);
		*pszUsername = wszDlgUser;
		*pszPassword = wszDlgPassword;
		*phwnd = NULL;
		return S_OK;
	}
	return E_NOINTERFACE;
}

// 
// IServiceProvider
// 

ULONG CCustomControlSite::XServiceProvider::AddRef()
{
	METHOD_PROLOGUE(CCustomControlSite, ServiceProvider)
	return pThis->ExternalAddRef();
}

ULONG CCustomControlSite::XServiceProvider::Release()
{                            
	METHOD_PROLOGUE(CCustomControlSite, ServiceProvider)
	return pThis->ExternalRelease();
}

HRESULT CCustomControlSite::XServiceProvider::QueryInterface(REFIID riid, void **ppvObj)
{
	METHOD_PROLOGUE(CCustomControlSite, ServiceProvider)
	HRESULT hr = (HRESULT)pThis->ExternalQueryInterface(&riid, ppvObj);
	return hr;
}

STDMETHODIMP CCustomControlSite::XServiceProvider::QueryService(REFGUID sid, REFIID iid, LPVOID * ppv)
{
 	METHOD_PROLOGUE(CCustomControlSite, ServiceProvider)

	if (sid == IID_IAuthenticate || iid == IID_IAuthenticate) {
		return (HRESULT)pThis->ExternalQueryInterface(&iid, ppv);
    } else {
		*ppv = NULL;
	}
		
	return E_NOINTERFACE;
}
