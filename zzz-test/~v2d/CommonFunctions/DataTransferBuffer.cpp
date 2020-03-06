#include "stdafx.h"
#include "DataTransferBuffer.h"

namespace NS_DATATRANSFER
{
	HRESULT SendLengthBuffer(ISendReceive * pSR, CONST CLengthBuffer & LenBuffer)
	{
		HRESULT hr = S_OK;
		ULONG nSent = 0;
		hr = pSR->Send(LenBuffer.GetGrandBuffer(), LenBuffer.GetGrandLength(), &nSent);
		if (FAILED(hr))
		{
			_OLD_DAPIERR((0, DRNPOS, TEXT("Send"), hr));
		}
		else if (nSent != LenBuffer.GetGrandLength())
		{
			hr = HRESULT_FROM_WIN32(ERROR_BAD_LENGTH);
			_OLD_DAPIERR((0, DRNPOS, TEXT("Send"), hr, TEXT("[%u, %I64d]"), nSent, LenBuffer.GetGrandLength()));
		}
		return hr;
	}

	HRESULT ReceiveLengthBuffer(ISendReceive * pSR, CLengthBuffer & LenBuffer)
	{
		struct CLenHeader
		{
			LONGLONG m_GrandLen;
			LONGLONG m_DataLen;
		} LenHeader;

		ZeroMemory(&LenHeader, sizeof(CLenHeader));

		HRESULT hr = S_OK;
		DWORD dwLastError = 0;

		do 
		{
			//////////////////////////////////////////////////////////////////////////
			ULONG nReceived = 0;
			hr = pSR->Receive(&LenHeader, CLengthBuffer::HEADER_LENGTH, &nReceived);
			if (FAILED(hr))
			{
				_OLD_DAPIERR((0, DRNPOS, TEXT("Receive Header"), hr));
				break;
			}
			else if (nReceived != CLengthBuffer::HEADER_LENGTH)
			{
				hr = HRESULT_FROM_WIN32(ERROR_BAD_LENGTH);
				_OLD_DAPIERR((0, DRNPOS, TEXT("Receive Header"), hr, TEXT("[%u, %u]"), nReceived, CLengthBuffer::HEADER_LENGTH));
				break;
			}

			//////////////////////////////////////////////////////////////////////////
			LenBuffer.Allocate(LenHeader.m_DataLen);
			if (LenBuffer.GetGrandLength() <= CLengthBuffer::HEADER_LENGTH)
			{
				break;
			}

			//////////////////////////////////////////////////////////////////////////
			nReceived = 0;
			hr = pSR->Receive(LenBuffer.GetDataBuffer(), LenBuffer.GetGrandLength() - CLengthBuffer::HEADER_LENGTH, &nReceived);
			if (FAILED(hr))
			{
				_OLD_DAPIERR((0, DRNPOS, TEXT("Receive Data"), hr));
				break;
			}
			else if (nReceived !=  (LenBuffer.GetGrandLength() - CLengthBuffer::HEADER_LENGTH))
			{
				hr = HRESULT_FROM_WIN32(ERROR_BAD_LENGTH);
				_OLD_DAPIERR((0, DRNPOS, TEXT("Receive Data"), hr, TEXT("[%u, %I64d]"), nReceived, LenBuffer.GetGrandLength() - CLengthBuffer::HEADER_LENGTH));
				break;
			}

		} while (FALSE);

		return hr;
	}
}