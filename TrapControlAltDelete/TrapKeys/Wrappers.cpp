#include "stdafx.h"
#include "tchar.h"
#include "assert.h"
#include "Wrappers.h"


#ifdef _DEBUG
   #undef THIS_FILE
   static char THIS_FILE[]=__FILE__;
   #define new DEBUG_NEW
#endif


//--------------------------------------------------------------------
// Windows 2000 wrappers
// 

// defined in winuser.h and only #if(WINVER >= 0x0500)
// 
#define GR_GDIOBJECTS     0       /* Count of GDI objects */
#define GR_USEROBJECTS    1       /* Count of USER objects */

typedef int (CALLBACK * GETGUIRESOURCES_PROC)(HANDLE, DWORD);
DWORD GetGDIObjectsCount(HANDLE hProcess)
{
   DWORD Count = 0;

   HMODULE hUser32 = ::LoadLibrary(_T("USER32.DLL"));
   if (hUser32 != NULL)
   {
      GETGUIRESOURCES_PROC pfnGetGuiResources;
      pfnGetGuiResources = (GETGUIRESOURCES_PROC)
         ::GetProcAddress(hUser32, _T("GetGuiResources"));
      if (pfnGetGuiResources != NULL)
         Count = pfnGetGuiResources(hProcess, GR_GDIOBJECTS);

   // don't forget to release called DLL
      ::FreeLibrary(hUser32);
   }

   return(Count);
}


DWORD GetUSERObjectsCount(HANDLE hProcess)
{
   DWORD Count = 0;

   HMODULE hUser32 = ::LoadLibrary(_T("USER32.DLL"));
   if (hUser32 != NULL)
   {
      GETGUIRESOURCES_PROC pfnGetGuiResources;
      pfnGetGuiResources = (GETGUIRESOURCES_PROC)
         ::GetProcAddress(hUser32, _T("GetGuiResources"));
      if (pfnGetGuiResources != NULL)
         Count = pfnGetGuiResources(hProcess, GR_USEROBJECTS);

   // don't forget to release called DLL
      ::FreeLibrary(hUser32);
   }

   return(Count);
}
//
//--------------------------------------------------------------------



//--------------------------------------------------------------------
// NTDLL wrapper class
// 

CNTDLLWrapper::CNTDLLWrapper()
{
   m_bIsValid = FALSE;
   m_pfnNtQueryInformationProcess = NULL;

   m_hNTDLL = ::LoadLibrary(_T("NTDLL.DLL"));
   if (m_hNTDLL != NULL)
   {
      m_pfnNtQueryInformationProcess = (NTQUERYINFORMATIONPROCESS_PROC)
         ::GetProcAddress(m_hNTDLL, _T("NtQueryInformationProcess"));
      m_bIsValid = (m_pfnNtQueryInformationProcess != NULL);
   }
}

CNTDLLWrapper::~CNTDLLWrapper()
{
   if (m_hNTDLL != NULL)
      ::FreeLibrary(m_hNTDLL);
}

BOOL CNTDLLWrapper::IsValid()
{
   return(m_bIsValid);
}


BOOL CNTDLLWrapper::QueryInformationProcess(HANDLE hProcess, PROCESSINFOCLASS pic, LPVOID pBuffer, DWORD cbBuffer)
{
// sanity checks
   assert(pBuffer != NULL);
   assert(cbBuffer != 0);
   if ((pBuffer == NULL) || (cbBuffer == 0))
      return(FALSE);

   if (!IsValid())
      return(FALSE);

// fail by default
   int iReturn = -1;

   try
   {
   // wrap the call
      ZeroMemory(pBuffer, cbBuffer);
      DWORD dwLen = 0;

      iReturn = m_pfnNtQueryInformationProcess(hProcess, pic, pBuffer, cbBuffer, &dwLen);
   }
   catch(...)
   {
      assert(FALSE);
   }

   return(iReturn >= 0);
}
//
//--------------------------------------------------------------------



//--------------------------------------------------------------------
// PSAPI wrapper class
//

