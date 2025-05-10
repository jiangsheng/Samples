// PicViewView.cpp : implementation of the CPicViewView class
//

#include "stdafx.h"
#include <atlconv.h>
#include "PicView.h"

#include "PicViewDoc.h"
#include "PicViewView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPicViewView
CFolderItemInfo::CFolderItemInfo()
{
	iIcon=-2;
	state=0;
	ZeroMemory(tszDisplayName,sizeof(TCHAR)*MAX_PATH);
	ZeroMemory(tszPath,sizeof(TCHAR)*MAX_PATH);
	bFailLoadPic=FALSE;
}
#ifdef _DEBUG
void CFolderItemInfo::Dump(CDumpContext& dc) const
{
	dc<<tszDisplayName<<tszPath;
}
#endif
IMPLEMENT_DYNCREATE(CPicViewView, CListView)

BEGIN_MESSAGE_MAP(CPicViewView, CListView)
	//{{AFX_MSG_MAP(CPicViewView)
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnGetdispinfo)
	ON_NOTIFY_REFLECT(LVN_ODCACHEHINT, OnOdcachehint)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPicViewView construction/destruction

CPicViewView::CPicViewView()
{
	// TODO: add construction code here
	m_pMalloc=NULL;
	m_psfDesktop=NULL;
	m_pidlFolder=NULL;
	m_psfFolder=NULL;
}

CPicViewView::~CPicViewView()
{
}

BOOL CPicViewView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style&=~LVS_TYPEMASK;
	cs.style|=LVS_ICON|LVS_OWNERDATA;
	return CListView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CPicViewView drawing

void CPicViewView::OnDraw(CDC* pDC)
{
	CPicViewDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

void CPicViewView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();
	GetListCtrl().SetCallbackMask(LVIS_OVERLAYMASK);
}

/////////////////////////////////////////////////////////////////////////////
// CPicViewView diagnostics

#ifdef _DEBUG
void CPicViewView::AssertValid() const
{
	CListView::AssertValid();
}

void CPicViewView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CPicViewDoc* CPicViewView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPicViewDoc)));
	return (CPicViewDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPicViewView message handlers
int CPicViewView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;
	HRESULT	hr	= SHGetMalloc(&m_pMalloc);	if(FAILED(hr))	return -1;
	hr	= SHGetDesktopFolder(&m_psfDesktop);if(FAILED(hr))	return -1;
	SHFILEINFO shfi;
	ZeroMemory(&shfi,sizeof(SHFILEINFO));
	HIMAGELIST hi=(HIMAGELIST)SHGetFileInfo(NULL,0,&shfi,sizeof(SHFILEINFO),SHGFI_ICON |SHGFI_SYSICONINDEX|SHGFI_SMALLICON);
	GetListCtrl().SetImageList(CImageList::FromHandle(hi),LVSIL_SMALL);
	hi=(HIMAGELIST)SHGetFileInfo(NULL,0,&shfi,sizeof(SHFILEINFO),SHGFI_ICON |SHGFI_SYSICONINDEX|SHGFI_LARGEICON);
	GetListCtrl().SetImageList(CImageList::FromHandle(hi),LVSIL_NORMAL);
	return 0;
}
void CPicViewView::OnDestroy() 
{
	LeaveFolder();
	if(m_psfDesktop){
		m_psfDesktop->Release();
		m_psfDesktop=NULL;
	}
	if(m_pMalloc){
		m_pMalloc->Release();
		m_pMalloc=NULL;
	}	
	CListView::OnDestroy();	
}

void	CPicViewView::EnterFolder(LPCITEMIDLIST pidl)
{
    USES_CONVERSION;
	m_pidlFolder=ILClone(pidl);
	if(m_pidlFolder){
		LPENUMIDLIST ppenum = NULL;
		LPITEMIDLIST pidlItems = NULL;
		ULONG celtFetched;
		HRESULT hr;
		hr = m_psfDesktop->BindToObject(
			m_pidlFolder, NULL, IID_IShellFolder, (LPVOID *) &m_psfFolder);
		if(SUCCEEDED(hr)){
			hr = m_psfFolder->EnumObjects(NULL,SHCONTF_FOLDERS | SHCONTF_NONFOLDERS, &ppenum);
			if(SUCCEEDED(hr)){
				while( hr = ppenum->Next(1,&pidlItems, &celtFetched) == S_OK && (celtFetched) == 1){
					m_arpFolderItems.Add(pidlItems);
				}
			}
		}
		GetListCtrl().SetItemCount(m_arpFolderItems.GetSize());
    }
}

