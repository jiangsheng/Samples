//---------------------------------------------------------------------------
//作者    ：韦覃武
//网上呢称：BCB_FANS(四大名捕之追杀令)（此为CSDN和www.driverdevelop.com之帐号）
//E-Mail  ：slwqw@163.com
//日期    ：2002-10-20
//
//功能    ：在2000下屏蔽Ctrl + Alt + Del组合键。（在Windows 2000 Professional SP3
//          中文版平台下面测试通过）
//原理    ：采用远程线程注入技术，装载一个DLL到Winlogon进程，然后截获SAS窗口的窗
//          口过程，接管WM_HOTKEY消息，以达到屏蔽Ctrl + Alt + Del之目的。
//开发语言：Borland C++Builder 5.0 Patch2
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

#include "Hook.h"

//---------------------------------------------------------------------------
//错误代码格式化函数
CString __fastcall SysErrorMessage(int ErrorCode)
{
  char Buffer[256];

  FormatMessage(FORMAT_MESSAGE_ARGUMENT_ARRAY | FORMAT_MESSAGE_FROM_SYSTEM,
    NULL,ErrorCode,
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
    (LPTSTR)Buffer,sizeof(Buffer),NULL); 

  return CString(Buffer);
}
//---------------------------------------------------------------------------

#ifdef UNICODE

const char* const LoadLibraryFuncStr = "LoadLibraryW";
const char* const GetModuleHandleFuncStr = "GetModuleHandleW";

#else

const char* const LoadLibraryFuncStr = "LoadLibraryA";
const char* const GetModuleHandleFuncStr = "GetModuleHandleA";

#endif

const char* const FreeLibraryFuncStr = "FreeLibrary";
const char* const GetProcAddressFuncStr = "GetProcAddress";
const char* const GetLastErrorFuncStr = "GetLastError";

//---------------------------------------------------------------------------
const char* const RemoteDllName = "RemoteDll.Dll";
const char* const RemoteProcessName = "Winlogon.exe";

typedef HINSTANCE (WINAPI *PLOADLIBRARY)(LPBYTE);
typedef BOOL (WINAPI *PFREELIBRARY)(HINSTANCE);
typedef HMODULE (WINAPI* PGETMODULEHANDLE)(LPBYTE);
typedef PVOID (WINAPI* PGETPROCADDRESS)(HINSTANCE,LPBYTE);
typedef DWORD (WINAPI* PGETLASTERROR)(VOID);

BOOL  __fastcall EnablePrivilege(char* PrivilegeName,BOOL IsEnable);
DWORD __fastcall GetPIDFromName(char* ProcName);

//---------------------------------------------------------------------------

typedef struct
{
  PLOADLIBRARY pfnLoadLibrary;
  unsigned char DllName[1024];

} INJECTLIBINFO;

typedef struct
{
  PFREELIBRARY pfnFreeLibrary;
  PGETMODULEHANDLE pfnGetModuleHandle;
  PGETLASTERROR pfnGetLastError;

  DWORD dwReturnValue;
  unsigned char DllName[1024];

} DEINJECTLIBINFO;

