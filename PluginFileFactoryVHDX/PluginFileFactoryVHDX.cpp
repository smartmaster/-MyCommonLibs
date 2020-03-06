#include "stdafx.h"

#include "PluginFileFactoryVHDX.h"


HRESULT CPluginFileFactoryVHDX::CreateFileHelper(CONST BODY_FILE_DESCRITION & FileDesc, IN ULONG ulDesiredAccess, IN ULONG ulShareMode, IN ULONG ulCreationDisposition, IN ULONG ulFlagsAndAttributes, OUT IFileDevice ** ppIFileDevice)
{
	CreateInstanceCDiskImageVhdxParamsXml paramsXml;
	paramsXml.FromString(FIELD_NAME(CreateInstanceCDiskImageVhdxParamsXml), FileDesc.m_MoreContext);
	CreateInstanceCDiskImageVhdxParams params;
	params.parentVhdxFileName = paramsXml.parentVhdxFileName.GetLength() ? paramsXml.parentVhdxFileName.GetString() : nullptr;
	params.vhdxFileName = paramsXml.vhdxFileName.GetString();
	params.DesiredAccess = paramsXml.DesiredAccess;
	params.CreationDisposition = paramsXml.CreationDisposition;
	params.VirtualDiskSize = paramsXml.VirtualDiskSize;
	params.LogicalSectorSize = paramsXml.LogicalSectorSize;
	params.PhysicalSectorSize = paramsXml.PhysicalSectorSize;
	params.BlockSize = paramsXml.BlockSize;
	params.VhdxDiskType = paramsXml.VhdxDiskType;

	IDiskImageVirtual * pIDiskImageVirtual = nullptr;
	HRESULT hr = CreateInstanceCDiskImageVhdx(params, &pIDiskImageVirtual, TRUE);
	*ppIFileDevice = pIDiskImageVirtual;
	return hr;
}

HRESULT CPluginFileFactoryVHDX::IsBitmapAvailable(IN IFileDevice * pIFileDevice, OUT BOOL & bAvailable)
{
	HRESULT hr = S_OK;
	bAvailable = FALSE;
	IDiskImageVirtual * p = dynamic_cast<IDiskImageVirtual *>(pIFileDevice);
	if (nullptr != p)
	{
		bAvailable = TRUE;
	}
	else
	{
		hr = E_INVALIDARG;
		D_API_ERR(0, TEXT("dynamic_cast"), hr, TEXT(""));
	}
	return hr;
}

HRESULT CPluginFileFactoryVHDX::BlockSize(IN IFileDevice * pIFileDevice, OUT LONG & BlockSize)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	IDiskImageVirtual * p = dynamic_cast<IDiskImageVirtual *>(pIFileDevice);
	if (nullptr != p)
	{
		BlockSize = p->GetSectorSize();
	}
	else
	{
		hr = E_INVALIDARG;
		D_API_ERR(0, TEXT("dynamic_cast"), hr, TEXT(""));
	}
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CPluginFileFactoryVHDX::BlockCount(IN IFileDevice * pIFileDevice, OUT LONGLONG & BlockCount)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	IDiskImageVirtual * p = dynamic_cast<IDiskImageVirtual *>(pIFileDevice);
	if (nullptr != p)
	{
#if 1
		BlockCount = p->GetBlockCount() * p->GetBlockSize() / p->GetSectorSize();
#else
		LONGLONG fileSize = 0;
		p->GetFileSize(&fileSize);
		BlockCount = fileSize / p->GetSectorSize();
#endif
		
	}
	else
	{
		hr = E_INVALIDARG;
		D_API_ERR(0, TEXT("dynamic_cast"), hr, TEXT(""));
	}
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CPluginFileFactoryVHDX::GetBlocksBitmap(IN IFileDevice * pIFileDevice, LONGLONG StarBlock, LONGLONG Count, IFileDevice * pBitmapFile)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	IDiskImageVirtual * p = dynamic_cast<IDiskImageVirtual *>(pIFileDevice);
	if (nullptr == p)
	{
		hr = E_INVALIDARG;
		D_API_ERR(0, TEXT("dynamic_cast"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	LONGLONG offset = StarBlock * p->GetSectorSize();
	LONGLONG size = Count * p->GetSectorSize();
	if (offset % p->GetBlockSize() || size % p->GetBlockSize())
	{
		hr = E_INVALIDARG;
		D_API_ERR(0, TEXT("E_INVALIDARG"), hr, TEXT("offset=%I64d, size=%I64d"), offset, size);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	LONGLONG StartBlockTmp = offset / p->GetBlockSize();
	LONGLONG CountBlockTmp = size / p->GetBlockSize();
	hr = p->GetBitmap(StartBlockTmp, CountBlockTmp, pBitmapFile);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("GetBitmap"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}


HRESULT CreateInstanceIPluginFileFactory(IPluginFileFactory ** pp)
{
	return CREATE_INSTCANCE_COBJECT(new CPluginFileFactoryVHDX(), pp, TRUE);
}