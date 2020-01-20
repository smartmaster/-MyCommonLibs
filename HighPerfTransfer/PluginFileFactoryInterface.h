#pragma once 

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include "CommandList.h"

#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicDTraceLib.h"
#include "..\PublicHeader\PublicSimpleUnknown.h"

struct IPluginFileFactory : ISimpleUnknown
{
	//struct ISimpleUnknown : public IUnknown
	//{
	//	//MIDL_INTERFACE("00000000-0000-0000-C000-000000000046")
	//	//	IUnknown
	//	//{
	//	//public:
	//	//	BEGIN_INTERFACE
	//	//		virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
	//	//		/* [in] */ REFIID riid,
	//	//		/* [iid_is][out] */ void **ppvObject) = 0;

	//	//		virtual ULONG STDMETHODCALLTYPE AddRef( void) = 0;

	//	//		virtual ULONG STDMETHODCALLTYPE Release( void) = 0;

	//	//	END_INTERFACE
	//	//};

	//	virtual LPCTSTR WhoAmI() = 0;
	//	virtual HRESULT CreateMember() = 0;
	//	virtual HRESULT DestroyMember() = 0;
	//};

	virtual HRESULT CreateFileHelper(
		CONST BODY_FILE_DESCRITION & FileDesc, 
		IN ULONG ulDesiredAccess, 
		IN ULONG ulShareMode, 
		IN ULONG ulCreationDisposition, 
		IN ULONG ulFlagsAndAttributes, 
		OUT IFileDevice ** ppIFileDevice) = 0;

	virtual HRESULT IsBitmapAvailable(IN IFileDevice * pIFileDevice, OUT BOOL & bAvailable) = 0;
	virtual HRESULT BlockSize(IN IFileDevice * pIFileDevice, OUT LONG & BlockSize) = 0;
	virtual HRESULT BlockCount(IN IFileDevice * pIFileDevice, OUT LONGLONG & BlockCount) = 0;
	//one bit represents one block, bit streams will be written into pBitmapFile
	virtual HRESULT GetBlocksBitmap(IN IFileDevice * pIFileDevice, LONGLONG StarBlock, LONGLONG Count, IFileDevice * pBitmapFile) = 0;
};

//////////////////////////////////////////////////////////////////////////
//Plug-in module export the API
//HRESULT CreateInstanceIPluginFileFactory(IPluginFileFactory ** pp);


//////////////////////////////////////////////////////////////////////////
HRESULT CreateInstanceIPluginFileFactoryFromModule(LPCTSTR modulePath, LPCTSTR apiName, IPluginFileFactory ** pp); //<sonmi01>2015-5-19 ###???