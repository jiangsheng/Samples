#if !defined(AFX_WRAPPERS_H__6548FC16_8EE3_4E70_A213_AE4D52768EC5__INCLUDED_)
#define AFX_WRAPPERS_H__6548FC16_8EE3_4E70_A213_AE4D52768EC5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "NtQuery.h"
#include "psapi.h"
#include "tlHelp32.h"
#include "wtsapi32.h"

DWORD GetGDIObjectsCount(HANDLE hProcess);
DWORD GetUSERObjectsCount(HANDLE hProcess);


class CNTDLLWrapper  
{
// construction/destruction
//
public:
   CNTDLLWrapper();
   virtual ~CNTDLLWrapper();


// C++ Interface
// 
public:
   BOOL IsValid();

   BOOL QueryInformationProcess(
      HANDLE            hProcess, 
      PROCESSINFOCLASS  pic, 
      LPVOID            pBuffer, 
      DWORD             cbBuffer
      );


// implementation details
//
typedef int (CALLBACK * NTQUERYINFORMATIONPROCESS_PROC)(HANDLE, PROCESSINFOCLASS, LPVOID, DWORD, PDWORD);

private:
   BOOL     m_bIsValid;
   HMODULE  m_hNTDLL;

   NTQUERYINFORMATIONPROCESS_PROC m_pfnNtQueryInformationProcess;
};




class CPSAPIWrapper  
{
// construction/destruction
// 
public:
   CPSAPIWrapper();
   virtual ~CPSAPIWrapper();


// C++ Interface
// 
public:
   BOOL IsValid();

   DWORD GetModuleFileNameEx(
     HANDLE    hProcess,      // handle to process
     HMODULE   hModule,       // handle to module
     LPTSTR    lpFilename,    // path buffer
     DWORD     nSize          // maximum characters to retrieve
   );

   BOOL EnumProcesses(
      DWORD*   lpidProcess,   // array of process identifiers
      DWORD    cb,            // size of array
      DWORD*   cbNeeded       // number of bytes returned
   );

   BOOL EnumProcessModules(
      HANDLE   hProcess,      // handle to process
      HMODULE* lphModule,     // array of module handles
      DWORD    cb,            // size of array
      LPDWORD  lpcbNeeded     // number of bytes required
   );

   BOOL GetProcessMemoryInfo(
      HANDLE                     hProcess,         // handle to process
      PPROCESS_MEMORY_COUNTERS   ppsmemCounters,   // buffer
      DWORD                      cb                // size of buffer
   );

   DWORD GetModuleBaseName(
      HANDLE   hProcess,      // handle to process
      HMODULE  hModule,       // handle to module
      LPTSTR   lpBaseName,    // base name buffer
      DWORD    nSize          // maximum characters to retrieve
   );


// implementation details
// 
typedef BOOL (WINAPI *ENUMPROCESSES_PROC)(DWORD*, DWORD, DWORD*);
typedef DWORD (WINAPI *GETMODULEFILENAMEEX_PROC)(HANDLE, HMODULE, LPTSTR, DWORD);
typedef BOOL (WINAPI *ENUMPROCESSMODULES_PROC)(HANDLE, HMODULE*, DWORD, LPDWORD);
typedef BOOL (WINAPI *GETPROCESSMEMORYINFO_PROC)(HANDLE, PPROCESS_MEMORY_COUNTERS, DWORD);
typedef DWORD (WINAPI *GETMODULEBASENAME)(HANDLE, HMODULE, LPTSTR, DWORD);

private:
   BOOL     m_bIsValid;
   HMODULE  m_hPSAPI;

   ENUMPROCESSES_PROC         m_pfnEnumProcesses;
   GETMODULEFILENAMEEX_PROC   m_pfnGetModuleFileNameEx;
   ENUMPROCESSMODULES_PROC    m_pfnEnumProcessModules;
   GETPROCESSMEMORYINFO_PROC  m_pfnGetProcessMemoryInfo;
   GETMODULEBASENAME          m_pfnGetModuleBaseName;
};



class CTOOLHELPWrapper
{
// construction/destruction
// 
public:
   CTOOLHELPWrapper();
   virtual ~CTOOLHELPWrapper();


// C++ Interface
// 
public:
   BOOL IsValid();

