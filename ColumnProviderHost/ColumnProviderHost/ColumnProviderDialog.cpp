// ColumnProviderDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ColumnProviderHost.h"
#include "ColumnProviderDialog.h"
#include "afxdialogex.h"


// ColumnProviderDialog dialog

IMPLEMENT_DYNAMIC(ColumnProviderDialog, CDialogEx)

ColumnProviderDialog::ColumnProviderDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(ColumnProviderDialog::IDD, pParent)
{

}

ColumnProviderDialog::~ColumnProviderDialog()
{
}

void ColumnProviderDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PROVIDERS, columnProviderList);
	DDX_Control(pDX, IDC_STATIC_ColumnProvider, labelChooseColumnProvider);
}


BEGIN_MESSAGE_MAP(ColumnProviderDialog, CDialogEx)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_PROVIDERS, &ColumnProviderDialog::OnLvnGetdispinfoListProviders)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// ColumnProviderDialog message handlers


void ColumnProviderDialog::OnLvnGetdispinfoListProviders(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	if(pDispInfo ->item.iItem<0)
		return;
	if(pDispInfo ->item.iItem>=columnProviderKeys.size())
		return;
	ColumnProviderKey* pColumnProviderKey =columnProviderKeys[pDispInfo ->item.iItem];
	switch(pDispInfo ->item.iSubItem)
	{
		case ColumnProviderDialogIdColumn:
			if(pDispInfo ->item.mask&LVIF_TEXT)
			{
				lstrcpyn(pDispInfo ->item.pszText,pColumnProviderKey->classId,pDispInfo ->item.cchTextMax);
			}
			break;
		case ColumnProviderDialogDescriptionColumn:
			if(pDispInfo ->item.mask&LVIF_TEXT)
			{
				lstrcpyn(pDispInfo ->item.pszText,pColumnProviderKey->description,pDispInfo ->item.cchTextMax);
			}
			break;
	}
	*pResult = 0;
}


BOOL ColumnProviderDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	CRegKey registryKey;
	registryKey.Open(HKEY_CLASSES_ROOT,_T("Folder\\ShellEx\\ColumnHandlers"),KEY_READ);
	CString bufferString;
	LPTSTR buffer=bufferString.GetBufferSetLength(_MAX_PATH);
	int keyIndex=0;
	DWORD keyNameLength=_MAX_PATH;
	while(ERROR_SUCCESS==registryKey.EnumKey(keyIndex++,buffer,&keyNameLength))
	{
		ColumnProviderKey* pColumnProviderKey =new ColumnProviderKey();
		pColumnProviderKey ->classId=buffer;
		CRegKey providerKey ;
		providerKey.Open(registryKey,pColumnProviderKey ->classId,KEY_READ);
		keyNameLength=_MAX_PATH;
		providerKey.QueryStringValue(NULL,buffer,&keyNameLength);		
		pColumnProviderKey ->description=buffer;
		keyNameLength=_MAX_PATH;
		columnProviderKeys.push_back(pColumnProviderKey);
	}
	bufferString.ReleaseBuffer();
	size_t providerCount=columnProviderKeys.size();
	if(providerCount==0)
	{
		labelChooseColumnProvider.SetWindowText(_T("No column provider found"));
		GetDlgItem(IDOK)->EnableWindow(FALSE);
	}
	else
	{
		const TCHAR* szName [] = {
		_T("Class Id"), _T("Description")};

		for (int iColumn = 0; iColumn <ColumnProviderDialogColumnMax; iColumn++)
		{
			int nFormat = (iColumn ==ColumnProviderDialogColumnMax) ? LVCFMT_RIGHT : LVCFMT_LEFT;

			columnProviderList.InsertColumn(iColumn, szName [iColumn], nFormat, 100, iColumn);
		}

		columnProviderList.SetItemCount(providerCount);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void ColumnProviderDialog::OnDestroy()
{
	CDialogEx::OnDestroy();
	for each(ColumnProviderKey* pColumnProviderKey in columnProviderKeys)
	{
		if(pColumnProviderKey !=NULL)
			delete pColumnProviderKey ;
	}
	// TODO: Add your message handler code here
}

struct vertex

{

float x,y,z;

};

struct circular

{

vertex center;

float radius;

};
void ColumnProviderDialog::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	if(columnProviderList.GetSelectedCount()==0)
	{
		AfxMessageBox(IDS_Please_Select);
		return;
	}
	int selectedItem=columnProviderList.GetNextItem(-1,LVNI_SELECTED);
	ColumnProviderKey* pColumnProviderKey =columnProviderKeys[selectedItem];
	this->selectedColumnProviderId=pColumnProviderKey ->classId;	
	CDialogEx::OnOK();
}
