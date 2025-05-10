// DlgAuthenticate.cpp : implementation file
//

#include "stdafx.h"
#include "IEAutomation.h"
#include "DlgAuthenticate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAuthenticate dialog


CDlgAuthenticate::CDlgAuthenticate(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAuthenticate::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAuthenticate)
	m_strUserName = _T("");
	m_strPassword = _T("");
	//}}AFX_DATA_INIT
}


void CDlgAuthenticate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAuthenticate)
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUserName);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAuthenticate, CDialog)
	//{{AFX_MSG_MAP(CDlgAuthenticate)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAuthenticate message handlers
