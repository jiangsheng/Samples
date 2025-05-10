////////////////////////////////////////////////////////////////
// MSDN Magazine -- September 2002
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
// Compiles with Visual Studio 6.0 and Visual Studio .NET on Windows XP.
//
#include "stdafx.h"
#include "resource.h"
#include "StatLink.h"
#include "TaskKeyMgr.h"
#include <shlwapi.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//begin add by jiangsheng 2002-11-3
//from Display and Print Devices: Windows DDK
//The following function determines 
//whether the current operating system version is Windows 2000.
BOOL  Is_Win2000()
{
  OSVERSIONINFOEX osvi;
  DWORDLONG dwlConditionMask = 0;

  // Initialize the OSVERSIONINFOEX structure.

  ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
  osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
  osvi.dwMajorVersion = 5;
  osvi.dwMinorVersion = 0;

  // Initialize the condition mask.
  VER_SET_CONDITION( dwlConditionMask, VER_MAJORVERSION, VER_EQUAL );
  VER_SET_CONDITION( dwlConditionMask, VER_MINORVERSION, VER_EQUAL );

  // Perform the test.
  return VerifyVersionInfo(
      &osvi,
      VER_MAJORVERSION | VER_MINORVERSION,
      dwlConditionMask);
}
  
//Platform SDK: Windows System Information 
//The following example determines whether the application is running on 
//Windows 2000 Service Pack 1 or later.
BOOL Is_Win2000_SP1_or_Later () 
{
   OSVERSIONINFOEX osvi;
   DWORDLONG dwlConditionMask = 0;

   // Initialize the OSVERSIONINFOEX structure.

   ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
   osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
   osvi.dwMajorVersion = 5;
   osvi.dwMinorVersion = 0;
   osvi.wServicePackMajor = 1;

   // Initialize the condition mask.

   VER_SET_CONDITION( dwlConditionMask, VER_MAJORVERSION, 
      VER_GREATER_EQUAL );
   VER_SET_CONDITION( dwlConditionMask, VER_MINORVERSION, 
      VER_GREATER_EQUAL );
   VER_SET_CONDITION( dwlConditionMask, VER_SERVICEPACKMAJOR, 
      VER_GREATER_EQUAL );

   // Perform the test.

   return VerifyVersionInfo(
      &osvi, 
      VER_MAJORVERSION | VER_MINORVERSION | VER_SERVICEPACKMAJOR,
      dwlConditionMask);
}
//Testing for other conditions involves a simple modification to this code. 
//For example, the following code verifies that the application is running on 
//Windows 2000 Professional.

BOOL Is_Win2000_Professional () 
{
   OSVERSIONINFOEX osvi;
   DWORDLONG dwlConditionMask = 0;

   // Initialize the OSVERSIONINFOEX structure.

   ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
   osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
   osvi.dwMajorVersion = 5;
   osvi.wProductType = VER_NT_WORKSTATION;

   // Initialize the condition mask.

   VER_SET_CONDITION( dwlConditionMask, VER_MAJORVERSION, 
      VER_GREATER_EQUAL );
   VER_SET_CONDITION( dwlConditionMask, VER_PRODUCT_TYPE, 
      VER_EQUAL );

   // Perform the test.

   return VerifyVersionInfo(
      &osvi, 
      VER_MAJORVERSION | VER_PRODUCT_TYPE,
      dwlConditionMask);
}
//Finally, the following example tests whether Terminal Services is installed.

BOOL Is_Terminal_Services () 
{
   OSVERSIONINFOEX osvi;
   DWORDLONG dwlConditionMask = 0;

   // Initialize the OSVERSIONINFOEX structure.

   ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
   osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
   osvi.wSuiteMask = VER_SUITE_TERMINAL;

   // Initialize the condition mask.

   VER_SET_CONDITION( dwlConditionMask, VER_SUITENAME, 
      VER_AND );

   // Perform the test.

   return VerifyVersionInfo(
      &osvi, 
      VER_SUITENAME,
      dwlConditionMask);
}
//end add by jiangsheng 2002-11-3

//////////////////
// Main dialog
//
class CMyDialog : public CDialog {
public:
	CMyDialog(CWnd* pParent = NULL) : CDialog(IDD_MYDIALOG, pParent) { }

protected:
	HICON m_hIcon;
	CStaticLink	m_wndLink1;
	CStaticLink	m_wndLink2;
	CStaticLink	m_wndLink3;

	virtual BOOL OnInitDialog();

