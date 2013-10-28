// ColumnProviderHostView.h : interface of the CColumnProviderHostView class
//

#pragma once

#include "resource.h"


#include "ColumnProviderHostList.h"
class CColumnProviderHostView : public CView
{
protected: // create from serialization only
	CColumnProviderHostView();
	DECLARE_DYNCREATE(CColumnProviderHostView)

public:

// Attributes
public:
	CColumnProviderHostDoc* GetDocument() const;
	ColumnProviderHostList&	GetShellList(){return m_wndList;}
// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct
	
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
// Implementation
public:
	virtual ~CColumnProviderHostView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg LRESULT OnChangeFolder(WPARAM,LPARAM);

	DECLARE_MESSAGE_MAP()
	virtual void OnDraw(CDC* /*pDC*/);
	ColumnProviderHostList	m_wndList;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	UINT_PTR timerRefreshId;
	afx_msg void OnDestroy();
};

#ifndef _DEBUG  // debug version in ColumnProviderHostView.cpp
inline CColumnProviderHostDoc* CColumnProviderHostView::GetDocument() const
   { return reinterpret_cast<CColumnProviderHostDoc*>(m_pDocument); }
#endif

