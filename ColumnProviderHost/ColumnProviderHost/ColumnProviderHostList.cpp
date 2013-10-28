// ColumnProviderHostList.cpp : implementation file
//

#include "stdafx.h"
#include "ColumnProviderHost.h"
#include "ColumnProviderHostDoc.h"
#include "ColumnProviderHostList.h"


// CColumnProviderHostList

IMPLEMENT_DYNAMIC(ColumnProviderHostList, CMFCShellListCtrl)

	ColumnProviderHostList::ColumnProviderHostList()
	:pDocument(NULL),suspendRefresh(TRUE)
{

}

ColumnProviderHostList::~ColumnProviderHostList()
{
}


BEGIN_MESSAGE_MAP(ColumnProviderHostList, CMFCShellListCtrl)
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, &ColumnProviderHostList::OnLvnGetdispinfo)
	//ON_NOTIFY_REFLECT(LVN_ODCACHEHINT, &ColumnProviderHostList::OnLvnOdcachehint)
	//ON_NOTIFY_REFLECT(LVN_ODFINDITEM, &ColumnProviderHostList::OnLvnOdfinditem)
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CColumnProviderHostList message handlers


void ColumnProviderHostList::OnSetColumns()
{
	suspendRefresh=true;
	// Delete all columns
	int nColumnCount = GetHeaderCtrl().GetItemCount();
	for (int i = 0; i < nColumnCount; i++)
	{
		DeleteColumn(0);
	}

	const TCHAR* szName [] = {
		_T("Name"), _T("Size"), _T("Type"), _T("Modified")};

		for (int iColumn = 0; iColumn <= AFX_ShellList_ColumnModified; iColumn++)
		{
			InsertColumn(iColumn, 
				szName [iColumn], 
				LVCFMT_LEFT, 
				100, 
				iColumn);
		}
		if(pDocument!=NULL)
		{

			pDocument->GenerateProviderColumns();
			int providerColumnCount=pDocument->GetProviderColumnCount();
			;

			for(int iProviderColumn= 0; iProviderColumn< providerColumnCount; iProviderColumn++)
			{
				int columnIndex=AFX_ShellList_ColumnModified+1+iProviderColumn;
				InsertColumn(columnIndex, 
					pDocument->GetProviderColumn(iProviderColumn)->wszTitle
					, iProviderColumn==providerColumnCount-1?LVCFMT_RIGHT:LVCFMT_LEFT, 100, 
					columnIndex);			
			}		
		}
		suspendRefresh=false;
}
CString ColumnProviderHostList::OnGetItemText(int iItem, int iColumn, LPAFX_SHELLITEMINFO pItem)
{
	CString itemText;
	if(iColumn<=AFX_ShellList_ColumnModified)
		return CMFCShellListCtrl::OnGetItemText(iItem, iColumn, pItem);
	if(pDocument!=NULL)
	{
		itemText=pDocument->OnGetItemText(iColumn-AFX_ShellList_ColumnModified-1, pItem);
	}
	return itemText;
}


