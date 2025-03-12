#if !defined(AFX_SEARCHOPTIONDLG_H__319B7C79_2B39_4376_B6D5_BA092B5FC9DA__INCLUDED_)
#define AFX_SEARCHOPTIONDLG_H__319B7C79_2B39_4376_B6D5_BA092B5FC9DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SearchOptionDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSearchOptionDlg dialog

class CSearchOptionDlg : public CDialog
{
// Construction
public:
	CSearchOptionDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSearchOptionDlg)
	enum { IDD = IDD_DIALOG_SEARCH_OPTIONS };
	CString	m_strQuery;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSearchOptionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSearchOptionDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEARCHOPTIONDLG_H__319B7C79_2B39_4376_B6D5_BA092B5FC9DA__INCLUDED_)
