// MFCWinFormHybridDoc.cpp : implementation of the CMFCWinFormHybridDoc class
//

#include "stdafx.h"
#include "MFCWinFormHybrid.h"

#include "MFCWinFormHybridDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCWinFormHybridDoc

IMPLEMENT_DYNCREATE(CMFCWinFormHybridDoc, CDocument)

BEGIN_MESSAGE_MAP(CMFCWinFormHybridDoc, CDocument)
END_MESSAGE_MAP()


// CMFCWinFormHybridDoc construction/destruction

CMFCWinFormHybridDoc::CMFCWinFormHybridDoc()
{
	// TODO: add one-time construction code here

}

CMFCWinFormHybridDoc::~CMFCWinFormHybridDoc()
{
}

BOOL CMFCWinFormHybridDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CMFCWinFormHybridDoc serialization

void CMFCWinFormHybridDoc::Serialize(CArchive& ar)
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


// CMFCWinFormHybridDoc diagnostics

#ifdef _DEBUG
void CMFCWinFormHybridDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMFCWinFormHybridDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMFCWinFormHybridDoc commands
