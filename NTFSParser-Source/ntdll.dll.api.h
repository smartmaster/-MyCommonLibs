#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicDTraceLib.h"

#define NTDllAPITable_GetProcAddress(hDll, proc) \
this->proc = (decltype(this->proc))(GetProcAddress(this->hDll, #proc)); \
if (nullptr == this->proc) \
{ \
	LastError = GetLastError(); \
	hr = HRESULT_FROM_WIN32(LastError); \
	D_API_ERR(0, TEXT("GetProcAddress"), hr, TEXT("%s"), TEXT(#proc)); \
}

struct NTDllAPITable
{
private:
	HMODULE m_hDll;

public:
	NTSTATUS (*RtlDecompressBuffer)(
		_In_   USHORT CompressionFormat,
		_Out_  PUCHAR UncompressedBuffer,
		_In_   ULONG UncompressedBufferSize,
		_In_   PUCHAR CompressedBuffer,
		_In_   ULONG CompressedBufferSize,
		_Out_  PULONG FinalUncompressedSize
		);
	NTSTATUS (*RtlDecompressBufferEx)(
		_In_   USHORT CompressionFormat,
		_Out_  PUCHAR UncompressedBuffer,
		_In_   ULONG UncompressedBufferSize,
		_In_   PUCHAR CompressedBuffer,
		_In_   ULONG CompressedBufferSize,
		_Out_  PULONG FinalUncompressedSize,
		_In_   PVOID WorkSpace
		);

	NTSTATUS (*RtlDecompressFragment)(
		_In_   USHORT CompressionFormat,
		_Out_  PUCHAR UncompressedFragment,
		_In_   ULONG UncompressedFragmentSize,
		_In_   PUCHAR CompressedBuffer,
		_In_   ULONG CompressedBufferSize,
		_In_   ULONG FragmentOffset,
		_Out_  PULONG FinalUncompressedSize,
		_In_   PVOID WorkSpace
		);
	
	NTSTATUS (*RtlCompressBuffer)(
		_In_   USHORT CompressionFormatAndEngine,
		_In_   PUCHAR UncompressedBuffer,
		_In_   ULONG UncompressedBufferSize,
		_Out_  PUCHAR CompressedBuffer,
		_In_   ULONG CompressedBufferSize,
		_In_   ULONG UncompressedChunkSize,
		_Out_  PULONG FinalCompressedSize,
		_In_   PVOID WorkSpace
		);

	NTSTATUS (*RtlGetCompressionWorkSpaceSize)(
		_In_   USHORT CompressionFormatAndEngine,
		_Out_  PULONG CompressBufferWorkSpaceSize,
		_Out_  PULONG CompressFragmentWorkSpaceSize
		);



public:
	//////////////////////////////////////////////////////////////////////////
	//<sonmi01>2015-6-4 ###???
	NTDllAPITable();
	~NTDllAPITable();

	HRESULT Init();
};