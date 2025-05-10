#ifndef HOOKH
#define HOOKH

extern	void __fastcall InjectFunc();
extern	void __fastcall DeinjectFunc();
extern	CString __fastcall SysErrorMessage(DWORD dwLastError );
#endif