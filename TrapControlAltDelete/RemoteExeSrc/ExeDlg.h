// ExeDlg.h : header file
//

#if !defined(AFX_EXEDLG_H__64E88DD0_D099_4037_9AF2_7DAC677EEF66__INCLUDED_)
#define AFX_EXEDLG_H__64E88DD0_D099_4037_9AF2_7DAC677EEF66__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CExeDlg dialog

class CExeDlg : public CDialog
{
// Construction
public:
	CExeDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CExeDlg)
	enum { IDD = IDD_EXE_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CExeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOkBtn();
	afx_msg void OnCancelBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXEDLG_H__64E88DD0_D099_4037_9AF2_7DAC677EEF66__INCLUDED_)
