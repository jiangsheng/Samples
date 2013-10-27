// MFCWinFormHybridDoc.h : interface of the CMFCWinFormHybridDoc class
//


#pragma once


class CMFCWinFormHybridDoc : public CDocument
{
protected: // create from serialization only
	CMFCWinFormHybridDoc();
	DECLARE_DYNCREATE(CMFCWinFormHybridDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CMFCWinFormHybridDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


