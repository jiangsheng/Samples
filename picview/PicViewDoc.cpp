// PicViewDoc.cpp : implementation of the CPicViewDoc class
//

#include "stdafx.h"
#include "PicView.h"
#include <atlconv.h>

#include "PicViewDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPicViewDoc

IMPLEMENT_DYNCREATE(CPicViewDoc, CDocument)

BEGIN_MESSAGE_MAP(CPicViewDoc, CDocument)
	//{{AFX_MSG_MAP(CPicViewDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPicViewDoc construction/destruction

CPicViewDoc::CPicViewDoc()
{
	// TODO: add one-time construction code here
}

CPicViewDoc::~CPicViewDoc()
{
}

BOOL CPicViewDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	CFolderChange	FolderChange;
	SHGetSpecialFolderLocation(NULL,CSIDL_MYPICTURES,&FolderChange.m_pidlFolder);
	UpdateAllViews(NULL,HintFolderChange,&FolderChange);
	ILFree (FolderChange.m_pidlFolder);
	return TRUE;
}

BOOL CPicViewDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	USES_CONVERSION;
	CFolderChange	FolderChange;
	SHILCreateFromPath(T2OLE(lpszPathName),&FolderChange.m_pidlFolder,NULL);
	UpdateAllViews(NULL,HintFolderChange,&FolderChange);
	ILFree (FolderChange.m_pidlFolder);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CPicViewDoc serialization

void CPicViewDoc::Serialize(CArchive& ar)
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
// CPicViewDoc diagnostics

#ifdef _DEBUG
void CPicViewDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPicViewDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPicViewDoc commands


