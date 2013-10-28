#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ColumnProviderHost.h"
#endif

#include "ColumnProviderHostDoc.h"
#include "ColumnProviderHostView.h"

#include "MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CColumnProviderHostView

IMPLEMENT_DYNCREATE(CColumnProviderHostView, CView)

BEGIN_MESSAGE_MAP(CColumnProviderHostView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()ON_WM_CREATE()
	ON_WM_SIZE()

	ON_WM_ERASEBKGND()
	ON_WM_SETFOCUS()
	ON_REGISTERED_MESSAGE(AFX_WM_CHANGE_CURRENT_FOLDER, OnChangeFolder)

	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CColumnProviderHostView construction/destruction

CColumnProviderHostView::CColumnProviderHostView()
	: timerRefreshId(-1)
{
	// TODO: add construction code here

}

CColumnProviderHostView::~CColumnProviderHostView()
{
}

void CColumnProviderHostView::OnDraw(CDC* /*pDC*/)
{
	// TODO: Add your specialized code here and/or call the base class
}
BOOL CColumnProviderHostView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style &= ~WS_BORDER;
	return CView::PreCreateWindow(cs);
}

void CColumnProviderHostView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	CColumnProviderHostDoc* pDocument=GetDocument();
	pDocument->SetShellList(&m_wndList);
	m_wndList.OnSetColumns();
	OnChangeFolder (0, 0);	// To update mainframe's address bar
}

void CColumnProviderHostView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CColumnProviderHostView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CColumnProviderHostView diagnostics

#ifdef _DEBUG
void CColumnProviderHostView::AssertValid() const
{
	CView::AssertValid();
}

void CColumnProviderHostView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CColumnProviderHostDoc* CColumnProviderHostView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CColumnProviderHostDoc)));
	return (CColumnProviderHostDoc*)m_pDocument;
}
#endif //_DEBUG


// CColumnProviderHostView message handlers

int CColumnProviderHostView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rectDummy (0, 0, 0, 0);
	m_wndList.Create (WS_CHILD | WS_VISIBLE | LVS_REPORT, rectDummy, this, 1);
	timerRefreshId=SetTimer(1001,2000,NULL);
	return 0;
}

void CColumnProviderHostView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	m_wndList.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void CColumnProviderHostView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	if (bActivate)
	{
		GetDocument()->SetRelatedList ();		
	}
	
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}
BOOL CColumnProviderHostView::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
}

void CColumnProviderHostView::OnSetFocus(CWnd* pOldWnd) 
{
	CView::OnSetFocus(pOldWnd);
	m_wndList.SetFocus ();
}

LRESULT CColumnProviderHostView::OnChangeFolder(WPARAM,LPARAM)
{
	CString strPath;
	if (!m_wndList.GetCurrentFolder (strPath) &&
		!m_wndList.GetCurrentFolderName (strPath))
	{
		strPath = _T("????");
	}

	GetDocument()->UpdateAddressBox (strPath);

	return 0;
}





void CColumnProviderHostView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(timerRefreshId==nIDEvent)
	{
		m_wndList.RefreshDisplay();
	}
	CView::OnTimer(nIDEvent);
}


void CColumnProviderHostView::OnDestroy()
{
	if(timerRefreshId!=-1)
	{
		KillTimer(timerRefreshId);
	}
	CView::OnDestroy();

	// TODO: Add your message handler code here
}
