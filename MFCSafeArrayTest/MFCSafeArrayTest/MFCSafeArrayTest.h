
// MFCSafeArrayTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMFCSafeArrayTestApp: 
// �йش����ʵ�֣������ MFCSafeArrayTest.cpp
//

class CMFCSafeArrayTestApp : public CWinApp
{
public:
	CMFCSafeArrayTestApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMFCSafeArrayTestApp theApp;