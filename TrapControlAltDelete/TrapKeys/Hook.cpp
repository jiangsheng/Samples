//---------------------------------------------------------------------------
//作者    ：韦覃武,jiangsheng
//网上呢称：BCB_FANS(四大名捕之追杀令)（此为CSDN和www.driverdevelop.com之帐号）jiangsheng（此为CSDN帐号）
//E-Mail  ：slwqw@163.com
//日期    ：2002-10-20
//2002-11-5 jingsheng修改
//功能    ：在2000下屏蔽Ctrl + Alt + Del组合键。（在Windows 2000 Professional SP3
//          中文版平台下面测试通过）
//原理    ：采用远程线程注入技术，装载一个DLL到Winlogon进程，然后截获SAS窗口的窗
//          口过程，接管WM_HOTKEY消息，以达到屏蔽Ctrl + Alt + Del之目的。
//开发语言：Borland C++Builder 5.0 Patch2，Visual C++ 6.0 SP5
//技术比较：关于在2000下面如何屏蔽Ctrl + Alt + Del组合键，一种常被提到的解决方法就
//	    是使用自己写的GINA去替换MSGINA.DLL，然后在WlxLoggedOnSAS里边直接返回
//	    WLX_SAS_ACTION_NONE。嘿嘿，说到底这并不是真正地屏蔽了这个组合键，只是
//	    直接返回WLX_SAS_ACTION_NONE时，Winlogon进程又自动从"Winlogon"桌面切换
//	    回原来的"Default"桌面了，而不是显示安全对话框，所以看起来被屏蔽了：），
//	    使用那种方法明显地看到桌面在闪烁！但是使用本文的方法时，你不会看到任
//	    何闪烁！
//鸣谢    ：www.driverdevelop.com上的icube和lu0。
//版权    ：转载请注明原作者：）

//---------------------------------------------------------------------------

#include "stdafx.h"
#include <tlhelp32.h>
#include <lmerr.h>

#include "Hook.h"
//add by jiangsheng 2002-11-5
#include "TaskKeyMgr.h"
#include "Wrappers.h"
extern	BOOL Is_Terminal_Services () ;
//end add by jiangsheng 2002-11-5
//---------------------------------------------------------------------------
//错误代码格式化函数
//replaced by jiangsheng 2002-11-5 
CString __fastcall SysErrorMessage(DWORD dwLastError )
{
	CString strRet(_T("Unknown error"));
	HMODULE hModule = NULL; // default to system source
    LPSTR MessageBuffer;
    DWORD dwBufferLength;

    DWORD dwFormatFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_IGNORE_INSERTS |
        FORMAT_MESSAGE_FROM_SYSTEM ;

    //
    // If dwLastError is in the network range, 
    //  load the message source.
    //

    if(dwLastError >= NERR_BASE && dwLastError <= MAX_NERR) {
        hModule = LoadLibraryEx(
            TEXT("netmsg.dll"),
            NULL,
            LOAD_LIBRARY_AS_DATAFILE
            );

        if(hModule != NULL)
            dwFormatFlags |= FORMAT_MESSAGE_FROM_HMODULE;
    }

    //
    // Call FormatMessage() to allow for message 
    //  text to be acquired from the system 
    //  or from the supplied module handle.
    //

    if(dwBufferLength = FormatMessageA(
        dwFormatFlags,
        hModule, // module to get message from (NULL == system)
        dwLastError,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // default language
        (LPSTR) &MessageBuffer,
        0,
        NULL
        ))
    {

        //
        // Output message string on stderr.
        //
		strRet=CString(MessageBuffer,dwBufferLength);
        //
        // Free the buffer allocated by the system.
        //
        LocalFree(MessageBuffer);
    }

    //
    // If we loaded a message source, unload it.
    //
    if(hModule != NULL)
        FreeLibrary(hModule);
	return strRet;
}
//end replaced by jiangsheng 2002-11-5 
//---------------------------------------------------------------------------

