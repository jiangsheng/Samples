// WDSSampleDoc.cpp : implementation of the CWDSSampleDoc class
//

#include "stdafx.h"
#include "WDSSample.h"

#include "WDSSampleDoc.h"
#include "SearchOptionDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWDSSampleDoc

IMPLEMENT_DYNCREATE(CWDSSampleDoc, CDocument)

BEGIN_MESSAGE_MAP(CWDSSampleDoc, CDocument)
	//{{AFX_MSG_MAP(CWDSSampleDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWDSSampleDoc construction/destruction

CWDSSampleDoc::CWDSSampleDoc()
{
	// TODO: add one-time construction code here

}

CWDSSampleDoc::~CWDSSampleDoc()
{
}

BOOL CWDSSampleDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	CSearchOptionDlg dlg;
	if(dlg.DoModal()!=IDOK)return -1;
	m_strQuery=dlg.m_strQuery;
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CWDSSampleDoc serialization

void CWDSSampleDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		ar<<m_strQuery;
	}
	else
	{
		// TODO: add loading code here
		ar>>m_strQuery;
	}
	
}

/////////////////////////////////////////////////////////////////////////////
// CWDSSampleDoc diagnostics

#ifdef _DEBUG
void CWDSSampleDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWDSSampleDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWDSSampleDoc commands