void CPicViewView::LeaveFolder()
{
	GetListCtrl().SetItemCount(0);
	for(int i=0;i<m_arpFolderItems.GetSize();i++){
		m_pMalloc->Free((LPITEMIDLIST)m_arpFolderItems[i]);
	}
	m_arpFolderItems.RemoveAll();
	ClearCache();
	if(m_pMalloc){
		if(m_pidlFolder){
			m_pMalloc->Free(m_pidlFolder);
			m_pidlFolder=NULL;
		}
	}
	if(m_psfFolder){
		m_psfFolder->Release();
		m_psfFolder=NULL;
	}
}

void CPicViewView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(lHint){
	case	CPicViewDoc::HintDefault:CListView::OnUpdate(pSender,lHint,pHint);break;
	case	CPicViewDoc::HintFolderChange:
		{
			CFolderChange* pFolderChange=(CFolderChange*)pHint;
			OnFolderChange(pFolderChange);
		}
		break;
	}	
}
void CPicViewView::OnFolderChange(CFolderChange* pFolderChange)
{
	if(m_pMalloc==NULL||m_psfDesktop==NULL)return;
	LeaveFolder();
	EnterFolder(pFolderChange->m_pidlFolder);
	TCHAR	tszDisplayName[_MAX_PATH];
	theApp.SHGetDisplayNameOf(pFolderChange->m_pidlFolder,tszDisplayName);
	GetDocument()->SetTitle(tszDisplayName);
}

void	CPicViewView::ClearCache()
{
	POSITION	pos=m_mapCache.GetStartPosition();
	LPITEMIDLIST	Key;
	CFolderItemInfo* Value;
	while(pos){
		m_mapCache.GetNextAssoc(pos,Key,Value);
		delete	Value;
	}
	m_mapCache.RemoveAll();
}
void	CPicViewView::PrepCache( int iFrom, int iTo )
{
	for(int i=iFrom;i<iTo;i++){
		LPCITEMIDLIST	pidlItem=m_arpFolderItems[i];
		CFolderItemInfo* pItemInfo=FindItemInCache(pidlItem);
		if(pItemInfo==NULL){
			pItemInfo=new	CFolderItemInfo;
			GetItemInfo(pidlItem,pItemInfo);
			m_mapCache.SetAt((LPITEMIDLIST)pidlItem,pItemInfo);
		}
	}
}
CFolderItemInfo* CPicViewView::FindItemInCache(LPCITEMIDLIST pidl)
{
	CFolderItemInfo*	pRet=NULL;
	if(m_mapCache.Lookup((LPITEMIDLIST)pidl,pRet)){
		return pRet;
	}
	return NULL;
}
void	CPicViewView::GetItemInfo(LPCITEMIDLIST pidl,CFolderItemInfo* pItemInfo)
{
	HRESULT	hr = theApp.SHGetDisplayNameOf(m_psfFolder,pidl,pItemInfo->tszDisplayName);

	IShellIcon* pShellIcon=NULL;
	hr=m_psfFolder->QueryInterface(IID_IShellIcon,(LPVOID*)&pShellIcon);
	if (SUCCEEDED(hr)&&pShellIcon){
		pShellIcon->GetIconOf(pidl,0,&pItemInfo->iIcon);
		pShellIcon->Release();
	}
	IShellIconOverlay*	pShellIconOverlay  =NULL;
	hr=m_psfFolder->QueryInterface(IID_IShellIconOverlay,(LPVOID*)&pShellIconOverlay);
	if (SUCCEEDED(hr)&&pShellIconOverlay){
		int nOverlay=0;
		pShellIconOverlay->GetOverlayIndex(pidl,&nOverlay);
		pItemInfo->state=INDEXTOOVERLAYMASK (nOverlay);
		pShellIconOverlay->Release();
	}
	LPITEMIDLIST	pidlItemFull=ILCombine(m_pidlFolder,pidl);
	if(pidlItemFull){
		if(SHGetPathFromIDList(pidlItemFull,pItemInfo->tszPath)){
			USES_CONVERSION;
			hr=OleLoadPicturePath(
				T2OLE(pItemInfo->tszPath)
				,NULL,0,RGB(255,255,255)
				,IID_IPicture,(LPVOID*)&pItemInfo->pic.m_pPict);
			if(FAILED(hr)){
				pItemInfo->bFailLoadPic=TRUE;
				TRACE("OleLoadPicturePath failed %s\r\n",pItemInfo->tszPath);
			}
		}
		m_pMalloc->Free(pidlItemFull);
	}
}
void CPicViewView::OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	if(pDispInfo->item.iItem==-1)return;
	HRESULT	hr=S_OK;
	LPCITEMIDLIST	pidlItem=m_arpFolderItems[pDispInfo->item.iItem];
	CFolderItemInfo* pFolderItemInfo=FindItemInCache(pidlItem);
	BOOL bCached=TRUE;
	if(pFolderItemInfo==NULL){
		bCached=FALSE;
		pFolderItemInfo=new CFolderItemInfo;
		GetItemInfo(pidlItem,pFolderItemInfo);
	}
	if(pDispInfo->item.mask&LVIF_TEXT){
		lstrcpyn(pDispInfo->item.pszText,pFolderItemInfo->tszDisplayName,pDispInfo->item.cchTextMax);
	}
	if(pDispInfo->item.mask&LVIF_IMAGE){
		pDispInfo->item.iImage=pFolderItemInfo->iIcon;
	}
	if(pDispInfo->item.mask&LVIF_STATE){
		pDispInfo->item.state=pFolderItemInfo->state;
	}
	if(!bCached)
		delete	pFolderItemInfo;
	*pResult = 0;
}