#ifdef UNICODE
	LPCSTR  LoadLibraryFuncStr = "LoadLibraryW";
	LPCSTR  GetModuleHandleFuncStr = "GetModuleHandleW";
#else
	LPCSTR  LoadLibraryFuncStr = "LoadLibraryA";
	LPCSTR  GetModuleHandleFuncStr = "GetModuleHandleA";
#endif
LPCSTR  FreeLibraryFuncStr = "FreeLibrary";
LPCSTR  GetProcAddressFuncStr = "GetProcAddress";
LPCSTR  GetLastErrorFuncStr = "GetLastError";

//---------------------------------------------------------------------------
//removed by jiangsheng 2002-11-5
//const char* const RemoteDllName = "RemoteDll.Dll";
//end removed by jiangsheng 2002-11-5
LPCTSTR szRemoteProcessName = "Winlogon.exe";

typedef HINSTANCE (WINAPI *PLOADLIBRARY)(LPCTSTR );
typedef BOOL (WINAPI *PFREELIBRARY)(HINSTANCE);
typedef HMODULE (WINAPI* PGETMODULEHANDLE)(LPCTSTR );
typedef PVOID (WINAPI* PGETPROCADDRESS)(HINSTANCE,LPCSTR);
typedef DWORD (WINAPI* PGETLASTERROR)(VOID);

BOOL  __fastcall EnablePrivilege(LPCTSTR lpszPrivilegeName,BOOL bEnable);
DWORD __fastcall GetPIDFromName(LPCTSTR lpszProcName);

//---------------------------------------------------------------------------

typedef struct
{
	PLOADLIBRARY pfnLoadLibrary;
	PGETLASTERROR pfnGetLastError;
	TCHAR	szDllName[1024];
	DWORD	dwReturnValue;	
} INJECTLIBINFO;

typedef struct
{
	PFREELIBRARY pfnFreeLibrary;
	PGETMODULEHANDLE pfnGetModuleHandle;
	PGETLASTERROR pfnGetLastError;

	DWORD	dwReturnValue;
	TCHAR	szDllName[1024];

} DEINJECTLIBINFO;

//---------------------------------------------------------------------------
//远程线程，用来装载DLL
static DWORD WINAPI ThreadFuncAttach(INJECTLIBINFO *pInfo)
{
  HINSTANCE hDll=NULL;
	pInfo->dwReturnValue = 0;
  hDll = (HINSTANCE)pInfo->pfnLoadLibrary(pInfo->szDllName);
  if(hDll == NULL)
    pInfo->dwReturnValue = pInfo->pfnGetLastError();
  return((DWORD)hDll);
}

//---------------------------------------------------------------------------

static void AfterThreadFuncAttach(void)
{
}

//---------------------------------------------------------------------------
//远程线程，用来卸载DLL
static DWORD WINAPI ThreadFuncDetach(DEINJECTLIBINFO *pInfo)
{
  HINSTANCE hDll = NULL;
  BOOL bResult=FALSE;
  BOOL bHasFoundModule = FALSE;

  pInfo->dwReturnValue = 0;//意味成功，如果这个值不是0，则是一个错误代码。

  while((hDll = pInfo->pfnGetModuleHandle(pInfo->szDllName)) != NULL)
  {
    bHasFoundModule = TRUE;

    bResult = pInfo->pfnFreeLibrary(hDll);
    if(bResult == FALSE)
    {
      pInfo->dwReturnValue = pInfo->pfnGetLastError();
      break;
    }
  }

  if(pInfo->dwReturnValue == 0 && !bHasFoundModule)
  {
    pInfo->dwReturnValue = pInfo->pfnGetLastError();
  }

  return 1;
}

//---------------------------------------------------------------------------

static void AfterThreadFuncDetach(void)
{
}

