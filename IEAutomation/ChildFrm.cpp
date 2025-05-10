// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "IEAutomation.h"

#include "ChildFrm.h"
#include "IEAutomationDoc.h"
#include "IEAutomationView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame
IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_CBN_SELENDOK(AFX_IDW_TOOLBAR + 1,OnNewAddress)
	ON_COMMAND_RANGE(0xe00, 0xfff, OnFavorite)
	ON_COMMAND(IDOK, OnNewAddressEnter)
	ON_NOTIFY(TBN_DROPDOWN, AFX_IDW_TOOLBAR, OnDropDown)
	ON_COMMAND(ID_FAVORITES_DROPDOWN, DoNothing)
	ON_COMMAND(ID_FONT_DROPDOWN, DoNothing)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction
static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
	
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers
/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers
int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CImageList img;
	CString str;

	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndReBar.Create(this))
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}

	if (!m_wndToolBar.CreateEx(this))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	// set up toolbar properties
	m_wndToolBar.GetToolBarCtrl().SetButtonWidth(50, 150);
	m_wndToolBar.GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);

	img.Create(IDB_HOTTOOLBAR, 22, 0, RGB(255, 0, 255));
	m_wndToolBar.GetToolBarCtrl().SetHotImageList(&img);
	img.Detach();
	img.Create(IDB_COLDTOOLBAR, 22, 0, RGB(255, 0, 255));
	m_wndToolBar.GetToolBarCtrl().SetImageList(&img);
	img.Detach();
	m_wndToolBar.ModifyStyle(0, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT);
	m_wndToolBar.SetButtons(NULL, 9);

	// set up each toolbar button
	m_wndToolBar.SetButtonInfo(0, ID_GO_BACK, TBSTYLE_BUTTON, 0);
	str.LoadString(IDS_BACK);
	m_wndToolBar.SetButtonText(0, str);
	m_wndToolBar.SetButtonInfo(1, ID_GO_FORWARD, TBSTYLE_BUTTON, 1);
	str.LoadString(IDS_FORWARD);
	m_wndToolBar.SetButtonText(1, str);
	m_wndToolBar.SetButtonInfo(2, ID_VIEW_STOP, TBSTYLE_BUTTON, 2);
	str.LoadString(IDS_STOP);
	m_wndToolBar.SetButtonText(2, str);
	m_wndToolBar.SetButtonInfo(3, ID_VIEW_REFRESH, TBSTYLE_BUTTON, 3);
	str.LoadString(IDS_REFRESH);
	m_wndToolBar.SetButtonText(3, str);
	m_wndToolBar.SetButtonInfo(4, ID_GO_START_PAGE, TBSTYLE_BUTTON, 4);
	str.LoadString(IDS_HOME);
	m_wndToolBar.SetButtonText(4, str);
	m_wndToolBar.SetButtonInfo(5, ID_GO_SEARCH_THE_WEB, TBSTYLE_BUTTON, 5);
	str.LoadString(IDS_SEARCH);
	m_wndToolBar.SetButtonText(5, str);
	m_wndToolBar.SetButtonInfo(6, ID_FAVORITES_DROPDOWN, TBSTYLE_BUTTON | TBSTYLE_DROPDOWN, 6);
	str.LoadString(IDS_FAVORITES);
	m_wndToolBar.SetButtonText(6, str);
	m_wndToolBar.SetButtonInfo(7, ID_FILE_PRINT, TBSTYLE_BUTTON, 7);
	str.LoadString(IDS_PRINT);
	m_wndToolBar.SetButtonText(7, str);
	m_wndToolBar.SetButtonInfo(8, ID_FONT_DROPDOWN, TBSTYLE_BUTTON | TBSTYLE_DROPDOWN, 8);
	str.LoadString(IDS_FONT);
	m_wndToolBar.SetButtonText(8, str);

	CRect rectToolBar;

	// set up toolbar button sizes
	m_wndToolBar.GetItemRect(0, &rectToolBar);
	m_wndToolBar.SetSizes(rectToolBar.Size(), CSize(30,20));

	// create a combo box for the address bar
	if (!m_wndAddress.Create(CBS_DROPDOWN | WS_CHILD, CRect(0, 0, 200, 120), this, AFX_IDW_TOOLBAR + 1))
	{
		TRACE0("Failed to create combobox\n");
		return -1;      // fail to create
	}
	CEdit*	pEdit=m_wndAddress.GetEditCtrl();
	if(pEdit){
		::SHAutoComplete(pEdit->GetSafeHwnd(),SHACF_DEFAULT);
		#pragma message(__LOC__"use custom autocomplete Objects if u need better user experience.\r\n") 
	}
	// create the animation control
	m_wndAnimate.Create(WS_CHILD | WS_VISIBLE, CRect(0, 0, 10, 10), this, AFX_IDW_TOOLBAR + 2);
	m_wndAnimate.Open(IDR_MFCAVI);

	// add the toolbar, animation, and address bar to the rebar
	m_wndReBar.AddBar(&m_wndToolBar);
	m_wndReBar.AddBar(&m_wndAnimate, NULL, NULL, RBBS_FIXEDSIZE | RBBS_FIXEDBMP);
	str.LoadString(IDS_ADDRESS);
	m_wndReBar.AddBar(&m_wndAddress, str, NULL, RBBS_FIXEDBMP | RBBS_BREAK);

	// set up min/max sizes and ideal sizes for pieces of the rebar
	REBARBANDINFO rbbi;

	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_SIZE;
	rbbi.cxMinChild = rectToolBar.Width();
	rbbi.cyMinChild = rectToolBar.Height();
	rbbi.cx = rbbi.cxIdeal = rectToolBar.Width() * 9;
	m_wndReBar.GetReBarCtrl().SetBandInfo(0, &rbbi);
	rbbi.cxMinChild = 0;

	CRect rectAddress;

	rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE;
	m_wndAddress.GetEditCtrl()->GetWindowRect(&rectAddress);
	rbbi.cyMinChild = rectAddress.Height() + 10;
	rbbi.cxIdeal = 200;
	m_wndReBar.GetReBarCtrl().SetBandInfo(2, &rbbi);

	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_FIXED);

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	//here GetActiveDocument returns NULL
	//go get it from Create Context
	MDICREATESTRUCT * pMDICreateStruct=(MDICREATESTRUCT * )lpCreateStruct->lpCreateParams;
	CCreateContext *pCreateContext=(CCreateContext *)pMDICreateStruct->lParam;
	BuildFavoritesMenu((CIEAutomationDoc *)pCreateContext->m_pCurrentDoc);
	return 0;
}
void	CChildFrame::BuildFavoritesMenu(CIEAutomationDoc * pDoc/*=NULL*/)
{
	if(pDoc==NULL)
		pDoc=(CIEAutomationDoc *)GetActiveDocument();
	if(pDoc==NULL)
		return;
// set up Favorites menu
	TCHAR           sz[MAX_PATH];
	TCHAR           szPath[MAX_PATH];
	HKEY            hKey;
	DWORD           dwSize;
	CMenu*          pMenu=NULL;

	
	CMenu& rMenu=pDoc->m_menuDefault;
	CMenu* pPopup=NULL;

	//find the favorites menu since its position is dynamic
	int nMenuCount=rMenu.GetMenuItemCount();
	for(int i=0;i<nMenuCount;i++){
		pPopup =rMenu.GetSubMenu(i);
		if(pPopup){
			if(pPopup->GetMenuItemID(0)==ID_FAVORITES_ADD){
				pMenu =pPopup;
			}
		}
	}
	if(pMenu==NULL)return;
	// first get rid of bogus submenu items.
	m_astrFavoriteURLs.RemoveAll();
	while(pMenu->DeleteMenu(0, MF_BYPOSITION));
	// find out from the registry where the favorites are located.
	if(RegOpenKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\User Shell Folders"), &hKey) != ERROR_SUCCESS)
	{
		TRACE0("Favorites folder not found\n");
	}
	dwSize = sizeof(sz);
	RegQueryValueEx(hKey, _T("Favorites"), NULL, NULL, (LPBYTE)sz, &dwSize);
	ExpandEnvironmentStrings(sz, szPath, MAX_PATH);
	RegCloseKey(hKey);

	BuildFavoritesMenu(szPath, 0, pMenu);
	//insert menu item to find it again
	pMenu->InsertMenu(0,MF_BYPOSITION|MF_STRING | MF_ENABLED, ID_FAVORITES_MANAGE, _T("&Manage..."));
	pMenu->InsertMenu(0,MF_BYPOSITION|MF_STRING | MF_ENABLED, ID_FAVORITES_ADD, _T("&Add..."));	
	//update menu
	OnUpdateFrameMenu(TRUE,this,NULL);
	((CFrameWnd*)AfxGetMainWnd())->OnUpdateFrameMenu(NULL);
	DrawMenuBar();
	AfxGetMainWnd()->DrawMenuBar();
}
//from MFCIE sample
int CChildFrame::BuildFavoritesMenu(LPCTSTR pszPath, int nStartPos, CMenu* pMenu)
{
	CString         strPath(pszPath);
	CString         strPath2;
	CString         str;
	WIN32_FIND_DATA wfd;
	HANDLE          h;
	int             nPos;
	int             nEndPos;
	int             nNewEndPos;
	int             nLastDir;
	TCHAR           buf[INTERNET_MAX_PATH_LENGTH];
	CStringArray    astrFavorites;
	CStringArray    astrDirs;
	CMenu*          pSubMenu;


	// make sure there's a trailing backslash
	if(strPath[strPath.GetLength() - 1] != _T('\\'))
		strPath += _T('\\');
	strPath2 = strPath;
	strPath += "*.*";

	// now scan the directory, first for .URL files and then for subdirectories
	// that may also contain .URL files
	h = FindFirstFile(strPath, &wfd);
	if(h != INVALID_HANDLE_VALUE)
	{
		nEndPos = nStartPos;
		do
		{
			if((wfd.dwFileAttributes & (FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_HIDDEN|FILE_ATTRIBUTE_SYSTEM))==0)
			{
				str = wfd.cFileName;
				if(str.Right(4) == _T(".url"))
				{
					// an .URL file is formatted just like an .INI file, so we can
					// use GetPrivateProfileString() to get the information we want
					::GetPrivateProfileString(_T("InternetShortcut"), _T("URL"),
											  _T(""), buf, INTERNET_MAX_PATH_LENGTH,
											  strPath2 + str);
					str = str.Left(str.GetLength() - 4);

					// scan through the array and perform an insertion sort
					// to make sure the menu ends up in alphabetic order
					for(nPos = nStartPos ; nPos < nEndPos ; ++nPos)
					{
						if(str.CompareNoCase(astrFavorites[nPos]) < 0)
							break;
					}
					astrFavorites.InsertAt(nPos, str);
					m_astrFavoriteURLs.InsertAt(nPos, buf);
					++nEndPos;
				}
			}
		} while(FindNextFile(h, &wfd));
		FindClose(h);
		// Now add these items to the menu
		for(nPos = nStartPos ; nPos < nEndPos ; ++nPos)
		{
			pMenu->AppendMenu(MF_STRING | MF_ENABLED, 0xe00 + nPos, astrFavorites[nPos]);
		}


		// now that we've got all the .URL files, check the subdirectories for more
		nLastDir = 0;
		h = FindFirstFile(strPath, &wfd);
		ASSERT(h != INVALID_HANDLE_VALUE);
		do
		{
			if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				// ignore the current and parent directory entries
				if(lstrcmp(wfd.cFileName, _T(".")) == 0 || lstrcmp(wfd.cFileName, _T("..")) == 0)
					continue;

				for(nPos = 0 ; nPos < nLastDir ; ++nPos)
				{
					if(astrDirs[nPos].CompareNoCase(wfd.cFileName) > 0)
						break;
				}
				pSubMenu = new CMenu;
				pSubMenu->CreatePopupMenu();

				// call this function recursively.
				nNewEndPos = BuildFavoritesMenu(strPath2 + wfd.cFileName, nEndPos, pSubMenu);
				if(nNewEndPos != nEndPos)
				{
					// only intert a submenu if there are in fact .URL files in the subdirectory
					nEndPos = nNewEndPos;
					pMenu->InsertMenu(nPos, MF_BYPOSITION | MF_POPUP | MF_STRING, (UINT)pSubMenu->m_hMenu, wfd.cFileName);
					pSubMenu->Detach();
					astrDirs.InsertAt(nPos, wfd.cFileName);
					++nLastDir;
				}
				delete pSubMenu;
			}
		} while(FindNextFile(h, &wfd));
		FindClose(h);
	}
	return nEndPos;
}

