#pragma once


// AddressBox

class AddressBox : public CComboBoxEx
{
	DECLARE_DYNAMIC(AddressBox)

public:
	AddressBox();
	virtual ~AddressBox();
	virtual HRESULT get_accName(VARIANT, BSTR *pszName);
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