//---------------------------------------------------------------------------
//EnablePrivilege
BOOL __fastcall EnablePrivilege(LPCTSTR	lpszPrivilegeName,BOOL bEnable)
{
  HANDLE hToken;
  TOKEN_PRIVILEGES tp;
  LUID luid;

  if(!OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES |
                       TOKEN_QUERY | TOKEN_READ,&hToken))
    return FALSE;

  if(!LookupPrivilegeValue(NULL, lpszPrivilegeName, &luid))
    return TRUE;

  tp.PrivilegeCount           = 1;
  tp.Privileges[0].Luid       = luid;
  tp.Privileges[0].Attributes = (bEnable) ? SE_PRIVILEGE_ENABLED : 0;

  AdjustTokenPrivileges(hToken,FALSE,&tp,NULL,NULL,NULL);

  CloseHandle(hToken);

  return (GetLastError() == ERROR_SUCCESS);
}
//---------------------------------------------------------------------------
//通过进程名称得到进程的ID（这里使用方法Toolhelp函数，因此在NT上无法使用，当然也
//可以PSAPI函数，这样就可以在NT上使用了）
//GetPIDFromName
DWORD __fastcall GetPIDFromName(LPCTSTR	lpszProcName)
{
//  CString Str;
  HANDLE hSnapshot;
  PROCESSENTRY32 ProcStruct;
  DWORD dwProcessID = -1;
  //added by jiangsheng 2002-11-8
  BOOL	bIsTerminalServices=Is_Terminal_Services();
  if(bIsTerminalServices){
	//get current session ID
	CWTSWrapper	WTS;
	if (WTS.IsValid())
	{
		DWORD	dwCurSessionID = -1;
		LPTSTR  pSessionInfo=NULL;
		DWORD   dwBytes;
  		if(WTS.WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE,WTS_CURRENT_SESSION,
			WTSSessionId, (LPTSTR*)&pSessionInfo, &dwBytes)){

			dwCurSessionID =*((DWORD*)pSessionInfo);
			// enumerate processes
			PWTS_PROCESS_INFO pProcessInfo = NULL;
			DWORD             ProcessCount = 0;
			BOOL bFound;
			if (WTS.WTSEnumerateProcesses(WTS_CURRENT_SERVER_HANDLE, 0, 1, 
				&pProcessInfo, &ProcessCount)){
				for (DWORD CurrentProcess = 0; CurrentProcess < ProcessCount; CurrentProcess++){
					CString strCurExePath(pProcessInfo[CurrentProcess].pProcessName);
					CString strRemoteProc(lpszProcName);
					strCurExePath.MakeLower();
					strRemoteProc.MakeLower();
					bFound = (strCurExePath.Find(strRemoteProc) != -1);
					if(bFound	&&	dwCurSessionID==pProcessInfo[CurrentProcess].SessionId)	{
						dwProcessID = pProcessInfo[CurrentProcess].ProcessId;
						break;
					}
				}
			}
			WTS.WTSFreeMemory(pSessionInfo);
		}
	}
  }
  else{
//end added by jiangsheng 2002-11-8
	  BOOL bResult;

	  hSnapshot = CreateToolhelp32Snapshot((DWORD)TH32CS_SNAPPROCESS,0);
	  ProcStruct.dwSize = sizeof(PROCESSENTRY32);
	  bResult = Process32First(hSnapshot,&ProcStruct);
	  while(bResult)
	  {
		BOOL bFound;

		CString strCurExePath(ProcStruct.szExeFile);
		CString strRemoteProc(lpszProcName);
    
		strCurExePath.MakeLower();
		strRemoteProc.MakeLower();

		bFound = (strCurExePath.Find(strRemoteProc) != -1);

		if(bFound)
		{
			dwProcessID = ProcStruct.th32ProcessID;
			break;
		}
		bResult = Process32Next(hSnapshot,&ProcStruct);
	  }
	  CloseHandle(hSnapshot);
  }

  return dwProcessID;
}



