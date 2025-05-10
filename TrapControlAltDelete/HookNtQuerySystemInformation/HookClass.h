#ifndef HOOKCLASS_H
#define HOOKCLASS_H
//---------------------------------------------------------------------------

class TAPIHook
{
private:
       static PVOID pvMaxAppAddr;     // �������ַ֮�ϵĵ�ַ�ǹ���DLL���ڵĵ�ַ
                                      //������W9X���ã�

       static TAPIHook* pHeadHook;    // ��һ��TAPIHook����
       TAPIHook* pNextHook;           // ��һ��TAPIHook����

public:
       PCSTR FDllName;                // ��Hook�ĸ�DLL��ߵĺ���
       PCSTR FDllFuncName;            // Hook DLL����ĸ�����
       PROC  pfnOrig;                 // �Ǹ�������������ַ
       PROC  pfnHook;                 // �Զ��庯���ĵ�ַ
       bool  FIsHookSelfDll;          // �Ƿ�Hook DLL�������Ӧ��������һЩϵͳ��
       				      // ������GetProcAddress...�ȣ�DLL�����ǲ���
                                      // �����ǽ���Hook�ģ������������ѭ����

       TAPIHook();
       TAPIHook(PSTR DllName, PSTR DllFuncName, PROC pfnHook_Target,bool IsHookSelfDll);
       ~TAPIHook();

       // �ڵ�ǰ���̵�����ģ����Hookָ���ĺ���
       void SetupHook(PSTR DllName, PSTR DllFuncName, PROC pfnHook_Target,bool IsHookSelfDll);

       // ���غ�����������ַ
       operator PROC() { return(pfnOrig); }

public:
       // �õ�������������ַ
       static FARPROC WINAPI GetProcAddressRaw(HMODULE hmod, PCSTR pszProcName);

private:

       // ���ҽ������е�ģ�飬Ȼ��һһ����Hook
       static void WINAPI ReplaceIATEntryInAllModules(PCSTR DllName,
            PROC pfnCurrent,PROC pfnNew,bool IsHookSelfDll);

       // Hookָ��ģ���е�ָ������
       static void WINAPI ReplaceIATEntryInModule(PCSTR DllName,
            PROC pfnCurrent,PROC pfnNew,HMODULE hModCaller);

private:
       // ����̬װ��DLLʱ������Hook�Ǹ�DLL����Ӧ����
       static void WINAPI FixupNewlyLoadedModule(HMODULE hmod, DWORD dwFlags);

       // �ػ���4��ϵͳ�������Ա�Hookװ��֮���DLL
       static HMODULE WINAPI LoadLibraryA(PCSTR  pszModulePath);
       static HMODULE WINAPI LoadLibraryW(PCWSTR pszModulePath);
       static HMODULE WINAPI LoadLibraryExA(PCSTR  pszModulePath,HANDLE hFile, DWORD dwFlags);
       static HMODULE WINAPI LoadLibraryExW(PCWSTR pszModulePath,HANDLE hFile, DWORD dwFlags);

       // ��̬�õ������ĵ�ַʱ��Ҳ�������Hook
       static FARPROC WINAPI GetProcAddress(HMODULE hmod, PCSTR pszProcName);

private:

       //ȱʡ��API Hook
       static TAPIHook LoadLibraryAHook;
       static TAPIHook LoadLibraryWHook;
       static TAPIHook LoadLibraryExAHook;
       static TAPIHook LoadLibraryExWHook;
       static TAPIHook GetProcAddressHook;
};


#endif