// WDSSampleView.cpp : implementation of the CWDSSampleView class

//Author:Sheng Jiang(sheng_jiang(at)sina.com
//blog: blog.joycode.com
//History: July 19 : create

#include "stdafx.h"
#include "initguid.h"
#include "WDSSample.h"

#include "WDSSampleDoc.h"
#include "WDSSampleView.h"
#include "atlconv.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
CString g_GetValueString(_variant_t var)
{
	USES_CONVERSION;
	CString strRet;
	switch(var.vt)
	{
		case	VT_NULL:
		case	VT_EMPTY:
		case	VT_ERROR:
		case	VT_UNKNOWN:
		case	VT_DISPATCH:
			return strRet;
		default:
		{
			HRESULT hr=VariantChangeType(&var,&var,VARIANT_LOCALBOOL,VT_BSTR);
			if(SUCCEEDED(hr))
			{
				try{
					_bstr_t bstr=var;
					strRet=OLE2T(bstr);
					return strRet;
				}
				catch(...)
				{
				}
			}
		}
		break;
	}
	return strRet;
}
/////////////////////////////////////////////////////////////////////////////
// CWDSSampleView
// Obtain the error message for a given HRESULT
   CString LogCrackHR( HRESULT hr )
   {
      LPVOID  lpMsgBuf;
      CString strTmp;

      ::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER |
                       FORMAT_MESSAGE_FROM_SYSTEM,
                       NULL,
                       hr,
                       MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                       (LPTSTR) &lpMsgBuf,
                       0,
                       NULL );

        // STR_TMP is defined within LOG.CPP to provide safe format string
        // for both ANSI and UNICODE
        strTmp.Format( "%s", (char *) lpMsgBuf );


      // Free the buffer.
      ::LocalFree( lpMsgBuf );

      return strTmp;
   }

IMPLEMENT_DYNCREATE(CWDSSampleView, CListView)

BEGIN_MESSAGE_MAP(CWDSSampleView, CListView)
	//{{AFX_MSG_MAP(CWDSSampleView)
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnGetdispinfo)
	ON_NOTIFY_REFLECT(LVN_ODCACHEHINT, OnOdcachehint)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CListView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWDSSampleView construction/destruction

CWDSSampleView::CWDSSampleView()
{
	// TODO: add construction code here

}

CWDSSampleView::~CWDSSampleView()
{
}

BOOL CWDSSampleView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	if(m_pSearchDesktop.CreateInstance(CLSID_SearchDesktop))
	{
		AfxMessageBox(IDS_FAILED_TO_CREATE_SEARCH_ENGINE);
		return	FALSE;
	}
	cs.style&=~LVS_TYPEMASK;
	cs.style|=LVS_REPORT|LVS_OWNERDATA;
	return CListView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CWDSSampleView drawing

void CWDSSampleView::OnDraw(CDC* pDC)
{
	CWDSSampleDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}
#define MINCOLWIDTH 32  /* or whatever */

void LoadStringArray(CStringArray& ar,CString Text)
{
	ar.RemoveAll(); 
	int nPos = 0;
	int nCol=0;
	while ((nPos = Text.Find(_T(","))) != -1){ 
		CString strItem; 
		strItem = Text.Left(nPos); 
		ar.Add(strItem); 
		strItem = Text.Mid(nPos + 1); 
		Text = strItem; 
		nCol++; 
	} 
	ar.Add(Text); 
}
void SaveStringArray(const CStringArray& ar,CString& Text)
{
	Text.Empty();
	for(int i=0;i<ar.GetSize();i++)
	{
		Text+=ar[i];
		if(i!=ar.GetSize()-1)
			Text+=_T(",");
	}
}

