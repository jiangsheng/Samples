// CmdTargetPlus.h : header file
//

#ifndef __CMDTARGETPLUS_H__
#define __CMDTARGETPLUS_H__

/////////////////////////////////////////////////////////////////////////////
// CCmdTargetPlus command target
//from KB Q168214
class CCmdTargetPlus : public CCmdTarget
{
	DECLARE_DYNCREATE(CCmdTargetPlus)

public:
	CCmdTargetPlus();           // protected constructor used by dynamic creation
	virtual ~CCmdTargetPlus();

// Attributes
public:

// Operations
public:
	void AFX_CDECL FireEvent(DISPID dispid, BYTE* pbParams, ...);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCmdTargetPlus)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Events
	static const AFX_DATA AFX_EVENTMAP_ENTRY _eventEntries[];
	virtual const AFX_EVENTMAP* GetEventMap() const;
	static const AFX_DATA AFX_EVENTMAP eventMap;
	const AFX_EVENTMAP_ENTRY* GetEventMapEntry(LPCTSTR pszName,
		DISPID* pDispid) const;
	void FireEventV(DISPID dispid, BYTE* pbParams, va_list argList);

	// Generated message map functions
	//{{AFX_MSG(CCmdTargetPlus)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CCmdTargetPlus)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	// Connection point container
	virtual LPCONNECTIONPOINT GetConnectionHook(REFIID iid);
	virtual BOOL GetExtraConnectionPoints(CPtrArray* pConnPoints);

	// Connection point for events - from COleControl
	BEGIN_CONNECTION_PART(CCmdTargetPlus, EventConnPt)
		virtual void OnAdvise(BOOL bAdvise);
		virtual REFIID GetIID();
		virtual LPUNKNOWN QuerySinkInterface(LPUNKNOWN pUnkSink);
	END_CONNECTION_PART(EventConnPt)

	// IProvideClassInfo2
	BEGIN_INTERFACE_PART(ProvideClassInfo, IProvideClassInfo2)
		INIT_INTERFACE_PART(COleControl, ProvideClassInfo)
		STDMETHOD(GetClassInfo)(LPTYPEINFO* ppTypeInfo);
		STDMETHOD(GetGUID)(DWORD dwGuidKind, GUID* pGUID);
	END_INTERFACE_PART(ProvideClassInfo)

public:
	virtual HRESULT GetClassID(LPCLSID pclsid);

	BOOL GetDispatchIID(IID* pIID);

	const IID* m_piidEvents;            // IID for control events
	const IID* m_piidPrimary;			  // IID for primary idispatch
};

/////////////////////////////////////////////////////////////////////////////

#endif // __CMDTARGETPLUS_H__
