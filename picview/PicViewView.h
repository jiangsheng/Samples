// PicViewView.h : interface of the CPicViewView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PICVIEWVIEW_H__3905E738_D7B0_439F_BAE0_89AE6D18EC34__INCLUDED_)
#define AFX_PICVIEWVIEW_H__3905E738_D7B0_439F_BAE0_89AE6D18EC34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxtempl.h>
#include <afxctl.h>
class	CFolderChange;
class	CFolderItemInfo:public	CObject
{
public:
	CFolderItemInfo();
	int   iIcon;
	UINT  state; 
	TCHAR	tszDisplayName[_MAX_PATH];
	TCHAR	tszPath[_MAX_PATH];
	BOOL	bFailLoadPic;
	CPictureHolder	pic;
#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
#endif
};
class CPicViewView : public CListView
{
protected: // create from serialization only
	CPicViewView();
	DECLARE_DYNCREATE(CPicViewView)

// Attributes
public:
	CPicViewDoc* GetDocument();
//	CTypedPtrMap<CMapStringToPtr,CString,CachedBitmap*>	m_mapFileNameTo;
		// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPicViewView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPicViewView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	LPMALLOC		m_pMalloc;
	IShellFolder *	m_psfDesktop;
	LPITEMIDLIST	m_pidlFolder;
	IShellFolder *	m_psfFolder;
	CTypedPtrArray<CPtrArray,LPITEMIDLIST>	m_arpFolderItems;
	CTypedPtrMap<CMapPtrToPtr,LPITEMIDLIST,CFolderItemInfo*>	m_mapCache;
	void	OnFolderChange(CFolderChange* pFolderChange);
	void	LeaveFolder();
	void	EnterFolder(LPCITEMIDLIST pidl);
	void	GetItemInfo(LPCITEMIDLIST pidl,CFolderItemInfo* pItemInfo);
	void	PrepCache( int iFrom, int iTo );
	void	ClearCache();
	CFolderItemInfo* FindItemInCache(LPCITEMIDLIST pidl);

// Generated message map functions
protected:
	//{{AFX_MSG(CPicViewView)
	afx_msg void OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnOdcachehint(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in PicViewView.cpp
inline CPicViewDoc* CPicViewView::GetDocument()
   { return (CPicViewDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PICVIEWVIEW_H__3905E738_D7B0_439F_BAE0_89AE6D18EC34__INCLUDED_)
