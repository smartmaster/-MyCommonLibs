#pragma once

#include "DiskImageInterface.h"

#include "ParserChainVHDX.h"
#include "FileDeviceForVhdxChainCacheIO.h"
#include "CacheIO.h"

#include "..\PublicHeader\PublicMiscHelper.h"

class CDiskImageVhdx : public IDiskImageVirtual
{
private:
	LONG								m_RefCount; /***INTERNAL***/

	CString								m_parentVhdxFileName; /***INCOMMING***/
	CString								m_CurrentVhdxFilename; /***INCOMMING***/
	DWORD								m_DesiredAccess; /***INCOMMING***/ //currently not used. for later use to support read and write
	DWORD								m_CreationDisposition; /***INCOMMING***/ //currently not used. for later use to support read and write
	LONGLONG							m_VirtualDiskSize; /***INCOMMING***/
	LONG								m_LogicalSectorSize; /***INCOMMING***/
	LONG								m_PhysicalSectorSize; /***INCOMMING***/
	LONG								m_BlockSize; /***INCOMMING***/
	LONG								m_vhdxType; /***INCOMMING***/ //<sonmi01>2014-4-21 ###???

	vector<CString>						m_VhdxFiles; /***INTERNAL***/
	ParserChainVHDXBase					m_ChainVHDXBase; /***INTERNAL***//***TO CLEANUP***/

	CFileDeviceForVhdxChainCacheIO *	m_pFileDeviceForVhdxChainCacheIO; /***INTERNAL***//***TO CLEANUP***/
	CCacheIO *							m_pCacheIOChainVHDX; /***INTERNAL***/ /***TO CLEANUP***/


	LONGLONG							m_CurrentPointer; /***INTERNAL***/

private:
	static BOOL FindParentVhdxFile(ParserSeparateVHDX * pVHDX, CONST CString & strChild, CString & strParent);
	static BOOL IsChildParent(CONST ParserSeparateVHDX * child, CONST ParserSeparateVHDX * parent);
	HRESULT ChainVHDX();
	HRESULT ChainExistingVHDXHelper(LPCTSTR strTopChild, DWORD DesiredAccess);

public:
	CDiskImageVhdx(CONST CreateInstanceCDiskImageVhdxParams & params);

	IMP_CLASS_DESTRUCTOR(CDiskImageVhdx);


	//struct  IDiskImageVirtual : public IFileDevice
	//{
	//	//struct IFileDevice : public ISimpleUnknown
	//	//{
	//	//	//struct ISimpleUnknown : public IUnknown
	//	//	//{
	//	//	//	//MIDL_INTERFACE("00000000-0000-0000-C000-000000000046")
	//	//	//	//	IUnknown
	//	//	//	//{
	//	//	//	//public:
	//	//	//	//	BEGIN_INTERFACE
	//	//	//	//		virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
	//	//	//	//		/* [in] */ REFIID riid,
	//	//	//	//		/* [iid_is][out] */ void **ppvObject) = 0;

	//	//	//	//		virtual ULONG STDMETHODCALLTYPE AddRef( void) = 0;

	//	//	//	//		virtual ULONG STDMETHODCALLTYPE Release( void) = 0;

	//	//	//	//	END_INTERFACE
	//	//	//	//};
	IMP_IUNKNOWN_METHODS(m_RefCount);

	//	//	//	virtual LPCTSTR WhoAmI() = 0;
	IMP_WHO_AM_I(TEXT("CDiskImageVhdx:IDiskImageVirtual"));

	virtual HRESULT CreateMember();
	virtual HRESULT DestroyMember();

	//	//	//};

	virtual HRESULT SetPointerEx(LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod);
	virtual HRESULT Read(ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer);
	virtual HRESULT Write(ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer);
	virtual HRESULT GetFileSize(LONGLONG * pFileSize);  //virtual disk provision size
	virtual HANDLE GetRawHandle();
	//	//};

	virtual LONG GetBlockSize(); //a number of sectors;
	virtual LONG GetBlockCount();

	virtual LONG GetSectorSize();

	virtual LONG GetBitmapSizePerBlock();
	virtual HRESULT GetBlocKBitmap(LONG BlockIndex, IN OUT LONG Len, BYTE * Buffer, BOOL & bContainOne, BOOL & bAllOne); //<sonmi01>2013-10-29 ###???

	virtual HRESULT GetBitmap(LONG StarBlock, LONG Count, IFileDevice * pBitmapFile); //one bit represents one sector (not one block), bit streams will be written into pBitmapFile
	
	virtual VOID SetReadRange(LONG VDiskStartIndex, LONG VDiskCount);

	virtual CONST vector<CString> & VDiskFileList();

	//<sonmi01>2013-8-28 ###???
	virtual HRESULT FlushData();
	virtual HRESULT FlushMetadata();
	//};
};