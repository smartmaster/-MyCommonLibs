#pragma once

#include "..\PublicHeader\PublicPluginFileFactoryInterface.h"
#include "..\PublicHeader\PublicPluginFileFactoryConfig.h"
#include "..\PublicHeader\PublicDiskImageInterface.h"


class CPluginFileFactoryVHDX : public IPluginFileFactory
{
private:
	LONG m_refCount;

public:
	CPluginFileFactoryVHDX() :
		m_refCount(0)
	{

	}

	IMP_CLASS_DESTRUCTOR(CPluginFileFactoryVHDX);

public:
	//struct IPluginFileFactory : ISimpleUnknown
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
	IMP_IUNKNOWN_METHODS(m_refCount);

	//	//	virtual LPCTSTR WhoAmI() = 0;
	IMP_WHO_AM_I(TEXT("class CPluginFileFactoryVHDX : public IPluginFileFactory"));
	//	//	virtual HRESULT CreateMember() = 0;
	virtual HRESULT CreateMember()
	{
		return S_OK;
	}
	//	//	virtual HRESULT DestroyMember() = 0;
	virtual HRESULT DestroyMember()
	{
		return S_OK;
	}
	//	//};

	//	virtual HRESULT CreateFileHelper(
	//		CONST BODY_FILE_DESCRITION & FileDesc,
	//		IN ULONG ulDesiredAccess,
	//		IN ULONG ulShareMode,
	//		IN ULONG ulCreationDisposition,
	//		IN ULONG ulFlagsAndAttributes,
	//		OUT IFileDevice ** ppIFileDevice) = 0;
	virtual HRESULT CreateFileHelper(
		CONST BODY_FILE_DESCRITION & FileDesc,
		IN ULONG ulDesiredAccess,
		IN ULONG ulShareMode,
		IN ULONG ulCreationDisposition,
		IN ULONG ulFlagsAndAttributes,
		OUT IFileDevice ** ppIFileDevice);

	//	virtual HRESULT IsBitmapAvailable(IN IFileDevice * pIFileDevice, OUT BOOL & bAvailable) = 0;
	virtual HRESULT IsBitmapAvailable(IN IFileDevice * pIFileDevice, OUT BOOL & bAvailable);
	//	virtual HRESULT BlockSize(IN IFileDevice * pIFileDevice, OUT LONG & BlockSize) = 0;
	virtual HRESULT BlockSize(IN IFileDevice * pIFileDevice, OUT LONG & BlockSize);
	//	virtual HRESULT BlockCount(IN IFileDevice * pIFileDevice, OUT LONG & BlockCount) = 0;
	virtual HRESULT BlockCount(IN IFileDevice * pIFileDevice, OUT LONGLONG & BlockCount);
	//	//one bit represents one block, bit streams will be written into pBitmapFile
	//	virtual HRESULT GetBlocksBitmap(IN IFileDevice * pIFileDevice, LONGLONG StarBlock, LONGLONG Count, IFileDevice * pBitmapFile) = 0;
	virtual HRESULT GetBlocksBitmap(IN IFileDevice * pIFileDevice, LONGLONG StarBlock, LONGLONG Count, IFileDevice * pBitmapFile);
	//};
};

