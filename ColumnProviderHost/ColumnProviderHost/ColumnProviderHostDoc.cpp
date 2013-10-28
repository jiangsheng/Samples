// ColumnProviderHostDoc.cpp : implementation of the CColumnProviderHostDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ColumnProviderHost.h"
#endif

#include "ColumnProviderHostList.h"
#include "ColumnProviderHostDoc.h"
#include "ColumnProviderDialog.h"
#include "AddressBox.h"
#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CColumnProviderHostDoc

IMPLEMENT_DYNCREATE(CColumnProviderHostDoc, CDocument)

BEGIN_MESSAGE_MAP(CColumnProviderHostDoc, CDocument)	
	ON_COMMAND(ID_FOLDER_UP, OnFolderUp)
	ON_UPDATE_COMMAND_UI(ID_FOLDER_UP, OnUpdateFolderUp)

	ON_COMMAND(ID_VIEW_LARGEICON, OnViewLargeicon)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LARGEICON, OnUpdateViewLargeicon)
	ON_COMMAND(ID_VIEW_DETAILS, OnViewDetails)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DETAILS, OnUpdateViewDetails)
	ON_COMMAND(ID_VIEW_LIST, OnViewList)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LIST, OnUpdateViewList)
	ON_COMMAND(ID_VIEW_SMALLICON, OnViewSmallicon)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SMALLICON, OnUpdateViewSmallicon)

	
	ON_COMMAND(ID_COPY_TO, OnCopyTo)
	ON_COMMAND(ID_MOVE_TO, OnMoveTo)
	ON_COMMAND(ID_VIEW_REFRESH, OnViewRefresh)
	ON_COMMAND(ID_VIEW_REFRESHCOLUMNS, &CColumnProviderHostDoc::OnViewRefreshcolumns)
END_MESSAGE_MAP()


// CColumnProviderHostDoc construction/destruction

CColumnProviderHostDoc::CColumnProviderHostDoc()
	:pAddressBox(NULL),
	pShellTree(NULL),
	pShellList(NULL)
{
	// TODO: add one-time construction code here

}

void CColumnProviderHostDoc::ClearProviderColumns()
{
	for each(SHCOLUMNINFO* shellColumnInfo in providerColumns)
	{
		if(shellColumnInfo!=NULL)
			delete shellColumnInfo ;
	}
	providerColumns.clear();
}
CColumnProviderHostDoc::~CColumnProviderHostDoc()
{
	ClearProviderColumns();
}

BOOL CColumnProviderHostDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	ColumnProviderDialog columnProviderDialog;
	if(columnProviderDialog.DoModal()!=IDOK)
		return FALSE;
	columnProviderClassId=columnProviderDialog.selectedColumnProviderId;

	
	IID classId;
	ZeroMemory(&classId,sizeof(classId));
	CLSIDFromString(columnProviderClassId,&classId);
	pColumnProvider.CoCreateInstance(classId);
	if(pColumnProvider==NULL)
		return FALSE;
	return TRUE;
}

SHCONTF CColumnProviderHostDoc::GetItemTypes()
{
	SHCONTF	types = (SHCONTF)(SHCONTF_FOLDERS);
	SHELLSTATE shellState;
	ZeroMemory(&shellState,sizeof(SHELLSTATE));
	SHGetSetSettings(&shellState,SSF_SHOWALLOBJECTS|SSF_SHOWSYSFILES|SSF_SHOWSUPERHIDDEN,false);
	if(shellState.fShowAllObjects)
		types |=SHCONTF_INCLUDEHIDDEN;
	
	if(shellState.fShowSuperHidden)
		types |=SHCONTF_INCLUDESUPERHIDDEN;
	return types ;
}


// CColumnProviderHostDoc serialization

void CColumnProviderHostDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CColumnProviderHostDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CColumnProviderHostDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CColumnProviderHostDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CColumnProviderHostDoc diagnostics

#ifdef _DEBUG
void CColumnProviderHostDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CColumnProviderHostDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CColumnProviderHostDoc commands

void CColumnProviderHostDoc::UpdateAddressBox (LPCTSTR lpszPath)
{
	ASSERT (lpszPath != NULL);
	if(pAddressBox==NULL)
	{
		defaultAddress=lpszPath;
	}
	else
	{
		pAddressBox->GetEditCtrl()->SetWindowText (lpszPath);
	}
}

void CColumnProviderHostDoc::OnFolderUp() 
{
	pShellList->DisplayParentFolder ();
	pShellTree->SetFocus();
}

void CColumnProviderHostDoc::OnUpdateFolderUp(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable (!pShellList->IsDesktop ());
}
void CColumnProviderHostDoc::NavigateToList(CString newPath)
{
	pShellList->DisplayFolder (newPath);
}
void  CColumnProviderHostDoc::SetAddressBox(AddressBox* pAddressBox)
{
	this->pAddressBox=pAddressBox;
	if(pAddressBox->GetSafeHwnd()!=NULL)
		UpdateAddressBox(defaultAddress);
}


