// CustomOccManager.h: interface for the CCustomOccManager class.
//
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//=--------------------------------------------------------------------------=
//  (C) Copyright 1996-1999 Microsoft Corporation. All Rights Reserved.
//=--------------------------------------------------------------------------=
// 
// NOTE: 
// Some of the code in this file is MFC implementation specific.
// Changes in future versions of MFC implementation may require
// the code to be changed. Please check the readme of this
// sample for more information 
// 
#if !defined(AFX_CUSTOMOCCMANAGER_H__0F4BA643_A211_4676_9301_6C898B2E789B__INCLUDED_)
#define AFX_CUSTOMOCCMANAGER_H__0F4BA643_A211_4676_9301_6C898B2E789B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//Knowledge Base  
//Q236312 HOWTO: Disable the Default Pop-up Menu for CHtmlView

class CCustomOccManager : public COccManager  
{
public:
	CCustomOccManager();
	virtual ~CCustomOccManager();
	COleControlSite* CreateSite(COleControlContainer* pCtrlCont);

};

#endif // !defined(AFX_CUSTOMOCCMANAGER_H__0F4BA643_A211_4676_9301_6C898B2E789B__INCLUDED_)