HRESULT ColumnProviderHostList::EnumObjects(LPSHELLFOLDER pParentFolder, LPITEMIDLIST pidlParent)
{
	if(pDocument==NULL) return S_OK;

	ASSERT_VALID(this);
	CWinAppEx* pApp=(CWinAppEx*) AfxGetApp();
	CShellManager* pShellManager=pApp->GetShellManager();
	ASSERT_VALID(pShellManager);

	LPENUMIDLIST pEnum = NULL;
	HRESULT hRes = pParentFolder->EnumObjects(NULL, pDocument->GetItemTypes(), &pEnum);

	if (SUCCEEDED(hRes) && pEnum != NULL)
	{
		LPITEMIDLIST pidlTemp;
		DWORD dwFetched = 1;
		LPAFX_SHELLITEMINFO pItem;

		//enumerate the item's PIDLs
		while (pEnum->Next(1, &pidlTemp, &dwFetched) == S_OK && dwFetched)
		{
			LVITEM lvItem;
			ZeroMemory(&lvItem, sizeof(lvItem));

			//fill in the TV_ITEM structure for this item
			lvItem.mask = LVIF_PARAM | LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;

			//AddRef the parent folder so it's pointer stays valid
			pParentFolder->AddRef();

			//put the private information in the lParam
			pItem = (LPAFX_SHELLITEMINFO)GlobalAlloc(GPTR, sizeof(AFX_SHELLITEMINFO));

			pItem->pidlRel = pidlTemp;
			pItem->pidlFQ = pShellManager->ConcatenateItem(pidlParent, pidlTemp);

			pItem->pParentFolder = pParentFolder;
			lvItem.lParam = (LPARAM)pItem;

			lvItem.pszText = _T("");
			lvItem.iImage = OnGetItemIcon(GetItemCount(), pItem);

			//determine if the item is shared
			DWORD dwAttr = SFGAO_DISPLAYATTRMASK;
			pParentFolder->GetAttributesOf(1, (LPCITEMIDLIST*)&pidlTemp, &dwAttr);

			if (dwAttr & SFGAO_SHARE)
			{
				lvItem.mask |= LVIF_STATE;
				lvItem.stateMask |= LVIS_OVERLAYMASK;
				lvItem.state |= INDEXTOOVERLAYMASK(1); //1 is the index for the shared overlay image
			}

			if (dwAttr & SFGAO_GHOSTED)
			{
				lvItem.mask |= LVIF_STATE;
				lvItem.stateMask |= LVIS_CUT;
				lvItem.state |= LVIS_CUT;
			}

			int iItem = InsertItem(&lvItem);
			if (iItem >= 0)
			{
				// Set columns:
				const int nColumns = m_wndHeader.GetItemCount();
				for (int iColumn = 0; iColumn < nColumns; iColumn++)
				{
					if(iColumn <=AFX_ShellList_ColumnModified)
						SetItemText(iItem, iColumn,OnGetItemText(iItem, iColumn,pItem));
					else
						SetItemText(iItem, iColumn,LPSTR_TEXTCALLBACK);
				}
			}

			dwFetched = 0;
		}

		pEnum->Release();
	}
	if(suspendRefresh)
	{
		GetParent()->PostMessage(AFX_WM_CHANGE_CURRENT_FOLDER);
	}
	//hack tell foldersize we are monitering this folder
	CString currentFolder;
	this->GetCurrentFolder(currentFolder);
	pDocument->NotifyFolderSize(currentFolder);	
	return hRes;
}

void ColumnProviderHostList::OnLvnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: Add your control notification handler code here
	if(pDispInfo->item.mask&LVIF_TEXT)
	{
		if(pDispInfo->item.iSubItem>AFX_ShellList_ColumnModified)
		{
			lstrcpyn(pDispInfo->item.pszText,
				OnGetItemText(
					pDispInfo->item.iItem,
					pDispInfo->item.iSubItem,
					(AFX_SHELLITEMINFO*)pDispInfo->item.lParam)
				,pDispInfo->item.cchTextMax);
		}
	}
	*pResult = 0;
}


void ColumnProviderHostList::OnLvnOdcachehint(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVCACHEHINT pCacheHint = reinterpret_cast<LPNMLVCACHEHINT>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void ColumnProviderHostList::OnLvnOdfinditem(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVFINDITEM pFindInfo = reinterpret_cast<LPNMLVFINDITEM>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


int ColumnProviderHostList::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMFCShellListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	if(pDocument!=NULL)
	{
		SetItemTypes(pDocument->GetItemTypes());
		OnSetColumns();
	}
	return 0;
}

int ColumnProviderHostList::OnCompareItems(LPARAM lParam1, LPARAM lParam2, int iColumn)
{
	if(iColumn<=AFX_ShellList_ColumnModified)
		return CMFCShellListCtrl::OnCompareItems(lParam1, lParam2, iColumn);
	ASSERT_VALID(this);
	LPAFX_SHELLITEMINFO pItem1 = (LPAFX_SHELLITEMINFO)lParam1;
	LPAFX_SHELLITEMINFO pItem2 = (LPAFX_SHELLITEMINFO)lParam2;
	
	CString itemText1;
	CString itemText2;
	int nRes = 0;
	if(pDocument!=NULL)
	{
		itemText1=pDocument->OnGetItemText(iColumn-AFX_ShellList_ColumnModified-1,pItem1);
		itemText2=pDocument->OnGetItemText(iColumn-AFX_ShellList_ColumnModified-1,pItem2);
	}
	
	
	return itemText1.Compare(itemText2);
}
