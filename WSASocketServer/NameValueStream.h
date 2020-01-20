#pragma once

#include <string>
using namespace std;

#include "..\PublicHeader\PublicGetCurrentModuleName.h"
#include "..\PublicHeader\PublicDTraceLib.h"
#include "..\PublicHeader\PublicFileDeviceInterface.h"


/*********************************************************
stream format:

NV:				NameSize Name ValueSize Value
ENDDING:		ZERO ZERO
SimpleStrut:	ByteOrder NV NV NV NV ENDING

StructHeader:	Name NEG_ONE
ComplexStruct:	ByteOrder NV NV StructHeader SimpleStrut NV NV NV StructHeader SimpleStrut NV NV ENDING
ComplexStruct:	ByteOrder NV NV StructHeader SubComplexStruct NV NV NV StructHeader SubComplexStruct NV NV ENDING
/********************************************************/

class CNameValueStream
{
	IFileDevice * m_pIFileDevice; /***INCOMMING***/

public:
	enum DATA_BYTE_ORDER
	{
		LITTLE_ENDIAN = 0x00000000,
		BIG_ENDIAN = 0xffffffff,
		NET_ORDER = 0xff0000ff
	};

public:
	CNameValueStream(IFileDevice * pFileDevice) :
		m_pIFileDevice(pFileDevice)
	{
	}

	~CNameValueStream()
	{
	}


	//HRESULT Reset()
	//{
	//	LONGLONG NewFilePointer = 0;
	//	return m_pIFileDevice->SetPointerEx(0, &NewFilePointer, FILE_BEGIN);
	//}

	//////////////////////////////////////////////////////////////////////////
	HRESULT SetByteOrder(DATA_BYTE_ORDER ByteOrder)
	{
		return WriteData<LONG>(ByteOrder);
	}


	//////////////////////////////////////////////////////////////////////////
	//Read / Write name value pair

