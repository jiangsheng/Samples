// IEAutomationDoc.h : interface of the CIEAutomationDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IEAUTOMATIONDOC_H__336819C4_60BC_46A5_BEB5_BD92739B4C4B__INCLUDED_)
#define AFX_IEAUTOMATIONDOC_H__336819C4_60BC_46A5_BEB5_BD92739B4C4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CIEAutomationDoc : public CDocument
{
protected: // create from serialization only
	CIEAutomationDoc();
	DECLARE_DYNCREATE(CIEAutomationDoc)

// Attributes
public:
	//Q145857 How to Use Multiple Menus in MFC App That Uses GetDefaultMenu
	CMenu	m_menuDefault;
	HMENU GetDefaultMenu();
	//for scripting
	CString	m_strScript;
// Operations
public: 

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIEAutomationDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CIEAutomationDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CIEAutomationDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IEAUTOMATIONDOC_H__336819C4_60BC_46A5_BEB5_BD92739B4C4B__INCLUDED_)
