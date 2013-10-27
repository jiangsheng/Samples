// MFCWinFormHybridView.h : interface of the CMFCWinFormHybridView class
//


#pragma once

#include <afxwinforms.h>   // MFC Windows Forms support
#include "managedControls.h"
class CMFCWinFormHybridView : public CFormView
{
protected: // create from serialization only
	CMFCWinFormHybridView();
	DECLARE_DYNCREATE(CMFCWinFormHybridView)

public:
	enum{ IDD = IDD_MFCWINFORMHYBRID_FORM };

// Attributes
public:
	CMFCWinFormHybridDoc* GetDocument() const;
	CWinFormsControl<MFCWinFormHybrid::managedControls> m_ctrl1;
// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CMFCWinFormHybridView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	BEGIN_DELEGATE_MAP( CMFCWinFormHybridView )
		EVENT_DELEGATE_ENTRY( OnFileExitClicked, Object^, EventArgs^ )
		EVENT_DELEGATE_ENTRY( OnEditCopyClicked, Object^, EventArgs^ )
		EVENT_DELEGATE_ENTRY( OnEditPasteClicked, Object^, EventArgs^ )
	END_DELEGATE_MAP()

	void OnFileExitClicked( System::Object^ sender, System::EventArgs^ e ) ;
	void OnEditCopyClicked( System::Object^ sender, System::EventArgs^ e ) ;
	void OnEditPasteClicked( System::Object^ sender, System::EventArgs^ e ) ;
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnDestroy();
};

#ifndef _DEBUG  // debug version in MFCWinFormHybridView.cpp
inline CMFCWinFormHybridDoc* CMFCWinFormHybridView::GetDocument() const
   { return reinterpret_cast<CMFCWinFormHybridDoc*>(m_pDocument); }
#endif

