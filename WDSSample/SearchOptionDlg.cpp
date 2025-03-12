// SearchOptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WDSSample.h"
#include "SearchOptionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSearchOptionDlg dialog


CSearchOptionDlg::CSearchOptionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSearchOptionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSearchOptionDlg)
	m_strQuery = _T("");
	//}}AFX_DATA_INIT
}


void CSearchOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSearchOptionDlg)
	DDX_Text(pDX, IDC_EDIT1, m_strQuery);
	//}}AFX_DATA_MAP
	m_strQuery.TrimLeft();
	m_strQuery.TrimRight();
	if(pDX->m_bSaveAndValidate&&m_strQuery.IsEmpty())
	{
		AfxMessageBox(IDS_EMPTY_SEARCH_STRING);
		pDX->Fail();
	}
}


BEGIN_MESSAGE_MAP(CSearchOptionDlg, CDialog)
	//{{AFX_MSG_MAP(CSearchOptionDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSearchOptionDlg message handlers
