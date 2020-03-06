#include "stdafx.h"
#include "STCopy.h"

#include "..\PublicHeader\PublicMiscHelper.h"
#include "VMemPtr.h"

HRESULT CSTCopy::CreateMember()
{
	HRESULT hr = S_OK;
	m_RWBuffer.Allocate(IO_BUFFER_MAX_DATA_LENGTH);
	if (!m_RWBuffer)
	{
		D_API_ERR(0, TEXT("Allocate"), hr, TEXT("AllocateSize=%u"), IO_BUFFER_MAX_DATA_LENGTH);
		hr = E_OUTOFMEMORY;
	}
	return hr;
}

HRESULT CSTCopy::DestroyMember()
{
	m_RWBuffer.Free();
	return S_OK;
}

CSTCopy::CSTCopy( IFileDevice * pSource, IFileDevice * pDest, CONST map<LONGLONG, LONGLONG> * pDataRange , LONGLONG OffsetDiff ) :
	m_pSource(pSource),
	m_pDest(pDest),
	m_pDataRange(pDataRange),
	m_OffsetDiff(OffsetDiff),
	m_RefCount(0),
	m_bStartPerf(FALSE),
	m_StopTimer(TRUE)
{

}

HRESULT CSTCopy::Copy()
{
	HRESULT hr = S_OK;


	if (!m_bStartPerf)
	{
		m_bStartPerf = TRUE;
		m_StopTimer.Start();
	}

	LONGLONG TotalDataLen = 0;

	for (map<LONGLONG, LONGLONG>::const_iterator citer = m_pDataRange->begin();
		citer != m_pDataRange->end();
		++citer)
	{
		LONGLONG DataLength = citer->second - citer->first;
		hr = ProcessBuffer(citer->first, DataLength);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("ProcessBuffer"), hr, TEXT("[%I64d, %I64d]"), citer->first, citer->second - citer->first);
			break;
		}
		TotalDataLen += DataLength;
	}

	//////////////////////////////////////////////////////////////////////////
	if (SUCCEEDED(hr))
	{
		m_StopTimer.End();
		LONGLONG ElaspsedMilSec = m_StopTimer.Timing();

		CString strAverageSpeedSec;
		CString strAverageSpeedMin;
		if (ElaspsedMilSec)
		{
			LONGLONG AverageSpeedSec = TotalDataLen * 1000 / ElaspsedMilSec;
			LONGLONG AverageSpeedMin = TotalDataLen * 1000 * 60 / ElaspsedMilSec;

			CMiscHelper::NumberToString( AverageSpeedSec, strAverageSpeedSec );
			CMiscHelper::NumberToString( AverageSpeedMin, strAverageSpeedMin );
		}

		CString strTotalDataLen, strElaspsedMilSec;
		CMiscHelper::NumberToString( TotalDataLen, strTotalDataLen );
		CMiscHelper::NumberToString( ElaspsedMilSec, strElaspsedMilSec );

		D_INFO(0, TEXT("DataSize=%s Bytes, ElapsedTime=%s MillSec, AverageSpeed=%s Bytes/Sec (%s Bytes/Min)"), 
			strTotalDataLen.GetString(), strElaspsedMilSec.GetString(), strAverageSpeedSec.GetString(), strAverageSpeedMin.GetString());
	}

	return hr;
}

