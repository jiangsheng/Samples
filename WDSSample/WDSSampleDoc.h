// WDSSampleDoc.h : interface of the CWDSSampleDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WDSSAMPLEDOC_H__EC94488E_9E43_41E3_A7CD_4FA4B7D67771__INCLUDED_)
#define AFX_WDSSAMPLEDOC_H__EC94488E_9E43_41E3_A7CD_4FA4B7D67771__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CWDSSampleDoc : public CDocument
{
protected: // create from serialization only
	CWDSSampleDoc();
	DECLARE_DYNCREATE(CWDSSampleDoc)

// Attributes
public:
	CString	m_strQuery;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWDSSampleDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWDSSampleDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CWDSSampleDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WDSSAMPLEDOC_H__EC94488E_9E43_41E3_A7CD_4FA4B7D67771__INCLUDED_)
