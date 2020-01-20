

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Tue Jun 30 17:52:00 2015
 */
/* Compiler settings for aaaioserver.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0603 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

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

#ifndef __aaaioserver_i_h__
#define __aaaioserver_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IMyObj_FWD_DEFINED__
#define __IMyObj_FWD_DEFINED__
typedef interface IMyObj IMyObj;

#endif 	/* __IMyObj_FWD_DEFINED__ */


#ifndef __MyObj_FWD_DEFINED__
#define __MyObj_FWD_DEFINED__

#ifdef __cplusplus
typedef class MyObj MyObj;
#else
typedef struct MyObj MyObj;
#endif /* __cplusplus */

#endif 	/* __MyObj_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_aaaioserver_0000_0000 */
/* [local] */ 

typedef /* [helpstring][version][uuid] */  DECLSPEC_UUID("861A36FD-5D37-4ED8-8272-ED8D56D0D3C0") struct MyParams
    {
    long Code;
    long CodeEx;
    long Flags;
    long FlagsEx;
    BSTR StringData;
    SAFEARRAY * ArrayData;
    } 	MyParams;



extern RPC_IF_HANDLE __MIDL_itf_aaaioserver_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_aaaioserver_0000_0000_v0_0_s_ifspec;

#ifndef __IMyObj_INTERFACE_DEFINED__
#define __IMyObj_INTERFACE_DEFINED__

/* interface IMyObj */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IMyObj;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("64A6C08E-C141-498C-B8AC-BD29337EF10F")
    IMyObj : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE IO( 
            /* [in] */ MyParams *input,
            /* [out] */ MyParams *output) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IMyObjVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMyObj * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMyObj * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMyObj * This);
        
        HRESULT ( STDMETHODCALLTYPE *IO )( 
            IMyObj * This,
            /* [in] */ MyParams *input,
            /* [out] */ MyParams *output);
        
        END_INTERFACE
    } IMyObjVtbl;

    interface IMyObj
    {
        CONST_VTBL struct IMyObjVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMyObj_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMyObj_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMyObj_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMyObj_IO(This,input,output)	\
    ( (This)->lpVtbl -> IO(This,input,output) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMyObj_INTERFACE_DEFINED__ */



#ifndef __aaaioserverLib_LIBRARY_DEFINED__
#define __aaaioserverLib_LIBRARY_DEFINED__

/* library aaaioserverLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_aaaioserverLib;

EXTERN_C const CLSID CLSID_MyObj;

#ifdef __cplusplus

class DECLSPEC_UUID("8E654261-462F-4E02-BBCB-2940094C7D2F")
MyObj;
#endif
#endif /* __aaaioserverLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  LPSAFEARRAY_UserSize(     unsigned long *, unsigned long            , LPSAFEARRAY * ); 
unsigned char * __RPC_USER  LPSAFEARRAY_UserMarshal(  unsigned long *, unsigned char *, LPSAFEARRAY * ); 
unsigned char * __RPC_USER  LPSAFEARRAY_UserUnmarshal(unsigned long *, unsigned char *, LPSAFEARRAY * ); 
void                      __RPC_USER  LPSAFEARRAY_UserFree(     unsigned long *, LPSAFEARRAY * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


