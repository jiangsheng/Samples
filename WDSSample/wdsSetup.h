

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Wed Jun 29 15:34:15 2005
 */
/* Compiler settings for \Isc26\private\LocalSearch\Sdk\wdsSetup.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __wdsSetup_h__
#define __wdsSetup_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ISearchCrawlScopeManager_FWD_DEFINED__
#define __ISearchCrawlScopeManager_FWD_DEFINED__
typedef interface ISearchCrawlScopeManager ISearchCrawlScopeManager;
#endif 	/* __ISearchCrawlScopeManager_FWD_DEFINED__ */


#ifndef __IEnumSearchFolder_FWD_DEFINED__
#define __IEnumSearchFolder_FWD_DEFINED__
typedef interface IEnumSearchFolder IEnumSearchFolder;
#endif 	/* __IEnumSearchFolder_FWD_DEFINED__ */


#ifndef __ISearchProtocolOptions_FWD_DEFINED__
#define __ISearchProtocolOptions_FWD_DEFINED__
typedef interface ISearchProtocolOptions ISearchProtocolOptions;
#endif 	/* __ISearchProtocolOptions_FWD_DEFINED__ */


#ifndef __ISearchManager_FWD_DEFINED__
#define __ISearchManager_FWD_DEFINED__
typedef interface ISearchManager ISearchManager;
#endif 	/* __ISearchManager_FWD_DEFINED__ */


#ifndef __SearchManager_FWD_DEFINED__
#define __SearchManager_FWD_DEFINED__

#ifdef __cplusplus
typedef class SearchManager SearchManager;
#else
typedef struct SearchManager SearchManager;
#endif /* __cplusplus */

#endif 	/* __SearchManager_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

/* interface __MIDL_itf_wdsSetup_0000 */
/* [local] */ 





extern RPC_IF_HANDLE __MIDL_itf_wdsSetup_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_wdsSetup_0000_v0_0_s_ifspec;

#ifndef __ISearchCrawlScopeManager_INTERFACE_DEFINED__
#define __ISearchCrawlScopeManager_INTERFACE_DEFINED__

/* interface ISearchCrawlScopeManager */
/* [unique][helpstring][uuid][local][object] */ 


