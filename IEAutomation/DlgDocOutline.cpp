// DlgDocOutline.cpp : implementation file
//

#include "stdafx.h"
#include "IEAutomation.h"
#include "DlgDocOutline.h"
#include "DlgInputText.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDocOutline dialog


CDlgDocOutline::CDlgDocOutline(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDocOutline::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDocOutline)
	//}}AFX_DATA_INIT
}


void CDlgDocOutline::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDocOutline)
	DDX_Control(pDX, IDC_TREE, m_wndTree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDocOutline, CDialog)
	//{{AFX_MSG_MAP(CDlgDocOutline)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_OUTHTML, OnButtonEditOuthtml)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_FRAME_SOURCE, OnButtonEditFrameSource)
	ON_NOTIFY(TVN_DELETEITEM, IDC_TREE, OnDeleteitemTree)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_KICKIDLE,OnKickIdle)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_EDIT_OUTHTML,OnUpdateButtonEditOuthtml)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_EDIT_FRAME_SOURCE,OnUpdateButtonEditFrameSource)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDocOutline message handlers

void CDlgDocOutline::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CDlgDocOutline::OnCancel() 
{
	//CDialog::OnCancel();
}

BOOL CDlgDocOutline::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString strCaption;
	CComBSTR	bstrTitle,bstrURL;
	if(m_pHtmlDoc){
		m_pHtmlDoc->get_title(&bstrTitle);
		m_pHtmlDoc->get_URL(&bstrURL);
		CString strTitle(bstrTitle.m_str);
		CString strURL(bstrURL.m_str);
		strCaption.Format(IDS_DOCUMENT_OUTLINE,strTitle,strURL);
		BuildDocOutline();
	}
	UpdateDialogControls(this,FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgDocOutline::OnButtonEditOuthtml() 
{
	HTREEITEM hItem=m_wndTree.GetSelectedItem();
	if(hItem==NULL)return;
	TVITEM tvi;
	ZeroMemory(&tvi,sizeof(tvi));
	tvi.mask=TVIF_HANDLE|TVIF_PARAM;
	tvi.hItem=hItem;
	m_wndTree.GetItem(&tvi);
	LPDISPATCH spNode=(LPDISPATCH)tvi.lParam;
	CComQIPtr<IHTMLDOMNode> pNode(spNode);
	CComQIPtr<IHTMLElement> pElement(pNode);
	if(pElement){
		CComBSTR bstrOuterHTML;
		pElement->get_outerHTML(&bstrOuterHTML);
		CString	strOuterHTML(bstrOuterHTML);
		CDlgInputText	DlgInputText;
		DlgInputText.m_strText=strOuterHTML;
		if(DlgInputText.DoModal()!=IDOK)
			return;
		strOuterHTML=DlgInputText.m_strText;
		bstrOuterHTML.Empty();
		bstrOuterHTML.m_str=strOuterHTML.AllocSysString();
		HRESULT	hr=pElement->put_outerHTML(bstrOuterHTML.m_str);
		if(SUCCEEDED(hr)){
			BuildDocOutline();
		}
	}
	
}

void CDlgDocOutline::OnButtonEditFrameSource() 
{
	HTREEITEM hItem=m_wndTree.GetSelectedItem();
	if(hItem==NULL)return;
	TVITEM tvi;
	ZeroMemory(&tvi,sizeof(tvi));
	tvi.mask=TVIF_HANDLE|TVIF_PARAM;
	tvi.hItem=hItem;
	m_wndTree.GetItem(&tvi);
	LPDISPATCH spNode=(LPDISPATCH)tvi.lParam;
	CComQIPtr<IHTMLDOMNode> pNode(spNode);
	CComQIPtr<IHTMLElement> pElement(pNode);
	CComQIPtr<IWebBrowser2> pWebBrowser(pElement);
	if(pWebBrowser){
		CComPtr<IDispatch> spDocument;
		pWebBrowser->get_Document(&spDocument);
		CComQIPtr<IHTMLDocument2> pDocument(spDocument);
		if(pDocument){
			CDlgDocOutline	DlgDocOutline;
			DlgDocOutline.m_pHtmlDoc=pDocument;
			DlgDocOutline.DoModal();
			BuildDocOutline();
		}
	}
	
}
LRESULT	CDlgDocOutline::OnKickIdle(WPARAM wp,LPARAM lp)
{
	UpdateDialogControls(this,FALSE);
	return 0;
}

void CDlgDocOutline::OnDeleteitemTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	LPDISPATCH pNode=(LPDISPATCH)pNMTreeView->itemOld.lParam;
	if(pNode)
		pNode->Release();
	*pResult = 0;
}
void	CDlgDocOutline::PopulateTree(
		HTREEITEM hParent,
		LPDISPATCH spNode)
{
	if(hParent==NULL)return;
	CComQIPtr<IHTMLDOMNode> pNode(spNode);
	if(pNode==NULL)return;
	if(pNode){
		CComPtr<IDispatch> spChildren;
		if(FAILED(pNode->get_childNodes(&spChildren)))return;
		CComQIPtr<IHTMLDOMChildrenCollection> pChildrenCollection(spChildren);
		if(pChildrenCollection){
			long lLen=0;
			pChildrenCollection->get_length(&lLen);
			for(long l=0;l<lLen;l++){
				CComPtr<IDispatch> spChild;
				pChildrenCollection->item(l,&spChild);
				CComQIPtr<IHTMLDOMNode> pChild(spChild);
				if(pChild){
					HTREEITEM hItem=InsertItem(hParent,(LPDISPATCH)spChild);
					CComBSTR bstrNodeName;
					pNode->get_nodeName(&bstrNodeName);
					CString	strNodeName(bstrNodeName.m_str);
					TRACE("Insert Child Node#%d %s\r\n",l,strNodeName);
					PopulateTree(hItem,pChild);
				}
			}
		}
		CComPtr<IDispatch> spAttiributes;
		if(FAILED(pNode->get_attributes(&spAttiributes)))return;
		CComQIPtr<IHTMLAttributeCollection> pAttributeCollection(spAttiributes);
		if(pAttributeCollection){
			long lLen=0;
			pAttributeCollection->get_length(&lLen);
			for(long l=0;l<lLen;l++){
				CComPtr<IDispatch> spChild;
				COleVariant	varIndex(l,VT_I4);
				pAttributeCollection->item(&varIndex,&spChild);
				CComQIPtr<IHTMLDOMAttribute> pAttribute(spChild);
				if(pAttribute){
					VARIANT_BOOL bSpecified=VARIANT_FALSE;
					pAttribute->get_specified(&bSpecified);
					if(bSpecified){
						HTREEITEM hItem=InsertItem(hParent,(LPDISPATCH)spChild);
						CComBSTR bstrNodeName;
						pAttribute->get_nodeName(&bstrNodeName);
						CString	strNodeName(bstrNodeName.m_str);
						TRACE("Insert Attribute Node#%d %s\r\n",l,strNodeName);
						PopulateTree(hItem,spChild);
					}
				}
			}
		}
	}
}
/*
void CDlgDocOutline::OnGetdispinfoTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	LPDISPATCH spNode=(LPDISPATCH)pTVDispInfo->item.lParam;
	if(spNode&&(pTVDispInfo->item.mask &TVIF_TEXT)){
		CComQIPtr<IHTMLDOMNode> pNode(spNode);
		CString strText=GetNodeInfo(pNode);
		lstrcpyn(pTVDispInfo->item.pszText,strText,pTVDispInfo->item.cchTextMax);
	}
	*pResult = 0;
}*/
CString	CDlgDocOutline::GetNodeInfo(LPDISPATCH lpDisp)
{
	CString strText;
	CComQIPtr<IHTMLDOMNode> pNode(lpDisp);
	CComQIPtr<IHTMLDOMAttribute> pAttribute(lpDisp);
	if(pNode){
		long lNodeType=0;
		CComBSTR bstrNodeName;
		COleVariant varNodeValue;
		pNode->get_nodeName(&bstrNodeName);
		CString	strNodeName(bstrNodeName.m_str);
		pNode->get_nodeType(&lNodeType);
		pNode->get_nodeValue(&varNodeValue);
		switch(lNodeType){
		default:
			case	0:
				VariantChangeType(
					&varNodeValue,
					&varNodeValue,VARIANT_LOCALBOOL,VT_BSTR);
				strText.Format(IDS_NOTEINFO_ATTRIBUTE,strNodeName,CString(varNodeValue.bstrVal));break;
			case	1://element
				strText.Format(IDS_NOTEINFO_ELEMENT,strNodeName);break;
			case	3://text
				VariantChangeType(
					&varNodeValue,
					&varNodeValue,VARIANT_LOCALBOOL,VT_BSTR);
				strText.Format(IDS_NOTEINFO_TEXT,CString(varNodeValue.bstrVal));break;
			case	8://comment
				VariantChangeType(
					&varNodeValue,
					&varNodeValue,VARIANT_LOCALBOOL,VT_BSTR);
				strText.Format(IDS_NOTEINFO_COMMENT,CString(varNodeValue.bstrVal));break;

		}
	}
	else if(pAttribute){
		CComBSTR bstrNodeName;
		COleVariant varNodeValue;
		pAttribute->get_nodeName(&bstrNodeName);
		CString	strNodeName(bstrNodeName.m_str);
		pAttribute->get_nodeValue(&varNodeValue);
				VariantChangeType(
					&varNodeValue,
					&varNodeValue,VARIANT_LOCALBOOL,VT_BSTR);
		strText.Format(IDS_NOTEINFO_ATTRIBUTE,strNodeName,CString(varNodeValue.bstrVal));
	}
	return strText;
}
HTREEITEM CDlgDocOutline::InsertItem(HTREEITEM hParent,
		LPDISPATCH lpDisp)
{
	TVINSERTSTRUCT tvins;
	ZeroMemory(&tvins,sizeof(tvins));
	CString	strText=GetNodeInfo(lpDisp);
	tvins.hParent=hParent;
	tvins.hInsertAfter=TVI_LAST;
	tvins.item.mask=TVIF_TEXT|TVIF_PARAM;
	tvins.item.pszText=(LPTSTR)(LPCTSTR)strText;
	tvins.item.lParam=(LPARAM)lpDisp;
	HTREEITEM hItem=m_wndTree.InsertItem(&tvins);
	if(hItem){
		lpDisp->AddRef();
	}
	return hItem;
}
void	CDlgDocOutline::BuildDocOutline()
{
	m_wndTree.DeleteAllItems();
	CComQIPtr<IHTMLDocument3>	pHtmlDoc3(m_pHtmlDoc);
	if(pHtmlDoc3){
		CComQIPtr<IHTMLElement> pDocumentElement;
		pHtmlDoc3->get_documentElement(&pDocumentElement);
		CComQIPtr<IHTMLDOMNode> pDOMNode(pDocumentElement);
		if(pDOMNode){
			CComQIPtr<IDispatch> spDomNode(pDOMNode);
			HTREEITEM hRoot=InsertItem(NULL,(LPDISPATCH)spDomNode);
			CComBSTR bstrNodeName;
			pDOMNode->get_nodeName(&bstrNodeName);
			CString	strNodeName(bstrNodeName.m_str);
			TRACE("Insert root Node %s\r\n",strNodeName);
			PopulateTree(hRoot,pDOMNode);
		}
	}

}
void	CDlgDocOutline::OnUpdateButtonEditOuthtml(CCmdUI* pCmdUI)
{
	HTREEITEM hItem=m_wndTree.GetSelectedItem();
	if(hItem==NULL)return;
	TVITEM tvi;
	ZeroMemory(&tvi,sizeof(tvi));
	tvi.mask=TVIF_HANDLE|TVIF_PARAM;
	tvi.hItem=hItem;
	m_wndTree.GetItem(&tvi);
	LPDISPATCH spNode=(LPDISPATCH)tvi.lParam;
	CComQIPtr<IHTMLDOMNode> pNode(spNode);
	CComQIPtr<IHTMLElement> pElement(pNode);
	pCmdUI->Enable(!(pElement==NULL));
}
void	CDlgDocOutline::OnUpdateButtonEditFrameSource(CCmdUI* pCmdUI)
{
	HTREEITEM hItem=m_wndTree.GetSelectedItem();
	if(hItem==NULL)return;
	TVITEM tvi;
	ZeroMemory(&tvi,sizeof(tvi));
	tvi.mask=TVIF_HANDLE|TVIF_PARAM;
	tvi.hItem=hItem;
	m_wndTree.GetItem(&tvi);
	LPDISPATCH spNode=(LPDISPATCH)tvi.lParam;
	CComQIPtr<IHTMLDOMNode> pNode(spNode);
	CComQIPtr<IHTMLElement> pElement(pNode);
	CComQIPtr<IWebBrowser2> pWebBrowser(pElement);
	pCmdUI->Enable(!(pWebBrowser==NULL));
}