CPSAPIWrapper::CPSAPIWrapper()
{
   m_bIsValid = FALSE;

// load PSAPI 
   m_hPSAPI = ::LoadLibrary(_T("PSAPI.DLL"));
   if (m_hPSAPI != NULL)
   {
   // get functions address
      m_pfnEnumProcesses = (ENUMPROCESSES_PROC)
         ::GetProcAddress(m_hPSAPI, _T("EnumProcesses"));
      m_pfnEnumProcessModules = (ENUMPROCESSMODULES_PROC)
         ::GetProcAddress(m_hPSAPI, _T("EnumProcessModules"));;
      m_pfnGetProcessMemoryInfo = (GETPROCESSMEMORYINFO_PROC)
         ::GetProcAddress(m_hPSAPI, _T("GetProcessMemoryInfo"));;

#ifdef _UNICODE
      m_pfnGetModuleFileNameEx = (GETMODULEFILENAMEEX_PROC)
         ::GetProcAddress(m_hPSAPI, _T("GetModuleFileNameExW"));
      m_pfnGetModuleBaseName = (GETMODULEBASENAME)
         ::GetProcAddress(m_hPSAPI, _T("GetModuleBaseNameW"));;
#else
      m_pfnGetModuleFileNameEx = (GETMODULEFILENAMEEX_PROC)
         ::GetProcAddress(m_hPSAPI, _T("GetModuleFileNameExA"));
      m_pfnGetModuleBaseName = (GETMODULEBASENAME)
         ::GetProcAddress(m_hPSAPI, _T("GetModuleBaseNameA"));;
#endif
      
   // check everything is fine
      m_bIsValid =
         (m_pfnEnumProcesses != NULL) &&
         (m_pfnGetModuleFileNameEx != NULL) &&
         (m_pfnEnumProcessModules != NULL) &&
         (m_pfnGetProcessMemoryInfo != NULL) &&
         (m_pfnGetModuleBaseName != NULL);
   }
}

CPSAPIWrapper::~CPSAPIWrapper()
{
   if (m_hPSAPI != NULL)
      ::FreeLibrary(m_hPSAPI);
}


BOOL CPSAPIWrapper::IsValid()
{
   return(m_bIsValid);
}


DWORD CPSAPIWrapper::GetModuleFileNameEx(
  HANDLE    hProcess,      // handle to process
  HMODULE   hModule,       // handle to module
  LPTSTR    lpFilename,    // path buffer
  DWORD     nSize          // maximum characters to retrieve
)
{
// sanity checks
   assert(IsValid());
   if (!IsValid())
      return(FALSE);
   
   DWORD dwReturn = 0;
   try
   {
      dwReturn = m_pfnGetModuleFileNameEx(hProcess, hModule, lpFilename, nSize);
   }
   catch(...)
   {
      assert(FALSE);
   }

   return(dwReturn);
}


BOOL CPSAPIWrapper::EnumProcesses(
   DWORD*   lpidProcess,   // array of process identifiers
   DWORD    cb,            // size of array
   DWORD*   cbNeeded       // number of bytes returned
)
{
// sanity checks
   assert(IsValid());
   if (!IsValid())
      return(FALSE);
   
   BOOL bReturn = FALSE;
   try
   {
      bReturn = m_pfnEnumProcesses(lpidProcess, cb, cbNeeded);
   }
   catch(...)
   {
      assert(FALSE);
   }

   return(bReturn);
}


BOOL CPSAPIWrapper::EnumProcessModules(
   HANDLE   hProcess,      // handle to process
   HMODULE* lphModule,     // array of module handles
   DWORD    cb,            // size of array
   LPDWORD  lpcbNeeded     // number of bytes required
)
{
// sanity checks
   assert(IsValid());
   if (!IsValid())
      return(FALSE);
   
   BOOL bReturn = FALSE;
   try
   {
      bReturn = m_pfnEnumProcessModules(hProcess, lphModule, cb, lpcbNeeded);
   }
   catch(...)
   {
      assert(FALSE);
   }

   return(bReturn);
}


BOOL CPSAPIWrapper::GetProcessMemoryInfo(
   HANDLE                     hProcess,         // handle to process
   PPROCESS_MEMORY_COUNTERS   ppsmemCounters,   // buffer
   DWORD                      cb                // size of buffer
)
{
// sanity checks
   assert(IsValid());
   if (!IsValid())
      return(FALSE);
   
   BOOL bReturn = FALSE;
   try
   {
      bReturn = m_pfnGetProcessMemoryInfo(hProcess, ppsmemCounters, cb);
   }
   catch(...)
   {
      assert(FALSE);
   }

   return(bReturn);
}


