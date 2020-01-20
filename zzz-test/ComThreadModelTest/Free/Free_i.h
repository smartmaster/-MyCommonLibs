

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Fri Jul 18 18:06:02 2014
 */
/* Compiler settings for Free.idl:
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

#ifndef __Free_i_h__
#define __Free_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IObjFree_FWD_DEFINED__
#define __IObjFree_FWD_DEFINED__
typedef interface IObjFree IObjFree;

#endif 	/* __IObjFree_FWD_DEFINED__ */


#ifndef __ObjFree_FWD_DEFINED__
#define __ObjFree_FWD_DEFINED__

#ifdef __cplusplus
typedef class ObjFree ObjFree;
#else
typedef struct ObjFree ObjFree;
#endif /* __cplusplus */

#endif 	/* __ObjFree_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IObjFree_INTERFACE_DEFINED__
#define __IObjFree_INTERFACE_DEFINED__

/* interface IObjFree */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IObjFree;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("162EF895-CC72-4DCC-BEE3-DF80A375CD66")
    IObjFree : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Print( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IObjFreeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IObjFree * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IObjFree * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IObjFree * This);
        
        HRESULT ( STDMETHODCALLTYPE *Print )( 
            IObjFree * This);
        
        END_INTERFACE
    } IObjFreeVtbl;

    interface IObjFree
    {
        CONST_VTBL struct IObjFreeVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IObjFree_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IObjFree_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IObjFree_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IObjFree_Print(This)	\
    ( (This)->lpVtbl -> Print(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IObjFree_INTERFACE_DEFINED__ */



#ifndef __FreeLib_LIBRARY_DEFINED__
#define __FreeLib_LIBRARY_DEFINED__

/* library FreeLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_FreeLib;

EXTERN_C const CLSID CLSID_ObjFree;

#ifdef __cplusplus

class DECLSPEC_UUID("CF93CDFA-FDE0-462A-A0FE-E89FE1EC32F4")
ObjFree;
#endif
#endif /* __FreeLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


