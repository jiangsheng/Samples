// ChildFrm.h : interface of the CChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDFRM_H__5D0705CC_DAE6_4E9B_84D4_89B76009F5D8__INCLUDED_)
#define AFX_CHILDFRM_H__5D0705CC_DAE6_4E9B_84D4_89B76009F5D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class	CIEAutomationDoc;
class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// Attributes
public:
	//for Favorites Menu
	//from MFCIE sample
	CStringArray m_astrFavoriteURLs;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL
	//from MFCIE sample
	void SetAddress(LPCTSTR lpszUrl);
	void StartAnimation();
	int BuildFavoritesMenu(LPCTSTR pszPath, int nStartPos, CMenu* pMenu);
	//for MDI menu replacement
	void	BuildFavoritesMenu(CIEAutomationDoc * pDoc=NULL);
// Implementation
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	//from MFCIE sample
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CReBar      m_wndReBar;
	CComboBoxEx m_wndAddress;
	CAnimateCtrl m_wndAnimate;

// Generated message map functions
protected:
	//{{AFX_MSG(CChildFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void DoNothing();
	afx_msg void OnNewAddress();
	afx_msg void OnNewAddressEnter();
	afx_msg void OnFavorite(UINT nID);
	afx_msg void OnDropDown(NMHDR* pNotifyStruct, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDFRM_H__5D0705CC_DAE6_4E9B_84D4_89B76009F5D8__INCLUDED_)
