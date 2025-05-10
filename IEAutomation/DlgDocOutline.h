#if !defined(AFX_DLGDOCOUTLINE_H__F56BA407_BE35_447F_A308_D503DE5F35C2__INCLUDED_)
#define AFX_DLGDOCOUTLINE_H__F56BA407_BE35_447F_A308_D503DE5F35C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDocOutline.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgDocOutline dialog
#include <mshtml.h>
class CDlgDocOutline : public CDialog
{
// Construction
public:
	CDlgDocOutline(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgDocOutline)
	enum { IDD = IDD_DLG_DOC_OUTLINE };
	CTreeCtrl	m_wndTree;
	//}}AFX_DATA
	//document interface if the document to be diaplayed
	CComPtr<IHTMLDocument2>	m_pHtmlDoc;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDocOutline)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//insert child nodes and attribute nodes for given node
	void	PopulateTree(
		HTREEITEM hParent,
		LPDISPATCH lpParentDisp);
	//get readable information from node 
	CString	GetNodeInfo(LPDISPATCH lpDisp);
	//insert a node 
	HTREEITEM InsertItem(HTREEITEM hParent,
		LPDISPATCH lpDisp);
	//build document outline
	void	BuildDocOutline();
	// Generated message map functions
	//{{AFX_MSG(CDlgDocOutline)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonEditOuthtml();
	afx_msg void OnButtonEditFrameSource();
	afx_msg void OnDeleteitemTree(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	//idle update support
	afx_msg LRESULT	OnKickIdle(WPARAM wp,LPARAM lp);
	afx_msg void OnUpdateButtonEditOuthtml(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonEditFrameSource(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDOCOUTLINE_H__F56BA407_BE35_447F_A308_D503DE5F35C2__INCLUDED_)