DWORD CPSAPIWrapper::GetModuleBaseName(
   HANDLE   hProcess,      // handle to process
   HMODULE  hModule,       // handle to module
   LPTSTR   lpBaseName,    // base name buffer
   DWORD    nSize          // maximum characters to retrieve
)
{
// sanity checks
   if (!IsValid())
      return(FALSE);
   
   DWORD dwReturn = 0;
   try
   {
      dwReturn = m_pfnGetModuleBaseName(hProcess, hModule, lpBaseName, nSize);
   }
   catch(...)
   {
      assert(FALSE);
   }

   return(dwReturn);
}
//
//--------------------------------------------------------------------



//--------------------------------------------------------------------
// TOOLHELP32 wrappers
//

CTOOLHELPWrapper::CTOOLHELPWrapper()
{
   m_bIsValid = FALSE;

   m_hKERNEL32 = ::GetModuleHandle(_T("KERNEL32.DLL"));
   
   if (m_hKERNEL32 != NULL)
   {
      m_pfnCreateToolhelp32Snapshot = (CREATETOOLHELP32SNAPSHOT_PROC)
         ::GetProcAddress(m_hKERNEL32, _T("CreateToolhelp32Snapshot"));
      m_pfnModule32First = (MODULE32FIRST_PROC)
         ::GetProcAddress(m_hKERNEL32, _T("Module32First"));
      m_pfnModule32Next = (MODULE32NEXT_PROC)
         ::GetProcAddress(m_hKERNEL32, _T("Module32Next"));
      m_pfnModule32FirstW = (MODULE32FIRSTW_PROC)
         ::GetProcAddress(m_hKERNEL32, _T("Module32FirstW"));
      m_pfnModule32NextW = (MODULE32NEXTW_PROC)
         ::GetProcAddress(m_hKERNEL32, _T("Module32NextW"));

   // check everything is fine
      m_bIsValid =
         (m_pfnCreateToolhelp32Snapshot != NULL) &&
         (m_pfnModule32First != NULL) &&
         (m_pfnModule32Next != NULL) &&
         (m_pfnModule32FirstW != NULL) &&
         (m_pfnModule32NextW != NULL);
   }
}


CTOOLHELPWrapper::~CTOOLHELPWrapper()
{
// no need to call FreeLibrary() because we called GetModuleHandle()
// instead of LoadLibrary()
}


BOOL CTOOLHELPWrapper::IsValid()
{
   return(m_bIsValid);
}


HANDLE CTOOLHELPWrapper::CreateToolhelp32Snapshot(DWORD dwFlags, DWORD th32ProcessID)
{
// sanity checks
   if (!IsValid())
      return(FALSE);
   
   HANDLE hReturn = 0;
   try
   {
      hReturn = m_pfnCreateToolhelp32Snapshot(dwFlags, th32ProcessID);
   }
   catch(...)
   {
      assert(FALSE);
   }

   return(hReturn);
}


BOOL CTOOLHELPWrapper::Module32First(HANDLE hSnapshot, LPMODULEENTRY32 lpme)
{
// sanity checks
   assert(IsValid());
   if (!IsValid())
      return(FALSE);
   
   BOOL bReturn = FALSE;
   try
   {
      bReturn = m_pfnModule32First(hSnapshot, lpme);
   }
   catch(...)
   {
      assert(FALSE);
   }

   return(bReturn);
}


BOOL CTOOLHELPWrapper::Module32Next(HANDLE hSnapshot, LPMODULEENTRY32 lpme)
{
// sanity checks
   assert(IsValid());
   if (!IsValid())
      return(FALSE);
   
   BOOL bReturn = FALSE;
   try
   {
      bReturn = m_pfnModule32Next(hSnapshot, lpme);
   }
   catch(...)
   {
      assert(FALSE);
   }

   return(bReturn);
}


BOOL CTOOLHELPWrapper::Module32FirstW(HANDLE hSnapshot, LPMODULEENTRY32W lpme)
{
// sanity checks
   assert(IsValid());
   if (!IsValid())
      return(FALSE);
   
   BOOL bReturn = FALSE;
   try
   {
      bReturn = m_pfnModule32FirstW(hSnapshot, lpme);
   }
   catch(...)
   {
      assert(FALSE);
   }

   return(bReturn);
}


