

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Tue Jan 05 16:07:00 2016
 */
/* Compiler settings for AaaServerExe.idl:
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

#ifndef __AaaServerExe_i_h__
#define __AaaServerExe_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IOuterObject_FWD_DEFINED__
#define __IOuterObject_FWD_DEFINED__
typedef interface IOuterObject IOuterObject;

#endif 	/* __IOuterObject_FWD_DEFINED__ */


#ifndef __IOuterObjectDotNet_FWD_DEFINED__
#define __IOuterObjectDotNet_FWD_DEFINED__
typedef interface IOuterObjectDotNet IOuterObjectDotNet;

#endif 	/* __IOuterObjectDotNet_FWD_DEFINED__ */


#ifndef __OuterObject_FWD_DEFINED__
#define __OuterObject_FWD_DEFINED__

#ifdef __cplusplus
typedef class OuterObject OuterObject;
#else
typedef struct OuterObject OuterObject;
#endif /* __cplusplus */

#endif 	/* __OuterObject_FWD_DEFINED__ */


#ifndef __OuterObjectDotNet_FWD_DEFINED__
#define __OuterObjectDotNet_FWD_DEFINED__

#ifdef __cplusplus
typedef class OuterObjectDotNet OuterObjectDotNet;
#else
typedef struct OuterObjectDotNet OuterObjectDotNet;
#endif /* __cplusplus */

#endif 	/* __OuterObjectDotNet_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IOuterObject_INTERFACE_DEFINED__
#define __IOuterObject_INTERFACE_DEFINED__

/* interface IOuterObject */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IOuterObject;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BD16E1DA-F350-4EF8-A7A5-3C6905554A52")
    IOuterObject : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE OutDynamicArray( 
            /* [out] */ LONG *pLength,
            /* [size_is][size_is][out] */ WCHAR **ppBuffer) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IOuterObjectVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IOuterObject * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IOuterObject * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IOuterObject * This);
        
        HRESULT ( STDMETHODCALLTYPE *OutDynamicArray )( 
            IOuterObject * This,
            /* [out] */ LONG *pLength,
            /* [size_is][size_is][out] */ WCHAR **ppBuffer);
        
        END_INTERFACE
    } IOuterObjectVtbl;

    interface IOuterObject
    {
        CONST_VTBL struct IOuterObjectVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOuterObject_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IOuterObject_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IOuterObject_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IOuterObject_OutDynamicArray(This,pLength,ppBuffer)	\
    ( (This)->lpVtbl -> OutDynamicArray(This,pLength,ppBuffer) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IOuterObject_INTERFACE_DEFINED__ */


#ifndef __IOuterObjectDotNet_INTERFACE_DEFINED__
#define __IOuterObjectDotNet_INTERFACE_DEFINED__

/* interface IOuterObjectDotNet */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IOuterObjectDotNet;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8D3C870F-7368-4DD3-8188-B697F2E10746")
    IOuterObjectDotNet : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Nop( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IOuterObjectDotNetVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IOuterObjectDotNet * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IOuterObjectDotNet * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IOuterObjectDotNet * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IOuterObjectDotNet * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IOuterObjectDotNet * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IOuterObjectDotNet * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IOuterObjectDotNet * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Nop )( 
            IOuterObjectDotNet * This);
        
        END_INTERFACE
    } IOuterObjectDotNetVtbl;

    interface IOuterObjectDotNet
    {
        CONST_VTBL struct IOuterObjectDotNetVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOuterObjectDotNet_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IOuterObjectDotNet_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IOuterObjectDotNet_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IOuterObjectDotNet_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IOuterObjectDotNet_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IOuterObjectDotNet_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IOuterObjectDotNet_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IOuterObjectDotNet_Nop(This)	\
    ( (This)->lpVtbl -> Nop(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IOuterObjectDotNet_INTERFACE_DEFINED__ */



#ifndef __AaaServerExeLib_LIBRARY_DEFINED__
#define __AaaServerExeLib_LIBRARY_DEFINED__

/* library AaaServerExeLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_AaaServerExeLib;

EXTERN_C const CLSID CLSID_OuterObject;

#ifdef __cplusplus

class DECLSPEC_UUID("2505D95E-689F-4E3F-866E-01E328FEE89B")
OuterObject;
#endif

EXTERN_C const CLSID CLSID_OuterObjectDotNet;

#ifdef __cplusplus

class DECLSPEC_UUID("6A92E926-1E73-4678-A2CA-3B85F01DD06F")
OuterObjectDotNet;
#endif
#endif /* __AaaServerExeLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


