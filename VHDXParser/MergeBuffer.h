#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>



VOID MergeBuffer(OUT BYTE * ParentBuffer, IN CONST BYTE * ChildBuffer, IN BYTE * BitmapBuffer, IN LONGLONG BitCount, IN LONG BlockSizePerBit);

//return value: TRUE - at least one bit set; FALSE - no bit set
VOID OrBitmapBuffer(OUT BYTE * ParentBuffer, IN CONST BYTE * ChildBuffer, LONG BufferLen, BOOL & bContainOne, BOOL & bAllOne); //<sonmi01>2013-10-29 ###???
BOOL IsAllZero( CONST BYTE * Buffer, LONG BufferLen );
BOOL IsAllOne(CONST BYTE * Buffer, LONG BufferLen);
BOOL ContainsOne(CONST BYTE * Buffer, LONG BufferLen);

VOID DiffBitmapBuffer(OUT BYTE * ParentBuffer, IN CONST BYTE * ChildBuffer, LONG BufferLen, BOOL & bContainOne, BOOL & bAllOne); //<sonmi01>2013-10-29 ###???
