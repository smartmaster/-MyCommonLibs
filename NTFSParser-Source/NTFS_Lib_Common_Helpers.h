#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

template<typename DEST_TYPE, typename SRC_TYPE>
VOID CopyAtByteOffset(DEST_TYPE * dest, CONST SRC_TYPE * src, LONG Offset, LONG Len)
{
	CopyMemory((BYTE *)dest, (CONST BYTE*)src + Offset, Len);
}

template<typename DEST_TYPE, typename SRC_TYPE>
DEST_TYPE * GetAtByteOffset(DEST_TYPE * dest, SRC_TYPE * src, LONG Offset)
{
	return (DEST_TYPE *)((BYTE*)src + Offset);
}
