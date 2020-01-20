

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0595 */
/* at Mon Sep 03 02:09:41 2012
 */
/* Compiler settings for ServerVssLayer.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.00.0595 
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

#ifndef __ServerVssLayer_i_h__
#define __ServerVssLayer_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IVssLayerObject_FWD_DEFINED__
#define __IVssLayerObject_FWD_DEFINED__
typedef interface IVssLayerObject IVssLayerObject;

#endif 	/* __IVssLayerObject_FWD_DEFINED__ */


#ifndef __VssLayerObject_FWD_DEFINED__
#define __VssLayerObject_FWD_DEFINED__

#ifdef __cplusplus
typedef class VssLayerObject VssLayerObject;
#else
typedef struct VssLayerObject VssLayerObject;
#endif /* __cplusplus */

#endif 	/* __VssLayerObject_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IVssLayerObject_INTERFACE_DEFINED__
#define __IVssLayerObject_INTERFACE_DEFINED__

/* interface IVssLayerObject */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IVssLayerObject;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6E6884E7-E676-4522-B3D3-38A26ED9FD48")
    IVssLayerObject : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE ShadowCopy( 
            /* [in] */ SAFEARRAY * psaArgvs,
            /* [in] */ IStorage *pISorageXmls,
            /* [in] */ IStorage *pISorageVssResults) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE VssBackupComponentsRelease( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IVssLayerObjectVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IVssLayerObject * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IVssLayerObject * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IVssLayerObject * This);
        
        HRESULT ( STDMETHODCALLTYPE *ShadowCopy )( 
            IVssLayerObject * This,
            /* [in] */ SAFEARRAY * psaArgvs,
            /* [in] */ IStorage *pISorageXmls,
            /* [in] */ IStorage *pISorageVssResults);
        
        HRESULT ( STDMETHODCALLTYPE *VssBackupComponentsRelease )( 
            IVssLayerObject * This);
        
        END_INTERFACE
    } IVssLayerObjectVtbl;

    interface IVssLayerObject
    {
        CONST_VTBL struct IVssLayerObjectVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVssLayerObject_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IVssLayerObject_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IVssLayerObject_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IVssLayerObject_ShadowCopy(This,psaArgvs,pISorageXmls,pISorageVssResults)	\
    ( (This)->lpVtbl -> ShadowCopy(This,psaArgvs,pISorageXmls,pISorageVssResults) ) 

#define IVssLayerObject_VssBackupComponentsRelease(This)	\
    ( (This)->lpVtbl -> VssBackupComponentsRelease(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IVssLayerObject_INTERFACE_DEFINED__ */



#ifndef __ServerVssLayerLib_LIBRARY_DEFINED__
#define __ServerVssLayerLib_LIBRARY_DEFINED__

/* library ServerVssLayerLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_ServerVssLayerLib;

EXTERN_C const CLSID CLSID_VssLayerObject;

#ifdef __cplusplus

class DECLSPEC_UUID("585D2F07-73E9-4DB6-B93C-D9531D906326")
VssLayerObject;
#endif
#endif /* __ServerVssLayerLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  LPSAFEARRAY_UserSize(     unsigned long *, unsigned long            , LPSAFEARRAY * ); 
unsigned char * __RPC_USER  LPSAFEARRAY_UserMarshal(  unsigned long *, unsigned char *, LPSAFEARRAY * ); 
unsigned char * __RPC_USER  LPSAFEARRAY_UserUnmarshal(unsigned long *, unsigned char *, LPSAFEARRAY * ); 
void                      __RPC_USER  LPSAFEARRAY_UserFree(     unsigned long *, LPSAFEARRAY * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


