// CmdTargetPlus.cpp : implementation file
//

#include "stdafx.h"
//#include "mfcaxscrvb.h"
#include "CmdTargetPlus.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCmdTargetPlus

IMPLEMENT_DYNCREATE(CCmdTargetPlus, CCmdTarget)

CCmdTargetPlus::CCmdTargetPlus()
{
	EnableAutomation();

	EnableConnections();
}

CCmdTargetPlus::~CCmdTargetPlus()
{
}


void CCmdTargetPlus::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CCmdTargetPlus, CCmdTarget)
	//{{AFX_MSG_MAP(CCmdTargetPlus)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CCmdTargetPlus, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CCmdTargetPlus)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_ICmdTargetPlus to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {2BC41DC1-422F-11D0-A1A9-000000000000}
static const IID IID_ICmdTargetPlus =
{ 0x2bc41dc1, 0x422f, 0x11d0, { 0xa1, 0xa9, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } };

BEGIN_INTERFACE_MAP(CCmdTargetPlus, CCmdTarget)
	INTERFACE_PART(CCmdTargetPlus, IID_ICmdTargetPlus, Dispatch)
	INTERFACE_PART(CCmdTargetPlus, IID_IConnectionPointContainer, ConnPtContainer)
	INTERFACE_PART(CCmdTargetPlus, IID_IProvideClassInfo, ProvideClassInfo)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCmdTargetPlus message handlers

/////////////////////////////////////////////////////////////////////////////
// Event Map Stuff from COleControl

const AFX_EVENTMAP CCmdTargetPlus::eventMap = { NULL, NULL };

const AFX_EVENTMAP* CCmdTargetPlus::GetEventMap() const
{
	return &eventMap;
}

const AFX_EVENTMAP_ENTRY* CCmdTargetPlus::GetEventMapEntry(
		LPCTSTR pszName,
		DISPID* pDispid) const
{
	ASSERT(pszName != NULL);
	ASSERT(pDispid != NULL);

	const AFX_EVENTMAP* pEventMap = GetEventMap();
	const AFX_EVENTMAP_ENTRY* pEntry;
	DISPID dispid = MAKELONG(1, 0);

	while (pEventMap != NULL)
	{
		pEntry = pEventMap->lpEntries;

		// Scan entries in this event map

		if (pEntry != NULL)
		{
			while (pEntry->pszName != NULL)
			{
				if (_tcscmp(pEntry->pszName, pszName) == 0)
				{
					if (pEntry->dispid != DISPID_UNKNOWN)
						dispid = pEntry->dispid;

					*pDispid = dispid;
					return pEntry;
				}

				++pEntry;
				++dispid;
			}
		}

		// If we didn't find it, go to the base class's event map

		pEventMap = pEventMap->lpBaseEventMap;
		dispid = MAKELONG(1, HIWORD(dispid)+1);
	}

	// If we reach here, the event isn't supported

	return NULL;
}

void CCmdTargetPlus::FireEventV(DISPID dispid, BYTE* pbParams,
	va_list argList)
{
	COleDispatchDriver driver;

	POSITION pos = m_xEventConnPt.GetStartPosition();
	LPDISPATCH pDispatch;
	while (pos != NULL)
	{
		pDispatch = (LPDISPATCH)m_xEventConnPt.GetNextConnection(pos);
		ASSERT(pDispatch != NULL);
		driver.AttachDispatch(pDispatch, FALSE);
		TRY
			driver.InvokeHelperV(dispid, DISPATCH_METHOD, VT_EMPTY, NULL,
				pbParams, argList);
		END_TRY
		driver.DetachDispatch();
	}
}
;
void AFX_CDECL CCmdTargetPlus::FireEvent(DISPID dispid, BYTE* pbParams, ...)
{
	va_list argList;
	va_start(argList, pbParams);
	FireEventV(dispid, pbParams, argList);
	va_end(argList);
}

void CCmdTargetPlus::XEventConnPt::OnAdvise(BOOL bAdvise)
{
	METHOD_PROLOGUE_EX(CCmdTargetPlus, EventConnPt)
//	pThis->OnEventAdvise(bAdvise);
	TRACE("Event conn pt advise\n");
}