void CChildFrame::OnFavorite(UINT nID)
{
	((CIEAutomationView*)GetActiveView())->Navigate2(m_astrFavoriteURLs[nID-0xe00], 0, NULL);
}

void CChildFrame::SetAddress(LPCTSTR lpszUrl)
{
	// This is called when the browser has completely loaded the new location,
	// so make sure the text in the address bar is up to date and stop the
	// animation.
	if(m_wndAddress.m_hWnd)
		m_wndAddress.SetWindowText(lpszUrl);
	if(m_wndAnimate.m_hWnd){
		m_wndAnimate.Stop();
		m_wndAnimate.Seek(0);
	}
}

void CChildFrame::StartAnimation()
{
	// Start the animation.  This is called when the browser begins to
	// navigate to a new location
	if(m_wndAnimate.m_hWnd)
		m_wndAnimate.Play(0, -1, -1);
}

void CChildFrame::OnNewAddress()
{
	// gets called when an item in the Address combo box is selected
	// just navigate to the newly selected location.
	CString str;

	m_wndAddress.GetLBText(m_wndAddress.GetCurSel(), str);
	((CIEAutomationView*)GetActiveView())->Navigate2(str, 0, NULL);
}

void CChildFrame::OnNewAddressEnter()
{
	// gets called when an item is entered manually into the edit box portion
	// of the Address combo box.
	// navigate to the newly selected location and also add this address to the
	// list of addresses in the combo box.
	CString str;

	m_wndAddress.GetEditCtrl()->GetWindowText(str);
	((CIEAutomationView*)GetActiveView())->Navigate2(str, 0, NULL);

	COMBOBOXEXITEM item;

	item.mask = CBEIF_TEXT;
	item.iItem = -1;
	item.pszText = (LPTSTR)(LPCTSTR)str;
	m_wndAddress.InsertItem(&item);
}

