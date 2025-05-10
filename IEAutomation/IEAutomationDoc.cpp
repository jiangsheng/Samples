// IEAutomationDoc.cpp : implementation of the CIEAutomationDoc class
//

#include "stdafx.h"
#include "IEAutomation.h"

#include "IEAutomationDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIEAutomationDoc

IMPLEMENT_DYNCREATE(CIEAutomationDoc, CDocument)

BEGIN_MESSAGE_MAP(CIEAutomationDoc, CDocument)
	//{{AFX_MSG_MAP(CIEAutomationDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIEAutomationDoc construction/destruction

CIEAutomationDoc::CIEAutomationDoc()
{
	// TODO: add one-time construction code here
	if(!m_menuDefault.LoadMenu(IDR_IEAUTOTYPE)){
		TRACE(_T("Error Loading Menu\r\n"));
	}
	m_strScript.LoadString(IDS_SAMPLE_SCRIPT);
}

CIEAutomationDoc::~CIEAutomationDoc()
{
}

BOOL CIEAutomationDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CIEAutomationDoc serialization

void CIEAutomationDoc::Serialize(CArchive& ar)
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

/////////////////////////////////////////////////////////////////////////////
// CIEAutomationDoc diagnostics

#ifdef _DEBUG
void CIEAutomationDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CIEAutomationDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CIEAutomationDoc commands
HMENU CIEAutomationDoc::GetDefaultMenu()
{
	return m_menuDefault;
}