   HANDLE CreateToolhelp32Snapshot(DWORD dwFlags, DWORD th32ProcessID);
   BOOL Module32First(HANDLE hSnapshot, LPMODULEENTRY32 lpme);
   BOOL Module32Next(HANDLE hSnapshot, LPMODULEENTRY32 lpme);
   BOOL Module32FirstW(HANDLE hSnapshot, LPMODULEENTRY32W lpme);
   BOOL Module32NextW(HANDLE hSnapshot, LPMODULEENTRY32W lpme);

    
// Implementation details
// 
typedef HANDLE (WINAPI *CREATETOOLHELP32SNAPSHOT_PROC)(DWORD, DWORD);
typedef BOOL (WINAPI *MODULE32FIRST_PROC)(HANDLE, LPMODULEENTRY32);
typedef BOOL (WINAPI *MODULE32NEXT_PROC)(HANDLE, LPMODULEENTRY32);
typedef BOOL (WINAPI *MODULE32FIRSTW_PROC)(HANDLE, LPMODULEENTRY32W);
typedef BOOL (WINAPI *MODULE32NEXTW_PROC)(HANDLE, LPMODULEENTRY32W);
    
private:
   BOOL     m_bIsValid;
   HMODULE  m_hKERNEL32;

   CREATETOOLHELP32SNAPSHOT_PROC m_pfnCreateToolhelp32Snapshot;
   MODULE32FIRST_PROC            m_pfnModule32First;
   MODULE32NEXT_PROC             m_pfnModule32Next;
   MODULE32FIRSTW_PROC           m_pfnModule32FirstW;
   MODULE32NEXTW_PROC            m_pfnModule32NextW;
};




class CWTSWrapper
{
// construction/destruction
//
public:
   CWTSWrapper();
   ~CWTSWrapper();


// C++ Interface
// 
public:
   BOOL IsValid();

   HANDLE WTSOpenServer(LPTSTR szServer);
   VOID WTSCloseServer(HANDLE hServer);
   BOOL ProcessIdToSessionId(DWORD dwProcessId, DWORD *pSessionId);
   BOOL WTSEnumerateProcesses(HANDLE hServer, DWORD Reserved, DWORD Version, PWTS_PROCESS_INFO *ppProcessInfo, DWORD* pCount);
   VOID WTSFreeMemory(PVOID pMemory);
   BOOL WTSEnumerateSessions(HANDLE hServer, DWORD Reserved, DWORD Version, PWTS_SESSION_INFO* ppSessionInfo, DWORD* pCount);
   BOOL WTSQuerySessionInformation(HANDLE hServer, DWORD SessionId, WTS_INFO_CLASS WTSInfoClass, LPTSTR *ppBuffer, DWORD *pBytesReturned);

static LPCTSTR WTSGetConnectState(WTS_CONNECTSTATE_CLASS eClass);


// implementation details
//
typedef HANDLE (CALLBACK * WTSOPENSERVER_PROC)(LPTSTR);
typedef VOID (CALLBACK * WTSCLOSESERVER_PROC)(HANDLE);
typedef BOOL (CALLBACK * PROCESSIDTOSESSIONID_PROC)(DWORD, DWORD*);
typedef BOOL (CALLBACK * WTSENUMERATEPROCESSES_PROC)(HANDLE, DWORD, DWORD, PWTS_PROCESS_INFO*, DWORD*);
typedef VOID (CALLBACK * WTSFREEMEMORY_PROC)(PVOID);
typedef BOOL (CALLBACK * WTSENUMERATESESSIONS_PROC)(HANDLE, DWORD, DWORD, PWTS_SESSION_INFO*, DWORD*);
typedef BOOL (CALLBACK * WTSQUERYSESSIONINFORMATION_PROC)(HANDLE, DWORD, WTS_INFO_CLASS, LPTSTR*, DWORD*);


private:
   BOOL     m_bIsValid;
   HMODULE  m_hWTS;

   WTSOPENSERVER_PROC               m_pfnWTSOpenServer;
   WTSCLOSESERVER_PROC              m_pfnWTSCloseServer;
   PROCESSIDTOSESSIONID_PROC        m_pfnProcessIdToSessionId;
   WTSENUMERATEPROCESSES_PROC       m_pfnWTSEnumerateProcesses;
   WTSENUMERATESESSIONS_PROC        m_pfnWTSEnumerateSessions;
   WTSQUERYSESSIONINFORMATION_PROC  m_pfnWTSQuerySessionInformation;

   WTSFREEMEMORY_PROC         m_pfnWTSFreeMemory;
};



#endif // !defined(AFX_WRAPPERS_H__6548FC16_8EE3_4E70_A213_AE4D52768EC5__INCLUDED_)
