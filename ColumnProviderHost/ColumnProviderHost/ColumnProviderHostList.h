#pragma once


// CColumnProviderHostList
class CColumnProviderHostDoc;
class ColumnProviderHostList : public CMFCShellListCtrl
{
	DECLARE_DYNAMIC(ColumnProviderHostList)

public:
	ColumnProviderHostList();
	virtual ~ColumnProviderHostList();

	void RefreshDisplay()
	{
		this->Invalidate();
		for(int item=this->GetTopIndex();item<this->GetCountPerPage()+1;item++)
		{
			this->Update(item);
		}
	}
	void SetDocument(CColumnProviderHostDoc* pDocument)
	{
		this->pDocument=pDocument;
		GetParent()->PostMessage(WM_COMMAND,ID_VIEW_REFRESH);
	}
	virtual void OnSetColumns();
	virtual CString OnGetItemText(int iItem, int iColumn, LPAFX_SHELLITEMINFO pItem);
	virtual HRESULT EnumObjects(LPSHELLFOLDER pParentFolder, LPITEMIDLIST pidlParent);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLvnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnOdcachehint(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnOdfinditem(NMHDR *pNMHDR, LRESULT *pResult);
	CColumnProviderHostDoc* pDocument;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		bool suspendRefresh;
	virtual HRESULT Refresh()
	{
		if(suspendRefresh)
		{
			GetParent()->PostMessage(WM_COMMAND,ID_VIEW_REFRESHCOLUMNS);
			return S_OK;
		}
		else
			return CMFCShellListCtrl::Refresh();
	}
	virtual int OnCompareItems(LPARAM lParam1, LPARAM lParam2, int iColumn);
};


