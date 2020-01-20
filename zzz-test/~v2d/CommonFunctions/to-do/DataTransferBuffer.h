#pragma once

#include "stdafx.h"
#include "DataTransfer.h"
#include "LengthBuffer.h"


namespace NS_DATATRANSFER
{
	HRESULT SendLengthBuffer(ISendReceive * pSR, CONST CLengthBuffer & LenBuffer);
	HRESULT ReceiveLengthBuffer(ISendReceive * pSR, CLengthBuffer & LenBuffer);
}