//---------------------------------------------------------------------------
//                            插入代码
//---------------------------------------------------------------------------
//InjectFunc
void __fastcall InjectFunc()
{
  HANDLE hRemoteProcess=NULL;
  DWORD  dwRemoteProcess=NULL;

  DWORD dwThreadSize=0;
  INJECTLIBINFO InjectLibInfo;
  PVOID pRemoteThread=NULL;
  PVOID pRemoteParam=NULL;
  DWORD dwWriten=0;
  DWORD dwRet=0;

  //提升本进程权限然后打开目的进程
  EnablePrivilege(SE_DEBUG_NAME,true);

  dwRemoteProcess = GetPIDFromName(szRemoteProcessName);
  if(dwRemoteProcess == (DWORD)-1)
  {
    MessageBox(NULL,_T("Failed to Query Process ID."),NULL,MB_OK | MB_APPLMODAL | MB_ICONWARNING);
    return;
  }
  hRemoteProcess = OpenProcess(PROCESS_ALL_ACCESS,false,dwRemoteProcess);
  if(hRemoteProcess == NULL)
  {
    MessageBox(NULL,_T("Failed to Open Process. Err = ") + SysErrorMessage(GetLastError()),
    	       NULL,MB_OK | MB_APPLMODAL | MB_ICONWARNING);
    return;
  }

  //初始化参数
  ZeroMemory(&InjectLibInfo,sizeof(INJECTLIBINFO ));
  InjectLibInfo.pfnLoadLibrary = (PLOADLIBRARY)GetProcAddress(GetModuleHandle("Kernel32.dll"),LoadLibraryFuncStr);
  InjectLibInfo.pfnGetLastError = (PGETLASTERROR)GetProcAddress(GetModuleHandle("Kernel32.dll"),GetLastErrorFuncStr);
  lstrcpyn(InjectLibInfo.szDllName,CTaskKeyMgr::strRemoteDllName,CTaskKeyMgr::strRemoteDllName.GetLength()+1);
  //在远程线程分配内存来存放参数
  pRemoteParam = VirtualAllocEx(hRemoteProcess,NULL,sizeof(INJECTLIBINFO),MEM_COMMIT,PAGE_READWRITE);
  if(pRemoteParam == NULL)
  {
    MessageBox(NULL,_T("Failed to Allocate Memory at Remote Process for Param.Err = ") + SysErrorMessage(GetLastError()),
    	       NULL,MB_OK | MB_APPLMODAL | MB_ICONWARNING);
    return;
  }
  dwRet = WriteProcessMemory(hRemoteProcess,pRemoteParam,(LPVOID)&InjectLibInfo,sizeof(INJECTLIBINFO),&dwWriten);
  if(dwRet == 0)
  {
    MessageBox(NULL,_T("Failed to Write Param to Remote Process.Err = ") + SysErrorMessage(GetLastError()),
    	       NULL,MB_OK | MB_APPLMODAL | MB_ICONWARNING);
    return;           
  }

  //拷贝线程体
  dwThreadSize = (int)AfterThreadFuncAttach - (int)ThreadFuncAttach + 1024 + sizeof(INJECTLIBINFO);

  pRemoteThread = VirtualAllocEx(hRemoteProcess,NULL,dwThreadSize,MEM_COMMIT,PAGE_READWRITE);
  if(pRemoteThread == NULL) 
  {
    MessageBox(NULL,_T("Failed to Allocate Memory at Remote Process for Thread Code.Err = ") + SysErrorMessage(GetLastError()),
    	       NULL,MB_OK | MB_APPLMODAL | MB_ICONWARNING);
    return;
  }
  dwRet = WriteProcessMemory(hRemoteProcess,pRemoteThread,(LPVOID)ThreadFuncAttach,dwThreadSize,&dwWriten);
  if(dwRet == 0)
  {
    MessageBox(NULL,_T("Failed to Write Thread Code to Remote Process.Err = ") + SysErrorMessage(GetLastError()),
    	       NULL,MB_OK | MB_APPLMODAL | MB_ICONWARNING);
    return;
  }

  //启动远程线程
  HANDLE hRemoteThread;

  hRemoteThread = CreateRemoteThread(hRemoteProcess,0,0,(DWORD(__stdcall *)(VOID*))pRemoteThread,(INJECTLIBINFO*)pRemoteParam,0,&dwWriten);
  ::WaitForSingleObject(hRemoteThread,INFINITE);
//  CloseHandle(hRemoteThread);
  if(hRemoteThread == NULL)
  {
    MessageBox(NULL,_T("Failed to create unload thread.Err=") + SysErrorMessage(GetLastError()),NULL,MB_OK | MB_APPLMODAL | MB_ICONWARNING);
  }
  else
  {
//    CloseHandle(hRemoteThread);
  }

  //读卸载返回值
  dwRet = ReadProcessMemory(hRemoteProcess,pRemoteParam,(LPVOID)&InjectLibInfo,sizeof(INJECTLIBINFO),&dwWriten);
  if(dwRet == 0)
  {
    MessageBox(NULL,_T("Unable to read load return value.Err=") + SysErrorMessage(GetLastError()),
    	       NULL,MB_OK | MB_APPLMODAL | MB_ICONWARNING);
  }
  else
  {
    if(InjectLibInfo.dwReturnValue == 0)
    {
		;
    }
    else
    {
      MessageBox(NULL,_T("Failed to load library to Winlogon.Err=") + SysErrorMessage(InjectLibInfo.dwReturnValue),NULL,MB_OK | MB_APPLMODAL | MB_ICONWARNING);
    }
  }

  //恢复权限
  EnablePrivilege(SE_DEBUG_NAME,false);
  CloseHandle(hRemoteProcess);
}
//---------------------------------------------------------------------------
//                             卸载线程
//---------------------------------------------------------------------------
//DeinjectFunc
void __fastcall DeinjectFunc()
{
  HANDLE hRemoteProcess=NULL;
  DWORD  dwRemoteProcess=0;

  DWORD dwThreadSize=0;
  DEINJECTLIBINFO DeinjectLibInfo;
  
  PVOID pRemoteThread=NULL;
  PVOID pRemoteParam=NULL;
  DWORD dwWriten=0;
  DWORD Ret=0;

  //提升本进程权限然后打开目的进程
  EnablePrivilege(SE_DEBUG_NAME,true);

  dwRemoteProcess = GetPIDFromName(szRemoteProcessName);
  if(dwRemoteProcess == (DWORD)-1)
  {
    MessageBox(NULL,_T("Failed to Query Process ID."),NULL,MB_OK | MB_APPLMODAL | MB_ICONWARNING);
    return;
  }
  hRemoteProcess = OpenProcess(PROCESS_ALL_ACCESS,false,dwRemoteProcess);
  if(hRemoteProcess == NULL)
  {
    MessageBox(NULL,_T("Failed to Open Process. Err = ") + SysErrorMessage(GetLastError()),
    	       NULL,MB_OK | MB_APPLMODAL | MB_ICONWARNING);
    return;
  }

  //初始化参数
  ZeroMemory(&DeinjectLibInfo,sizeof(DEINJECTLIBINFO ));
  DeinjectLibInfo.pfnFreeLibrary = (PFREELIBRARY)GetProcAddress(GetModuleHandle("Kernel32.dll"),FreeLibraryFuncStr);
  DeinjectLibInfo.pfnGetModuleHandle = (PGETMODULEHANDLE)GetProcAddress(GetModuleHandle("Kernel32.dll"),GetModuleHandleFuncStr);
  DeinjectLibInfo.pfnGetLastError = (PGETLASTERROR)GetProcAddress(GetModuleHandle("Kernel32.dll"),GetLastErrorFuncStr);

  lstrcpyn(DeinjectLibInfo.szDllName,CTaskKeyMgr::strRemoteDllName,CTaskKeyMgr::strRemoteDllName.GetLength()+1);

  //在远程线程分配内存来存放参数
  pRemoteParam = VirtualAllocEx(hRemoteProcess,NULL,sizeof(DEINJECTLIBINFO),MEM_COMMIT,PAGE_READWRITE);
  if(pRemoteParam == NULL)
  {
    MessageBox(NULL,_T("Failed to Allocate Memory at Remote Process.Err = ") + SysErrorMessage(GetLastError()),
    	       NULL,MB_OK | MB_APPLMODAL | MB_ICONWARNING);
  }
  Ret = WriteProcessMemory(hRemoteProcess,pRemoteParam,(LPVOID)&DeinjectLibInfo,sizeof(DEINJECTLIBINFO),&dwWriten);
  if(Ret == 0)
  {
    MessageBox(NULL,_T("Failed to Write Param to Remote Process.Err = ") + SysErrorMessage(GetLastError()),
    	       NULL,MB_OK | MB_APPLMODAL | MB_ICONWARNING);
    return;
  }

  //拷贝线程体
  dwThreadSize = (int)AfterThreadFuncDetach - (int)ThreadFuncDetach + 1024 + sizeof(DEINJECTLIBINFO);

  pRemoteThread = VirtualAllocEx(hRemoteProcess,NULL,dwThreadSize,MEM_COMMIT,PAGE_READWRITE);
  if(pRemoteThread == NULL)
  {
    MessageBox(NULL,_T("Failed to Allocate Memory at Remote Process for Thread Code.Err = ") + SysErrorMessage(GetLastError()),
    	       NULL,MB_OK | MB_APPLMODAL | MB_ICONWARNING);
    return;
  }
  Ret = WriteProcessMemory(hRemoteProcess,pRemoteThread,(LPVOID)ThreadFuncDetach,dwThreadSize,&dwWriten);
  if(Ret == 0)
  {
    MessageBox(NULL,_T("Failed to Write Thread Code to Remote Process.Err = ") + SysErrorMessage(GetLastError()),
    	       NULL,MB_OK | MB_APPLMODAL | MB_ICONWARNING);
    return;
  }

  //启动远程线程
  HANDLE hRemoteThread;

  hRemoteThread = CreateRemoteThread(hRemoteProcess ,0,0,(DWORD(__stdcall *)(VOID*))pRemoteThread,(DEINJECTLIBINFO*)pRemoteParam,0,&dwWriten);
  if(hRemoteThread == NULL)
  {
    MessageBox(NULL,_T("Failed to create remote unload thread.Err=") + SysErrorMessage(GetLastError()),NULL,MB_OK | MB_APPLMODAL | MB_ICONWARNING);
  }
  else
  {
    CloseHandle(hRemoteThread);
    //MessageBox(NULL,"Create remote unload thread successful",NULL,MB_OK | MB_APPLMODAL | MB_ICONWARNING);
  }

  //读卸载返回值
  Ret = ReadProcessMemory(hRemoteProcess,pRemoteParam,(LPVOID)&DeinjectLibInfo,sizeof(DEINJECTLIBINFO),&dwWriten);
  if(Ret == 0)
  {
    MessageBox(NULL,_T("Unable to read unload return value.Err=") + SysErrorMessage(GetLastError()),
    	       NULL,MB_OK | MB_APPLMODAL | MB_ICONWARNING);
  }
  else
  {
    if(DeinjectLibInfo.dwReturnValue == 0)
    {

    }
    else
    {
      MessageBox(NULL,_T("Failed to unload .Err=")+ SysErrorMessage(DeinjectLibInfo.dwReturnValue),NULL,MB_OK | MB_APPLMODAL | MB_ICONWARNING);
    }
  }

  //恢复权限
  CloseHandle(hRemoteProcess);
  EnablePrivilege(SE_DEBUG_NAME,false);
}
//---------------------------------------------------------------------------