void CChildFrame::DoNothing()
{
	// this is here only so that the toolbar buttons for the dropdown menus
	// will have a callback, and thus will not be disabled.
}

void CChildFrame::OnDropDown(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	// this function handles the dropdown menus from the toolbar
	NMTOOLBAR* pNMToolBar = (NMTOOLBAR*)pNotifyStruct;
	CRect rect;

	// translate the current toolbar item rectangle into screen coordinates
	// so that we'll know where to pop up the menu
	m_wndToolBar.GetToolBarCtrl().GetRect(pNMToolBar->iItem, &rect);
	rect.top = rect.bottom;
	::ClientToScreen(pNMToolBar->hdr.hwndFrom, &rect.TopLeft());
	if(pNMToolBar->iItem == ID_FONT_DROPDOWN)
	{
		CMenu menu;
		CMenu* pPopup;

		// the font popup is stored in a resource
		menu.LoadMenu(IDR_FONT_POPUP);
		pPopup = menu.GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, rect.left, rect.top + 1, AfxGetMainWnd());
	}
	else if(pNMToolBar->iItem == ID_FAVORITES_DROPDOWN)
	{
		CMenu& rMenu=((CIEAutomationDoc *)GetActiveDocument())->m_menuDefault;
		CMenu* pPopup;

		// for the favorties popup, just steal the menu from the main window
		//pPopup = rMenu.GetSubMenu(rMenu.GetMenuItemCount()-4);
		int nMenuCount=rMenu.GetMenuItemCount();
		for(int i=0;i<nMenuCount;i++){
			pPopup =rMenu.GetSubMenu(i);
			if(pPopup){
				if(pPopup->GetMenuItemID(0)==ID_FAVORITES_ADD){
					pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, rect.left, rect.top + 1, AfxGetMainWnd());
				}
			}
		}
		
	}
	*pResult = TBDDRET_DEFAULT;
}
