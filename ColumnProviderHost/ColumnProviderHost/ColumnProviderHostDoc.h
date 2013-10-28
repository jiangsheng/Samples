// ColumnProviderHostDoc.h : interface of the CColumnProviderHostDoc class
//


#pragma once

class AddressBox;
class ColumnProviderHostList;
#define PIPE_NAME              TEXT("FolderSize")
#define PIPE_BUFFER_SIZE_IN    4096
#define PIPE_BUFFER_SIZE_OUT   4096
#define PIPE_DEFAULT_TIME_OUT  1000

class CColumnProviderHostDoc : public CDocument
{
protected: // create from serialization only
	CColumnProviderHostDoc();
	DECLARE_DYNCREATE(CColumnProviderHostDoc)

	// Attributes
public:
	void SetAddressBox(AddressBox* pAddressBox);
	SHCONTF GetItemTypes();
	void SetRelatedList();
	void SetShellList(ColumnProviderHostList*	pShellList);
	void SetShellTree(CMFCShellTreeCtrl* pShellTree)
	{
		this->pShellTree=pShellTree;
	}
	int GetProviderColumnCount(){return providerColumns.size();}
	SHCOLUMNINFO* GetProviderColumn(int index){return providerColumns[index];}
	CString OnGetItemText(int iColumn, LPAFX_SHELLITEMINFO pItem);
	// Operations
public:
	void NavigateToList(CString newPath);
	void UpdateAddressBox(LPCTSTR lpszPath);

	void GenerateProviderColumns();
	// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

	// Implementation
public:
	virtual ~CColumnProviderHostDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// Generated message map functions
protected:

	afx_msg void OnFolderUp();
	afx_msg void OnUpdateFolderUp(CCmdUI* pCmdUI);

	afx_msg void OnViewLargeicon();
	afx_msg void OnUpdateViewLargeicon(CCmdUI* pCmdUI);
	afx_msg void OnViewDetails();
	afx_msg void OnUpdateViewDetails(CCmdUI* pCmdUI);
	afx_msg void OnViewList();
	afx_msg void OnUpdateViewList(CCmdUI* pCmdUI);
	afx_msg void OnViewSmallicon();
	afx_msg void OnUpdateViewSmallicon(CCmdUI* pCmdUI);

	afx_msg void OnCopyTo();
	afx_msg void OnMoveTo();
	afx_msg void OnViewRefresh();
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
private:	

	AddressBox* pAddressBox;
	CMFCShellTreeCtrl*	pShellTree;
	ColumnProviderHostList*	pShellList;
	CString defaultAddress;
	CString columnProviderClassId;
	CComPtr<IColumnProvider> pColumnProvider;
	CString GetProviderColumnName();
	vector<SHCOLUMNINFO*> providerColumns;
	void ClearProviderColumns();
public:
	afx_msg void OnViewRefreshcolumns();
	enum PIPE_CLIENT_REQUEST
   {
   	PCR_GETFOLDERSIZE,
   	PCR_GETUPDATEDFOLDERS
   };
	bool WriteGetFolderSizeRequest(HANDLE h, const CString& strFolder)
	{
		if (strFolder.GetLength() > USHRT_MAX)
			return false;
		size_t buflen = sizeof(short) * 2 + strFolder.GetLength() * sizeof(TCHAR);
		vector<BYTE> buffer;		
		buffer.resize(buflen);
		LPBYTE p=buffer.data();
		unsigned short data = PCR_GETFOLDERSIZE;
		memcpy(p, &data, sizeof(short));
		p += sizeof(short);
		data = (unsigned short)strFolder.GetLength();
		memcpy(p, &data, sizeof(short));
		p += sizeof(short);
		memcpy(p, (LPCTSTR)strFolder, data * sizeof(TCHAR));
		p += data * sizeof(wchar_t);
		DWORD dwBytesWritten;
		BOOL bWrote = WriteFile(h, p, (DWORD)buflen, &dwBytesWritten, NULL);
		return bWrote && dwBytesWritten == buflen;
	}

	void NotifyFolderSize(CString folder)
	{
		if(columnProviderClassId.Compare(
			_T("{04DAAD08-70EF-450E-834A-DCFAF9B48748}"))!=0)
			return;

		// connect to the server and get the folders to update
		// try twice to connect to the pipe
		HANDLE hPipe = CreateFile(TEXT("\\\\.\\pipe\\") PIPE_NAME, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (hPipe == INVALID_HANDLE_VALUE)
		{
			if (GetLastError() == ERROR_PIPE_BUSY)
			{
				if (WaitNamedPipe(TEXT("\\\\.\\pipe\\") PIPE_NAME, 1000))
				{
					hPipe = CreateFile(TEXT("\\\\.\\pipe\\") PIPE_NAME, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
				}
			}
		}

		if (hPipe != INVALID_HANDLE_VALUE)
		{
			// Tell the Service what Folders we are interested at
			WriteGetFolderSizeRequest(hPipe, folder);
			CloseHandle(hPipe);
		}
	}
};
