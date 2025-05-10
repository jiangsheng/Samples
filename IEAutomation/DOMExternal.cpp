// DOMExternal.cpp : implementation file
//

#include "stdafx.h"
#include "IEAutomation.h"
#include "DOMExternal.h"
#include "IEAutomationView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDOMExternal
// Note: we add support for IID_IDOMExternal to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {127148C0-2337-4923-B70A-6DA929E10451}
static const IID IID_IDOMExternal =
{ 0x127148c0, 0x2337, 0x4923, { 0xb7, 0xa, 0x6d, 0xa9, 0x29, 0xe1, 0x4, 0x51 } };
//{A7E7F294-7887-4B37-851E-EF9EDCBA2EBF}
static const IID	IID_IDOMExternalEvents=
{ 0xA7E7F294, 0x7887, 0x4B37, { 0x85, 0x1E, 0xEF, 0x9E, 0xDC, 0xBA, 0x2E, 0xBF } };
//{761F8EA8-5593-4C63-B853-08836021E27B}
static const IID	CLSID_IDOMExternal=
{ 0x761F8EA8, 0x5593, 0x4C63, { 0xB8, 0x53, 0x08, 0x83, 0x60, 0x21, 0xE2, 0x7B } };

// Use OLETYPELIB of master dialog?
IMPLEMENT_OLETYPELIB(CDOMExternal, _tlid, _wVerMajor, _wVerMinor)

IMPLEMENT_DYNCREATE(CDOMExternal, CCmdTargetPlus)

CDOMExternal::CDOMExternal()
{
	EnableAutomation();
	EnableTypeLib();
	// To keep the application running as long as an OLE automation 
	//	object is active, the constructor calls AfxOleLockApp.
	
	AfxOleLockApp();
	m_piidEvents = &IID_IDOMExternalEvents;
	m_piidPrimary = &IID_IDOMExternal;
	m_pView=NULL;
	if(FAILED(m_pShellUIHelper.CoCreateInstance(CLSID_ShellUIHelper)))
		return	;
}

CDOMExternal::~CDOMExternal()
{
	// To terminate the application when all objects created with
	// 	with OLE automation, the destructor calls AfxOleUnlockApp.
	
	AfxOleUnlockApp();
}


void CDOMExternal::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTargetPlus::OnFinalRelease();
}



BEGIN_MESSAGE_MAP(CDOMExternal, CCmdTargetPlus)
	//{{AFX_MSG_MAP(CDOMExternal)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CDOMExternal, CCmdTargetPlus)
	//{{AFX_DISPATCH_MAP(CDOMExternal)
	DISP_PROPERTY_EX(CDOMExternal, "menuArguments", GetMenuArguments, SetNotSupported, VT_DISPATCH)
	DISP_FUNCTION(CDOMExternal, "AddChannel", AddChannel, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CDOMExternal, "AddDesktopComponent", AddDesktopComponent, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CDOMExternal, "AutoCompleteSaveForm", AutoCompleteSaveForm, VT_EMPTY, VTS_VARIANT)
	DISP_FUNCTION(CDOMExternal, "AutoScan", AutoScan, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_VARIANT)
	DISP_FUNCTION(CDOMExternal, "ImportExportFavorites", ImportExportFavorites, VT_EMPTY, VTS_BOOL VTS_BSTR)
	DISP_FUNCTION(CDOMExternal, "IsSubscribed", IsSubscribed, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CDOMExternal, "NavigateAndFind", NavigateAndFind, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_VARIANT)
	DISP_FUNCTION(CDOMExternal, "ShowBrowserUI", ShowBrowserUI, VT_VARIANT, VTS_BSTR VTS_VARIANT)
	DISP_FUNCTION(CDOMExternal, "AddFavorite", AddFavorite, VT_EMPTY, VTS_BSTR VTS_VARIANT)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

BEGIN_EVENT_MAP(CDOMExternal, CCmdTargetPlus)
	//{{AFX_EVENT_MAP(CDOMExternal)
	EVENT_CUSTOM("BeforeNavigate2", FireBeforeNavigate2, VTS_DISPATCH  VTS_PVARIANT  VTS_PVARIANT  VTS_PVARIANT  VTS_PVARIANT  VTS_PVARIANT  VTS_PBOOL)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()

BEGIN_INTERFACE_MAP(CDOMExternal, CCmdTargetPlus)
	INTERFACE_PART(CDOMExternal, IID_IDOMExternal, Dispatch)
END_INTERFACE_MAP()

// {761F8EA8-5593-4C63-B853-08836021E27B}
IMPLEMENT_OLECREATE(CDOMExternal, "IEAutomation.DOMExternal", 0x761f8ea8, 0x5593, 0x4c63, 0xb8, 0x53, 0x8, 0x83, 0x60, 0x21, 0xe2, 0x7b)
// {761F8EA8-5593-4C63-B853-08836021E27B}
//static const CLSID clsid =
//{ 0x8a483a03, 0x99c1, 0x4073, { 0x8b, 0x75, 0x96, 0x7d, 0xe3, 0xc3, 0x95, 0x3 } };

/////////////////////////////////////////////////////////////////////////////
// CDOMExternal message handlers
HRESULT CDOMExternal::GetClassID(LPCLSID pclsid)
{
	*pclsid = CLSID_IDOMExternal;
	return NOERROR;
}

LPDISPATCH CDOMExternal::GetMenuArguments() 
{
	// TODO: Add your property handler here

	return NULL;
}

void CDOMExternal::SetNotSupported(LPDISPATCH newValue) 
{
	AfxThrowNotSupportedException();
}

