#pragma once

#include "SeparateVhdxInterface.h"
#include "ParserSeparateVHDX.h"
#include "FileDeviceForVhdxCacheIO.h"

#include "MergeBuffer.h"

class CSeparateVhdx : public ISeparateVhdx
{
private:
	CString m_vhdxFileName; /***INCOMMING***/

	LONG m_RefCount;  /***INTERNAL***/
	

	LONGLONG m_CurrentPointer; /***INTERNAL***/
	//IFileDevice * m_pVhdxFile; /***INTERNAL***//***TO CLEANUP***/ //<sonmi01>2013-10-23 ###???
	ParserSeparateVHDX * m_pParserSeparateVHDX; /***INTERNAL***//***TO CLEANUP***/
	CFileDeviceForVhdxCacheIO * m_pCFileDeviceForVhdxCacheIO; /***INTERNAL***//***TO CLEANUP***/
	CCacheIO * m_pCCacheIO; /***INTERNAL***//***TO CLEANUP***/

	CVMemPtr<BYTE> m_BitmapBuffer; /***INTERNAL***/
	LONG m_BitmapBufferSize; /***INTERNAL***/
	LONG m_LogicalBitmapBufferSize; /***INTERNAL***/
	LONG m_CahcedBitmapOfPhysicalBlockIndex; /***INTERNAL***/

	wstring m_ParentVhdxName; /***INTERNAL***/

public:
	CSeparateVhdx(LPCTSTR pVhdxFile);

	IMP_CLASS_DESTRUCTOR(CSeparateVhdx);

public:
	//struct ISeparateVhdx : public IFileDevice
	//{
	//	//struct IFileDevice : public ISimpleUnknown
	//	//{
	//	//	struct ISimpleUnknown : public IUnknown
	//	//	{
	//	//		//MIDL_INTERFACE("00000000-0000-0000-C000-000000000046")
	//	//		//	IUnknown
	//	//		//{
	//	//		//public:
	//	//		//	BEGIN_INTERFACE
	//	//		//		virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
	//	//		//		/* [in] */ REFIID riid,
	//	//		//		/* [iid_is][out] */ void **ppvObject) = 0;

	//	//		//		virtual ULONG STDMETHODCALLTYPE AddRef( void) = 0;

	//	//		//		virtual ULONG STDMETHODCALLTYPE Release( void) = 0;

	//	//		//	END_INTERFACE
	//	//		//};
	IMP_IUNKNOWN_METHODS(m_RefCount);

	//	//		virtual LPCTSTR WhoAmI() = 0;
	IMP_WHO_AM_I(TEXT("CSeparateVhdx:ISeparateVhdx"));

	virtual HRESULT CreateMember();


	virtual HRESULT DestroyMember();
	//	//	};

	virtual HRESULT SetPointerEx(LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod);

	virtual HRESULT Read(ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer);

	virtual HRESULT Write(ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer)
	{
		ATLASSERT(FALSE); //assert never call this function for backup read
		return E_NOTIMPL; 
	}

	virtual HRESULT GetFileSize(LONGLONG * pFileSize);

	virtual HANDLE GetRawHandle()
	{
		return INVALID_HANDLE_VALUE;
	}

	//	//};

	virtual LONG GetPhysicalBlockSize(); //<sonmi01>2015-1-31 ###???
	virtual LONG GetLogicalBlockSize();

	virtual LONG GetLogicalBlockCount();

	virtual HRESULT ReadLogicalBlock(LONG LogicalBlockIndex, LONG BufferLen, BYTE * pBuffer);


	virtual HRESULT ReadLogicalBlockBitmap(LONG LogicalBlockIndex, LONG BufferLen, BYTE * pBuffer, BOOL & bContainOne, BOOL & bAllOne); //<sonmi01>2013-10-29 ###???

	virtual LONG GetSectorSize();

	virtual GUID GetUniqueID() const;

	virtual GUID GetParentID() const;

	virtual const wstring& GetParentDiskName() const;

	//<sonmi01>2013-10-29 ###???
	virtual LONGLONG GetPhysicalAddress(LONGLONG LogicalAddress);
	//};

	//<sonmi01>2014-1-9 ###???
	virtual HRESULT DumpBatTable( LPCTSTR FileName ) const;
	virtual HRESULT OpenDumpedBatTable(LPCTSTR FileName);
	virtual HRESULT ReadDumpedBatEntry(LONG LogicalBlockIndex, VHDX_BAT_ENTRY & BatEntry);
	virtual HRESULT CloseDumpedBatTable();

	//<sonmi01>2014-1-13 ###???
	virtual HRESULT CompareWithDumpedBatForZeroedData(LONG LogicalBlockIndex, BOOL & bResult);

	//<sonmi01>2014-9-24 ###???
	virtual HRESULT GetBlocksBitmap(LONG & BlockSize, LONG & BlockCount, BYTE ** ppBuffer);
	virtual HRESULT FreeBlocksBitmapBuffer(BYTE * pBuffer);
	virtual EBlockState GetBlockState(LONGLONG logicalAddress); //<sonmi01>2015-2-15 ###???

	virtual HRESULT GetVDiskMiscInfo(VDiskMiscInfo & MiscInfo); //<sonmi01>2015-6-3 ###???
private:
	LONG LogicalBlockIndexToPhysical(LONG LogicalBlockIndex);

	static BOOL FindParentVhdxFile( ParserSeparateVHDX * pVHDX, CONST CString & strChild, CString & strParent );
};

//<sonmi01>2013-10-22 ###???
//HRESULT CreateInstanceCSeparateVhdx(LPCTSTR pVhdxFile, ISeparateVhdx ** ppISeparateVhdx, BOOL bCreateMember = TRUE);