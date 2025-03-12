

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Wed Jun 29 15:34:11 2005
 */
/* Compiler settings for \Isc26\private\LocalSearch\Sdk\wdsQuery.idl:
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

#ifndef __wdsQuery_h__
#define __wdsQuery_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ISearchDesktop_FWD_DEFINED__
#define __ISearchDesktop_FWD_DEFINED__
typedef interface ISearchDesktop ISearchDesktop;
#endif 	/* __ISearchDesktop_FWD_DEFINED__ */


#ifndef __SearchDesktop_FWD_DEFINED__
#define __SearchDesktop_FWD_DEFINED__

#ifdef __cplusplus
typedef class SearchDesktop SearchDesktop;
#else
typedef struct SearchDesktop SearchDesktop;
#endif /* __cplusplus */

#endif 	/* __SearchDesktop_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "msado15.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __ISearchDesktop_INTERFACE_DEFINED__
#define __ISearchDesktop_INTERFACE_DEFINED__

/* interface ISearchDesktop */
/* [local][ref][uuid][object] */ 


EXTERN_C const IID IID_ISearchDesktop;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A227843C-1D92-48bc-AED6-DCA566F1790E")
    ISearchDesktop : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE ExecuteSQLQuery( 
            /* [in] */ LPCWSTR lpcwstrSQL,
            /* [retval][out] */ _Recordset **ppiRs) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ExecuteQuery( 
            /* [in] */ LPCWSTR lpcwstrQuery,
            /* [in] */ LPCWSTR lpcwstrColumn,
            /* [in] */ LPCWSTR lpcwstrSort,
            /* [in] */ LPCWSTR lpcwstrRestriction,
            /* [retval][out] */ _Recordset **ppiRs) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISearchDesktopVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISearchDesktop * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISearchDesktop * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISearchDesktop * This);
        
        HRESULT ( STDMETHODCALLTYPE *ExecuteSQLQuery )( 
            ISearchDesktop * This,
            /* [in] */ LPCWSTR lpcwstrSQL,
            /* [retval][out] */ _Recordset **ppiRs);
        
        HRESULT ( STDMETHODCALLTYPE *ExecuteQuery )( 
            ISearchDesktop * This,
            /* [in] */ LPCWSTR lpcwstrQuery,
            /* [in] */ LPCWSTR lpcwstrColumn,
            /* [in] */ LPCWSTR lpcwstrSort,
            /* [in] */ LPCWSTR lpcwstrRestriction,
            /* [retval][out] */ _Recordset **ppiRs);
        
        END_INTERFACE
    } ISearchDesktopVtbl;

    interface ISearchDesktop
    {
        CONST_VTBL struct ISearchDesktopVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISearchDesktop_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISearchDesktop_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISearchDesktop_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISearchDesktop_ExecuteSQLQuery(This,lpcwstrSQL,ppiRs)	\
    (This)->lpVtbl -> ExecuteSQLQuery(This,lpcwstrSQL,ppiRs)

#define ISearchDesktop_ExecuteQuery(This,lpcwstrQuery,lpcwstrColumn,lpcwstrSort,lpcwstrRestriction,ppiRs)	\
    (This)->lpVtbl -> ExecuteQuery(This,lpcwstrQuery,lpcwstrColumn,lpcwstrSort,lpcwstrRestriction,ppiRs)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ISearchDesktop_ExecuteSQLQuery_Proxy( 
    ISearchDesktop * This,
    /* [in] */ LPCWSTR lpcwstrSQL,
    /* [retval][out] */ _Recordset **ppiRs);


void __RPC_STUB ISearchDesktop_ExecuteSQLQuery_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISearchDesktop_ExecuteQuery_Proxy( 
    ISearchDesktop * This,
    /* [in] */ LPCWSTR lpcwstrQuery,
    /* [in] */ LPCWSTR lpcwstrColumn,
    /* [in] */ LPCWSTR lpcwstrSort,
    /* [in] */ LPCWSTR lpcwstrRestriction,
    /* [retval][out] */ _Recordset **ppiRs);


void __RPC_STUB ISearchDesktop_ExecuteQuery_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISearchDesktop_INTERFACE_DEFINED__ */



#ifndef __WDSQuery_LIBRARY_DEFINED__
#define __WDSQuery_LIBRARY_DEFINED__

/* library WDSQuery */
/* [custom][version][lcid][helpstring][uuid] */ 


EXTERN_C const IID LIBID_WDSQuery;

EXTERN_C const CLSID CLSID_SearchDesktop;

#ifdef __cplusplus

class DECLSPEC_UUID("1AD68C99-00FB-416d-804B-C38DEE75D55E")
SearchDesktop;
#endif
#endif /* __WDSQuery_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


