//---------------------------------------------------------------------------
//����    ��Τ����,jiangsheng
//�����سƣ�BCB_FANS(�Ĵ�����֮׷ɱ��)����ΪCSDN��www.driverdevelop.com֮�ʺţ�jiangsheng����ΪCSDN�ʺţ�
//E-Mail  ��slwqw@163.com
//����    ��2002-10-20
//2002-11-5 jingsheng�޸�
//����    ����2000������Ctrl + Alt + Del��ϼ�������Windows 2000 Professional SP3
//          ���İ�ƽ̨�������ͨ����
//ԭ��    ������Զ���߳�ע�뼼����װ��һ��DLL��Winlogon���̣�Ȼ��ػ�SAS���ڵĴ�
//          �ڹ��̣��ӹ�WM_HOTKEY��Ϣ���Դﵽ����Ctrl + Alt + Del֮Ŀ�ġ�
//�������ԣ�Borland C++Builder 5.0 Patch2��Visual C++ 6.0 SP5
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
#include <lmerr.h>

#include "Hook.h"
//add by jiangsheng 2002-11-5
#include "TaskKeyMgr.h"
#include "Wrappers.h"
extern	BOOL Is_Terminal_Services () ;
//end add by jiangsheng 2002-11-5
//---------------------------------------------------------------------------
//��������ʽ������
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
//Զ���̣߳�����װ��DLL
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
//Զ���̣߳�����ж��DLL
static DWORD WINAPI ThreadFuncDetach(DEINJECTLIBINFO *pInfo)
{
  HINSTANCE hDll = NULL;
  BOOL bResult=FALSE;
  BOOL bHasFoundModule = FALSE;

  pInfo->dwReturnValue = 0;//��ζ�ɹ���������ֵ����0������һ��������롣

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
//ͨ���������Ƶõ����̵�ID������ʹ�÷���Toolhelp�����������NT���޷�ʹ�ã���ȻҲ
//����PSAPI�����������Ϳ�����NT��ʹ���ˣ�
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
//                            �������
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

  //����������Ȩ��Ȼ���Ŀ�Ľ���
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

  //��ʼ������
  ZeroMemory(&InjectLibInfo,sizeof(INJECTLIBINFO ));
  InjectLibInfo.pfnLoadLibrary = (PLOADLIBRARY)GetProcAddress(GetModuleHandle("Kernel32.dll"),LoadLibraryFuncStr);
  InjectLibInfo.pfnGetLastError = (PGETLASTERROR)GetProcAddress(GetModuleHandle("Kernel32.dll"),GetLastErrorFuncStr);
  lstrcpyn(InjectLibInfo.szDllName,CTaskKeyMgr::strRemoteDllName,CTaskKeyMgr::strRemoteDllName.GetLength()+1);
  //��Զ���̷߳����ڴ�����Ų���
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

  //�����߳���
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

  //����Զ���߳�
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

  //��ж�ط���ֵ
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
  HANDLE hRemoteProcess=NULL;
  DWORD  dwRemoteProcess=0;

  DWORD dwThreadSize=0;
  DEINJECTLIBINFO DeinjectLibInfo;
  
  PVOID pRemoteThread=NULL;
  PVOID pRemoteParam=NULL;
  DWORD dwWriten=0;
  DWORD Ret=0;

  //����������Ȩ��Ȼ���Ŀ�Ľ���
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

  //��ʼ������
  ZeroMemory(&DeinjectLibInfo,sizeof(DEINJECTLIBINFO ));
  DeinjectLibInfo.pfnFreeLibrary = (PFREELIBRARY)GetProcAddress(GetModuleHandle("Kernel32.dll"),FreeLibraryFuncStr);
  DeinjectLibInfo.pfnGetModuleHandle = (PGETMODULEHANDLE)GetProcAddress(GetModuleHandle("Kernel32.dll"),GetModuleHandleFuncStr);
  DeinjectLibInfo.pfnGetLastError = (PGETLASTERROR)GetProcAddress(GetModuleHandle("Kernel32.dll"),GetLastErrorFuncStr);

  lstrcpyn(DeinjectLibInfo.szDllName,CTaskKeyMgr::strRemoteDllName,CTaskKeyMgr::strRemoteDllName.GetLength()+1);

  //��Զ���̷߳����ڴ�����Ų���
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

  //�����߳���
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

  //����Զ���߳�
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

  //��ж�ط���ֵ
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

  //�ָ�Ȩ��
  CloseHandle(hRemoteProcess);
  EnablePrivilege(SE_DEBUG_NAME,false);
}
//---------------------------------------------------------------------------
