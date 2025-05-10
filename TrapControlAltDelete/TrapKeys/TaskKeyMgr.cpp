////////////////////////////////////////////////////////////////
// MSDN Magazine -- September 2002
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
// Compiles with Visual Studio 6.0 and Visual Studio .NET on Windows XP.
//

#include "StdAfx.h"
#include "TaskKeyMgr.h"
#include "hook.h"
#define HKCU HKEY_CURRENT_USER

// Magic registry key/value for "Remove Task Manager" policy.
//
LPCTSTR KEY_DisableTaskMgr =
	"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System";
LPCTSTR VAL_DisableTaskMgr = "DisableTaskMgr";


//////////////////
// Disable task-key related stuff.
// 
// dwFlags  = what to disable
// bDisable = disable (TRUE) or enable (FALSE)
// bBeep    = whether to beep for illegal keys (TASKKEYS only)
//
void CTaskKeyMgr::Disable(DWORD dwFlags, BOOL bDisable, BOOL bBeep)
{
	// task manager (Ctrl+Alt+Del)
	if (dwFlags & TASKMGR) {
		HKEY hk;
		if (RegOpenKey(HKCU, KEY_DisableTaskMgr,&hk)!=ERROR_SUCCESS)
			RegCreateKey(HKCU, KEY_DisableTaskMgr, &hk);

		if (bDisable) { // disable TM: set policy = 1
			DWORD val=1;
			RegSetValueEx(hk, VAL_DisableTaskMgr, NULL,
				REG_DWORD, (BYTE*)&val, sizeof(val));

		} else { // enable TM: remove policy 
			RegDeleteValue(hk,VAL_DisableTaskMgr);
		}
	}

	// task keys (Alt-TAB etc)
	if (dwFlags & TASKKEYS)
		::DisableTaskKeys(bDisable,bBeep); // install keyboard hook

	// task bar
	if (dwFlags & TASKBAR) {
		HWND hwnd = FindWindow("Shell_traywnd", NULL);
		EnableWindow(hwnd, !bDisable);
	}
	if (dwFlags & TASKLIST) {
		if(bDisable&&!IsProcessListItemDisabled()){
			//only work for windows 2000
			int (_cdecl *pfnHook)(DWORD);
	
			hMyMyNtQuerySystemInformation= LoadLibrary(_T("HookNTQSI.dll"));
			if(hMyMyNtQuerySystemInformation)
			{
				pfnHook = (int(*)(DWORD))GetProcAddress(hMyMyNtQuerySystemInformation,"Hook");
				pfnHook(GetCurrentProcessId());
			}
			else
			{
				MessageBox(NULL,(_T("Failed to load HookNTQSI.dll.Err =") + SysErrorMessage(GetLastError())),NULL,MB_OK | MB_APPLMODAL | MB_ICONWARNING);
			}
		}
		if(!bDisable&&IsProcessListItemDisabled()){
			int (_cdecl *pfnUnhook)();
			if(hMyMyNtQuerySystemInformation)
			{
				pfnUnhook = (int(*)())GetProcAddress(hMyMyNtQuerySystemInformation,"Unhook");
				pfnUnhook();
				FreeLibrary(hMyMyNtQuerySystemInformation);
				hMyMyNtQuerySystemInformation=NULL;
			}			
		}
	}

	if (dwFlags & CTRLALTDEL) {
		if(bDisable&&!IsCtrlAltDeleteDisabled()){
			InjectFunc();
			bInjectFuncLoaded=TRUE;
		}
		if(!bDisable&&IsCtrlAltDeleteDisabled()){
			DeinjectFunc();
			bInjectFuncLoaded=FALSE;
		}

	}
}

BOOL CTaskKeyMgr::IsTaskBarDisabled()
{
	HWND hwnd = FindWindow("Shell_traywnd", NULL);
	return IsWindow(hwnd) ? !IsWindowEnabled(hwnd) : TRUE;
}

BOOL CTaskKeyMgr::IsTaskMgrDisabled()
{
	HKEY hk;
	if (RegOpenKey(HKCU, KEY_DisableTaskMgr, &hk)!=ERROR_SUCCESS)
		return FALSE; // no key ==> not disabled

	DWORD val=0;
	DWORD len=4;
	return RegQueryValueEx(hk, VAL_DisableTaskMgr,
		NULL, NULL, (BYTE*)&val, &len)==ERROR_SUCCESS && val==1;
}

BOOL CTaskKeyMgr::IsCtrlAltDeleteDisabled(){return bInjectFuncLoaded;}
BOOL CTaskKeyMgr::IsProcessListItemDisabled(){return hMyMyNtQuerySystemInformation!=NULL;}
BOOL CTaskKeyMgr::bInjectFuncLoaded=FALSE;
HINSTANCE	CTaskKeyMgr::hMyMyNtQuerySystemInformation=NULL;
CString CTaskKeyMgr::strRemoteDllName;
