// CustomOccManager.cpp: implementation of the CCustomOccManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IEAutomation.h"

#include <Transact.h>
#undef AFX_DATA
#define AFX_DATA AFX_DATA_IMPORT
// NOTE: This line is a hardcoded reference to an MFC header file
//  this path may need to be changed to refer to the location of VC5 install
//  for successful compilation.
#include <..\src\occimpl.h>
#undef AFX_DATA
#define AFX_DATA AFX_DATA_EXPORT

#include "CustomControlSite.h"
#include "CustomOccManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomOccManager::CCustomOccManager()
{

}

CCustomOccManager::~CCustomOccManager()
{

}
COleControlSite* CCustomOccManager::CreateSite(COleControlContainer* pCtrlCont)
{
	CCustomControlSite *pSite = new CCustomControlSite(pCtrlCont);
	return pSite;
}