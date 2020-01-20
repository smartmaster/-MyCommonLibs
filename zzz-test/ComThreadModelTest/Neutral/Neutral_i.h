

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Fri Jul 18 18:06:31 2014
 */
/* Compiler settings for Neutral.idl:
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

#ifndef __Neutral_i_h__
#define __Neutral_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IObjNeutral_FWD_DEFINED__
#define __IObjNeutral_FWD_DEFINED__
typedef interface IObjNeutral IObjNeutral;

#endif 	/* __IObjNeutral_FWD_DEFINED__ */


#ifndef __ObjNeutral_FWD_DEFINED__
#define __ObjNeutral_FWD_DEFINED__

#ifdef __cplusplus
typedef class ObjNeutral ObjNeutral;
#else
typedef struct ObjNeutral ObjNeutral;
#endif /* __cplusplus */

#endif 	/* __ObjNeutral_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IObjNeutral_INTERFACE_DEFINED__
#define __IObjNeutral_INTERFACE_DEFINED__

/* interface IObjNeutral */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IObjNeutral;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8B843005-25A4-4399-A8EB-F5F571BFF839")
    IObjNeutral : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Print( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IObjNeutralVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IObjNeutral * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IObjNeutral * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IObjNeutral * This);
        
        HRESULT ( STDMETHODCALLTYPE *Print )( 
            IObjNeutral * This);
        
        END_INTERFACE
    } IObjNeutralVtbl;

    interface IObjNeutral
    {
        CONST_VTBL struct IObjNeutralVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IObjNeutral_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IObjNeutral_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IObjNeutral_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IObjNeutral_Print(This)	\
    ( (This)->lpVtbl -> Print(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IObjNeutral_INTERFACE_DEFINED__ */



#ifndef __NeutralLib_LIBRARY_DEFINED__
#define __NeutralLib_LIBRARY_DEFINED__

/* library NeutralLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_NeutralLib;

EXTERN_C const CLSID CLSID_ObjNeutral;

#ifdef __cplusplus

class DECLSPEC_UUID("F1259BBB-A075-4DB1-8677-469622215F77")
ObjNeutral;
#endif
#endif /* __NeutralLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