EXTERN_C const IID IID_ISearchCrawlScopeManager;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("694BC091-CEC8-41c6-937A-82E7458B17DB")
    ISearchCrawlScopeManager : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE AddDefaultUrl( 
            /* [string][in] */ LPCWSTR pszUrl,
            /* [in] */ BOOL fInclude) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISearchCrawlScopeManagerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISearchCrawlScopeManager * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISearchCrawlScopeManager * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISearchCrawlScopeManager * This);
        
        HRESULT ( STDMETHODCALLTYPE *AddDefaultUrl )( 
            ISearchCrawlScopeManager * This,
            /* [string][in] */ LPCWSTR pszUrl,
            /* [in] */ BOOL fInclude);
        
        END_INTERFACE
    } ISearchCrawlScopeManagerVtbl;

    interface ISearchCrawlScopeManager
    {
        CONST_VTBL struct ISearchCrawlScopeManagerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISearchCrawlScopeManager_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISearchCrawlScopeManager_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISearchCrawlScopeManager_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISearchCrawlScopeManager_AddDefaultUrl(This,pszUrl,fInclude)	\
    (This)->lpVtbl -> AddDefaultUrl(This,pszUrl,fInclude)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ISearchCrawlScopeManager_AddDefaultUrl_Proxy( 
    ISearchCrawlScopeManager * This,
    /* [string][in] */ LPCWSTR pszUrl,
    /* [in] */ BOOL fInclude);


void __RPC_STUB ISearchCrawlScopeManager_AddDefaultUrl_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISearchCrawlScopeManager_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_wdsSetup_0257 */
/* [local] */ 

typedef struct tagSearchFolder
    {
    BSTR bstrDisplayName;
    BSTR bstrPhysicalURL;
    } 	SearchFolder;



extern RPC_IF_HANDLE __MIDL_itf_wdsSetup_0257_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_wdsSetup_0257_v0_0_s_ifspec;

#ifndef __IEnumSearchFolder_INTERFACE_DEFINED__
#define __IEnumSearchFolder_INTERFACE_DEFINED__

/* interface IEnumSearchFolder */
/* [unique][uuid][object] */ 

typedef /* [unique] */ IEnumSearchFolder *LPENUMFOLDERENTRY;


EXTERN_C const IID IID_IEnumSearchFolder;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9C590DA1-8109-43cf-A2AF-2DB94A0EAF62")
    IEnumSearchFolder : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Next( 
            /* [in] */ ULONG celt,
            /* [length_is][size_is][out] */ SearchFolder *rgelt,
            /* [out] */ ULONG *pceltFetched) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Skip( 
            /* [in] */ ULONG celt) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Reset( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Clone( 
            /* [out] */ IEnumSearchFolder **ppenum) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEnumSearchFolderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IEnumSearchFolder * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IEnumSearchFolder * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IEnumSearchFolder * This);
        
        HRESULT ( STDMETHODCALLTYPE *Next )( 
            IEnumSearchFolder * This,
            /* [in] */ ULONG celt,
            /* [length_is][size_is][out] */ SearchFolder *rgelt,
            /* [out] */ ULONG *pceltFetched);
        
        HRESULT ( STDMETHODCALLTYPE *Skip )( 
            IEnumSearchFolder * This,
            /* [in] */ ULONG celt);
        
        HRESULT ( STDMETHODCALLTYPE *Reset )( 
            IEnumSearchFolder * This);
        
        HRESULT ( STDMETHODCALLTYPE *Clone )( 
            IEnumSearchFolder * This,
            /* [out] */ IEnumSearchFolder **ppenum);
        
        END_INTERFACE
    } IEnumSearchFolderVtbl;

    interface IEnumSearchFolder
    {
        CONST_VTBL struct IEnumSearchFolderVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEnumSearchFolder_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEnumSearchFolder_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEnumSearchFolder_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IEnumSearchFolder_Next(This,celt,rgelt,pceltFetched)	\
    (This)->lpVtbl -> Next(This,celt,rgelt,pceltFetched)

#define IEnumSearchFolder_Skip(This,celt)	\
    (This)->lpVtbl -> Skip(This,celt)

#define IEnumSearchFolder_Reset(This)	\
    (This)->lpVtbl -> Reset(This)

#define IEnumSearchFolder_Clone(This,ppenum)	\
    (This)->lpVtbl -> Clone(This,ppenum)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IEnumSearchFolder_Next_Proxy( 
    IEnumSearchFolder * This,
    /* [in] */ ULONG celt,
    /* [length_is][size_is][out] */ SearchFolder *rgelt,
    /* [out] */ ULONG *pceltFetched);


void __RPC_STUB IEnumSearchFolder_Next_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumSearchFolder_Skip_Proxy( 
    IEnumSearchFolder * This,
    /* [in] */ ULONG celt);


void __RPC_STUB IEnumSearchFolder_Skip_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumSearchFolder_Reset_Proxy( 
    IEnumSearchFolder * This);


void __RPC_STUB IEnumSearchFolder_Reset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumSearchFolder_Clone_Proxy( 
    IEnumSearchFolder * This,
    /* [out] */ IEnumSearchFolder **ppenum);


void __RPC_STUB IEnumSearchFolder_Clone_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IEnumSearchFolder_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_wdsSetup_0258 */
/* [local] */ 

#define PRTH_E_NO_APPLICATION               ((DWORD)PRTH_E_ACL_TOO_BIG+1)
#define PRTH_E_MIN_VERSION	               ((DWORD)PRTH_E_ACL_TOO_BIG+2)
#define PRTH_E_NOT_CONFIGURED               ((DWORD)PRTH_E_ACL_TOO_BIG+3)


extern RPC_IF_HANDLE __MIDL_itf_wdsSetup_0258_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_wdsSetup_0258_v0_0_s_ifspec;

#ifndef __ISearchProtocolOptions_INTERFACE_DEFINED__
#define __ISearchProtocolOptions_INTERFACE_DEFINED__

/* interface ISearchProtocolOptions */
/* [unique][helpstring][uuid][local][object] */ 


EXTERN_C const IID IID_ISearchProtocolOptions;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3C158646-6AA0-4a43-B4F6-2419E05A7B4B")
    ISearchProtocolOptions : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetRequirements( 
            /* [retval][out] */ BSTR *pbstrDescription) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CheckRequirements( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EnumSearchFolder( 
            /* [string][in] */ LPCOLESTR pwszUrl,
            /* [retval][out] */ IEnumSearchFolder **ppEnumSearchFolder) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDefaultCrawlScope( 
            /* [in] */ ISearchCrawlScopeManager *pCrawlScopeManager) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISearchProtocolOptionsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISearchProtocolOptions * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISearchProtocolOptions * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISearchProtocolOptions * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetRequirements )( 
            ISearchProtocolOptions * This,
            /* [retval][out] */ BSTR *pbstrDescription);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *CheckRequirements )( 
            ISearchProtocolOptions * This);
        
        HRESULT ( STDMETHODCALLTYPE *EnumSearchFolder )( 
            ISearchProtocolOptions * This,
            /* [string][in] */ LPCOLESTR pwszUrl,
            /* [retval][out] */ IEnumSearchFolder **ppEnumSearchFolder);
        
        HRESULT ( STDMETHODCALLTYPE *GetDefaultCrawlScope )( 
            ISearchProtocolOptions * This,
            /* [in] */ ISearchCrawlScopeManager *pCrawlScopeManager);
        
        END_INTERFACE
    } ISearchProtocolOptionsVtbl;

    interface ISearchProtocolOptions
    {
        CONST_VTBL struct ISearchProtocolOptionsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISearchProtocolOptions_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISearchProtocolOptions_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISearchProtocolOptions_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISearchProtocolOptions_GetRequirements(This,pbstrDescription)	\
    (This)->lpVtbl -> GetRequirements(This,pbstrDescription)

#define ISearchProtocolOptions_CheckRequirements(This)	\
    (This)->lpVtbl -> CheckRequirements(This)

#define ISearchProtocolOptions_EnumSearchFolder(This,pwszUrl,ppEnumSearchFolder)	\
    (This)->lpVtbl -> EnumSearchFolder(This,pwszUrl,ppEnumSearchFolder)

#define ISearchProtocolOptions_GetDefaultCrawlScope(This,pCrawlScopeManager)	\
    (This)->lpVtbl -> GetDefaultCrawlScope(This,pCrawlScopeManager)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISearchProtocolOptions_GetRequirements_Proxy( 
    ISearchProtocolOptions * This,
    /* [retval][out] */ BSTR *pbstrDescription);


void __RPC_STUB ISearchProtocolOptions_GetRequirements_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISearchProtocolOptions_CheckRequirements_Proxy( 
    ISearchProtocolOptions * This);


void __RPC_STUB ISearchProtocolOptions_CheckRequirements_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISearchProtocolOptions_EnumSearchFolder_Proxy( 
    ISearchProtocolOptions * This,
    /* [string][in] */ LPCOLESTR pwszUrl,
    /* [retval][out] */ IEnumSearchFolder **ppEnumSearchFolder);


void __RPC_STUB ISearchProtocolOptions_EnumSearchFolder_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISearchProtocolOptions_GetDefaultCrawlScope_Proxy( 
    ISearchProtocolOptions * This,
    /* [in] */ ISearchCrawlScopeManager *pCrawlScopeManager);


void __RPC_STUB ISearchProtocolOptions_GetDefaultCrawlScope_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISearchProtocolOptions_INTERFACE_DEFINED__ */


#ifndef __ISearchManager_INTERFACE_DEFINED__
#define __ISearchManager_INTERFACE_DEFINED__

/* interface ISearchManager */
/* [unique][helpstring][uuid][local][object] */ 


EXTERN_C const IID IID_ISearchManager;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7F3A3F52-5FF9-447c-8E8F-FDBAE5231F7A")
    ISearchManager : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE AddProtocol( 
            /* [string][in] */ LPCOLESTR pszProtocolName,
            /* [string][in] */ LPCOLESTR pszProgId) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE RemoveProtocol( 
            /* [string][in] */ LPCOLESTR pszProtocolName) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISearchManagerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISearchManager * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISearchManager * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISearchManager * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *AddProtocol )( 
            ISearchManager * This,
            /* [string][in] */ LPCOLESTR pszProtocolName,
            /* [string][in] */ LPCOLESTR pszProgId);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *RemoveProtocol )( 
            ISearchManager * This,
            /* [string][in] */ LPCOLESTR pszProtocolName);
        
        END_INTERFACE
    } ISearchManagerVtbl;

    interface ISearchManager
    {
        CONST_VTBL struct ISearchManagerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISearchManager_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISearchManager_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISearchManager_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISearchManager_AddProtocol(This,pszProtocolName,pszProgId)	\
    (This)->lpVtbl -> AddProtocol(This,pszProtocolName,pszProgId)

#define ISearchManager_RemoveProtocol(This,pszProtocolName)	\
    (This)->lpVtbl -> RemoveProtocol(This,pszProtocolName)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISearchManager_AddProtocol_Proxy( 
    ISearchManager * This,
    /* [string][in] */ LPCOLESTR pszProtocolName,
    /* [string][in] */ LPCOLESTR pszProgId);


void __RPC_STUB ISearchManager_AddProtocol_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISearchManager_RemoveProtocol_Proxy( 
    ISearchManager * This,
    /* [string][in] */ LPCOLESTR pszProtocolName);


void __RPC_STUB ISearchManager_RemoveProtocol_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISearchManager_INTERFACE_DEFINED__ */



#ifndef __WDSSetup_LIBRARY_DEFINED__
#define __WDSSetup_LIBRARY_DEFINED__

/* library WDSSetup */
/* [custom][helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_WDSSetup;

EXTERN_C const CLSID CLSID_SearchManager;

#ifdef __cplusplus

class DECLSPEC_UUID("963D107C-4A82-4756-9370-F6C67533DFF1")
SearchManager;
#endif
#endif /* __WDSSetup_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


