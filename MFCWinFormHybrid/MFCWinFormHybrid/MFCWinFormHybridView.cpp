// MFCWinFormHybridView.cpp : implementation of the CMFCWinFormHybridView class
//

#include "stdafx.h"
#include "MFCWinFormHybrid.h"

#include "MFCWinFormHybridDoc.h"
#include "MFCWinFormHybridView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCWinFormHybridView

IMPLEMENT_DYNCREATE(CMFCWinFormHybridView, CFormView)

BEGIN_MESSAGE_MAP(CMFCWinFormHybridView, CFormView)
	ON_COMMAND(ID_EDIT_COPY, &CMFCWinFormHybridView::OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, &CMFCWinFormHybridView::OnEditPaste)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CMFCWinFormHybridView construction/destruction

CMFCWinFormHybridView::CMFCWinFormHybridView()
	: CFormView(CMFCWinFormHybridView::IDD)
{
	// TODO: add construction code here

}

CMFCWinFormHybridView::~CMFCWinFormHybridView()
{
}

void CMFCWinFormHybridView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_ManagedControl(pDX, IDC_CTRL1, m_ctrl1);
}

BOOL CMFCWinFormHybridView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CMFCWinFormHybridView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	if(m_ctrl1.m_hWnd!=NULL)
	{
		m_ctrl1.GetControl()->TabStop=true;
		m_ctrl1.GetControl()->Visible=true;
		m_ctrl1.GetControl()->OnFileExitClicked+= MAKE_DELEGATE( System::EventHandler, OnFileExitClicked);
		m_ctrl1.GetControl()->OnEditCopyClicked+= MAKE_DELEGATE( System::EventHandler, OnEditCopyClicked);
		m_ctrl1.GetControl()->OnEditPasteClicked+= MAKE_DELEGATE( System::EventHandler, OnEditPasteClicked);
	}
	ResizeParentToFit();

}


// CMFCWinFormHybridView diagnostics

#ifdef _DEBUG
void CMFCWinFormHybridView::AssertValid() const
{
	CFormView::AssertValid();
}

void CMFCWinFormHybridView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CMFCWinFormHybridDoc* CMFCWinFormHybridView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCWinFormHybridDoc)));
	return (CMFCWinFormHybridDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCWinFormHybridView message handlers
void CMFCWinFormHybridView::OnFileExitClicked( System::Object^ sender, System::EventArgs^ e ) 
{
	AfxGetMainWnd()->PostMessage(WM_COMMAND,MAKEWPARAM(ID_APP_EXIT,1),0);
}void CMFCWinFormHybridView::OnEditCopyClicked( System::Object^ sender, System::EventArgs^ e ) 
{
	AfxGetMainWnd()->PostMessage(WM_COMMAND,MAKEWPARAM(ID_EDIT_COPY,1),0);
}void CMFCWinFormHybridView::OnEditPasteClicked( System::Object^ sender, System::EventArgs^ e ) 
{
	AfxGetMainWnd()->PostMessage(WM_COMMAND,MAKEWPARAM(ID_EDIT_PASTE,1),0);
}
BOOL IsEdit( CWnd* pWnd )
{
  ASSERT( pWnd != NULL );
  HWND hWnd = pWnd->GetSafeHwnd();
  if (hWnd == NULL)
     return FALSE;

  TCHAR szClassName[6];
  return ::GetClassName(hWnd, szClassName, 6) &&
         _tcsicmp(szClassName, _T("Edit")) == 0;
}
void CMFCWinFormHybridView::OnEditCopy()
{
	CEdit* pEdit = (CEdit*)GetFocus();
	ASSERT( IsEdit( pEdit) );
	pEdit->Copy();
}

void CMFCWinFormHybridView::OnEditPaste()
{
	CEdit* pEdit = (CEdit*)GetFocus();
	ASSERT( IsEdit( pEdit) );
	ASSERT(::IsClipboardFormatAvailable(CF_TEXT));
	if(IsClipboardFormatAvailable(CF_TEXT))
		pEdit->Paste();
}

void CMFCWinFormHybridView::OnDestroy()
{
	COleControlSite* pSite = GetOleControlSite(IDC_CTRL1);

	if (pSite != NULL) 
	{
		// Release pointers passed into QACONTAINER.
		//
		IUnknown* pUnk1 = (IUnknown*)(&(pSite->m_xOleClientSite));
		IUnknown* pUnk2 = (IUnknown*)(&(pSite->m_xPropertyNotifySink));
		IUnknown* pUnk3 = (IUnknown*)(&(pSite->m_xEventSink));
		System::IntPtr pUnknAsInt;
		System::Object^ oManagedWrapper;
		if (pUnk1 != NULL) {
			pUnknAsInt = static_cast<System::IntPtr>(pUnk1);
			oManagedWrapper = System::Runtime::InteropServices::Marshal::GetObjectForIUnknown(pUnknAsInt);
			System::Runtime::InteropServices::Marshal::FinalReleaseComObject(oManagedWrapper);
		}
		if (pUnk2 != NULL) {
			pUnknAsInt = static_cast<System::IntPtr>(pUnk2);
			oManagedWrapper = System::Runtime::InteropServices::Marshal::GetObjectForIUnknown(pUnknAsInt);
			System::Runtime::InteropServices::Marshal::FinalReleaseComObject(oManagedWrapper);
		}
		if (pUnk3 != NULL) {
			pUnknAsInt = static_cast<System::IntPtr>(pUnk3);
			oManagedWrapper = System::Runtime::InteropServices::Marshal::GetObjectForIUnknown(pUnknAsInt);
			System::Runtime::InteropServices::Marshal::FinalReleaseComObject(oManagedWrapper);
		}
// ...Repeat for pUnk2 & pUnk3...

	}

	CFormView::OnDestroy();

	// TODO: Add your message handler code here
}
