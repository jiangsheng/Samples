#if !defined(AFX_DLGAUTHENTICATE_H__137B2627_D7B0_4C4F_84B2_99B25948578F__INCLUDED_)
#define AFX_DLGAUTHENTICATE_H__137B2627_D7B0_4C4F_84B2_99B25948578F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAuthenticate.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAuthenticate dialog
//input password for KB Q329802 
class CDlgAuthenticate : public CDialog
{
// Construction
public:
	CDlgAuthenticate(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgAuthenticate)
	enum { IDD = IDD_DLG_AUTHENTICATE };
	CString	m_strUserName;
	CString	m_strPassword;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAuthenticate)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAuthenticate)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGAUTHENTICATE_H__137B2627_D7B0_4C4F_84B2_99B25948578F__INCLUDED_)