REFIID CCmdTargetPlus::XEventConnPt::GetIID()
{
	METHOD_PROLOGUE_EX_(CCmdTargetPlus, EventConnPt)
	if (pThis->m_piidEvents != NULL)
		return *(pThis->m_piidEvents);
	else
		return GUID_NULL;
}

LPUNKNOWN CCmdTargetPlus::XEventConnPt::QuerySinkInterface(LPUNKNOWN pUnkSink)
{
	METHOD_PROLOGUE_EX_(CCmdTargetPlus, EventConnPt)

	// First, QI for control-specific IID; failing that, QI for IDispatch
	LPUNKNOWN pUnkReturn = NULL;
	if (FAILED(pUnkSink->QueryInterface(*(pThis->m_piidEvents),
		reinterpret_cast<void**>(&pUnkReturn))))
	{
		pUnkSink->QueryInterface(IID_IDispatch,
			reinterpret_cast<void**>(&pUnkReturn));
	}
	return pUnkReturn;
}

/////////////////////////////////////////////////////////////////////////////
// COleControl::GetConnectionHook - override the connection map

LPCONNECTIONPOINT CCmdTargetPlus::GetConnectionHook(REFIID iid)
{
	if ((m_piidEvents != NULL) && IsEqualIID(iid, *m_piidEvents))
		return (LPCONNECTIONPOINT)((char*)&m_xEventConnPt +
				offsetof(CConnectionPoint, m_xConnPt));
	else
		return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// COleControl::GetExtraConnectionPoints - override the connection map

BOOL CCmdTargetPlus::GetExtraConnectionPoints(CPtrArray* pConnPoints)
{
	pConnPoints->Add((char*)&m_xEventConnPt +
		offsetof(CConnectionPoint, m_xConnPt));
	return TRUE;
}

BOOL CCmdTargetPlus::GetDispatchIID(IID* pIID)
{
	if (m_piidPrimary != NULL)
		*pIID = *m_piidPrimary;

	return (m_piidPrimary != NULL);
}

/////////////////////////////////////////////////////////////////////////////
// COleControl::XProvideClassInfo

STDMETHODIMP_(ULONG) CCmdTargetPlus::XProvideClassInfo::AddRef()
{
	METHOD_PROLOGUE_EX_(CCmdTargetPlus, ProvideClassInfo)
	return (ULONG)pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) CCmdTargetPlus::XProvideClassInfo::Release()
{
	METHOD_PROLOGUE_EX_(CCmdTargetPlus, ProvideClassInfo)
	return (ULONG)pThis->ExternalRelease();
}

STDMETHODIMP CCmdTargetPlus::XProvideClassInfo::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE_EX_(CCmdTargetPlus, ProvideClassInfo)
	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CCmdTargetPlus::XProvideClassInfo::GetClassInfo(
	LPTYPEINFO* ppTypeInfo)
{
	METHOD_PROLOGUE_EX_(CCmdTargetPlus, ProvideClassInfo)

	CLSID clsid;
	pThis->GetClassID(&clsid);

	return pThis->GetTypeInfoOfGuid(GetUserDefaultLCID(), clsid, ppTypeInfo);
}

STDMETHODIMP CCmdTargetPlus::XProvideClassInfo::GetGUID(DWORD dwGuidKind,
	GUID* pGUID)
{
	METHOD_PROLOGUE_EX_(CCmdTargetPlus, ProvideClassInfo)
	TRACE("GetGUID in provideclassinfo\n");
	
	if (dwGuidKind == GUIDKIND_DEFAULT_SOURCE_DISP_IID)
	{
		*pGUID = *pThis->m_piidEvents;
		return NOERROR;
	}
	else
	{
		*pGUID = GUID_NULL;
		return E_INVALIDARG;
	}
}

HRESULT CCmdTargetPlus::GetClassID(LPCLSID pclsid)
{
	*pclsid = CLSID_NULL;
	return S_OK;
}
