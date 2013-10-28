
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "ColumnProviderHost.h"

#include "MainFrm.h"
#include "ColumnProviderHostDoc.h"
#include "ColumnProviderHostView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_RESETTOOLBAR, OnToolbarReset)
	ON_REGISTERED_MESSAGE(AFX_WM_TOOLBARMENU, OnToolbarContextMenu)
	ON_COMMAND(ID_VIEW_FOLDERS, OnViewFolders)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FOLDERS, OnUpdateViewFolders)
	ON_CBN_SELENDOK(AFX_IDW_TOOLBAR + 1, OnNewAddress)
	ON_COMMAND(IDOK, OnNewAddressEnter)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	CCreateContext* pCreateContext=(CCreateContext*)lpCreateStruct->lpCreateParams;
	CColumnProviderHostDoc* pDocument= (CColumnProviderHostDoc*)pCreateContext->m_pCurrentDoc;
	
	

	// set the visual manager and style based on persisted value
	OnApplicationLook(theApp.m_nAppLook);

	InitBasicCommands();

	
	if (InitMenuBar() == -1)
		return -1;
	if (InitToolBar() == -1)
		return -1;
	if (InitAddressBox(pDocument) == -1)
		return -1;
	if (InitRebar() == -1)
		return -1;

	// TODO: Remove this if you don't want chevrons:
	m_wndMenuBar.EnableCustomizeButton (TRUE, -1, _T(""));
	m_wndToolBar.EnableCustomizeButton (TRUE, -1, _T(""));
	BOOL bToolTipsOn = CMFCToolBar::GetShowTooltips();
	
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	if (InitFolderPane(pDocument) == -1)
		return -1;

	InitDocking();

	

	return 0;
}