void CWDSSampleView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();
	CString strHeadings;
	VERIFY(strHeadings.LoadString(IDS_COLUMNS_GENERAL));
	LoadStringArray(m_astrHeaderText,strHeadings);
	for(int i=0;i<m_astrHeaderText.GetSize();i++){ 
		GetListCtrl().InsertColumn(i,m_astrHeaderText[i],LVCFMT_LEFT,-1,i); 
	} 	
	CString strQuery=GetDocument()->m_strQuery;
	if(!strQuery.IsEmpty())
		Search(strQuery);
	AutoSizeColumns();	
	// TODO: You may populate your ListView with items by directly accessing
	//  its list control through a call to GetListCtrl().
}
int CWDSSampleView::GetColumnCount() const {
     // get the header control
     CHeaderCtrl* pHeader = GetListCtrl().GetHeaderCtrl();
     // return the number of items in it - ie. the number of columns
     return pHeader?pHeader->GetItemCount():0;
}

void CWDSSampleView::AutoSizeColumns(int col /*=-1*/) {
     // Call this after your list control is filled
     SetRedraw(false);
     int mincol = col < 0 ? 0 : col;
     int maxcol = col < 0 ? GetColumnCount()-1 : col;
     for (col = mincol; col <= maxcol; col++) {
          GetListCtrl().SetColumnWidth(col,LVSCW_AUTOSIZE);
          int wc1 = GetListCtrl().GetColumnWidth(col);
          GetListCtrl().SetColumnWidth(col,LVSCW_AUTOSIZE_USEHEADER);
          int wc2 = GetListCtrl().GetColumnWidth(col);
          int wc = max(MINCOLWIDTH,max(wc1,wc2));
          GetListCtrl().SetColumnWidth(col,wc);
     }
     // RecalcHeaderTips(); *** uncomment this if you use my header tips method
     SetRedraw(true);
     // Invalidate(); *** uncomment this if you don't use my SetRedraw function
}

/////////////////////////////////////////////////////////////////////////////
// CWDSSampleView printing

BOOL CWDSSampleView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CWDSSampleView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CWDSSampleView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CWDSSampleView diagnostics

#ifdef _DEBUG
void CWDSSampleView::AssertValid() const
{
	CListView::AssertValid();
}

