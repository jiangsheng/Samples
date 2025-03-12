// WDSSampleView.h : interface of the CWDSSampleView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WDSSAMPLEVIEW_H__FB940149_F265_4C3E_9121_4E474B0E3896__INCLUDED_)
#define AFX_WDSSAMPLEVIEW_H__FB940149_F265_4C3E_9121_4E474B0E3896__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

DEFINE_GUID(CLSID_SearchDesktop,0x1AD68C99,0x00FB,0x416d,0x80,0x4B,0xC3,0x8D,0xEE,0x75,0xD5,0x5E);
#include <afxtempl.h>
class CWDSSampleView : public CListView
{
protected: // create from serialization only
	CWDSSampleView();
	DECLARE_DYNCREATE(CWDSSampleView)

// Attributes
public:
	CWDSSampleDoc* GetDocument();
	ISearchDesktopPtr	m_pSearchDesktop;
	CStringArray	m_astrHeaderText;
	WDSQuery::_RecordsetPtr m_pRecordset;
// Operations
public:
	int GetColumnCount() const;
	void AutoSizeColumns(int col =-1) ;
	CTypedPtrMap<CMapPtrToPtr,LPVOID,CStringArray*> m_mapCache;
	void	PrepCache(UINT from, UINT to);
	void	ClearCache();
	void	GetItemInfo(int iItem,CStringArray& strInfoCache);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWDSSampleView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	void	Search(LPCTSTR lpszQuery,LPCTSTR lpszSort=NULL);
	virtual ~CWDSSampleView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CWDSSampleView)
	afx_msg void OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnOdcachehint(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in WDSSampleView.cpp
inline CWDSSampleDoc* CWDSSampleView::GetDocument()
   { return (CWDSSampleDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WDSSAMPLEVIEW_H__FB940149_F265_4C3E_9121_4E474B0E3896__INCLUDED_)
