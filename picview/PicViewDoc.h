// PicViewDoc.h : interface of the CPicViewDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PICVIEWDOC_H__CBFA184B_8D95_4992_A6D7_9CF618E34A50__INCLUDED_)
#define AFX_PICVIEWDOC_H__CBFA184B_8D95_4992_A6D7_9CF618E34A50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class	CFolderChange:public	CObject
{
public:
	LPITEMIDLIST	m_pidlFolder;
};
class CPicViewDoc : public CDocument
{
protected: // create from serialization only
	CPicViewDoc();
	DECLARE_DYNCREATE(CPicViewDoc)

// Attributes
public:
	enum	enumUpdateHint{
		HintDefault
		,HintFolderChange
	};
// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPicViewDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPicViewDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CPicViewDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PICVIEWDOC_H__CBFA184B_8D95_4992_A6D7_9CF618E34A50__INCLUDED_)
