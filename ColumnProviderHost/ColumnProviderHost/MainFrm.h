// MainFrm.h : interface of the CMainFrame class
//

#pragma once
class CColumnProviderHostDoc;
#include "FolderDockablePane.h"
#include "AddressBox.h"
class CMainFrame : public CFrameWndEx
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	CMFCShellTreeCtrl& GetFolders ();

// Operations
public:
	void ActivateByTab();

// Overrides
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// Implementation
public:
	virtual ~CMainFrame();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CMFCMenuBar		m_wndMenuBar;
	CMFCStatusBar	m_wndStatusBar;
	CMFCToolBar		m_wndToolBar;
	CMFCReBar		m_wndReBar;
	FolderDockablePane		folderDockablePane;
	AddressBox		addressBox;
	
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarReset(WPARAM,LPARAM);
	afx_msg LRESULT OnToolbarContextMenu(WPARAM,LPARAM);
	afx_msg void OnViewFolders();
	afx_msg void OnUpdateViewFolders(CCmdUI* pCmdUI);
	afx_msg void OnNewAddressEnter();
	afx_msg void OnNewAddress();
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnShowPopupMenu (CMFCPopupMenu* pMenuPopup);

	void InitBasicCommands();
	int InitFolderPane(CColumnProviderHostDoc *pDocument);
	int InitRebar();
	int InitAddressBox(CColumnProviderHostDoc *pDocument);
	int InitToolBar();
	int InitMenuBar();
	void InitDocking();
};