int CMainFrame::InitFolderPane(CColumnProviderHostDoc *pDocument)
{

	CString folderDockablepaneCaption;
	folderDockablepaneCaption.LoadString(IDS_Folders);
	if (!folderDockablePane.Create (folderDockablepaneCaption,
		this, CRect (0, 0, 200, 200), TRUE, ID_VIEW_FOLDERS,
		WS_CHILD | WS_VISIBLE | CBRS_LEFT | CBRS_HIDE_INPLACE | WS_CAPTION))
	{
		TRACE0("Failed to create folders bar\n");
		return -1;      // fail to create
	}
	pDocument->SetShellTree(&folderDockablePane.GetShellTree());
	return 0;
}
int CMainFrame::InitRebar()
{
	// Each rebar pane will ocupy its own row:
	DWORD dwStyle = RBBS_GRIPPERALWAYS | RBBS_FIXEDBMP | RBBS_BREAK;

	if (!m_wndReBar.Create(this) ||
		!m_wndReBar.AddBar (&m_wndMenuBar) ||
		!m_wndReBar.AddBar (&m_wndToolBar, NULL, NULL, dwStyle) ||
		!m_wndReBar.AddBar(&addressBox, _T("Address"), NULL, dwStyle))
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.AdjustLayout ();
	m_wndToolBar.AdjustLayout ();
	return 0;
}
int CMainFrame::InitAddressBox(CColumnProviderHostDoc *pDocument)
{
	//----------------------------------------
	// Create a combo box for the address bar:
	//----------------------------------------
	if (!addressBox.Create (CBS_DROPDOWN | WS_CHILD, CRect(0, 0, 200, 120), this, AFX_IDW_TOOLBAR + 1))
	{
		TRACE0("Failed to create combobox\n");
		return -1;      // fail to create
	}
	pDocument->SetAddressBox(&addressBox);

	addressBox.EnableActiveAccessibility();
	return 0;
}
int CMainFrame::InitToolBar()
{
	
	// Detect color depth. 256 color toolbars can be used in the
	// high or true color modes only (bits per pixel is > 8):
	CClientDC dc (this);
	BOOL bIsHighColor = dc.GetDeviceCaps (BITSPIXEL) > 8;

	UINT uiToolbarHotID = bIsHighColor ? IDB_TOOLBAR256 : 0;
	UINT uiToolbarColdID = bIsHighColor ? IDB_TOOLBARCOLD256 : 0;
	UINT uiMenuID = bIsHighColor ? IDB_MENU256 : IDB_MENU16;

	if (!m_wndToolBar.CreateEx(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME, uiToolbarColdID, uiMenuID, FALSE, 0, 0, uiToolbarHotID))
	{
		TRACE0("Failed to create toolbar\n");
		
		return -1;      // fail to create
	}

	// Remove toolbar gripper and borders:
	m_wndToolBar.SetPaneStyle (m_wndToolBar.GetPaneStyle() &
		~(CBRS_GRIPPER | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	BOOL bValidString;
	CString strMainToolbarTitle;
	bValidString = strMainToolbarTitle.LoadString (IDS_MAIN_TOOLBAR);
	if(bValidString)
		m_wndToolBar.SetWindowText (strMainToolbarTitle);

	// TODO: Remove this if you don't want tool tips
	
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);
	return 0;
}
void CMainFrame::InitBasicCommands()
{
	CMFCToolBar::EnableQuickCustomization ();

	//---------------------------------
	// Set toolbar and menu image size:
	//---------------------------------
	CMFCToolBar::SetSizes (CSize (28, 28), CSize (22, 22));
	CMFCToolBar::SetMenuSizes (CSize (22, 22), CSize (16, 16));

	// TODO: Define your own basic commands. Be sure, that each pulldown
	// menu have at least one basic command.

	CList<UINT, UINT>	lstBasicCommands;

	lstBasicCommands.AddTail (ID_VIEW_TOOLBARS);
	lstBasicCommands.AddTail (ID_APP_EXIT);
	lstBasicCommands.AddTail (ID_APP_ABOUT);
	lstBasicCommands.AddTail (ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail (ID_VIEW_CUSTOMIZE);
	lstBasicCommands.AddTail (ID_COMMAND_HISTORY);
	lstBasicCommands.AddTail (ID_VIEW_LARGEICON);
	lstBasicCommands.AddTail (ID_VIEW_SMALLICON);
	lstBasicCommands.AddTail (ID_VIEW_LIST);
	lstBasicCommands.AddTail (ID_VIEW_DETAILS);
	lstBasicCommands.AddTail (ID_EDIT_CUT);
	lstBasicCommands.AddTail (ID_EDIT_COPY);
	lstBasicCommands.AddTail (ID_EDIT_PASTE);

	CMFCToolBar::SetBasicCommands (lstBasicCommands);
}
int CMainFrame::InitMenuBar()
{
	if (!m_wndMenuBar.Create (this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC);

	// Remove menubar gripper and borders:
	m_wndMenuBar.SetPaneStyle (m_wndMenuBar.GetPaneStyle() &
		~(CBRS_GRIPPER | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	// TODO: Remove this if you don't want tool tips
	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);
	return 0;
}
void CMainFrame::InitDocking()
{
	EnableDocking (CBRS_ALIGN_ANY);

	m_wndReBar.EnableDocking (CBRS_TOP);
	DockPane (&m_wndReBar);

	folderDockablePane.EnableDocking (CBRS_LEFT | CBRS_RIGHT);
	DockPane (&folderDockablePane);
}
/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnViewCustomize()
{
	//------------------------------------
	// Create a customize toolbars dialog:
	//------------------------------------
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog (this,
		TRUE /* Automatic menus scaning */
		);

	pDlgCust->Create ();
}

LRESULT CMainFrame::OnToolbarContextMenu(WPARAM,LPARAM lp)
{
	CPoint point (AFX_GET_X_LPARAM(lp), AFX_GET_Y_LPARAM(lp));

	CMenu menu;
	VERIFY(menu.LoadMenu (IDR_POPUP_TOOLBAR));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	if (pPopup)
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;
		pPopupMenu->Create (this, point.x, point.y, pPopup->Detach ());
	}

	return 0;
}

afx_msg LRESULT CMainFrame::OnToolbarReset(WPARAM wp, LPARAM)
{
	UINT uiToolBarId = (UINT) wp;
	if (uiToolBarId != IDR_MAINFRAME)
	{
		return 0;
	}

	// Replace "Back" and "Forward" buttons by the menu buttons
	// with the history lists:

	CMenu menuHistory;
	menuHistory.LoadMenu (IDR_HISTORY_POPUP);
	CString buttonText;
	buttonText.LoadString(IDS_Back);
	CMFCToolBarMenuButton btnBack (ID_GO_BACK, menuHistory, 
					GetCmdMgr ()->GetCmdImage (ID_GO_BACK), buttonText);
	btnBack.m_bText = TRUE;
	m_wndToolBar.ReplaceButton (ID_GO_BACK, btnBack);
	buttonText.LoadString(IDS_Forward);
	m_wndToolBar.ReplaceButton (ID_GO_FORWARD,
		CMFCToolBarMenuButton (ID_GO_FORWARD, menuHistory, 
					GetCmdMgr ()->GetCmdImage (ID_GO_FORWARD), _T("Forward")));
	buttonText.LoadString(IDS_Folders);
	// "Folders" button has a text label:
	m_wndToolBar.SetToolBarBtnText (m_wndToolBar.CommandToIndex (ID_VIEW_FOLDERS),
		buttonText);

	// Replace "Views" button by the menu button:
	CMenu menuViews;
	menuViews.LoadMenu (IDR_VIEWS_POPUP);
	buttonText.LoadString(IDS_Views);
	m_wndToolBar.ReplaceButton (ID_VIEW_VIEWS,
		CMFCToolBarMenuButton ((UINT)-1, menuViews, 
					GetCmdMgr ()->GetCmdImage (ID_VIEW_VIEWS), buttonText));

	return 0;
}

BOOL CMainFrame::OnShowPopupMenu (CMFCPopupMenu* pMenuPopup)
{
	//---------------------------------------------------------
	// Replace ID_VIEW_TOOLBARS menu item to the toolbars list:
	//---------------------------------------------------------
	CFrameWndEx::OnShowPopupMenu (pMenuPopup);

	if (pMenuPopup != NULL &&
		pMenuPopup->GetMenuBar ()->CommandToIndex (ID_VIEW_TOOLBARS) >= 0)
	{
		if (CMFCToolBar::IsCustomizeMode ())
		{
			//----------------------------------------------------
			// Don't show toolbars list in the cuztomization mode!
			//----------------------------------------------------
			return FALSE;
		}

		pMenuPopup->RemoveAllItems ();

		CMenu menu;
		VERIFY(menu.LoadMenu (IDR_POPUP_TOOLBAR));

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);

		if (pPopup)
		{
			pMenuPopup->GetMenuBar ()->ImportFromMenu (*pPopup, TRUE);
		}
	}

	return TRUE;
}

void CMainFrame::OnViewFolders() 
{
	ShowPane (&folderDockablePane,
					!(folderDockablePane.GetStyle () & WS_VISIBLE),
					FALSE,TRUE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateViewFolders(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (folderDockablePane.GetStyle () & WS_VISIBLE);
}

CMFCShellTreeCtrl& CMainFrame::GetFolders ()
{
	return folderDockablePane.GetShellTree();
}

void CMainFrame::OnNewAddress()
{
	// gets called when an item in the Address combo box is selected
	// just navigate to the newly selected location.
	CString str;

	addressBox.GetLBText(addressBox.GetCurSel(), str);
	CColumnProviderHostDoc* pDocument= (CColumnProviderHostDoc*)GetActiveDocument();
	pDocument->NavigateToList(str);

}

void CMainFrame::OnNewAddressEnter()
{
	CString str;

	addressBox.GetEditCtrl()->GetWindowText(str);
	CColumnProviderHostDoc* pDocument= (CColumnProviderHostDoc*)GetActiveDocument();
	pDocument->NavigateToList(str);

	COMBOBOXEXITEM item;

	item.mask = CBEIF_TEXT;
	item.iItem = -1;
	item.pszText = (LPTSTR)(LPCTSTR)str;
	addressBox.InsertItem(&item);
}


BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYUP)
	{	
		if (pMsg->wParam == VK_TAB)
		{
			ActivateByTab();
		}
	}
	return CFrameWndEx::PreTranslateMessage(pMsg);
}

void CMainFrame::ActivateByTab()
{
	enum eActiveWindow { eAdressBar = 0, eFolderBar = 1, eExplorerView = 2 };
	int  nActiveWindow = eAdressBar;

	CWnd *pWnd = GetFocus();
	if (pWnd != NULL)
	{
		ASSERT_VALID(pWnd);
		
		if (pWnd->IsKindOf(RUNTIME_CLASS(CMFCShellListCtrl)))
		{
			nActiveWindow = eExplorerView;
		}
		if (pWnd->IsKindOf(RUNTIME_CLASS(CMFCShellTreeCtrl)))
		{
			nActiveWindow = eFolderBar;
		}
	}
	switch (nActiveWindow)
	{
	case eAdressBar:
		if (folderDockablePane.IsVisible())
		{
			folderDockablePane.SetFocus();
		}
		else
		{
			SetFocus();
		}
		break;
	case eFolderBar:
		SetFocus();
		break;
	case eExplorerView:
		addressBox.SetFocus();
		break;
	default:
		break;
	}
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}
