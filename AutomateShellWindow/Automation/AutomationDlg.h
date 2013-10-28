// AutomationDlg.h : header file
//

#pragma once
#include "afxtempl.h"

typedef	CComQIPtr<IWebBrowser2> CComQIPtrIWebBrowser2;
// CAutomationDlg dialog
class CAutomationDlg : public CDialog
{
// Construction
public:
	CAutomationDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_AUTOMATION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//create a new shell window 
	//an Internet Explorer window if bIE is not zero
	//an Windows Explorer window otherwise.
	void	CreateNewShellWindow(BOOL bIE);
	//Attach to the active shell window 
	//an Internet Explorer window if bIE is not zero
	//an Windows Explorer window otherwise.
	void	AttachShellWindow(BOOL bIE);
	//IWebBrowser2 interface of the current attached shell window
	CComQIPtr<IWebBrowser2>	m_pWebBrowser2;
	//for DWebBrowserEvents2 events
	DWORD m_dwCookie;	
	void	AdviseSinkIE();
	void	UnadvisesinkIE();
	afx_msg void DocumentComplete(IDispatch *pDisp,VARIANT *URL);
	afx_msg void OnQuit();

	//true if the shell window is created by the program, 
	//false if it is attached to an existing window
	BOOL	m_bOwnIE;
	//demostrate some simple operation
	void	NavigateToSamplePage(BOOL bIE);
	//return the window class name of a given shell window
	CString	GetWindowClassName(IWebBrowser2* pwb);
	//starting URL or path for new shell windows
	CString m_strFileToFind;
	//for DShellWindowsEvents events
	DWORD m_dwCookieShellWindows;
	void WindowRegistered(long lCookie);
//	void WindowRevoked(long lCookie);
/*	DWORD m_hExplorerProcess;
    static BOOL CALLBACK EnumWindowsProc(HWND hwnd,LPARAM lParam);*/

	//state data for the WindowRegistered event handler

	//the current Shell Windows collection
	CComQIPtr<IShellWindows>	m_pShellWindows;
	typedef	CComQIPtr<IDispatch> CComQIPtrIDispatch;
	//list of previous shell windows to determin the new Windows Explorer window
	CTypedPtrList<CPtrList,CComQIPtrIDispatch*> m_listShellWindows;
	//shell item list to demostrate shell view selection .
	LPITEMIDLIST  m_pidlToNavigate;	
// Implementation
protected:
	HICON m_hIcon;
	//DShellWindowsEvents
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	DECLARE_DISPATCH_MAP();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonNewie();
	afx_msg void OnBnClickedButtonCurrentIe();
	afx_msg void OnBnClickedButtonNewExplorer();
	afx_msg void OnBnClickedButtonCurrentExplorer();
};
