// AddressBox.cpp : implementation file
//

#include "stdafx.h"
#include "ColumnProviderHost.h"
#include "AddressBox.h"


// AddressBox

IMPLEMENT_DYNAMIC(AddressBox, CComboBoxEx)

AddressBox::AddressBox()
{

}

AddressBox::~AddressBox()
{
}


BEGIN_MESSAGE_MAP(AddressBox, CComboBoxEx)
//	ON_WM_CREATE()
END_MESSAGE_MAP()



// AddressBox message handlers
HRESULT AddressBox::get_accName(VARIANT /*varChild*/, BSTR *pszName)
{
		CString strText = _T("Address");
		*pszName = strText.AllocSysString();
		return S_OK;
}





//int AddressBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
//{
//	if (CComboBoxEx::OnCreate(lpCreateStruct) == -1)
//		return -1;
//
//	// TODO:  Add your specialized creation code here
//
//	return 0;
//}
