

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Mon Jan 04 16:49:13 2016
 */
/* Compiler settings for AaaServerDll.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.00.0603 
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

#ifndef __AaaServerDll_i_h__
#define __AaaServerDll_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IInnerObject_FWD_DEFINED__
#define __IInnerObject_FWD_DEFINED__
typedef interface IInnerObject IInnerObject;

#endif 	/* __IInnerObject_FWD_DEFINED__ */


#ifndef __InnerObject_FWD_DEFINED__
#define __InnerObject_FWD_DEFINED__

#ifdef __cplusplus
typedef class InnerObject InnerObject;
#else
typedef struct InnerObject InnerObject;
#endif /* __cplusplus */

#endif 	/* __InnerObject_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IInnerObject_INTERFACE_DEFINED__
#define __IInnerObject_INTERFACE_DEFINED__

/* interface IInnerObject */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IInnerObject;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("DF89BFBE-A053-41FF-ADCC-A00089870492")
    IInnerObject : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE OutDynamicArray( 
            /* [out] */ LONG *pLength,
            /* [size_is][size_is][out] */ WCHAR **ppBuffer) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IInnerObjectVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IInnerObject * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IInnerObject * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IInnerObject * This);
        
        HRESULT ( STDMETHODCALLTYPE *OutDynamicArray )( 
            IInnerObject * This,
            /* [out] */ LONG *pLength,
            /* [size_is][size_is][out] */ WCHAR **ppBuffer);
        
        END_INTERFACE
    } IInnerObjectVtbl;

    interface IInnerObject
    {
        CONST_VTBL struct IInnerObjectVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IInnerObject_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IInnerObject_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IInnerObject_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IInnerObject_OutDynamicArray(This,pLength,ppBuffer)	\
    ( (This)->lpVtbl -> OutDynamicArray(This,pLength,ppBuffer) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IInnerObject_INTERFACE_DEFINED__ */



#ifndef __AaaServerDllLib_LIBRARY_DEFINED__
#define __AaaServerDllLib_LIBRARY_DEFINED__

/* library AaaServerDllLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_AaaServerDllLib;

EXTERN_C const CLSID CLSID_InnerObject;

#ifdef __cplusplus

class DECLSPEC_UUID("2B6C92B3-C783-49D7-A251-AF0D3FAE8664")
InnerObject;
#endif
#endif /* __AaaServerDllLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


