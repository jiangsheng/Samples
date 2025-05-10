// CustomControlSite.h: interface for the CCustomControlSite class.
//
//////////////////////////////////////////////////////////////////////
//=--------------------------------------------------------------------------=
//  (C) Copyright 1996-1999 Microsoft Corporation. All Rights Reserved.
//=--------------------------------------------------------------------------=
// 
// NOTE: 
// Some of the code in this file is MFC implementation specific.
// Changes in future versions of MFC implementation may require
// the code to be changed. Please check the readme of this
// sample for more information 
// 

#if !defined(AFX_CUSTOMCONTROLSITE_H__295F1FE9_0CE8_4DBE_B90A_298EC78D5492__INCLUDED_)
#define AFX_CUSTOMCONTROLSITE_H__295F1FE9_0CE8_4DBE_B90A_298EC78D5492__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//Knowledge Base  
//Q236312 HOWTO: Disable the Default Pop-up Menu for CHtmlView
#include <mshtmhst.h>
class	CIEAutomationView;
class CCustomControlSite : public COleControlSite  
{
public:
	
	CCustomControlSite(COleControlContainer *pCnt);
	~CCustomControlSite();
protected:
	//from MFC7
	CIEAutomationView*	m_pCustomImpl;
	DECLARE_INTERFACE_MAP();

	BEGIN_INTERFACE_PART(DocHostUIHandler2, IDocHostUIHandler2)
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
	END_INTERFACE_PART(DocHostUIHandler2)

	//input password for KB Q329802 
	BEGIN_INTERFACE_PART(Authenticate, IAuthenticate)

	STDMETHODIMP Authenticate( 
		HWND __RPC_FAR *phwnd,
		LPWSTR __RPC_FAR *pszUsername,
		LPWSTR __RPC_FAR *pszPassword);

	END_INTERFACE_PART(Authenticate)


	BEGIN_INTERFACE_PART(ServiceProvider, IServiceProvider)
		STDMETHODIMP QueryService(REFGUID guid, REFIID iid, LPVOID * ppv);
	END_INTERFACE_PART(ServiceProvider)  


};

#endif // !defined(AFX_CUSTOMCONTROLSITE_H__295F1FE9_0CE8_4DBE_B90A_298EC78D5492__INCLUDED_)