HRESULT CSTCopy::ProcessBuffer( LONGLONG StartOffset, LONGLONG DataLength )
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	LONGLONG NewFilePointer = 0;
	hr = m_pSource->SetPointerEx( 
		StartOffset,///* [in] */ LONGLONG llDistanceToMove,
		&NewFilePointer,///* [out] */ LONGLONG *lpNewFilePointer,
		FILE_BEGIN///* [in] */ ULONG ulMoveMethod
		);
	if (FAILED(hr))
	{	
		D_SET_LAST_STATUS(hr, hr);
		D_API_ERR(0, TEXT("SetPointerEx"), hr, TEXT("[StartOffset=%I64d, NewFilePointer=%I64d]"), StartOffset, NewFilePointer);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	LONGLONG DestOffset = StartOffset + m_OffsetDiff;
	LONGLONG NewFilePointerWrite = 0;
	hr = m_pDest->SetPointerEx( 
		DestOffset,///* [in] */ LONGLONG llDistanceToMove,
		&NewFilePointerWrite,///* [out] */ LONGLONG *lpNewFilePointer,
		FILE_BEGIN///* [in] */ ULONG ulMoveMethod
		);
	if (FAILED(hr))
	{	
		D_SET_LAST_STATUS(hr, hr);
		D_API_ERR(0, TEXT("SetPointerEx"), hr, TEXT("[llDistanceToMove=%I64d, liNewFilePointer=%I64d]"), DestOffset, NewFilePointerWrite);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	LONGLONG CurrentOffset = StartOffset;
	while (DataLength > 0)
	{
		//////////////////////////////////////////////////////////////////////////
		ULONG CurrentLength = IO_BUFFER_MAX_DATA_LENGTH;
		if (DataLength < IO_BUFFER_MAX_DATA_LENGTH)
		{
			CurrentLength  = DataLength;
		}

		//////////////////////////////////////////////////////////////////////////
		ULONG NumberOfBytesRead = 0;
		hr = m_pSource->Read( 
			CurrentLength,///* [in] */ ULONG nNumberOfBytesToRead,
			&NumberOfBytesRead,///* [out] */ ULONG *lpNumberOfBytesRead,
			m_RWBuffer.m_pData///* [size_is][out] */ BYTE *pBuffer
			);
		if (FAILED(hr))
		{	
			D_SET_LAST_STATUS(hr, hr);
			D_API_ERR(0, TEXT("Read"), hr, TEXT("[CurrentLength=%u, NumberOfBytesRead=%u, CurrentOffset=%I64d]"), CurrentLength, NumberOfBytesRead, CurrentOffset);
			break;
		}
		else if (SUCCEEDED(hr) && 0 == NumberOfBytesRead)
		{
			D_INFO(0, TEXT("File end hit, stop reading"));
			break;
		}


		ULONG NumberOfBytesWritten = 0;
		hr  = m_pDest->Write( 
			NumberOfBytesRead,///* [in] */ ULONG nNumberOfBytesToWrite,
			&NumberOfBytesWritten,///* [out] */ ULONG *lpNumberOfBytesWritten,
			m_RWBuffer.m_pData///* [size_is][in] */ BYTE *pBuffer
			);
		if (FAILED(hr) || NumberOfBytesWritten != NumberOfBytesRead)
		{	
			D_SET_LAST_STATUS(hr, hr);
			D_API_ERR(0, TEXT("Write"), hr, TEXT("[nNumberOfBytesToWrite=%u, ulNumberOfBytesWritten=%u]"), NumberOfBytesRead, NumberOfBytesWritten);
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		DataLength -= NumberOfBytesWritten;
		CurrentOffset += NumberOfBytesWritten;
	}
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}

	END_BLOCK(0);

	return hr;
}

HRESULT CSTCopy::InitParams( LPVOID pPrarams )
{
	CSTCopyParams * pCSTCopyParams = (CSTCopyParams *)(pPrarams);
	m_pSource = pCSTCopyParams->pSource;
	m_pDest = pCSTCopyParams->pDest;
	m_pDataRange = pCSTCopyParams->pDataRange;
	m_OffsetDiff = pCSTCopyParams->OffsetDiff;
	return S_OK;
}

HRESULT CreateInstanceCSTCopy( IFileDevice * pSource, IFileDevice * pDest, CONST map<LONGLONG, LONGLONG> * pDataRange, LONGLONG OffsetDiff, IFileCopy ** ppObj, BOOL bCreateMember )
{
	return CREATE_INSTCANCE_COBJECT(/*CSTCopy,*/ 
		new CSTCopy(pSource, pDest, pDataRange, OffsetDiff), 
		ppObj,
		bCreateMember);
}

//<sonmi01>2014-12-8 ###???
HRESULT SimpleSTCopy(IFileDevice * pSource, IFileDevice * pDest)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CONST LONG BUFFER_SIZE = 2 * 1024 * 1024;
	CVMemPtr<BYTE> apBuffer;
	apBuffer.Allocate(BUFFER_SIZE);
	if (nullptr == apBuffer.m_pData)
	{
		hr = E_OUTOFMEMORY;
		LEAVE_BLOCK(0);
	}

	ULONG NumberOfBytesRead = 0; 
	ULONG NumberOfBytesWritten = 0;
	for (;;)
	{
		hr = pSource->Read(BUFFER_SIZE, &NumberOfBytesRead, apBuffer.m_pData);
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("Read"), hr, TEXT(""));
			break;
		}


		hr = pDest->Write(NumberOfBytesRead, &NumberOfBytesWritten, apBuffer.m_pData);
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("Write"), hr, TEXT(""));
			break;
		}

		if (0 == NumberOfBytesRead)
		{
			break;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}
