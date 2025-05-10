#if !defined(AFX_DLGINPUTTEXT_H__6D01B831_43AD_47BD_B32E_63C4CABF93DF__INCLUDED_)
#define AFX_DLGINPUTTEXT_H__6D01B831_43AD_47BD_B32E_63C4CABF93DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgInputText.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgInputText dialog

class CDlgInputText : public CDialog
{
// Construction
public:
	CDlgInputText(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgInputText)
	enum { IDD = IDD_DLG_INPUT_TEXT };
	CString	m_strText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgInputText)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgInputText)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGINPUTTEXT_H__6D01B831_43AD_47BD_B32E_63C4CABF93DF__INCLUDED_)
