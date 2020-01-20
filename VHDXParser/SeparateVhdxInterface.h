#pragma once

#include "..\PublicHeader\PublicFileDeviceInterface.h"

#include "VHDXBATEntry.h" //<sonmi01>2014-1-9 ###???

#include <string>
using namespace std;

//<sonmi01>2015-2-15 ###???


struct ISeparateVhdx : public IFileDevice
{
	enum
	{
		VHDX_DISK_STORAGE_TYPE_FIXED = 0, 
		VHDX_DISK_STORAGE_TYPE_DYNAMIC, 
		VHDX_DISK_STORAGE_TYPE_DIFF
	};

	enum EBlockState
	{
		EBS_UNKNOWN,
		EBS_NOT_PRESENT,
		EBS_ALL_ZERO,
		EBS_ALLOCATED
	};

	//<sonmi01>2015-6-3 ###???
	struct VDiskMiscInfo
	{
		GUID m_Page83Data;
	};

	//struct IFileDevice : public ISimpleUnknown
	//{
	//	struct ISimpleUnknown : public IUnknown
	//	{
	//		//MIDL_INTERFACE("00000000-0000-0000-C000-000000000046")
	//		//	IUnknown
	//		//{
	//		//public:
	//		//	BEGIN_INTERFACE
	//		//		virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
	//		//		/* [in] */ REFIID riid,
	//		//		/* [iid_is][out] */ void **ppvObject) = 0;

	//		//		virtual ULONG STDMETHODCALLTYPE AddRef( void) = 0;

	//		//		virtual ULONG STDMETHODCALLTYPE Release( void) = 0;

	//		//	END_INTERFACE
	//		//};

	//		virtual LPCTSTR WhoAmI() = 0;
	//		virtual HRESULT CreateMember() = 0;
	//		virtual HRESULT DestroyMember() = 0;
	//	};

	//	virtual HRESULT SetPointerEx(LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod) = 0;
	//	virtual HRESULT Read(ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer) = 0;
	//	virtual HRESULT Write(ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer) = 0;
	//	virtual HRESULT GetFileSize(LONGLONG * pFileSize) = 0;
	//};

	virtual LONG GetPhysicalBlockSize() = 0; //<sonmi01>2015-1-31 ###???
	virtual LONG GetLogicalBlockSize() = 0;
	virtual LONG GetLogicalBlockCount() = 0;
	virtual HRESULT ReadLogicalBlock(LONG LogicalBlockIndex, LONG BufferLen, BYTE * pBuffer) = 0;
	virtual HRESULT ReadLogicalBlockBitmap(LONG LogicalBlockIndex, LONG BufferLen, BYTE * pBuffer, BOOL & bContainOne, BOOL & bAllOne) = 0; //<sonmi01>2013-10-29 ###???
	virtual LONG GetSectorSize() = 0;


	virtual GUID GetUniqueID() const = 0;
	virtual GUID GetParentID() const = 0;

	virtual const wstring& GetParentDiskName() const = 0;

	virtual LONGLONG GetPhysicalAddress(LONGLONG LogicalAddress) = 0; //<sonmi01>2013-10-29 ###???

	//<sonmi01>2014-1-9 ###???
	virtual HRESULT DumpBatTable( LPCTSTR FileName ) const = 0;
	virtual HRESULT OpenDumpedBatTable(LPCTSTR FileName) = 0;
	virtual HRESULT ReadDumpedBatEntry(LONG BlockIndex, VHDX_BAT_ENTRY & BatEntry) = 0;
	virtual HRESULT CloseDumpedBatTable() = 0;

	//<sonmi01>2014-1-13 ###???
	virtual HRESULT CompareWithDumpedBatForZeroedData(LONG LogicalBlockIndex, BOOL & bResult) = 0;

	//<sonmi01>2014-9-24 ###???
	virtual HRESULT GetBlocksBitmap(LONG & BlockSize, LONG & BlockCount, BYTE ** ppBuffer) = 0;
	virtual HRESULT FreeBlocksBitmapBuffer(BYTE * pBuffer) = 0;
	virtual EBlockState GetBlockState(LONGLONG logicalAddress) = 0; //<sonmi01>2015-2-15 ###???

	virtual HRESULT GetVDiskMiscInfo(VDiskMiscInfo & MiscInfo) = 0; //<sonmi01>2015-6-3 ###???
};

//////////////////////////////////////////////////////////////////////////
//API
//////////////////////////////////////////////////////////////////////////
HRESULT CreateInstanceCSeparateVhdx(LPCTSTR pVhdxFile, ISeparateVhdx ** ppISeparateVhdx, BOOL bCreateMember = TRUE);