BOOL CTOOLHELPWrapper::Module32NextW(HANDLE hSnapshot, LPMODULEENTRY32W lpme)
{
// sanity checks
   assert(IsValid());
   if (!IsValid())
      return(FALSE);
   
   BOOL bReturn = FALSE;
   try
   {
      bReturn = m_pfnModule32NextW(hSnapshot, lpme);
   }
   catch(...)
   {
      assert(FALSE);
   }

   return(bReturn);
}
//
//--------------------------------------------------------------------



//--------------------------------------------------------------------
// Windows Terminal Services wrappers
//
LPCTSTR CWTSWrapper::WTSGetConnectState(WTS_CONNECTSTATE_CLASS eClass)
{
   switch(eClass)
   {
      case WTSActive:         // User logged on to WinStation
         return(_T("WTSActive"));
         
      case WTSConnected:      // WinStation connected to client
         return(_T("WTSConnected"));
      
      case WTSConnectQuery:   // In the process of connecting to client
         return(_T("WTSConnectQuery"));
      
      case WTSShadow:         // Shadowing another WinStation
         return(_T("WTSShadow"));
      
      case WTSDisconnected:   // WinStation logged on without client
         return(_T("WTSDisconnected"));
      
      case WTSIdle:           // Waiting for client to connect
         return(_T("WTSIdle"));
      
      case WTSListen:         // WinStation is listening for connection
         return(_T("WTSListen"));
      
      case WTSReset:          // WinStation is being reset
         return(_T("WTSReset"));
      
      case WTSDown:           // WinStation is down due to error
         return(_T("WTSDown"));
      
      case WTSInit:           // WinStation in initialization
         return(_T("WTSInit"));
      
      default:
      { 
      static TCHAR szInfo[64];
         wsprintf(szInfo, _T("Unknown Connect State %u..."), eClass);
         return(szInfo);
      }
   }
}

CWTSWrapper::CWTSWrapper(void)
{
   m_bIsValid = FALSE;

// load PSAPI 
   m_hWTS = ::LoadLibrary(_T("Wtsapi32.dll"));
   if (m_hWTS != NULL)
   {
   // get functions address
      m_pfnWTSCloseServer = (WTSCLOSESERVER_PROC)
         ::GetProcAddress(m_hWTS, _T("WTSCloseServer"));
      m_pfnProcessIdToSessionId = (PROCESSIDTOSESSIONID_PROC)
         ::GetProcAddress(::GetModuleHandle(_T("kernel32.dll")), _T("ProcessIdToSessionId"));
      m_pfnWTSFreeMemory = (WTSFREEMEMORY_PROC)
         ::GetProcAddress(m_hWTS, _T("WTSFreeMemory"));

#ifdef _UNICODE
      m_pfnWTSOpenServer = (WTSOPENSERVER_PROC)
         ::GetProcAddress(m_hWTS, _T("WTSOpenServerW"));
      m_pfnWTSEnumerateProcesses = (WTSENUMERATEPROCESSES_PROC)
         ::GetProcAddress(m_hWTS, _T("WTSEnumerateProcessesW"));
      m_pfnWTSEnumerateSessions = (WTSENUMERATESESSIONS_PROC)
         ::GetProcAddress(m_hWTS, _T("WTSEnumerateSessionsW"));
      m_pfnWTSQuerySessionInformation = (WTSQUERYSESSIONINFORMATION_PROC)
         ::GetProcAddress(m_hWTS, _T("WTSQuerySessionInformationW"));
#else
      m_pfnWTSOpenServer = (WTSOPENSERVER_PROC)
         ::GetProcAddress(m_hWTS, _T("WTSOpenServerA"));
      m_pfnWTSEnumerateProcesses = (WTSENUMERATEPROCESSES_PROC)
         ::GetProcAddress(m_hWTS, _T("WTSEnumerateProcessesA"));
      m_pfnWTSEnumerateSessions = (WTSENUMERATESESSIONS_PROC)
         ::GetProcAddress(m_hWTS, _T("WTSEnumerateSessionsA"));
      m_pfnWTSQuerySessionInformation = (WTSQUERYSESSIONINFORMATION_PROC)
         ::GetProcAddress(m_hWTS, _T("WTSQuerySessionInformationA"));
#endif
      
   // check everything is fine
      m_bIsValid =
         (m_pfnWTSOpenServer != NULL) &&
         (m_pfnWTSCloseServer != NULL) &&
         (m_pfnProcessIdToSessionId != NULL) &&
         (m_pfnWTSEnumerateProcesses != NULL) &&
         (m_pfnWTSEnumerateSessions != NULL) &&
         (m_pfnWTSQuerySessionInformation != NULL) &&
         (m_pfnWTSFreeMemory != NULL);
   }

}

