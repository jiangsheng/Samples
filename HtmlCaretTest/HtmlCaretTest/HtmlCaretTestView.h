
// HtmlCaretTestView.h : interface of the CHtmlCaretTestView class
//

#pragma once


class CHtmlCaretTestView : public CHtmlEditView
{
protected: // create from serialization only
	CHtmlCaretTestView();
	DECLARE_DYNCREATE(CHtmlCaretTestView)

// Attributes
public:
	CHtmlCaretTestDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CHtmlCaretTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_DHTMLEDITING_CMDMAP(CHtmlCaretTestView)
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMovetoend();
};

#ifndef _DEBUG  // debug version in HtmlCaretTestView.cpp
inline CHtmlCaretTestDoc* CHtmlCaretTestView::GetDocument() const
   { return reinterpret_cast<CHtmlCaretTestDoc*>(m_pDocument); }
#endif

