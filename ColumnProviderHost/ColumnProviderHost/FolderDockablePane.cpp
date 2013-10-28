// FolderDockablePane.cpp : implementation file
//

#include "stdafx.h"
#include "ColumnProviderHost.h"
#include "FolderDockablePane.h"


// CFolderDockablePane
const int idTree = 1;
UINT shellChangeNotifyRegisterMessage = ::RegisterWindowMessage(_T("UWM_NOTIFY_RECYCLE_BIN"));
IMPLEMENT_DYNAMIC(FolderDockablePane, CDockablePane)

FolderDockablePane::FolderDockablePane()
{

}

FolderDockablePane::~FolderDockablePane()
{
}


BEGIN_MESSAGE_MAP(FolderDockablePane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_ERASEBKGND()
	ON_REGISTERED_MESSAGE(shellChangeNotifyRegisterMessage, OnShellChangeNotify)
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CFolderDockablePane message handlers



/////////////////////////////////////////////////////////////////////////////
// CFolderBar message handlers

int FolderDockablePane::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rectDummy (0, 0, 0, 0);
	const DWORD dwViewStyle =	WS_CHILD | WS_VISIBLE | TVS_HASLINES | 
								TVS_LINESATROOT | TVS_HASBUTTONS;

	shellTree.Create(dwViewStyle, rectDummy, this, idTree);
	
	LPITEMIDLIST ppidl;
	if (SHGetSpecialFolderLocation(GetSafeHwnd(),CSIDL_BITBUCKET, &ppidl) != NOERROR)
	{
		return 0;
	}

	SHChangeNotifyEntry scnIDL;
	scnIDL.pidl = ppidl;
	scnIDL.fRecursive = TRUE;

	shellChangeNotifyDeregisterCookie = SHChangeNotifyRegister(m_hWnd, SHCNRF_InterruptLevel | SHCNRF_ShellLevel|SHCNRF_RecursiveInterrupt, SHCNE_ALLEVENTS,  shellChangeNotifyRegisterMessage, 1, &scnIDL);
	return 0;
}

void FolderDockablePane::OnSize(UINT nType, int cx, int cy) 
{
	CDockablePane::OnSize(nType, cx, cy);
	
	shellTree.SetWindowPos (NULL, 0, 0,
		cx, cy,
		SWP_NOACTIVATE | SWP_NOZORDER);
}

void FolderDockablePane::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	shellTree.SetFocus ();
}

BOOL FolderDockablePane::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
}
LRESULT FolderDockablePane::OnShellChangeNotify(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	if (lParam == SHCNE_UPDATEIMAGE)
	{
		shellTree.Refresh();
	}

	if (lParam &(SHCNE_ATTRIBUTES|SHCNE_UPDATEITEM))
	{
		LPITEMIDLIST ppidl;
		if (SHGetSpecialFolderLocation(GetSafeHwnd(), CSIDL_BITBUCKET, &ppidl) != NOERROR)
		{
			return TRUE;
		}

		shellTree.SelectPath(ppidl);

		CMFCShellListCtrl* pList = shellTree.GetRelatedList();
		ASSERT_VALID(pList);

		pList->Refresh();
	}

	return TRUE;
}
void FolderDockablePane::OnDestroy()
{
	CDockablePane::OnDestroy();

	if (shellChangeNotifyDeregisterCookie != 0)
	{
		SHChangeNotifyDeregister(shellChangeNotifyDeregisterCookie);
	}
}