CWTSWrapper::~CWTSWrapper(void)
{
   if (m_hWTS != NULL)
      ::FreeLibrary(m_hWTS);
}


BOOL CWTSWrapper::IsValid()
{
   return(m_bIsValid);
}

HANDLE CWTSWrapper::WTSOpenServer(LPTSTR szServer)
{
// sanity checks
   assert(IsValid());
   if (!IsValid())
      return(NULL);
   
   HANDLE hReturn = NULL;
   try
   {
      hReturn = m_pfnWTSOpenServer(szServer);
   }
   catch(...)
   {
      assert(FALSE);
   }

   return(hReturn);
}

VOID CWTSWrapper::WTSCloseServer(HANDLE hServer)
{
// sanity checks
   assert(IsValid());
   if (!IsValid())
      return;
   
   try
   {
      m_pfnWTSCloseServer(hServer);
   }
   catch(...)
   {
      assert(FALSE);
   }
}

BOOL CWTSWrapper::ProcessIdToSessionId(DWORD dwProcessId, DWORD *pSessionId)
{
// sanity checks
   assert(IsValid());
   if (!IsValid())
      return(FALSE);
   
   BOOL bReturn = FALSE;
   try
   {
      bReturn = m_pfnProcessIdToSessionId(dwProcessId, pSessionId);
   }
   catch(...)
   {
      assert(FALSE);
   }

   return(bReturn);
}

BOOL CWTSWrapper::WTSEnumerateProcesses(HANDLE hServer, DWORD Reserved, DWORD Version, PWTS_PROCESS_INFO *ppProcessInfo, DWORD* pCount)
{
// sanity checks
   assert(IsValid());
   if (!IsValid())
      return(FALSE);
   
   BOOL bReturn = FALSE;
   try
   {
      bReturn = m_pfnWTSEnumerateProcesses(hServer, Reserved, Version, ppProcessInfo, pCount);
   }
   catch(...)
   {
      assert(FALSE);
   }

   return(bReturn);
}


BOOL CWTSWrapper::WTSEnumerateSessions(HANDLE hServer, DWORD Reserved, DWORD Version, PWTS_SESSION_INFO* ppSessionInfo, DWORD* pCount)
{
// sanity checks
   assert(IsValid());
   if (!IsValid())
      return(FALSE);
   
   BOOL bReturn = FALSE;
   try
   {
      bReturn = m_pfnWTSEnumerateSessions(hServer, Reserved, Version, ppSessionInfo, pCount);
   }
   catch(...)
   {
      assert(FALSE);
   }

   return(bReturn);
}


BOOL CWTSWrapper::WTSQuerySessionInformation(HANDLE hServer, DWORD SessionId, WTS_INFO_CLASS WTSInfoClass, LPTSTR *ppBuffer, DWORD *pBytesReturned)
{
// sanity checks
   assert(IsValid());
   if (!IsValid())
      return(FALSE);
   
   BOOL bReturn = FALSE;
   try
   {
      bReturn = m_pfnWTSQuerySessionInformation(hServer, SessionId, WTSInfoClass, ppBuffer, pBytesReturned);
   }
   catch(...)
   {
      assert(FALSE);
   }

   return(bReturn);
}


VOID CWTSWrapper::WTSFreeMemory(PVOID pMemory)
{
// sanity checks
   assert(IsValid());
   if (!IsValid())
      return;
   
   try
   {
      m_pfnWTSFreeMemory(pMemory);
   }
   catch(...)
   {
      assert(FALSE);
   }
}
//
//--------------------------------------------------------------------