void CPicViewView::OnOdcachehint(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLVCACHEHINT* pCacheHint = (NMLVCACHEHINT*)pNMHDR;
	PrepCache(0,min(5,m_arpFolderItems.GetSize()));
	PrepCache(pCacheHint->iFrom,pCacheHint->iTo);
	PrepCache(max(0,m_arpFolderItems.GetSize()-5),m_arpFolderItems.GetSize());
	*pResult = 0;
}
void CPicViewView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMLISTVIEW lpnm=(LPNMLISTVIEW)pNMHDR;
	if(lpnm->iItem==-1)return;
	*pResult = 0;
	HRESULT	hr=S_OK;
	LPCITEMIDLIST	pidlItem=m_arpFolderItems[lpnm->iItem];
	LPITEMIDLIST	pidlItemFull=ILCombine(m_pidlFolder,pidlItem);
	LPITEMIDLIST	pidlItemTarget=NULL;
	hr=theApp.SHGetTargetFolderIDList(pidlItemFull,&pidlItemTarget);
	if(pidlItemTarget){
		if(theApp.ILIsFolder(pidlItemTarget)){
			CFolderChange	FolderChange;
			FolderChange.m_pidlFolder=pidlItemTarget;
			OnFolderChange(&FolderChange);
		}
		else{
			SHELLEXECUTEINFO ShExecInfo;
			ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
			ShExecInfo.fMask = SEE_MASK_IDLIST;
			ShExecInfo.hwnd = NULL;
			ShExecInfo.lpVerb = NULL;
			ShExecInfo.lpFile = NULL;
			ShExecInfo.lpIDList= pidlItemTarget;
			ShExecInfo.lpParameters = NULL;
			ShExecInfo.lpDirectory = NULL;
			ShExecInfo.nShow = SW_MAXIMIZE;
			ShExecInfo.hInstApp = NULL;
			ShellExecuteEx(&ShExecInfo);
		}
		m_pMalloc->Free(pidlItemTarget);
		m_pMalloc->Free(pidlItemFull);
	}	
}
void CPicViewView::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVCUSTOMDRAW	lpNMCustomDraw = (LPNMLVCUSTOMDRAW) pNMHDR;
    switch(lpNMCustomDraw ->nmcd.dwDrawStage) {
	case CDDS_PREPAINT :	*pResult=CDRF_NOTIFYITEMDRAW;return;
    case CDDS_ITEMPREPAINT:*pResult=CDRF_NOTIFYPOSTPAINT;return;
	case	CDDS_ITEMPOSTPAINT:
		{
			int iItem=lpNMCustomDraw ->nmcd.dwItemSpec;
			if(iItem==-1){
				*pResult=CDRF_DODEFAULT;return;
			}
			CFolderItemInfo* pItemInfo=FindItemInCache(m_arpFolderItems[iItem]);
			if(pItemInfo==NULL||pItemInfo->bFailLoadPic||pItemInfo->pic.m_pPict==NULL){
				*pResult=CDRF_DODEFAULT;return;
			}
			CRect rectIcon;
			GetListCtrl().GetItemRect(iItem,&rectIcon,LVIR_ICON);
			CDC* pDC=CDC::FromHandle(lpNMCustomDraw->nmcd.hdc);
			pItemInfo->pic.Render(pDC,rectIcon,rectIcon);
		}
		*pResult=CDRF_NEWFONT;return;
    }
	* pResult=0;
}