void CColumnProviderHostDoc::OnViewLargeicon() 
{
	pShellList->ModifyStyle(LVS_TYPEMASK, LVS_ICON);
}

void CColumnProviderHostDoc::OnUpdateViewLargeicon(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio ((pShellList->GetStyle () & LVS_TYPEMASK) == LVS_ICON);
}

void CColumnProviderHostDoc::OnViewDetails() 
{
	pShellList->ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
}

void CColumnProviderHostDoc::OnUpdateViewDetails(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio ((pShellList->GetStyle () & LVS_TYPEMASK) == LVS_REPORT);
}

void CColumnProviderHostDoc::OnViewList() 
{
	pShellList->ModifyStyle(LVS_TYPEMASK, LVS_LIST);
}

void CColumnProviderHostDoc::OnUpdateViewList(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio ((pShellList->GetStyle () & LVS_TYPEMASK) == LVS_LIST);
}

void CColumnProviderHostDoc::OnViewSmallicon() 
{
	pShellList->ModifyStyle(LVS_TYPEMASK, LVS_SMALLICON);
}

void CColumnProviderHostDoc::OnUpdateViewSmallicon(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio ((pShellList->GetStyle () & LVS_TYPEMASK) == LVS_SMALLICON);
}
void CColumnProviderHostDoc::OnCopyTo() 
{
	CString strPath;
	if (pShellList->GetCurrentFolder (strPath) &&
		theApp.GetShellManager ()->BrowseForFolder (strPath, 
			pShellList, strPath, _T("Copy the selected item(s) to the folder:")))
	{
		AfxMessageBox (CString (_T("The selected path is: ")) + strPath);
	}
//	afxShellManager->BrowseForFolder (strPath, 
	//		pShellList, strPath, _T("Copy the selected item(s) to the folder:"));
}

void CColumnProviderHostDoc::OnMoveTo() 
{
	CString strPath;
	if (pShellList->GetCurrentFolder (strPath) &&
		theApp.GetShellManager ()->BrowseForFolder (strPath, 
			pShellList, strPath, _T("Move the selected item(s) to the folder:")))
	{
		AfxMessageBox (CString (_T("The selected path is: ")) + strPath);
	}
}

void CColumnProviderHostDoc::OnViewRefresh() 
{
	pShellList->Refresh ();
}

void CColumnProviderHostDoc::SetShellList(ColumnProviderHostList*	pShellList)
{
	this->pShellList=pShellList;
	pShellList->SetDocument(this);
}
void CColumnProviderHostDoc::SetRelatedList()
{
	if(pShellList!=NULL && pShellTree!=NULL)
		pShellTree->SetRelatedList (pShellList);

}
void CColumnProviderHostDoc::GenerateProviderColumns()
{
	if(pColumnProvider!=NULL)
	{
		ClearProviderColumns();
		SHCOLUMNINIT shellColumnInitializeData;
		ZeroMemory(&shellColumnInitializeData,sizeof(SHCOLUMNINIT));
		CString currentFolder;
		pShellList->GetCurrentFolder(currentFolder);
		lstrcpyn(shellColumnInitializeData.wszFolder,currentFolder,_MAX_PATH);
		HRESULT hr=pColumnProvider->Initialize(&shellColumnInitializeData);
		if(FAILED(hr)) return;
		DWORD columnIndex=0;
		do{
			SHCOLUMNINFO* pShellColumnInfo=new SHCOLUMNINFO;
			ZeroMemory(pShellColumnInfo,sizeof(SHCOLUMNINFO));
			hr=pColumnProvider->GetColumnInfo(columnIndex++,pShellColumnInfo);
			if(hr!=S_OK) 
			{
				delete pShellColumnInfo;
			}
			else
			{
				providerColumns.push_back(pShellColumnInfo);
			}
		}
		while(hr==S_OK);
		
	}
}
CString CColumnProviderHostDoc::OnGetItemText(int iColumn, LPAFX_SHELLITEMINFO pItem)
{
	SHCOLUMNINFO* columnInfo=GetProviderColumn(iColumn);
	CString itemText;
	SHCOLUMNDATA shellColumnData;
	ZeroMemory(&shellColumnData,sizeof(shellColumnData));
	TCHAR szPath [MAX_PATH];
	if (SHGetPathFromIDList(pItem->pidlFQ, szPath))
	{
		shellColumnData.dwFileAttributes=GetFileAttributes(szPath);
		shellColumnData.pwszExt=PathFindExtension(szPath);
		lstrcpyn(shellColumnData.wszFile,szPath,_MAX_PATH);

		COleVariant columnData;
		pColumnProvider->GetItemData(&columnInfo->scid,&shellColumnData,&columnData);
		columnData.ChangeType(VT_BSTR);
		if(columnData.vt==VT_BSTR)
		{
			CComBSTR bstr(columnData.bstrVal);
			itemText=bstr;
		}
	}
	return itemText;
}

void CColumnProviderHostDoc::OnViewRefreshcolumns()
{
	pShellList->Refresh();
}
