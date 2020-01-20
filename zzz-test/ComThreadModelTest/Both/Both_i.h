

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Fri Jul 18 18:06:02 2014
 */
/* Compiler settings for Both.idl:
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

#ifndef __Both_i_h__
#define __Both_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IObjBoth_FWD_DEFINED__
#define __IObjBoth_FWD_DEFINED__
typedef interface IObjBoth IObjBoth;

#endif 	/* __IObjBoth_FWD_DEFINED__ */


#ifndef __ObjBoth_FWD_DEFINED__
#define __ObjBoth_FWD_DEFINED__

#ifdef __cplusplus
typedef class ObjBoth ObjBoth;
#else
typedef struct ObjBoth ObjBoth;
#endif /* __cplusplus */

#endif 	/* __ObjBoth_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IObjBoth_INTERFACE_DEFINED__
#define __IObjBoth_INTERFACE_DEFINED__

/* interface IObjBoth */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IObjBoth;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("56577B88-B1D9-4E9A-8E5D-5BD9E0D54A09")
    IObjBoth : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Print( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IObjBothVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IObjBoth * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IObjBoth * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IObjBoth * This);
        
        HRESULT ( STDMETHODCALLTYPE *Print )( 
            IObjBoth * This);
        
        END_INTERFACE
    } IObjBothVtbl;

    interface IObjBoth
    {
        CONST_VTBL struct IObjBothVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IObjBoth_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IObjBoth_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IObjBoth_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IObjBoth_Print(This)	\
    ( (This)->lpVtbl -> Print(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IObjBoth_INTERFACE_DEFINED__ */



#ifndef __BothLib_LIBRARY_DEFINED__
#define __BothLib_LIBRARY_DEFINED__

/* library BothLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_BothLib;

EXTERN_C const CLSID CLSID_ObjBoth;

#ifdef __cplusplus

class DECLSPEC_UUID("56297545-4AC8-49BE-A979-B311BED73DF4")
ObjBoth;
#endif
#endif /* __BothLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


