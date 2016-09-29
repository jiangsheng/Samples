
// HtmlCaretTestDoc.cpp : implementation of the CHtmlCaretTestDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "HtmlCaretTest.h"
#endif

#include "HtmlCaretTestDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CHtmlCaretTestDoc

IMPLEMENT_DYNCREATE(CHtmlCaretTestDoc, CHtmlEditDoc)

BEGIN_MESSAGE_MAP(CHtmlCaretTestDoc, CHtmlEditDoc)
END_MESSAGE_MAP()


// CHtmlCaretTestDoc construction/destruction

CHtmlCaretTestDoc::CHtmlCaretTestDoc()
{
	// TODO: add one-time construction code here

}

CHtmlCaretTestDoc::~CHtmlCaretTestDoc()
{
}

BOOL CHtmlCaretTestDoc::OnNewDocument()
{
	if (!CHtmlEditDoc::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



#ifdef SHARED_HANDLERS

// Support for thumbnails
void CHtmlCaretTestDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CHtmlCaretTestDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CHtmlCaretTestDoc::SetSearchContent(const CString& value)
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

// CHtmlCaretTestDoc diagnostics

#ifdef _DEBUG
void CHtmlCaretTestDoc::AssertValid() const
{
	CHtmlEditDoc::AssertValid();
}

void CHtmlCaretTestDoc::Dump(CDumpContext& dc) const
{
	CHtmlEditDoc::Dump(dc);
}
#endif //_DEBUG


// CHtmlCaretTestDoc commands
