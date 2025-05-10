// DlgInputText.cpp : implementation file
//

#include "stdafx.h"
#include "IEAutomation.h"
#include "DlgInputText.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgInputText dialog


CDlgInputText::CDlgInputText(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInputText::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgInputText)
	m_strText = _T("");
	//}}AFX_DATA_INIT
}


void CDlgInputText::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgInputText)
	DDX_Text(pDX, IDC_EDIT, m_strText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgInputText, CDialog)
	//{{AFX_MSG_MAP(CDlgInputText)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgInputText message handlers
