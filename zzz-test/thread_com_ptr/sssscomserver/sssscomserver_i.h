

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Thu Nov 12 14:03:15 2015
 */
/* Compiler settings for sssscomserver.idl:
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

#ifndef __sssscomserver_i_h__
#define __sssscomserver_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __Isss_FWD_DEFINED__
#define __Isss_FWD_DEFINED__
typedef interface Isss Isss;

#endif 	/* __Isss_FWD_DEFINED__ */


#ifndef __sss_FWD_DEFINED__
#define __sss_FWD_DEFINED__

#ifdef __cplusplus
typedef class sss sss;
#else
typedef struct sss sss;
#endif /* __cplusplus */

#endif 	/* __sss_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __Isss_INTERFACE_DEFINED__
#define __Isss_INTERFACE_DEFINED__

/* interface Isss */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_Isss;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("17F3EED0-77E3-4DC9-BF71-11B8E33A8837")
    Isss : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Do( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IsssVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            Isss * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            Isss * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            Isss * This);
        
        HRESULT ( STDMETHODCALLTYPE *Do )( 
            Isss * This);
        
        END_INTERFACE
    } IsssVtbl;

    interface Isss
    {
        CONST_VTBL struct IsssVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define Isss_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define Isss_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define Isss_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define Isss_Do(This)	\
    ( (This)->lpVtbl -> Do(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __Isss_INTERFACE_DEFINED__ */



#ifndef __sssscomserverLib_LIBRARY_DEFINED__
#define __sssscomserverLib_LIBRARY_DEFINED__

/* library sssscomserverLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_sssscomserverLib;

EXTERN_C const CLSID CLSID_sss;

#ifdef __cplusplus

class DECLSPEC_UUID("2FA71697-6FDD-4CFD-8AA4-19061A619C67")
sss;
#endif
#endif /* __sssscomserverLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


