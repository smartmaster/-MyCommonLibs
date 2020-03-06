#include "stdafx.h"
#include "InterfaceSocketClient.h"

#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicDTraceLib.h"


HRESULT SendLengthAndBuffer(ISocketClient * pISocketClient, LONG Length, CONST BYTE * Buffer, LONG * LengthSent)
{
#if 1
	return pISocketClient->SendLengthAndBuffer(Length, Buffer, LengthSent);
#else
	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);

	ULONG DataSentLen = 0;

	LONG SendLen = htonl(Length);
	hr = pISocketClient->Write(sizeof(SendLen), &DataSentLen, (LPCBYTE)(&SendLen));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("SendLength"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	if (LengthSent)
	{
		*LengthSent += DataSentLen;
	}

	hr = pISocketClient->Write(Length, &DataSentLen, Buffer);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("SendBuffer"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	if (LengthSent)
	{
		*LengthSent += DataSentLen;
	}
	END_BLOCK(0);

	return hr;
#endif
}


HRESULT ReceiveLengthAndBuffer(ISocketClient * pISocketClient, LONG & Length, BYTE * Buffer, LONG * LengthRcv)
{
	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);

	ULONG DataRcvLen = 0;

	hr = pISocketClient->Read(sizeof(Length), &DataRcvLen, (LPBYTE)(&Length));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ReceiveLength"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	Length = ntohl(Length);
	if (LengthRcv)
	{
		*LengthRcv += DataRcvLen;
	}

	hr = pISocketClient->Read(Length, &DataRcvLen, Buffer);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ReceiveBuffer"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	if (LengthRcv)
	{
		*LengthRcv += DataRcvLen;
	}
	END_BLOCK(0);

	return hr;
}