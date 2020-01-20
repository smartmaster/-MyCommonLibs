#pragma once

#include "..\PublicHeader\PublicFileDeviceInterface.h"

#include "ParserSeparateVHDX.h"


class CFileDeviceForVhdxCacheIO : public IFileDevice
{
private:
	ParserSeparateVHDX * m_pSeparateVHDX; /***INCOMMING***/
	LONG m_CurrentBlockIndex; /***INTERNAL***/

public:
	CFileDeviceForVhdxCacheIO(ParserSeparateVHDX * pParserVHDXMetadata);

	~CFileDeviceForVhdxCacheIO();
	//struct IFileDevice : public ISimpleUnknown
	//{
	//	//struct ISimpleUnknown : public IUnknown
	//	//{
	//	//	//MIDL_INTERFACE("00000000-0000-0000-C000-000000000046")
	//	//	//	IUnknown
	//	//	//{
	//	//	//public:
	//	//	//	BEGIN_INTERFACE
	virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ void **ppvObject);

	virtual ULONG STDMETHODCALLTYPE AddRef( void);

	virtual ULONG STDMETHODCALLTYPE Release( void);

	//	//	//	END_INTERFACE
	//	//	//};

	virtual LPCTSTR WhoAmI();

	virtual HRESULT CreateMember();

	virtual HRESULT DestroyMember();
	//	//};

	virtual HRESULT SetPointerEx(LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod);
	virtual HRESULT Read(ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer);
	virtual HRESULT Write(ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer);
	virtual HRESULT GetFileSize(LONGLONG * pFileSize);
	virtual HANDLE GetRawHandle();

	//};

};