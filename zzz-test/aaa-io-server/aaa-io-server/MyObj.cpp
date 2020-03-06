// MyObj.cpp : Implementation of CMyObj

#include "stdafx.h"
#include "MyObj.h"

#include "MyParamsAuto.h"

// CMyObj



STDMETHODIMP CMyObj::IO(
	/* [in] */ MyParams *input,
	/* [out] */ MyParams *output)
{
	// TODO: Add your implementation code here
	switch (input->Code)
	{
	case 0:
	{
		MyParamsAuto * tempOut = static_cast<MyParamsAuto *>(output);
		tempOut->Zero();
		tempOut->Code = input->Code;
	}
	break;
	case 1:
	{
		MyParamsAuto * tempOut = static_cast<MyParamsAuto *>(output);
		tempOut->Set(input->StringData);
		tempOut->Set(input->ArrayData);
		tempOut->Code = input->Code;
	}
	break;

	case 2:
	{
		MyParamsAuto * tempOut = static_cast<MyParamsAuto *>(output);
		tempOut->Set(TEXT("this is from server"));
		BYTE buffer[] = "this is from server";
		tempOut->Set(buffer, sizeof(buffer));
		tempOut->Code = input->Code;
	}
	break;

	default:
	{
		MyParamsAuto * tempOut = static_cast<MyParamsAuto *>(output);
		tempOut->Zero();
		tempOut->Code = input->Code;
	}
	break;
	}

	return S_OK;
}
