

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0595 */
/* at Mon Sep 03 02:09:41 2012
 */
/* Compiler settings for ServerFileDevice.idl:
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

#ifndef __ServerFileDevice_i_h__
#define __ServerFileDevice_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IFileDeviceObject_FWD_DEFINED__
#define __IFileDeviceObject_FWD_DEFINED__
typedef interface IFileDeviceObject IFileDeviceObject;

#endif 	/* __IFileDeviceObject_FWD_DEFINED__ */


#ifndef __FileDeviceObject_FWD_DEFINED__
#define __FileDeviceObject_FWD_DEFINED__

#ifdef __cplusplus
typedef class FileDeviceObject FileDeviceObject;
#else
typedef struct FileDeviceObject FileDeviceObject;
#endif /* __cplusplus */

#endif 	/* __FileDeviceObject_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IFileDeviceObject_INTERFACE_DEFINED__
#define __IFileDeviceObject_INTERFACE_DEFINED__

/* interface IFileDeviceObject */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IFileDeviceObject;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("4C13F41F-B70B-418B-9DF5-0CC48603D9A4")
    IFileDeviceObject : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetPointerEx( 
            /* [in] */ LONGLONG llDistanceToMove,
            /* [out] */ LONGLONG *lpNewFilePointer,
            /* [in] */ ULONG ulMoveMethod) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Read( 
            /* [in] */ ULONG nNumberOfBytesToRead,
            /* [out] */ ULONG *lpNumberOfBytesRead,
            /* [length_is][size_is][out] */ BYTE *pBuffer) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Write( 
            /* [in] */ ULONG nNumberOfBytesToWrite,
            /* [out] */ ULONG *lpNumberOfBytesWritten,
            /* [size_is][in] */ BYTE *pBuffer) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Create( 
            /* [in] */ BSTR bstrFileName,
            /* [in] */ ULONG ulDesiredAccess,
            /* [in] */ ULONG ulShareMode,
            /* [in] */ ULONG ulCreationDisposition,
            /* [in] */ ULONG ulFlagsAndAttributes) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Close( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetFileSizeEx( 
            /* [out] */ LONGLONG *pllFileSize) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IFileDeviceObjectVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IFileDeviceObject * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IFileDeviceObject * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IFileDeviceObject * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetPointerEx )( 
            IFileDeviceObject * This,
            /* [in] */ LONGLONG llDistanceToMove,
            /* [out] */ LONGLONG *lpNewFilePointer,
            /* [in] */ ULONG ulMoveMethod);
        
        HRESULT ( STDMETHODCALLTYPE *Read )( 
            IFileDeviceObject * This,
            /* [in] */ ULONG nNumberOfBytesToRead,
            /* [out] */ ULONG *lpNumberOfBytesRead,
            /* [length_is][size_is][out] */ BYTE *pBuffer);
        
        HRESULT ( STDMETHODCALLTYPE *Write )( 
            IFileDeviceObject * This,
            /* [in] */ ULONG nNumberOfBytesToWrite,
            /* [out] */ ULONG *lpNumberOfBytesWritten,
            /* [size_is][in] */ BYTE *pBuffer);
        
        HRESULT ( STDMETHODCALLTYPE *Create )( 
            IFileDeviceObject * This,
            /* [in] */ BSTR bstrFileName,
            /* [in] */ ULONG ulDesiredAccess,
            /* [in] */ ULONG ulShareMode,
            /* [in] */ ULONG ulCreationDisposition,
            /* [in] */ ULONG ulFlagsAndAttributes);
        
        HRESULT ( STDMETHODCALLTYPE *Close )( 
            IFileDeviceObject * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetFileSizeEx )( 
            IFileDeviceObject * This,
            /* [out] */ LONGLONG *pllFileSize);
        
        END_INTERFACE
    } IFileDeviceObjectVtbl;

    interface IFileDeviceObject
    {
        CONST_VTBL struct IFileDeviceObjectVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IFileDeviceObject_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IFileDeviceObject_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IFileDeviceObject_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IFileDeviceObject_SetPointerEx(This,llDistanceToMove,lpNewFilePointer,ulMoveMethod)	\
    ( (This)->lpVtbl -> SetPointerEx(This,llDistanceToMove,lpNewFilePointer,ulMoveMethod) ) 

#define IFileDeviceObject_Read(This,nNumberOfBytesToRead,lpNumberOfBytesRead,pBuffer)	\
    ( (This)->lpVtbl -> Read(This,nNumberOfBytesToRead,lpNumberOfBytesRead,pBuffer) ) 

#define IFileDeviceObject_Write(This,nNumberOfBytesToWrite,lpNumberOfBytesWritten,pBuffer)	\
    ( (This)->lpVtbl -> Write(This,nNumberOfBytesToWrite,lpNumberOfBytesWritten,pBuffer) ) 

#define IFileDeviceObject_Create(This,bstrFileName,ulDesiredAccess,ulShareMode,ulCreationDisposition,ulFlagsAndAttributes)	\
    ( (This)->lpVtbl -> Create(This,bstrFileName,ulDesiredAccess,ulShareMode,ulCreationDisposition,ulFlagsAndAttributes) ) 

#define IFileDeviceObject_Close(This)	\
    ( (This)->lpVtbl -> Close(This) ) 

#define IFileDeviceObject_GetFileSizeEx(This,pllFileSize)	\
    ( (This)->lpVtbl -> GetFileSizeEx(This,pllFileSize) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IFileDeviceObject_INTERFACE_DEFINED__ */



#ifndef __ServerFileDeviceLib_LIBRARY_DEFINED__
#define __ServerFileDeviceLib_LIBRARY_DEFINED__

/* library ServerFileDeviceLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_ServerFileDeviceLib;

EXTERN_C const CLSID CLSID_FileDeviceObject;

#ifdef __cplusplus

class DECLSPEC_UUID("415700D7-BCA9-4F86-8A48-3592C8673946")
FileDeviceObject;
#endif
#endif /* __ServerFileDeviceLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


