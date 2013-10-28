#pragma once


// CFolderDockablePane

class FolderDockablePane : public CDockablePane
{
	DECLARE_DYNAMIC(FolderDockablePane)

public:
	FolderDockablePane();
	virtual ~FolderDockablePane();
	CMFCShellTreeCtrl& GetShellTree(){return shellTree;}
protected:
	DECLARE_MESSAGE_MAP()
	CMFCShellTreeCtrl	shellTree;
	ULONG shellChangeNotifyDeregisterCookie;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg	LRESULT OnShellChangeNotify(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
};


