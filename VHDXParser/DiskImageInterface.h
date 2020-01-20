#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <vector>
using namespace std;

#include "..\PublicHeader\PublicFileDeviceInterface.h"

struct  IDiskImageVirtual : public IFileDevice
{
	//struct IFileDevice : public ISimpleUnknown
	//{
	//	//struct ISimpleUnknown : public IUnknown
	//	//{
	//	//	//MIDL_INTERFACE("00000000-0000-0000-C000-000000000046")
	//	//	//	IUnknown
	//	//	//{
	//	//	//public:
	//	//	//	BEGIN_INTERFACE
	//	//	//		virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
	//	//	//		/* [in] */ REFIID riid,
	//	//	//		/* [iid_is][out] */ void **ppvObject) = 0;

	//	//	//		virtual ULONG STDMETHODCALLTYPE AddRef( void) = 0;

	//	//	//		virtual ULONG STDMETHODCALLTYPE Release( void) = 0;

	//	//	//	END_INTERFACE
	//	//	//};

	//	//	virtual LPCTSTR WhoAmI() = 0;
	//	//	virtual HRESULT CreateMember() = 0;
	//	//	virtual HRESULT DestroyMember() = 0;
	//	//};

	//	virtual HRESULT SetPointerEx(LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod) = 0;
	//	virtual HRESULT Read(ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer) = 0;
	//	virtual HRESULT Write(ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer) = 0;
	//	virtual HRESULT GetFileSize(LONGLONG * pFileSize) = 0;  //virtual disk provision size

	//};

	virtual LONG GetBlockSize() = 0; //a number of sectors
	virtual LONG GetBlockCount() = 0;

	virtual LONG GetSectorSize() = 0;

	virtual LONG GetBitmapSizePerBlock() = 0;
	virtual HRESULT GetBlocKBitmap(LONG BlockIndex, IN OUT LONG Len, BYTE * Buffer, BOOL & bContainOne, BOOL & bAllOne) = 0; //<sonmi01>2013-10-29 ###???

	// if Count < 0, then to get bitmap to the end of disk
	//one bit represents one sector (not one block), bit streams will be written into pBitmapFile
	virtual HRESULT GetBitmap(LONG StarBlock, LONG Count, IFileDevice * pBitmapFile) = 0; 

	//if it is to be called, must be before READ data and READ bitmap
	virtual VOID SetReadRange(LONG VDiskStartIndex, LONG VDiskCount) = 0; 

	virtual CONST vector<CString> & VDiskFileList() = 0;

	virtual HRESULT FlushData() = 0;
	virtual HRESULT FlushMetadata() = 0;
};

//////////////////////////////////////////////////////////////////////////
//API
//////////////////////////////////////////////////////////////////////////
/************************************************************************
1. DesiredAccess can only be GENERIC_READ or GENERIC_WRITE

2. if DesiredAccess is GENERIC_READ, 
		parentVhdxFileName			ignored
		vhdxFileName				required
		DesiredAccess				GENERIC_READ
		CreationDisposition			ignored (implies OPNE_EXISTING for READ)
		LONGLONG VirtualDiskSize	ignored
		LONG LogicalSectorSize		ignored
		LONG PhysicalSectorSize		ignored
		LONG BlockSize				ignored

3. if DesiredAccess is GENERIC_WRITE,
		a. if parentVhdxFileName is non-null,
				parentVhdxFileName			non-null
				vhdxFileName				required
				DesiredAccess				GENERIC_WRITE
				CreationDisposition			ignored (implies CREATE_NEW child for write)
				LONGLONG VirtualDiskSize	ignored
				LONG LogicalSectorSize		ignored
				LONG PhysicalSectorSize		ignored
				LONG BlockSize				optional (default 2M)

		b. if parentVhdxFileName is null,
				parentVhdxFileName			null
				vhdxFileName				required
				DesiredAccess				GENERIC_WRITE
				CreationDisposition			ignored (means OPEN_EXISTIN or CREATE_NEW for WRITE depends on file exits or not)
				LONGLONG VirtualDiskSize	required if CREATE_NEW
				LONG LogicalSectorSize		optional (default 512B if CREATE_NEW)
				LONG PhysicalSectorSize		optional (default 4096B if CREATE_NEW)
				LONG BlockSize				optional (default 2M if CREATE_NEW)

************************************************************************/
struct VHDX_DISK_TYPE_ENUM
{
	static CONST LONG DYNAMIC = 0;
	static CONST LONG FIXED = 1;
};
//////////////////////////////////////////////////////////////////////////
struct CreateInstanceCDiskImageVhdxParams
{
	LPCTSTR		parentVhdxFileName;
	LPCTSTR		vhdxFileName;
	DWORD		DesiredAccess;
	DWORD		CreationDisposition;
	LONGLONG	VirtualDiskSize;
	LONG		LogicalSectorSize;
	LONG		PhysicalSectorSize;
	LONG		BlockSize;
	LONG		VhdxDiskType; //<sonmi01>2014-4-21 ###??? //take effect only if create_new

	//CreateInstanceCDiskImageVhdxParams()
	//{
	//	ZeroMemory(this, sizeof(*this)); //all fields must be of primitive types so that it can be zeroed safely
	//}
};
HRESULT CreateInstanceCDiskImageVhdx(CONST CreateInstanceCDiskImageVhdxParams & params, IDiskImageVirtual ** ppIDiskImageVirtual, BOOL bCreateMember = TRUE);