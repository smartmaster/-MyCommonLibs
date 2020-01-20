

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Fri Jul 18 18:06:02 2014
 */
/* Compiler settings for Apartment.idl:
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

#ifndef __Apartment_i_h__
#define __Apartment_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IObjApartment_FWD_DEFINED__
#define __IObjApartment_FWD_DEFINED__
typedef interface IObjApartment IObjApartment;

#endif 	/* __IObjApartment_FWD_DEFINED__ */


#ifndef __ObjApartment_FWD_DEFINED__
#define __ObjApartment_FWD_DEFINED__

#ifdef __cplusplus
typedef class ObjApartment ObjApartment;
#else
typedef struct ObjApartment ObjApartment;
#endif /* __cplusplus */

#endif 	/* __ObjApartment_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IObjApartment_INTERFACE_DEFINED__
#define __IObjApartment_INTERFACE_DEFINED__

/* interface IObjApartment */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IObjApartment;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("451516EE-3703-4C01-B2BE-474FCE19617C")
    IObjApartment : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Print( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IObjApartmentVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IObjApartment * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IObjApartment * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IObjApartment * This);
        
        HRESULT ( STDMETHODCALLTYPE *Print )( 
            IObjApartment * This);
        
        END_INTERFACE
    } IObjApartmentVtbl;

    interface IObjApartment
    {
        CONST_VTBL struct IObjApartmentVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IObjApartment_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IObjApartment_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IObjApartment_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IObjApartment_Print(This)	\
    ( (This)->lpVtbl -> Print(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IObjApartment_INTERFACE_DEFINED__ */



#ifndef __ApartmentLib_LIBRARY_DEFINED__
#define __ApartmentLib_LIBRARY_DEFINED__

/* library ApartmentLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_ApartmentLib;

EXTERN_C const CLSID CLSID_ObjApartment;

#ifdef __cplusplus

class DECLSPEC_UUID("A2186E5A-E028-4BE4-8324-776E0C7DB731")
ObjApartment;
#endif
#endif /* __ApartmentLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


