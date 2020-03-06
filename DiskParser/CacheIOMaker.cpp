#include "stdafx.h"
#include "CacheIOMaker.h"


namespace SmartLib
{


CCacheIoMaker::CCacheIoMaker(IFileDevice * pFileDevice, BOOL bAsOwner, LONG cacheBlockSize, BOOL bWriteNewAndSequential, BOOL isFlushable) :
	m_bAsOwner(bAsOwner),
	m_cacheBlockSize(cacheBlockSize),
	m_bWriteNewAndSequential(bWriteNewAndSequential),
	m_isFlushable(isFlushable)
{
	if (bAsOwner)
	{
		m_pFileDevice = pFileDevice;
	}
	else
	{
		m_pFileDevice.Attach(pFileDevice);
	}
}

HRESULT CCacheIoMaker::CreateMember()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	hr = CreateInstanceCCacheIO(
		m_pFileDevice,//IFileDevice * pFileDevice, 
		m_bAsOwner,//BOOL bAsOwner, 
		m_cacheBlockSize,//LONG BlockSize, 
		m_bWriteNewAndSequential,//BOOL bWriteNewAndSequential,
		&m_cacheIo,//OUT ICCacheIO ** ppobj,
		TRUE//BOOL bCreateMember
		);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("CreateInstanceCCacheIO"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	m_cacheIoShadow = m_cacheIo.p;
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CCacheIoMaker::DestroyMember()
{
	HRESULT hr = Flush();

	m_cacheIo.Release();
	if (m_bAsOwner)
	{
		m_pFileDevice.Release();
	}
	else
	{
		m_pFileDevice.Detach();
	}

	return hr;
}

HRESULT CCacheIoMaker::SetPointerEx(LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod)
{
	return m_cacheIoShadow->SetPointerEx(llDistanceToMove, lpNewFilePointer, ulMoveMethod);
}

HRESULT CCacheIoMaker::Read(ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer)
{
	return m_cacheIoShadow->Read(nNumberOfBytesToRead, lpNumberOfBytesRead, pBuffer);
}

HRESULT CCacheIoMaker::Write(ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer)
{
	return m_cacheIoShadow->Write(nNumberOfBytesToWrite, lpNumberOfBytesWritten, pBuffer);
}

HRESULT CCacheIoMaker::GetFileSize(LONGLONG * pFileSize)
{
	return m_pFileDevice->GetFileSize(pFileSize);
}

HANDLE CCacheIoMaker::GetRawHandle()
{
	return m_pFileDevice->GetRawHandle();
}

HRESULT CCacheIoMaker::Flush()
{
	HRESULT hr = S_OK;
	if (m_cacheIo)
	{
		HRESULT hrTemp = m_cacheIo->Flush();
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("cacheIo::Flush"), hrTemp, TEXT(""));
			hr = hrTemp;
		}
	}

	if (m_isFlushable)
	{
		IFileDeviceFlush * fileFlush = dynamic_cast<IFileDeviceFlush*>(m_pFileDevice.p);
		if (nullptr != fileFlush)
		{
			HRESULT hrTemp = fileFlush->Flush();
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("File::Flush"), hrTemp, TEXT(""));
				hr = hrTemp;
			}
		}
	}

	return hr;
}

HRESULT CreateInstanceCCacheIoMaker(IFileDevice * pFileDevice, BOOL bAsOwner, LONG blockSize, BOOL bWriteNewAndSequential, BOOL isFlushable, IFileDeviceFlush ** ppObj, BOOL bCreateMmeber)
{
	return CREATE_INSTCANCE_COBJECT(
		new CCacheIoMaker(
			pFileDevice,
			bAsOwner,
			blockSize,
			bWriteNewAndSequential,
			isFlushable),
		ppObj,
		bCreateMmeber
		);
}

} //namespace SmartLib