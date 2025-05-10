////////////////////////////////////////////////////////////////
// MSDN Magazine -- September 2002
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
// Compiles with Visual Studio 6.0 and Visual Studio .NET on Windows XP.
//
#pragma once
#include "TaskKeyHook.h"

//////////////////
// Use this class to disable task keys, task manager and/or the taskbar.
// Call Disable with flags for items you want to disable; for example:
//
// CTaskMgrKeys::Disable(CTaskMgrKeys::ALL);
// 
class CTaskKeyMgr {
public:
	enum {
		TASKMGR  = 0x01,	// disable task manager (Ctrl+Alt+Del)
		TASKKEYS = 0x02,	// disable task keys (Alt-TAB, etc)
		TASKBAR  = 0x04,	// disable task bar
		TASKLIST  = 0x08,	// disable listing process list
		CTRLALTDEL  = 0x10,	// disable Ctrl+Alt+Del
		ALL=0xFFFF			// disable everything :(
	};
	static void Disable(DWORD dwItem,BOOL bDisable,BOOL bBeep=FALSE);

	static BOOL IsTaskMgrDisabled();
	static BOOL IsTaskBarDisabled();
	static BOOL IsCtrlAltDeleteDisabled();
	static BOOL IsProcessListItemDisabled();
	static BOOL AreTaskKeysDisabled() {
		return ::AreTaskKeysDisabled(); // call DLL 
	}
	static	CString strRemoteDllName;
protected:
	static	BOOL bInjectFuncLoaded;
	static	HINSTANCE	hMyMyNtQuerySystemInformation;
};


