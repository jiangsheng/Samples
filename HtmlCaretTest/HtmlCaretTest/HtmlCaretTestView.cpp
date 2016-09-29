
// HtmlCaretTestView.cpp : implementation of the CHtmlCaretTestView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "HtmlCaretTest.h"
#endif

#include "HtmlCaretTestDoc.h"
#include "HtmlCaretTestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHtmlCaretTestView

IMPLEMENT_DYNCREATE(CHtmlCaretTestView, CHtmlEditView)

BEGIN_MESSAGE_MAP(CHtmlCaretTestView, CHtmlEditView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CHtmlEditView::OnFilePrint)
	ON_COMMAND(IDM_MOVETOEND, &CHtmlCaretTestView::OnMovetoend)
END_MESSAGE_MAP()

BEGIN_DHTMLEDITING_CMDMAP(CHtmlCaretTestView)
	DHTMLEDITING_CMD_ENTRY(ID_EDIT_COPY, IDM_COPY)
	DHTMLEDITING_CMD_ENTRY(ID_EDIT_CUT, IDM_CUT)
	DHTMLEDITING_CMD_ENTRY(ID_EDIT_PASTE, IDM_PASTE)
	DHTMLEDITING_CMD_ENTRY(ID_EDIT_UNDO, IDM_UNDO)
END_DHTMLEDITING_CMDMAP()
// CHtmlCaretTestView construction/destruction

CHtmlCaretTestView::CHtmlCaretTestView()
{
	// TODO: add construction code here

}

CHtmlCaretTestView::~CHtmlCaretTestView()
{
}

BOOL CHtmlCaretTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CHtmlEditView::PreCreateWindow(cs);
}


// CHtmlCaretTestView printing



// CHtmlCaretTestView diagnostics

#ifdef _DEBUG
void CHtmlCaretTestView::AssertValid() const
{
	CHtmlEditView::AssertValid();
}

void CHtmlCaretTestView::Dump(CDumpContext& dc) const
{
	CHtmlEditView::Dump(dc);
}

CHtmlCaretTestDoc* CHtmlCaretTestView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHtmlCaretTestDoc)));
	return (CHtmlCaretTestDoc*)m_pDocument;
}
#endif //_DEBUG


// CHtmlCaretTestView message handlers


void CHtmlCaretTestView::OnMovetoend()
{
	IHTMLDocument2 *pHTMLDocument2;
	if(!GetDHtmlDocument(&pHTMLDocument2))return;

	IDisplayServices *pDisplayServices;
	HRESULT hRes = pHTMLDocument2->QueryInterface(IID_IDisplayServices, reinterpret_cast<void**>(&pDisplayServices));
	if (FAILED(hRes))
		return;
	IDisplayPointer *pDisplayPointer;
	hRes = pDisplayServices->CreateDisplayPointer(&pDisplayPointer);
	if (FAILED(hRes))
		return;

	IMarkupServices *pMarkupServices;
	hRes = pDisplayServices->QueryInterface(IID_IMarkupServices, reinterpret_cast<void**>(&pMarkupServices));
	if (FAILED(hRes))
		return;

	IMarkupPointer *pMarkupPointer;
	hRes = pMarkupServices->CreateMarkupPointer(&pMarkupPointer);
	if (FAILED(hRes))
		return;

	IHTMLElement *pElement;
	pHTMLDocument2->get_body(&pElement);
	hRes = pMarkupPointer->MoveAdjacentToElement(pElement, ELEM_ADJ_BeforeEnd);
	if (FAILED(hRes))
		return;

	hRes = pDisplayPointer->MoveToMarkupPointer(pMarkupPointer, NULL);
	if (FAILED(hRes))
		return;

	IHTMLCaret *pHTMLCaret;
	hRes = pDisplayServices->GetCaret(&pHTMLCaret);
	if (FAILED(hRes))
		return;

	hRes = pHTMLCaret->MoveCaretToPointerEx(pDisplayPointer, TRUE, TRUE, CARET_DIRECTION_FORWARD);
	if (FAILED(hRes))
		return;
}
