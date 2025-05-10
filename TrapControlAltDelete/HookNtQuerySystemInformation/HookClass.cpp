//#include <vcl.h>
#include <windows.h>
#pragma hdrstop

#define _WIN64

#include <Imagehlp.h>
#include <Tlhelp32.h>
#include <assert.h>

#include "HookClass.h"

//---------------------------------------------------------------------------
//�Ƿ�Hook ȱʡ��5��ϵͳ�����ı�־
extern bool IsHook;

//---------------------------------------------------------------------------
//��W9X�£���һ�����������ڵ���״̬ʱ��������������õ�ϵͳ�����ĵ�ַָ��һ��STUB
//��ߵĺ����ĵ�ַ�������������ĵ�ַ�����Ա�����е�����
PVOID TAPIHook::pvMaxAppAddr = NULL;
const byte PushOpCode = 0x68;       // PUSH ָ�����

//---------------------------------------------------------------------------

TAPIHook* TAPIHook::pHeadHook = NULL;    // Address of first object

//---------------------------------------------------------------------------

TAPIHook TAPIHook::LoadLibraryAHook("Kernel32.Dll","LoadLibraryA",(PROC)TAPIHook::LoadLibraryA,false);
TAPIHook TAPIHook::LoadLibraryWHook("Kernel32.Dll","LoadLibraryW",(PROC)TAPIHook::LoadLibraryW,false);
TAPIHook TAPIHook::LoadLibraryExAHook("Kernel32.Dll","LoadLibraryExA",(PROC)TAPIHook::LoadLibraryExA,false);
TAPIHook TAPIHook::LoadLibraryExWHook("Kernel32.Dll","LoadLibraryExW",(PROC)TAPIHook::LoadLibraryExW,false);
TAPIHook TAPIHook::GetProcAddressHook("Kernel32.Dll","GetProcAddress",(PROC)TAPIHook::GetProcAddress,false);

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

