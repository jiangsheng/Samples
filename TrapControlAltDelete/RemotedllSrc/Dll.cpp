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

#include <string>

using namespace std;

//---------------------------------------------------------------------------

HWND hSASWnd;
FARPROC FOldProc;

LRESULT CALLBACK SASWindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

BOOL CALLBACK EnumWindowsProc(HWND hwnd,LPARAM lParam);

//---------------------------------------------------------------------------

HANDLE hThread = NULL;
DWORD  dwThreadId;

DWORD WINAPI ThreadFunc();

//---------------------------------------------------------------------------
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
  switch(ul_reason_for_call)
  {
    case DLL_PROCESS_ATTACH :

      hThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadFunc,NULL,0,&dwThreadId);
      break;

    case DLL_PROCESS_DETACH :

      if(FOldProc != NULL)
      {
        SetWindowLong(hSASWnd,GWL_WNDPROC,long(FOldProc));
      }
      CloseHandle(hThread);
      break;
  }
  return TRUE;
}
//---------------------------------------------------------------------------
DWORD WINAPI ThreadFunc()
{
  HDESK hDesk;

  hDesk = OpenDesktop("Winlogon",0,false,MAXIMUM_ALLOWED);

  FOldProc = NULL;
  hSASWnd = NULL;

  EnumDesktopWindows(hDesk,(WNDENUMPROC)EnumWindowsProc,0);

  if(hSASWnd != NULL)
  {
    FOldProc = (FARPROC)SetWindowLong(hSASWnd,GWL_WNDPROC,long(SASWindowProc));
  }
  CloseHandle(hDesk); 

  return 1;
}
//---------------------------------------------------------------------------
//����"Winlogon"����Ĵ���
BOOL CALLBACK EnumWindowsProc(HWND hwnd,LPARAM lParam)
{
  char ClassBuf[128];

  GetWindowText(hwnd,ClassBuf,sizeof(ClassBuf));

  //���Լ�д��һ��ϵͳ����Ȼ������߲�ѯ"Winlogon"�����ϵĴ��ڣ����������ϴ���
  //����"SAS window"��
  string ClassName(ClassBuf); 
  

  if(ClassName.find("SAS window") != -1)
  {
    hSASWnd = hwnd;
    return false;
  }
  

  return true;
}
//---------------------------------------------------------------------------
//SAS���ڵĴ��ڹ���
LRESULT CALLBACK SASWindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
  //����Ctrl + Alt + Del
  if(uMsg == WM_HOTKEY)
  {
    WORD wKey = HIWORD(lParam);
    WORD wModifier = LOWORD(lParam);

    bool IsCtrlDown  = ((wModifier & VK_CONTROL) != 0);
    bool IsAltDown   = ((wModifier & VK_MENU) != 0);
    bool IsShiftDown = ((wModifier & VK_SHIFT) != 0);

    //����Ctrl + Alt + Del��ϼ�
    if(IsCtrlDown && IsAltDown && wKey == VK_DELETE)
    {
      return 1;
    }
    //����Ctrl + Shift + Esc��ϼ��������ϼ�����ʾ������������ɸ�����Ҫ�Ƿ����Ρ�
    else if(IsCtrlDown && IsShiftDown && wKey == VK_ESCAPE)
    {
      // Do nothing
    }                                   
  }

  return CallWindowProc((WNDPROC)FOldProc,hwnd,uMsg,wParam,lParam);
}
//---------------------------------------------------------------------------

