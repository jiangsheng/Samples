#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "ColumnProviderKey.h"

// ColumnProviderDialog dialog

class ColumnProviderDialog : public CDialogEx
{
	DECLARE_DYNAMIC(ColumnProviderDialog)

public:
	ColumnProviderDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~ColumnProviderDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_PROVIDER };
	enum { ColumnProviderDialogIdColumn,ColumnProviderDialogDescriptionColumn,ColumnProviderDialogColumnMax};
	vector<ColumnProviderKey*> columnProviderKeys;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl columnProviderList;
	CStatic labelChooseColumnProvider;
	CString selectedColumnProviderId;
	afx_msg void OnLvnGetdispinfoListProviders(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual void OnOK();
};