//---------------------------------------------------------------------------
//远程线程，用来装载DLL
static DWORD WINAPI ThreadFuncAttach(INJECTLIBINFO *pInfo)
{
  HINSTANCE hDll;

  hDll = (HINSTANCE)pInfo->pfnLoadLibrary(pInfo->DllName);

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
  BOOL Result;
  bool HasFoundModule = false;

  pInfo->dwReturnValue = 0;//意味成功，如果这个值不是0，则是一个错误代码。

  while((hDll = pInfo->pfnGetModuleHandle(pInfo->DllName)) != NULL)
  {
    HasFoundModule = true;

    Result = pInfo->pfnFreeLibrary(hDll);
    if(Result == 0)
    {
      pInfo->dwReturnValue = pInfo->pfnGetLastError();
      break;
    }
  }

  if(pInfo->dwReturnValue == 0 && !HasFoundModule)
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
BOOL __fastcall EnablePrivilege(char *PrivilegeName,BOOL IsEnable)
{
  HANDLE hToken;
  TOKEN_PRIVILEGES tp;
  LUID luid;

  if(!OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES |
                       TOKEN_QUERY | TOKEN_READ,&hToken))
    return FALSE;

  if(!LookupPrivilegeValue(NULL, PrivilegeName, &luid))
    return TRUE;

  tp.PrivilegeCount           = 1;
  tp.Privileges[0].Luid       = luid;
  tp.Privileges[0].Attributes = (IsEnable) ? SE_PRIVILEGE_ENABLED : 0;

  AdjustTokenPrivileges(hToken,FALSE,&tp,NULL,NULL,NULL);

  CloseHandle(hToken);

  return (GetLastError() == ERROR_SUCCESS);
}
//---------------------------------------------------------------------------
//通过进程名称得到进程的ID（这里使用方法Toolhelp函数，因此在NT上无法使用，当然也
//可以PSAPI函数，这样就可以在NT上使用了）
//GetPIDFromName
DWORD __fastcall GetPIDFromName(const char *ProcName)
{
//  CString Str;
  HANDLE hSnapshot;
  PROCESSENTRY32 ProcStruct;
  DWORD ProcessID = -1;
  BOOL Result;

  hSnapshot = CreateToolhelp32Snapshot((DWORD)TH32CS_SNAPPROCESS,0);
  ProcStruct.dwSize = sizeof(PROCESSENTRY32);

  Result = Process32First(hSnapshot,&ProcStruct);
  while(Result)
  {
    BOOL IsFound;

    CString CurExePath(ProcStruct.szExeFile);
    CString RemoteProc(ProcName);
    
    CurExePath.MakeLower();
    RemoteProc.MakeLower();

    IsFound = (CurExePath.Find(RemoteProc) != -1);

    if(IsFound)
    {
      ProcessID = ProcStruct.th32ProcessID;
      break;
    }
    Result = Process32Next(hSnapshot,&ProcStruct);
  }
  CloseHandle(hSnapshot);

  return ProcessID;
}