void CWDSSampleView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CWDSSampleDoc* CWDSSampleView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWDSSampleDoc)));
	return (CWDSSampleDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWDSSampleView message handlers

void	CWDSSampleView::Search(LPCTSTR lpszQuery,LPCTSTR lpszSort)
{
	CString strQuery(lpszQuery);if(strQuery.IsEmpty())return;
	CString strSort(lpszSort);
	USES_CONVERSION;
	HRESULT hr=S_OK;
	GetListCtrl().SetItemCount(0);
	ClearCache();
	try{
		CString strColumns;
		VERIFY(strColumns.LoadString(IDS_COLUMNS_GENERAL));
		if(strSort.IsEmpty())
			m_pRecordset=m_pSearchDesktop->ExecuteQuery(T2OLE(strQuery),
				T2OLE(strColumns),NULL,NULL);
		else
			m_pRecordset=m_pSearchDesktop->ExecuteQuery(T2OLE(strQuery),
				T2OLE(strColumns),T2OLE(strSort),NULL);
		int nItemCount=m_pRecordset->GetRecordCount();
		GetListCtrl().SetItemCount(nItemCount);
		
	}
	catch(_com_error&e)
	{
         // Crack _com_error
         _bstr_t bstrSource(e.Source());
         _bstr_t bstrDescription(e.Description());

         TRACE( "Exception thrown for classes generated by #import" );
         TRACE( "\tCode = %08lx\n",      e.Error());
         TRACE( "\tCode meaning = %s\n", e.ErrorMessage());
         TRACE( "\tSource = %s\n",       (LPCTSTR) bstrSource);
         TRACE( "\tDescription = %s\n",  (LPCTSTR) bstrDescription);

         // Errors Collection may not always be populated.
         if( FAILED( hr ) )
         {
            TRACE( "*** HRESULT ***" );
            TRACE( LogCrackHR( hr )  );
         }

	}
	catch(...)
	{
		TRACE( "*** Unhandled Exception ***" );
	}
}

void	CWDSSampleView::GetItemInfo(int iItem,CStringArray&	strInfoCache)
{
	strInfoCache.SetSize(m_astrHeaderText.GetSize());
	HRESULT hr;
	for(int i=0;i<m_astrHeaderText.GetSize();i++)
	{
		CString strField=m_astrHeaderText[i];
		try{
			m_pRecordset->MoveFirst();
			m_pRecordset->Move(iItem);
			strInfoCache.SetAt(i,
				g_GetValueString(
					m_pRecordset->Fields->Item[(LPCTSTR)strField]->Value)
					);
		}
		catch(_com_error&e)
		{
			 // Crack _com_error
			 _bstr_t bstrSource(e.Source());
			 _bstr_t bstrDescription(e.Description());

			 TRACE( "Exception thrown for classes generated by #import" );
			 TRACE( "\tCode = %08lx\n",      hr=e.Error());
			 TRACE( "\tCode meaning = %s\n", e.ErrorMessage());
			 TRACE( "\tSource = %s\n",       (LPCTSTR) bstrSource);
			 TRACE( "\tDescription = %s\n",  (LPCTSTR) bstrDescription);

			 // Errors Collection may not always be populated.
			 if( FAILED( hr ) )
			 {
				TRACE( "*** HRESULT ***" );
				TRACE( LogCrackHR( hr )  );
			 }

		}
		catch(...)
		{
			TRACE( "*** Unhandled Exception ***" );
		}		
	}
}
void CWDSSampleView::OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	if(pDispInfo->item.iItem==-1)return;
	pDispInfo->item.mask|=LVIF_DI_SETITEM;

	CStringArray* pItemInfo=NULL;
	BOOL bHit=m_mapCache.Lookup(
		reinterpret_cast<LPVOID>(pDispInfo->item.iItem),pItemInfo);
	if(!bHit)
	{
		pItemInfo=new CStringArray;
		GetItemInfo(pDispInfo->item.iItem,*pItemInfo);
	}
    if(pDispInfo->item.mask&LVIF_TEXT){
		lstrcpyn(
				pDispInfo->item.pszText
				,pItemInfo->GetAt(pDispInfo->item.iSubItem)
				,pDispInfo->item.cchTextMax);
    }
	if(!bHit)
	{
		delete pItemInfo;
	}
	*pResult = 0;
}

void CWDSSampleView::OnOdcachehint(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLVCACHEHINT* pCacheHint = (NMLVCACHEHINT*)pNMHDR;
	// TODO: Add your control notification handler code here
    PrepCache(0,min(30,GetListCtrl().GetItemCount()));
    PrepCache(pCacheHint->iFrom,pCacheHint->iTo);
    PrepCache(max(30,GetListCtrl().GetItemCount()-5),GetListCtrl().GetItemCount());
	*pResult = 0;
}

void CWDSSampleView::OnDestroy() 
{
	CListView::OnDestroy();
	
	// TODO: Add your message handler code here
	ClearCache();	
}
void CWDSSampleView::ClearCache() 
{
	POSITION pos=m_mapCache.GetStartPosition();
	LPVOID key;
	CStringArray* value;
	while(pos)
	{
		m_mapCache.GetNextAssoc(pos,key,value);
		if(value)
			delete	value;
	}
}
void	CWDSSampleView::PrepCache(UINT from, UINT to)
{
	CStringArray* pItemInfo=NULL;
	for(UINT i=from;i<to;i++)
	{
		BOOL bHit=m_mapCache.Lookup(reinterpret_cast<LPVOID>(i),pItemInfo);
		if(!bHit)
		{
			pItemInfo=new CStringArray;
			m_mapCache.SetAt(reinterpret_cast<LPVOID>(i),pItemInfo);
			GetItemInfo(i,*pItemInfo);
		}
	}
}