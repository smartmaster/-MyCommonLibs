

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0595 */
/* at Mon Sep 03 02:09:41 2012
 */
/* Compiler settings for ServerDiskVolumeInfo.idl:
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

#ifndef __ServerDiskVolumeInfo_i_h__
#define __ServerDiskVolumeInfo_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IDiskVolumeInfoObject_FWD_DEFINED__
#define __IDiskVolumeInfoObject_FWD_DEFINED__
typedef interface IDiskVolumeInfoObject IDiskVolumeInfoObject;

#endif 	/* __IDiskVolumeInfoObject_FWD_DEFINED__ */


#ifndef __DiskVolumeInfoObject_FWD_DEFINED__
#define __DiskVolumeInfoObject_FWD_DEFINED__

#ifdef __cplusplus
typedef class DiskVolumeInfoObject DiskVolumeInfoObject;
#else
typedef struct DiskVolumeInfoObject DiskVolumeInfoObject;
#endif /* __cplusplus */

#endif 	/* __DiskVolumeInfoObject_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IDiskVolumeInfoObject_INTERFACE_DEFINED__
#define __IDiskVolumeInfoObject_INTERFACE_DEFINED__

/* interface IDiskVolumeInfoObject */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IDiskVolumeInfoObject;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C4C51433-1A9F-44B8-9D11-2EB006FC8D4E")
    IDiskVolumeInfoObject : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE WriteDiskInfo( 
            /* [in] */ IStorage *pIStorage) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE WriteVolumeInfo( 
            /* [in] */ IStorage *pIStorage,
            /* [in] */ LONG bGetVolumeBitmap) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE WriteVdsDiskVolumeInfo( 
            /* [in] */ IStorage *pIStorageDisk,
            /* [in] */ IStorage *pIStorageVolume) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE WriteSingleDiskInfo( 
            /* [in] */ ULONG ulDiskNumber,
            /* [in] */ IStorage *pIStorage,
            /* [in] */ LONG bBreakOnError) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE WriteSingleVolumeInfo( 
            /* [in] */ BSTR bstrVolume,
            /* [in] */ LONG bGetVolumeBitmap,
            /* [in] */ IStorage *pIStorage,
            /* [in] */ LONG bBreakOnError) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IDiskVolumeInfoObjectVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDiskVolumeInfoObject * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDiskVolumeInfoObject * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDiskVolumeInfoObject * This);
        
        HRESULT ( STDMETHODCALLTYPE *WriteDiskInfo )( 
            IDiskVolumeInfoObject * This,
            /* [in] */ IStorage *pIStorage);
        
        HRESULT ( STDMETHODCALLTYPE *WriteVolumeInfo )( 
            IDiskVolumeInfoObject * This,
            /* [in] */ IStorage *pIStorage,
            /* [in] */ LONG bGetVolumeBitmap);
        
        HRESULT ( STDMETHODCALLTYPE *WriteVdsDiskVolumeInfo )( 
            IDiskVolumeInfoObject * This,
            /* [in] */ IStorage *pIStorageDisk,
            /* [in] */ IStorage *pIStorageVolume);
        
        HRESULT ( STDMETHODCALLTYPE *WriteSingleDiskInfo )( 
            IDiskVolumeInfoObject * This,
            /* [in] */ ULONG ulDiskNumber,
            /* [in] */ IStorage *pIStorage,
            /* [in] */ LONG bBreakOnError);
        
        HRESULT ( STDMETHODCALLTYPE *WriteSingleVolumeInfo )( 
            IDiskVolumeInfoObject * This,
            /* [in] */ BSTR bstrVolume,
            /* [in] */ LONG bGetVolumeBitmap,
            /* [in] */ IStorage *pIStorage,
            /* [in] */ LONG bBreakOnError);
        
        END_INTERFACE
    } IDiskVolumeInfoObjectVtbl;

    interface IDiskVolumeInfoObject
    {
        CONST_VTBL struct IDiskVolumeInfoObjectVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDiskVolumeInfoObject_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDiskVolumeInfoObject_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDiskVolumeInfoObject_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDiskVolumeInfoObject_WriteDiskInfo(This,pIStorage)	\
    ( (This)->lpVtbl -> WriteDiskInfo(This,pIStorage) ) 

#define IDiskVolumeInfoObject_WriteVolumeInfo(This,pIStorage,bGetVolumeBitmap)	\
    ( (This)->lpVtbl -> WriteVolumeInfo(This,pIStorage,bGetVolumeBitmap) ) 

#define IDiskVolumeInfoObject_WriteVdsDiskVolumeInfo(This,pIStorageDisk,pIStorageVolume)	\
    ( (This)->lpVtbl -> WriteVdsDiskVolumeInfo(This,pIStorageDisk,pIStorageVolume) ) 

#define IDiskVolumeInfoObject_WriteSingleDiskInfo(This,ulDiskNumber,pIStorage,bBreakOnError)	\
    ( (This)->lpVtbl -> WriteSingleDiskInfo(This,ulDiskNumber,pIStorage,bBreakOnError) ) 

#define IDiskVolumeInfoObject_WriteSingleVolumeInfo(This,bstrVolume,bGetVolumeBitmap,pIStorage,bBreakOnError)	\
    ( (This)->lpVtbl -> WriteSingleVolumeInfo(This,bstrVolume,bGetVolumeBitmap,pIStorage,bBreakOnError) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDiskVolumeInfoObject_INTERFACE_DEFINED__ */



#ifndef __ServerDiskVolumeInfoLib_LIBRARY_DEFINED__
#define __ServerDiskVolumeInfoLib_LIBRARY_DEFINED__

/* library ServerDiskVolumeInfoLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_ServerDiskVolumeInfoLib;

EXTERN_C const CLSID CLSID_DiskVolumeInfoObject;

#ifdef __cplusplus

class DECLSPEC_UUID("55EAC270-442D-4D79-B078-5EE44B810969")
DiskVolumeInfoObject;
#endif
#endif /* __ServerDiskVolumeInfoLib_LIBRARY_DEFINED__ */

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