//---------------------------------------------------------------------------
//                            插入代码
//---------------------------------------------------------------------------
//InjectFunc
void __fastcall InjectFunc()
{
  HANDLE hRemoteProcess;
  DWORD  dwRemoteProcess;

  DWORD dwThreadSize;
  INJECTLIBINFO InjectLibInfo;
  PVOID pRemoteThread;
  PVOID pRemoteParam;
  DWORD dwWriten;
  DWORD Ret;

  //提升本进程权限然后打开目的进程
  EnablePrivilege(SE_DEBUG_NAME,true);

  dwRemoteProcess = GetPIDFromName(RemoteProcessName);
  if(dwRemoteProcess == (DWORD)-1)
  {
    MessageBox(NULL,"Failed to Query Process ID.",NULL,MB_OK | MB_APPLMODAL | MB_ICONWARNING);
    return;
  }
  hRemoteProcess = OpenProcess(PROCESS_ALL_ACCESS,false,dwRemoteProcess);
  if(hRemoteProcess == NULL)
  {
    MessageBox(NULL,CString("Failed to Open Process. Err = " + SysErrorMessage(GetLastError())),
    	       NULL,MB_OK | MB_APPLMODAL | MB_ICONWARNING);
    return;
  }

  //初始化参数
  InjectLibInfo.pfnLoadLibrary = (PLOADLIBRARY)GetProcAddress(GetModuleHandle("Kernel32.dll"),
  				  LoadLibraryFuncStr);
  CopyMemory(InjectLibInfo.DllName,RemoteDllName,CString(RemoteDllName).GetLength());

  //在远程线程分配内存来存放参数
  pRemoteParam = VirtualAllocEx(hRemoteProcess,NULL,sizeof(INJECTLIBINFO),MEM_COMMIT,PAGE_READWRITE);
  if(pRemoteParam == NULL)
  {
    MessageBox(NULL,CString("Failed to Allocate Memory at Remote Process for Param.Err = " + SysErrorMessage(GetLastError())),
    	       NULL,MB_OK | MB_APPLMODAL | MB_ICONWARNING);
    return;
  }
  Ret = WriteProcessMemory(hRemoteProcess,pRemoteParam,(LPVOID)&InjectLibInfo,sizeof(INJECTLIBINFO),&dwWriten);
  if(Ret == 0)
  {
    MessageBox(NULL,CString("Failed to Write Param to Remote Process.Err = " + SysErrorMessage(GetLastError())),
    	       NULL,MB_OK | MB_APPLMODAL | MB_ICONWARNING);
    return;           
  }

  //拷贝线程体
  dwThreadSize = (int)AfterThreadFuncAttach - (int)ThreadFuncAttach + 1024 + sizeof(INJECTLIBINFO);

  pRemoteThread = VirtualAllocEx(hRemoteProcess,NULL,dwThreadSize,MEM_COMMIT,PAGE_READWRITE);
  if(pRemoteThread == NULL) 
  {
    MessageBox(NULL,CString("Failed to Allocate Memory at Remote Process for Thread Code.Err = " + SysErrorMessage(GetLastError())),
    	       NULL,MB_OK | MB_APPLMODAL | MB_ICONWARNING);
    return;
  }
  Ret = WriteProcessMemory(hRemoteProcess,pRemoteThread,(LPVOID)ThreadFuncAttach,dwThreadSize,&dwWriten);
  if(Ret == 0)
  {
    MessageBox(NULL,CString("Failed to Write Thread Code to Remote Process.Err = " + SysErrorMessage(GetLastError())),
    	       NULL,MB_OK | MB_APPLMODAL | MB_ICONWARNING);
    return;
  }

  //启动远程线程
  HANDLE hRemoteThread;

  hRemoteThread = CreateRemoteThread(hRemoteProcess,0,0,(DWORD(__stdcall *)(VOID*))pRemoteThread,(INJECTLIBINFO*)pRemoteParam,0,&dwWriten);
  CloseHandle(hRemoteThread);

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
  HANDLE hRemoteProcess;
  DWORD  dwRemoteProcess;

  DWORD dwThreadSize;
  DEINJECTLIBINFO DeinjectLibInfo;
  PVOID pRemoteThread;
  PVOID pRemoteParam;
  DWORD dwWriten;
  DWORD Ret;

  //提升本进程权限然后打开目的进程
  EnablePrivilege(SE_DEBUG_NAME,true);

  dwRemoteProcess = GetPIDFromName(RemoteProcessName);
  if(dwRemoteProcess == (DWORD)-1)
  {
    MessageBox(NULL,"Failed to Query Process ID.",NULL,MB_OK | MB_APPLMODAL | MB_ICONWARNING);
    return;
  }
  hRemoteProcess = OpenProcess(PROCESS_ALL_ACCESS,false,dwRemoteProcess);
  if(hRemoteProcess == NULL)
  {
    MessageBox(NULL,CString("Failed to Open Process. Err = " + SysErrorMessage(GetLastError())),
    	       NULL,MB_OK | MB_APPLMODAL | MB_ICONWARNING);
    return;
  }

  //初始化参数
  DeinjectLibInfo.pfnFreeLibrary = (PFREELIBRARY)GetProcAddress(GetModuleHandle("Kernel32.dll"),FreeLibraryFuncStr);
  DeinjectLibInfo.pfnGetModuleHandle = (PGETMODULEHANDLE)GetProcAddress(GetModuleHandle("Kernel32.dll"),GetModuleHandleFuncStr);
  DeinjectLibInfo.pfnGetLastError = (PGETLASTERROR)GetProcAddress(GetModuleHandle("Kernel32.dll"),GetLastErrorFuncStr);

  CopyMemory(DeinjectLibInfo.DllName,RemoteDllName,CString(RemoteDllName).GetLength());

  //在远程线程分配内存来存放参数
  pRemoteParam = VirtualAllocEx(hRemoteProcess,NULL,sizeof(DEINJECTLIBINFO),MEM_COMMIT,PAGE_READWRITE);
  if(pRemoteParam == NULL)
  {
    MessageBox(NULL,CString("Failed to Allocate Memory at Remote Process.Err = " + SysErrorMessage(GetLastError())),
    	       NULL,MB_OK | MB_APPLMODAL | MB_ICONWARNING);
  }
  Ret = WriteProcessMemory(hRemoteProcess,pRemoteParam,(LPVOID)&DeinjectLibInfo,sizeof(DEINJECTLIBINFO),&dwWriten);
  if(Ret == 0)
  {
    MessageBox(NULL,CString("Failed to Write Param to Remote Process.Err = " + SysErrorMessage(GetLastError())),
    	       NULL,MB_OK | MB_APPLMODAL | MB_ICONWARNING);
    return;
  }

  //拷贝线程体
  dwThreadSize = (int)AfterThreadFuncDetach - (int)ThreadFuncDetach + 1024 + sizeof(DEINJECTLIBINFO);

  pRemoteThread = VirtualAllocEx(hRemoteProcess,NULL,dwThreadSize,MEM_COMMIT,PAGE_READWRITE);
  if(pRemoteThread == NULL)
  {
    MessageBox(NULL,CString("Failed to Allocate Memory at Remote Process for Thread Code.Err = " + SysErrorMessage(GetLastError())),
    	       NULL,MB_OK | MB_APPLMODAL | MB_ICONWARNING);
    return;
  }
  Ret = WriteProcessMemory(hRemoteProcess,pRemoteThread,(LPVOID)ThreadFuncDetach,dwThreadSize,&dwWriten);
  if(Ret == 0)
  {
    MessageBox(NULL,CString("Failed to Write Thread Code to Remote Process.Err = " + SysErrorMessage(GetLastError())),
    	       NULL,MB_OK | MB_APPLMODAL | MB_ICONWARNING);
    return;
  }

  //启动远程线程
  HANDLE hRemoteThread;

  hRemoteThread = CreateRemoteThread(hRemoteProcess ,0,0,(DWORD(__stdcall *)(VOID*))pRemoteThread,(DEINJECTLIBINFO*)pRemoteParam,0,&dwWriten);
  if(hRemoteThread == NULL)
  {
    MessageBox(NULL,CString("建立卸载线程失败。原因：" + SysErrorMessage(GetLastError())),NULL,MB_OK | MB_APPLMODAL | MB_ICONWARNING);
  }
  else
  {
    CloseHandle(hRemoteThread);
    MessageBox(NULL,"建立卸载线程成功。",NULL,MB_OK | MB_APPLMODAL | MB_ICONWARNING);
  }

  //读卸载返回值
  Ret = ReadProcessMemory(hRemoteProcess,pRemoteParam,(LPVOID)&DeinjectLibInfo,sizeof(DEINJECTLIBINFO),&dwWriten);
  if(Ret == 0)
  {
    MessageBox(NULL,CString("读卸载返回值失败。原因 = " + SysErrorMessage(GetLastError())),
    	       NULL,MB_OK | MB_APPLMODAL | MB_ICONWARNING);
  }
  else
  {
    if(DeinjectLibInfo.dwReturnValue == 0)
    {
      MessageBox(NULL,"卸载成功。",NULL,MB_OK | MB_APPLMODAL | MB_ICONWARNING);
    }
    else
    {
      MessageBox(NULL,CString("卸载失败。原因：" + SysErrorMessage(DeinjectLibInfo.dwReturnValue)),NULL,MB_OK | MB_APPLMODAL | MB_ICONWARNING);
    }
  }

  //恢复权限
  CloseHandle(hRemoteProcess);
  EnablePrivilege(SE_DEBUG_NAME,false);
}
//---------------------------------------------------------------------------