TAPIHook::TAPIHook()
{
}
//---------------------------------------------------------------------------
TAPIHook::TAPIHook(PSTR DllName, PSTR DllFuncName, PROC pfnHook_Target,bool IsHookSelfDll)
{
  SetupHook(DllName,DllFuncName,pfnHook_Target,IsHookSelfDll);
}
//---------------------------------------------------------------------------
TAPIHook::~TAPIHook()
{
  ReplaceIATEntryInAllModules(FDllName,pfnHook,pfnOrig,FIsHookSelfDll);

  //ɾ��Hook�����ɾ����ǰHook
  TAPIHook* p = pHeadHook;

  if(p == this)
  {
    pHeadHook = p->pNextHook;
  }
  else
  {
    bool IsFound = false;

    // ����Hook��
    for(; !IsFound && (p->pNextHook != NULL); p = p->pNextHook)
    {
      if(p->pNextHook == this)
      {
        // ����ǰHook
        p->pNextHook = p->pNextHook->pNextHook;
        break;
      }
    }
  }
}
//---------------------------------------------------------------------------
//SetupHook
void TAPIHook::SetupHook(PSTR DllName, PSTR DllFuncName, PROC pfnHook_Target,bool IsHookSelfDll)
{
  pNextHook  = pHeadHook;    // The next node was at the head
  pHeadHook = this;          // This node is now at the head

  if(pvMaxAppAddr == NULL)
  {
    // Functions with address above lpMaximumApplicationAddress require
    // special processing (Windows 98 only)
    SYSTEM_INFO si;

    GetSystemInfo(&si);
    pvMaxAppAddr = si.lpMaximumApplicationAddress;
  }

  // Save information about this hooked function
  FDllName = DllName;
  FDllFuncName = DllFuncName;
  pfnHook = pfnHook_Target;
  pfnOrig = GetProcAddressRaw(GetModuleHandleA(FDllName), FDllFuncName);
  FIsHookSelfDll = IsHookSelfDll;

  assert(pfnOrig != NULL);  // Function doesn't exist

  if(pfnOrig > pvMaxAppAddr)
  {
    // The address is in a shared DLL; the address needs fixing up
    PBYTE pb = (PBYTE)pfnOrig;
    if(pb[0] == PushOpCode)
    {
      // Skip over the PUSH op code and grab the real address
      PVOID pv = * (PVOID*) &pb[1];
      pfnOrig = (PROC) pv;
    }
  }

  ///////

  ReplaceIATEntryInAllModules(FDllName,pfnOrig, pfnHook,FIsHookSelfDll);
}
//---------------------------------------------------------------------------
//����û��Hook DLL�����GetProcAddress����������GetProcAddress��Ȼָ����ȷ��
//Kernel32.Dll�е�GetProcAddress�ĵ�ַ����˲���������ѭ����
FARPROC WINAPI TAPIHook::GetProcAddressRaw(HMODULE hmod, PCSTR pszProcName)
{
  return (::GetProcAddress(hmod, pszProcName));
}
//---------------------------------------------------------------------------
//��ѯ����DLL�ľ��
static HMODULE ModuleFromAddress(PVOID pv)
{
  MEMORY_BASIC_INFORMATION mbi;

  return((VirtualQuery(pv, &mbi, sizeof(mbi)) != 0) ? (HMODULE) mbi.AllocationBase : NULL);
}
//---------------------------------------------------------------------------
//ReplaceIATEntryInAllModules
void WINAPI TAPIHook::ReplaceIATEntryInAllModules(PCSTR DllName,
  PROC pfnCurrent,PROC pfnNew,bool IsHookSelfDll)
{
  // �Ƿ�Hook DLL�������Ӧ��������һЩϵͳ��������GetProcAddress��LoadLibraryA��
  // LoadLibraryW��LoadLibraryEx��LoadLibraryExW��Щ������DLL�����ǲ��ܶ����ǽ�
  // ��Hook�ģ������������ѭ����

  HMODULE hThisModule = NULL;

  hThisModule = (IsHookSelfDll) ? NULL : ModuleFromAddress(ReplaceIATEntryInAllModules);

  /////
  HANDLE hSnapshot;
  MODULEENTRY32 ModEntry32;
  DWORD dwProcessId;
  BOOL Result;

  dwProcessId = GetCurrentProcessId();
  hSnapshot = CreateToolhelp32Snapshot((DWORD)TH32CS_SNAPMODULE,dwProcessId);

  ModEntry32.dwSize = sizeof(MODULEENTRY32);
  Result = Module32First(hSnapshot,&ModEntry32);
  while(Result)
  {
    if(ModEntry32.hModule != hThisModule)
      ReplaceIATEntryInModule(DllName,pfnCurrent,pfnNew,ModEntry32.hModule);

    Result = Module32Next(hSnapshot,&ModEntry32);
  }

  CloseHandle(hSnapshot);
}
//---------------------------------------------------------------------------
//ReplaceIATEntryInModule
void WINAPI TAPIHook::ReplaceIATEntryInModule(PCSTR DllName,
  PROC pfnCurrent,PROC pfnNew,HMODULE hModCaller)
{
  // ��ѯģ�麯�������ĵ�ַ
  DWORD ulSize;
  PIMAGE_IMPORT_DESCRIPTOR pImportDesc;

  pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR)ImageDirectoryEntryToData(hModCaller, true,
					    IMAGE_DIRECTORY_ENTRY_IMPORT, &ulSize);

  if(pImportDesc == NULL)
    return ;  // ģ��û�к��������

  // ��ѯģ���Ƿ�������ĳ��DLL�ĺ���
  for(;pImportDesc->Name; pImportDesc++)
  {
    PSTR pszModName = (PSTR) ((PBYTE)hModCaller + pImportDesc->Name);
    if(lstrcmpiA(pszModName, DllName) == 0)
      break;   // Found
  }

  if(pImportDesc->Name == 0)
    return ;  // ���ģ��û������ָ��DLL�ĺ���

  //��ѯģ���Ƿ���DLL��������ָ���ĺ���
  PIMAGE_THUNK_DATA pThunk = (PIMAGE_THUNK_DATA)((PBYTE)hModCaller + pImportDesc->FirstThunk);

  for(; pThunk->u1.Function; pThunk++)
  {
    // �õ����ģ�������ÿһ�������ĵ�ַ
    PROC* ppfn = (PROC*) &pThunk->u1.Function;

    // �Ƚ������ַ�Ƿ�������׼��Hook�ĺ�����ַ
    bool IsFound = (*ppfn == pfnCurrent);

    if(!IsFound && (*ppfn > pvMaxAppAddr))
    {
      // If this is not the function and the address is in a shared DLL,
      // then maybe we're running under a debugger on Windows 98. In this
      // case, this address points to an instruction that may have the
      // correct address.

      PBYTE pbInFunc = (PBYTE) *ppfn;
      if(pbInFunc[0] == PushOpCode)
      {
        // We see the PUSH instruction, the real function address follows
        ppfn = (PROC*) &pbInFunc[1];

        // Is this the function we're looking for?
        IsFound = (*ppfn == pfnCurrent);
      }
    }

    //�ҵ�����ҪHook�ĺ������滻���ĵ�ַΪ�����Զ���ĺ����ĵ�ַ
    if(IsFound)
    {
      DWORD dwOldProtect;

      //�����޸���Ӧ��ҳ�����ԣ������п����޷�д�����
      VirtualProtect(ppfn,sizeof(pfnNew),PAGE_READWRITE,&dwOldProtect);

      WriteProcessMemory(GetCurrentProcess(), ppfn, &pfnNew,sizeof(pfnNew), NULL);

      VirtualProtect(ppfn,sizeof(pfnNew),dwOldProtect,0);
      return ;  // We did it, get out
    }
  }
}
//---------------------------------------------------------------------------
//����̬װ��DLLʱ������Hook�Ǹ�DLL����Ӧ����
void WINAPI TAPIHook::FixupNewlyLoadedModule(HMODULE hMod, DWORD dwFlags)
{
  HMODULE hThisModule = NULL;

  //����Hook DLL����ĺ���
  hThisModule = ModuleFromAddress(FixupNewlyLoadedModule);
  if(hMod == hThisModule)
    return;

  // If a new module is loaded, hook the hooked functions
  if((hMod != NULL) && ((dwFlags & LOAD_LIBRARY_AS_DATAFILE) == 0))
  {
    for(TAPIHook* p = pHeadHook; p != NULL; p = p->pNextHook)
    {
      ReplaceIATEntryInModule(p->FDllName,p->pfnOrig, p->pfnHook, hMod);
    }
  }
}
//---------------------------------------------------------------------------
// ����û��Hook DLL�����LoadLibraryA����������LoadLibraryA��Ȼָ����ȷ��
// Kernel32.Dll�е�LoadLibraryA�ĵ�ַ����˲���������ѭ����LoadLibraryW��
// LoadLibraryExA��LoadLibraryExW���ơ�
HMODULE WINAPI TAPIHook::LoadLibraryA(PCSTR  pszModulePath)
{
  HMODULE hMod;

  hMod = ::LoadLibraryA(pszModulePath);
  if(!IsHook)
    return hMod;

  FixupNewlyLoadedModule(hMod, 0);

  return hMod;
}
//---------------------------------------------------------------------------
HMODULE WINAPI TAPIHook::LoadLibraryW(PCWSTR pszModulePath)
{
  HMODULE hMod;

  hMod = ::LoadLibraryW(pszModulePath);
  if(!IsHook)
    return hMod;

  FixupNewlyLoadedModule(hMod, 0);

  return hMod;
}
//---------------------------------------------------------------------------
HMODULE WINAPI TAPIHook::LoadLibraryExA(PCSTR  pszModulePath,HANDLE hFile, DWORD dwFlags)
{
  HMODULE hMod;

  hMod = ::LoadLibraryExA(pszModulePath,hFile,dwFlags);
  if(!IsHook)
    return hMod;

  FixupNewlyLoadedModule(hMod, dwFlags);

  return hMod;
}
//---------------------------------------------------------------------------
HMODULE WINAPI TAPIHook::LoadLibraryExW(PCWSTR pszModulePath,HANDLE hFile, DWORD dwFlags)
{
  HMODULE hMod;

  hMod = ::LoadLibraryExW(pszModulePath,hFile,dwFlags);
  if(!IsHook)
    return hMod;

  FixupNewlyLoadedModule(hMod, dwFlags);

  return hMod;
}
//---------------------------------------------------------------------------
//��̬��ѯ�����ĵ�ַ�������Ѿ�����DLL�Ѿ�Hook��GetProcAddress���������Բ�ѯһ��
//�����ĵ�ַʱ���Լ���GetProcAddress�������ȵõ�����Ȩ�����Ҫ��ѯ�ĺ������ڱ�
//Hook�ĺ�������ֱ�ӷ���Hook�����ĵ�ַ��������ԭ���ĺ����ĵ�ַ��
FARPROC WINAPI TAPIHook::GetProcAddress(HMODULE hmod, PCSTR pszProcName)
{
  //Get the true address of the function
  FARPROC pfn = GetProcAddressRaw(hmod, pszProcName);

  if(!IsHook) return pfn;

  //Is it one of the functions that we want hooked?
  TAPIHook* p = pHeadHook;

  for(;(pfn != NULL) && (p != NULL); p = p->pNextHook)
  {
    if(pfn == p->pfnOrig)
    {
      // The address to return matches an address we want to hook
      // Return the hook function address instead
      pfn = p->pfnHook;
      break;
    }
  }

  return pfn;
}
//---------------------------------------------------------------------------

