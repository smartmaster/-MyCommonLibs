

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Thu Sep 24 12:16:00 2015
 */
/* Compiler settings for MyObj.idl:
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

#ifndef __MyObj_i_h__
#define __MyObj_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ISimpleObj_FWD_DEFINED__
#define __ISimpleObj_FWD_DEFINED__
typedef interface ISimpleObj ISimpleObj;

#endif 	/* __ISimpleObj_FWD_DEFINED__ */


#ifndef __SimpleObj_FWD_DEFINED__
#define __SimpleObj_FWD_DEFINED__

#ifdef __cplusplus
typedef class SimpleObj SimpleObj;
#else
typedef struct SimpleObj SimpleObj;
#endif /* __cplusplus */

#endif 	/* __SimpleObj_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __ISimpleObj_INTERFACE_DEFINED__
#define __ISimpleObj_INTERFACE_DEFINED__

/* interface ISimpleObj */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_ISimpleObj;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("86D9D5EE-1941-4619-81FB-563920086E18")
    ISimpleObj : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Method( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ISimpleObjVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISimpleObj * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISimpleObj * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISimpleObj * This);
        
        HRESULT ( STDMETHODCALLTYPE *Method )( 
            ISimpleObj * This);
        
        END_INTERFACE
    } ISimpleObjVtbl;

    interface ISimpleObj
    {
        CONST_VTBL struct ISimpleObjVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISimpleObj_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISimpleObj_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISimpleObj_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISimpleObj_Method(This)	\
    ( (This)->lpVtbl -> Method(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISimpleObj_INTERFACE_DEFINED__ */



#ifndef __MyObjLib_LIBRARY_DEFINED__
#define __MyObjLib_LIBRARY_DEFINED__

/* library MyObjLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_MyObjLib;

EXTERN_C const CLSID CLSID_SimpleObj;

#ifdef __cplusplus

class DECLSPEC_UUID("4E33AC9E-0406-460D-936D-A729D7663491")
SimpleObj;
#endif
#endif /* __MyObjLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