void CDOMExternal::AddChannel(LPCTSTR URL) 
{
	if(m_pShellUIHelper){
		CString strURL(URL);
		BSTR bstrURL=strURL.AllocSysString();		
		m_pShellUIHelper->AddChannel(bstrURL);
		SysFreeString(bstrURL);
	}
}

void CDOMExternal::AddDesktopComponent(
	LPCTSTR URL
	, LPCTSTR Type
	, const VARIANT FAR& Left
	, const VARIANT FAR& Top
	, const VARIANT FAR& Width
	, const VARIANT FAR& Height) 
{
	if(m_pShellUIHelper){
		CString strURL(URL);
		CString strType(Type);
		BSTR bstrURL=strURL.AllocSysString();
		BSTR bstrType=strType.AllocSysString();
		COleVariant varLeft(Left);
		COleVariant varTop(Top);
		COleVariant varWidth(Width);
		COleVariant varHeight(Height);
		m_pShellUIHelper->AddDesktopComponent(bstrURL,bstrType,varLeft,varTop,varWidth,varHeight);
		SysFreeString(bstrURL);
		SysFreeString(bstrType);
	}
}

void CDOMExternal::AddFavorite(LPCTSTR URL, const VARIANT FAR& Title) 
{
	if(m_pShellUIHelper){
		CString strURL(URL);
		BSTR bstrURL=strURL.AllocSysString();
		COleVariant	varTitle(Title);
		m_pShellUIHelper->AddFavorite(bstrURL,&varTitle);
		SysFreeString(bstrURL);
		theApp.UpdateFavoritesMenu();
	}
}

void CDOMExternal::AutoCompleteSaveForm(const VARIANT FAR& rForm) 
{
	if(m_pShellUIHelper){
		COleVariant varForm(rForm);
		m_pShellUIHelper->AutoCompleteSaveForm(&varForm);
	}
}

void CDOMExternal::AutoScan(LPCTSTR strSearch, LPCTSTR strFailureUrl, const VARIANT FAR& rvarTargetFrame) 
{
	if(m_pShellUIHelper){
		CString stringSearch(strSearch);
		CString stringFailureUrl(strFailureUrl);
		BSTR bstrSearch=stringSearch.AllocSysString();
		BSTR bstrFailureUrl=stringFailureUrl.AllocSysString();
		COleVariant varTargetFrame(rvarTargetFrame);
		m_pShellUIHelper->AutoScan(bstrSearch,bstrFailureUrl,&varTargetFrame);
		SysFreeString(bstrSearch);
		SysFreeString(bstrFailureUrl);
	}
}

void CDOMExternal::ImportExportFavorites(BOOL fImport, LPCTSTR strImpExpPath) 
{
	if(m_pShellUIHelper){
		CString stringImpExpPath(strImpExpPath);
		BSTR bstrImpExpPath=stringImpExpPath.AllocSysString();
		m_pShellUIHelper->ImportExportFavorites(
			fImport?VARIANT_TRUE:VARIANT_FALSE,bstrImpExpPath);
		SysFreeString(bstrImpExpPath);
		theApp.UpdateFavoritesMenu();
	}
}

BOOL CDOMExternal::IsSubscribed(LPCTSTR URL) 
{
	if(m_pShellUIHelper){
		CString strURL(URL);
		BSTR bstrURL=strURL.AllocSysString();
		VARIANT_BOOL bSubscribed=VARIANT_FALSE;
		m_pShellUIHelper->IsSubscribed(bstrURL,&bSubscribed);		
		SysFreeString(bstrURL);
		return bSubscribed==VARIANT_TRUE;
	}

	return FALSE;
}

void CDOMExternal::NavigateAndFind(LPCTSTR URL, LPCTSTR strQuery, const VARIANT FAR& rvarTargetFrame) 
{
	if(m_pShellUIHelper){
		CString strURL(URL);
		CString stringQuery(strQuery);
		BSTR bstrURL=strURL.AllocSysString();
		BSTR bstrQuery=stringQuery.AllocSysString();
		COleVariant varTargetFrame(rvarTargetFrame);
		m_pShellUIHelper->NavigateAndFind(bstrURL,bstrQuery,&varTargetFrame);
		SysFreeString(bstrURL);
		SysFreeString(bstrQuery);
	}

}

VARIANT	CDOMExternal::ShowBrowserUI(LPCTSTR bstrName, const VARIANT FAR& rvarIn) 
{
	VARIANT varOut;
	VariantInit(&varOut);
	if(m_pShellUIHelper){
		CString strName(bstrName);
		BSTR bstringName=strName.AllocSysString();
		COleVariant varIn(rvarIn);
		m_pShellUIHelper->ShowBrowserUI(bstringName,&varIn,&varOut);
		SysFreeString(bstringName);
		theApp.UpdateFavoritesMenu();
	}
	return varOut;
}

void CDOMExternal::FireBeforeNavigate2(LPDISPATCH pDisp, VARIANT FAR* URL, VARIANT FAR* Flags, VARIANT FAR* TargetFrameName, VARIANT FAR* PostData, VARIANT FAR* Headers, BOOL FAR* Cancel)
{
	FireEvent(eventidBeforeNavigate2,EVENT_PARAM(VTS_DISPATCH  VTS_PVARIANT  VTS_PVARIANT  VTS_PVARIANT  VTS_PVARIANT  VTS_PVARIANT  VTS_PBOOL), pDisp, URL, Flags, TargetFrameName, PostData, Headers, Cancel);
}