	//////////////////////////////////////////////////////////////////////////
	template <typename T>
	HRESULT WriteNameValue(CONST wstring & Name, CONST T & Value)
	{
		//NameLengthInByte NameString ValueLengthInByte ValueBinaryData ...
		HRESULT hr = S_OK;
		BEGIN_BLOCK(0);
		//////////////////////////////////////////////////////////////////////////
		hr = WriteNameValueSize(Name, sizeof(T));
		if (FAILED(hr))
		{
			D_SET_LAST_STATUS(hr, hr);
			D_API_ERR(0, TEXT("WriteNameValueLength"), hr, TEXT("Name=%s, Length=%u"), Name.c_str(), sizeof(T));
			LEAVE_BLOCK(0);
		}

		//////////////////////////////////////////////////////////////////////////
		hr = WriteData<T>(Value);
		if (FAILED(hr))
		{
			D_SET_LAST_STATUS(hr, hr);
			D_API_ERR(0, TEXT("WriteData"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
		//////////////////////////////////////////////////////////////////////////
		END_BLOCK(0);

		return hr;
	}

	template <typename T>
	HRESULT WriteNameValue(CONST wstring & Name, CONST T * pValue, LONG Count)
	{
		//NameLengthInByte NameString ValueLengthInByte ValueBinaryData ...
		HRESULT hr = S_OK;
		BEGIN_BLOCK(0);
		//////////////////////////////////////////////////////////////////////////
		hr = WriteNameValueSize(Name, sizeof(T) * Count);
		if (FAILED(hr))
		{
			D_SET_LAST_STATUS(hr, hr);
			D_API_ERR(0, TEXT("WriteNameValueLength"), hr, TEXT("Name=%s, Length=%u"), Name.c_str(), sizeof(T) * Count);
			LEAVE_BLOCK(0);
		}

		//////////////////////////////////////////////////////////////////////////
		hr = WriteData<T>(pValue, Count);
		if (FAILED(hr))
		{
			D_SET_LAST_STATUS(hr, hr);
			D_API_ERR(0, TEXT("WriteData"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
		//////////////////////////////////////////////////////////////////////////
		END_BLOCK(0);

		return hr;
	}

	HRESULT WriteNameValueSize(CONST wstring & Name, LONG Length)
	{
		//NameLengthInByte NameString ValueLengthInByte ValueBinaryData ...
		HRESULT hr = S_OK;
		BEGIN_BLOCK(0);
		//////////////////////////////////////////////////////////////////////////
		LONG NameLengthInByte = (Name.size() + 1) * sizeof(WCHAR);
		hr = WriteData<LONG>(NameLengthInByte);
		if (FAILED(hr))
		{
			D_SET_LAST_STATUS(hr, hr);
			D_API_ERR(0, TEXT("WriteData"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
		//////////////////////////////////////////////////////////////////////////
		hr = WriteData<BYTE>((CONST BYTE*)(Name.c_str()), NameLengthInByte);
		if (FAILED(hr))
		{
			D_SET_LAST_STATUS(hr, hr);
			D_API_ERR(0, TEXT("WriteData"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
		//////////////////////////////////////////////////////////////////////////
		hr = WriteData<LONG>(Length);
		if (FAILED(hr))
		{
			D_SET_LAST_STATUS(hr, hr);
			D_API_ERR(0, TEXT("WriteData"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
		//////////////////////////////////////////////////////////////////////////
		END_BLOCK(0);

		return hr;
	}

	HRESULT WriteEnding()
	{
		HRESULT hr = S_OK;
		for (INT ii = 0; ii < 2; ++ ii)
		{
			LONG Zero = 0;
			hr = WriteData<LONG>(Zero);
			if (FAILED(hr))
			{
				D_SET_LAST_STATUS(hr, hr);
				D_API_ERR(0, TEXT("WriteData"), hr, TEXT(""));
				break;
			}
		}
		return hr;
	}

	HRESULT WriteStructHeader(CONST wstring & Name)
	{
		//NameLengthInByte NameString ValueLengthInByte ValueBinaryData ...
		HRESULT hr = S_OK;
		BEGIN_BLOCK(0);
		//////////////////////////////////////////////////////////////////////////
		CONST LONG NegOne = -1;
		hr = WriteNameValueSize(Name, NegOne);
		if (FAILED(hr))
		{
			D_SET_LAST_STATUS(hr, hr);
			D_API_ERR(0, TEXT("WriteNameValueLength"), hr, TEXT("Name=%s, Length=%u"), Name.c_str(), NegOne);
			LEAVE_BLOCK(0);
		}

		//////////////////////////////////////////////////////////////////////////
		END_BLOCK(0);

		return hr;
	}


	static BOOL IsStrcutHeader(CONST wstring & Name, LONG ValueSize)
	{
		return (Name.size() && -1 == ValueSize);
	}

	static BOOL IsEnding(CONST wstring & Name, LONG ValueSize)
	{
		return (Name.empty() && 0 == ValueSize);
	}

	//////////////////////////////////////////////////////////////////////////
	HRESULT GetByteOrder(DATA_BYTE_ORDER & ByteOrder)
	{
		LONG lByteOrder = 0x12345678;
		HRESULT hr = ReadData<LONG>(lByteOrder);
		ByteOrder = (DATA_BYTE_ORDER)(lByteOrder);
		return hr;
	}

	//////////////////////////////////////////////////////////////////////////
	HRESULT ReadNameValueSize(wstring & Name, LONG & ValueSize)
	{
		//NameLengthInByte NameString ValueLengthInByte ValueBinaryData ...
		HRESULT hr = S_OK;
		BEGIN_BLOCK(0);
		//////////////////////////////////////////////////////////////////////////
		LONG NameLengthInByte = 0;
		hr = ReadData<LONG>(NameLengthInByte);
		if (FAILED(hr))
		{
			D_SET_LAST_STATUS(hr, hr);
			D_API_ERR(0, TEXT("WriteData"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
		//////////////////////////////////////////////////////////////////////////
		if (NameLengthInByte)
		{
			//CHeapPtr<WCHAR> apStringBuffer;
			//apStringBuffer.Allocate(NameLengthInByte / sizeof(WCHAR));
			TCHAR szName[512] = {0};
			ATLASSERT(sizeof(szName) >= NameLengthInByte);
			hr = ReadData<BYTE>((BYTE*)(szName), NameLengthInByte);
			if (FAILED(hr))
			{
				D_SET_LAST_STATUS(hr, hr);
				D_API_ERR(0, TEXT("WriteData"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}
			Name = szName;
			//apStringBuffer.Free();
		}
		else
		{
			Name.clear();
		}

		//////////////////////////////////////////////////////////////////////////
		hr = ReadData<LONG>(ValueSize);
		if (FAILED(hr))
		{
			D_SET_LAST_STATUS(hr, hr);
			D_API_ERR(0, TEXT("WriteData"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
		//////////////////////////////////////////////////////////////////////////
		END_BLOCK(0);

		return hr;
	}

	HRESULT ReadValueBinaryData(BYTE * pBuffer, LONG ValueSize)
	{
		return ReadData<BYTE>(pBuffer, ValueSize);
	}

	template <typename T>
	HRESULT ReadValueData(T & Value)
	{
		return ReadValueBinaryData((BYTE *)(&Value), sizeof(T));
	}
	
	template <typename T>
	HRESULT ReadValueData(T * pValue, LONG Count)
	{
		return ReadValueBinaryData((BYTE *)(pValue), sizeof(T) * Count);
	}

public:
	//////////////////////////////////////////////////////////////////////////
	//Read / Write basic data

	template < typename T>
	HRESULT WriteData(CONST T & Data)
	{
		ULONG NumberOfBytesWritten = 0;
		HRESULT hr = m_pIFileDevice->Write(sizeof(T), &NumberOfBytesWritten, (CONST BYTE*)(&Data));
		if (SUCCEEDED(hr) && (sizeof(T) != NumberOfBytesWritten))
		{
			hr = HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER);
		}
		return hr;
	}

	template < typename T>
	HRESULT WriteData(CONST T * pData, LONG Count)
	{
		ULONG NumberOfBytesWritten = 0;
		HRESULT hr = m_pIFileDevice->Write(sizeof(T) * Count, &NumberOfBytesWritten, (CONST BYTE*)(pData));
		if (SUCCEEDED(hr) && (sizeof(T) * Count != NumberOfBytesWritten))
		{
			hr = HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER);
		}
		return hr;
	}

	template < typename T>
	HRESULT ReadData(T & Data)
	{
		ULONG NumberOfBytesRead = 0;
		HRESULT hr = m_pIFileDevice->Read(sizeof(T), &NumberOfBytesRead, (BYTE*)(&Data));
		if (SUCCEEDED(hr) && (sizeof(T) != NumberOfBytesRead))
		{
			hr = HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER);
		}
		return hr;
	}

	template < typename T>
	HRESULT ReadData(T * pData, LONG Count)
	{
		ULONG NumberOfBytesRead = 0;
		HRESULT hr = m_pIFileDevice->Read(sizeof(T) * Count, &NumberOfBytesRead, (BYTE*)(pData));
		if (SUCCEEDED(hr) && (sizeof(T) * Count != NumberOfBytesRead))
		{
			hr = HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER);
		}
		return hr;
	}
};
