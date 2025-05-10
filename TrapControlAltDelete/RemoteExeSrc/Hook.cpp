//---------------------------------------------------------------------------
//����    ��Τ����
//�����سƣ�BCB_FANS(�Ĵ�����֮׷ɱ��)����ΪCSDN��www.driverdevelop.com֮�ʺţ�
//E-Mail  ��slwqw@163.com
//����    ��2002-10-20
//
//����    ����2000������Ctrl + Alt + Del��ϼ�������Windows 2000 Professional SP3
//          ���İ�ƽ̨�������ͨ����
//ԭ��    ������Զ���߳�ע�뼼����װ��һ��DLL��Winlogon���̣�Ȼ��ػ�SAS���ڵĴ�
//          �ڹ��̣��ӹ�WM_HOTKEY��Ϣ���Դﵽ����Ctrl + Alt + Del֮Ŀ�ġ�
//�������ԣ�Borland C++Builder 5.0 Patch2
//�����Ƚϣ�������2000�����������Ctrl + Alt + Del��ϼ���һ�ֳ����ᵽ�Ľ��������
//	    ��ʹ���Լ�д��GINAȥ�滻MSGINA.DLL��Ȼ����WlxLoggedOnSAS���ֱ�ӷ���
//	    WLX_SAS_ACTION_NONE���ٺ٣�˵�����Ⲣ���������������������ϼ���ֻ��
//	    ֱ�ӷ���WLX_SAS_ACTION_NONEʱ��Winlogon�������Զ���"Winlogon"�����л�
//	    ��ԭ����"Default"�����ˣ���������ʾ��ȫ�Ի������Կ������������ˣ�����
//	    ʹ�����ַ������Եؿ�����������˸������ʹ�ñ��ĵķ���ʱ���㲻�ῴ����
//	    ����˸��
//��л    ��www.driverdevelop.com�ϵ�icube��lu0��
//��Ȩ    ��ת����ע��ԭ���ߣ���

//---------------------------------------------------------------------------

#include "stdafx.h"
#include <tlhelp32.h>

#include "Hook.h"

//---------------------------------------------------------------------------
//��������ʽ������
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
//Զ���̣߳�����װ��DLL
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
//Զ���̣߳�����ж��DLL
static DWORD WINAPI ThreadFuncDetach(DEINJECTLIBINFO *pInfo)
{
  HINSTANCE hDll = NULL;
  BOOL Result;
  bool HasFoundModule = false;

  pInfo->dwReturnValue = 0;//��ζ�ɹ���������ֵ����0������һ��������롣

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
//ͨ���������Ƶõ����̵�ID������ʹ�÷���Toolhelp�����������NT���޷�ʹ�ã���ȻҲ
//����PSAPI�����������Ϳ�����NT��ʹ���ˣ�
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
//                            �������
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

  //����������Ȩ��Ȼ���Ŀ�Ľ���
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

  //��ʼ������
  InjectLibInfo.pfnLoadLibrary = (PLOADLIBRARY)GetProcAddress(GetModuleHandle("Kernel32.dll"),
  				  LoadLibraryFuncStr);
  CopyMemory(InjectLibInfo.DllName,RemoteDllName,CString(RemoteDllName).GetLength());

  //��Զ���̷߳����ڴ�����Ų���
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

  //�����߳���
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

  //����Զ���߳�
  HANDLE hRemoteThread;

  hRemoteThread = CreateRemoteThread(hRemoteProcess,0,0,(DWORD(__stdcall *)(VOID*))pRemoteThread,(INJECTLIBINFO*)pRemoteParam,0,&dwWriten);
  CloseHandle(hRemoteThread);

  //�ָ�Ȩ��
  EnablePrivilege(SE_DEBUG_NAME,false);
  CloseHandle(hRemoteProcess);
}
//---------------------------------------------------------------------------
//                             ж���߳�
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

  //����������Ȩ��Ȼ���Ŀ�Ľ���
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

  //��ʼ������
  DeinjectLibInfo.pfnFreeLibrary = (PFREELIBRARY)GetProcAddress(GetModuleHandle("Kernel32.dll"),FreeLibraryFuncStr);
  DeinjectLibInfo.pfnGetModuleHandle = (PGETMODULEHANDLE)GetProcAddress(GetModuleHandle("Kernel32.dll"),GetModuleHandleFuncStr);
  DeinjectLibInfo.pfnGetLastError = (PGETLASTERROR)GetProcAddress(GetModuleHandle("Kernel32.dll"),GetLastErrorFuncStr);

  CopyMemory(DeinjectLibInfo.DllName,RemoteDllName,CString(RemoteDllName).GetLength());

  //��Զ���̷߳����ڴ�����Ų���
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

  //�����߳���
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

  //����Զ���߳�
  HANDLE hRemoteThread;

  hRemoteThread = CreateRemoteThread(hRemoteProcess ,0,0,(DWORD(__stdcall *)(VOID*))pRemoteThread,(DEINJECTLIBINFO*)pRemoteParam,0,&dwWriten);
  if(hRemoteThread == NULL)
  {
    MessageBox(NULL,CString("����ж���߳�ʧ�ܡ�ԭ��" + SysErrorMessage(GetLastError())),NULL,MB_OK | MB_APPLMODAL | MB_ICONWARNING);
  }
  else
  {
    CloseHandle(hRemoteThread);
    MessageBox(NULL,"����ж���̳߳ɹ���",NULL,MB_OK | MB_APPLMODAL | MB_ICONWARNING);
  }

  //��ж�ط���ֵ
  Ret = ReadProcessMemory(hRemoteProcess,pRemoteParam,(LPVOID)&DeinjectLibInfo,sizeof(DEINJECTLIBINFO),&dwWriten);
  if(Ret == 0)
  {
    MessageBox(NULL,CString("��ж�ط���ֵʧ�ܡ�ԭ�� = " + SysErrorMessage(GetLastError())),
    	       NULL,MB_OK | MB_APPLMODAL | MB_ICONWARNING);
  }
  else
  {
    if(DeinjectLibInfo.dwReturnValue == 0)
    {
      MessageBox(NULL,"ж�سɹ���",NULL,MB_OK | MB_APPLMODAL | MB_ICONWARNING);
    }
    else
    {
      MessageBox(NULL,CString("ж��ʧ�ܡ�ԭ��" + SysErrorMessage(DeinjectLibInfo.dwReturnValue)),NULL,MB_OK | MB_APPLMODAL | MB_ICONWARNING);
    }
  }

  //�ָ�Ȩ��
  CloseHandle(hRemoteProcess);
  EnablePrivilege(SE_DEBUG_NAME,false);
}
//---------------------------------------------------------------------------