	// command/UI update handlers
	afx_msg void OnDisableTaskMgr();
	afx_msg void OnDisableTaskKeys();
	afx_msg void OnDisableTaskbar();
	afx_msg void OnDisableProcessListItem();
	afx_msg void OnDisableCtrlAltDelete();
	afx_msg void OnUpdateDisableTaskMgr(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDisableTaskKeys(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDisableTaskbar(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDisableProcessListItem(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDisableCtrlAltDelete(CCmdUI* pCmdUI);
	afx_msg LRESULT OnKickIdle(WPARAM,LPARAM);

	DECLARE_MESSAGE_MAP()
};

//////////////////
// Standard MFC dialog app class in 14 lines + comments
//
class CMyApp : public CWinApp {
public:
	virtual BOOL InitInstance() {
		// init app: run dialog
		//begin add by jiangsheng 2002-11-3
		const char* const RemoteDllName = "RemoteDll.Dll";
		TCHAR szEXEPath[_MAX_PATH];
		GetModuleFileName(NULL,szEXEPath,_MAX_PATH);
		::PathRemoveFileSpec(szEXEPath);
		::PathAppend(szEXEPath,RemoteDllName);
		CTaskKeyMgr::strRemoteDllName=szEXEPath;
		if(!Is_Win2000()){
			::AfxMessageBox("Must run on Windows 2000 or later!");
			return FALSE;
		}
		//end add by jiangsheng 2002-11-3
		CMyDialog dlg;
		m_pMainWnd = &dlg;
		dlg.DoModal();
		return FALSE;
	}
	virtual int ExitInstance() {
		// For safety, re-enable everything on exit
		CTaskKeyMgr::Disable(CTaskKeyMgr::ALL, FALSE);
		return 0;
	}
} theApp;

BEGIN_MESSAGE_MAP(CMyDialog, CDialog)
	ON_COMMAND(IDC_DISABLE_TASKKEYS,OnDisableTaskKeys)
	ON_COMMAND(IDC_DISABLE_TASKBAR, OnDisableTaskbar)
	ON_COMMAND(IDC_DISABLE_TASKMGR, OnDisableTaskMgr)
	ON_COMMAND(IDC_PROCESS_VISIBLE, OnDisableProcessListItem)
	ON_COMMAND(IDC_CTRLALTDELETE, OnDisableCtrlAltDelete)
	ON_UPDATE_COMMAND_UI(IDC_DISABLE_TASKKEYS, OnUpdateDisableTaskKeys)
	ON_UPDATE_COMMAND_UI(IDC_DISABLE_TASKBAR, OnUpdateDisableTaskbar)
	ON_UPDATE_COMMAND_UI(IDC_DISABLE_TASKMGR, OnUpdateDisableTaskMgr)
	ON_UPDATE_COMMAND_UI(IDC_PROCESS_VISIBLE, OnUpdateDisableProcessListItem)
	ON_UPDATE_COMMAND_UI(IDC_PROCESS_VISIBLE, OnUpdateDisableCtrlAltDelete)
	ON_MESSAGE(WM_KICKIDLE,OnKickIdle)
END_MESSAGE_MAP()

//////////////////
// Initialize dialog: subclass hyperlinks and load icon
//
BOOL CMyDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// init my hyperlinks
	m_wndLink1.SubclassDlgItem(IDC_PDURL,this);
	m_wndLink2.SubclassDlgItem(IDC_MSDNURL,this);
	m_wndLink3.SubclassDlgItem(IDC_MSDNLINK,this);

	// Set the icon for this dialog. MFC doesn't do it for dialog apps.
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	return TRUE;
}

////////////////////////////////////////////////////////////////
// Command/UI update handlers. You can write this n your sleep--Zzzz.

void CMyDialog::OnDisableTaskKeys()
{
	CTaskKeyMgr::Disable(CTaskKeyMgr::TASKKEYS,
		!CTaskKeyMgr::AreTaskKeysDisabled(), TRUE); // beep
}

void CMyDialog::OnUpdateDisableTaskKeys(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(CTaskKeyMgr::AreTaskKeysDisabled());
}

void CMyDialog::OnDisableTaskbar()
{
	CTaskKeyMgr::Disable(CTaskKeyMgr::TASKBAR,
		!CTaskKeyMgr::IsTaskBarDisabled());
}

void CMyDialog::OnUpdateDisableTaskbar(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(CTaskKeyMgr::IsTaskBarDisabled());
}

void CMyDialog::OnDisableTaskMgr()
{
	CTaskKeyMgr::Disable(CTaskKeyMgr::TASKMGR,
		!CTaskKeyMgr::IsTaskMgrDisabled());
}

void CMyDialog::OnUpdateDisableTaskMgr(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(CTaskKeyMgr::IsTaskMgrDisabled());
}

void CMyDialog::OnDisableProcessListItem(){
	CTaskKeyMgr::Disable(CTaskKeyMgr::TASKLIST,
		!CTaskKeyMgr::IsProcessListItemDisabled());
}
void CMyDialog::OnUpdateDisableProcessListItem(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(CTaskKeyMgr::IsProcessListItemDisabled());
}

void CMyDialog::OnDisableCtrlAltDelete(){
	CTaskKeyMgr::Disable(CTaskKeyMgr::CTRLALTDEL,
		!CTaskKeyMgr::IsCtrlAltDeleteDisabled());
}
void CMyDialog::OnUpdateDisableCtrlAltDelete(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(CTaskKeyMgr::IsCtrlAltDeleteDisabled());
}
//////////////////
// This is required to make ON_UPDATE_COMMAND_UI work properly.
// Cf. my column in the July 1997 Microsoft Systems Journal.
// 
LRESULT CMyDialog::OnKickIdle(WPARAM wp, LPARAM lCount)
{
	UpdateDialogControls(this, TRUE);
	return 0;
}
