

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Fri Jul 18 18:06:02 2014
 */
/* Compiler settings for Single.idl:
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

#ifndef __Single_i_h__
#define __Single_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IObjSingle_FWD_DEFINED__
#define __IObjSingle_FWD_DEFINED__
typedef interface IObjSingle IObjSingle;

#endif 	/* __IObjSingle_FWD_DEFINED__ */


#ifndef __ObjSingle_FWD_DEFINED__
#define __ObjSingle_FWD_DEFINED__

#ifdef __cplusplus
typedef class ObjSingle ObjSingle;
#else
typedef struct ObjSingle ObjSingle;
#endif /* __cplusplus */

#endif 	/* __ObjSingle_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IObjSingle_INTERFACE_DEFINED__
#define __IObjSingle_INTERFACE_DEFINED__

/* interface IObjSingle */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IObjSingle;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0CF1E0A7-9689-4130-81C7-11BDE7F53CD2")
    IObjSingle : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Print( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IObjSingleVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IObjSingle * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IObjSingle * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IObjSingle * This);
        
        HRESULT ( STDMETHODCALLTYPE *Print )( 
            IObjSingle * This);
        
        END_INTERFACE
    } IObjSingleVtbl;

    interface IObjSingle
    {
        CONST_VTBL struct IObjSingleVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IObjSingle_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IObjSingle_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IObjSingle_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IObjSingle_Print(This)	\
    ( (This)->lpVtbl -> Print(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IObjSingle_INTERFACE_DEFINED__ */



#ifndef __SingleLib_LIBRARY_DEFINED__
#define __SingleLib_LIBRARY_DEFINED__

/* library SingleLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_SingleLib;

EXTERN_C const CLSID CLSID_ObjSingle;

#ifdef __cplusplus

class DECLSPEC_UUID("660E1C80-F274-46BA-9B41-01829BE9B067")
ObjSingle;
#endif
#endif /* __SingleLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


