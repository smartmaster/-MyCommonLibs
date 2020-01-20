#pragma once


#include <tchar.h>
#include <windows.h>
#include <atlbase.h>

#include "ParserSeparateVHDX.h"
#include "MergeBuffer.h"


class  ParserChainVHDXBase
{
public:
	//0 - the youngest child, 1 - child, ... , (n-1) - the oldest parent
	vector<ParserSeparateVHDX*> m_Chain; /***INTERNAL TO CLEANUP***/
	LONG m_MaxBlockSize; //VHDX chain can have different block size for separate VHDX
	LONG m_MaxBitmapSize; 
	LONG m_MaxBlockSizeVhdxIndex;
	CVMemPtr<BYTE> m_HelperBufferData;
	CVMemPtr<BYTE> m_HelperBufferBitmap;
	CVMemPtr<BYTE> m_HelperBufferBitmapChild; //<sonmi01>2013-10-29 ###???
	//CVMemPtr<BYTE> m_HelperBufferBitmapDiff; //<sonmi01>2013-10-29 ###???

	LONG m_ReadStartIndex; //to support consecutive child VHDX read
	LONG m_ReadCount;

public:
	ParserChainVHDXBase();

	virtual ~ParserChainVHDXBase();

public:
	HRESULT FindMaxBlockSizeAndAllocateBuffer(); //be the first call 
	VOID SetReadRange(LONG VDiskStartIndex, LONG VDiskCount);
	HRESULT ReadBlocK(IN BOOL bFillZero, IN LONG BlockIndex, OUT BYTE * pBuffer, IN LONG BufferLen);
	HRESULT ReadBlocKBitmap(LONG BlockIndex, IN OUT LONG Len, BYTE * Buffer, BOOL & bContainOne, BOOL & bAllOne); //<sonmi01>2013-10-29 ###???

	

	HRESULT WriteBlocK(IN LONG BlockIndex, CONST BYTE * pBuffer, IN LONG BufferLen);
	HRESULT UpdateBAT();
	

private:
	
	HRESULT SeparateVhdxReadMaxBlocK(ParserSeparateVHDX * pSeparateVHDX, IN BOOL bFillZero, IN LONG BlockIndex, OUT BYTE * pBuffer, IN LONG BufferLen);
	HRESULT SeparateVhdxReadMaxBitmap(ParserSeparateVHDX * pSeparateVHDX, LONG BlockIndex, IN LONG Len, BYTE * Buffer);
	BOOL SeparateVhdxNeedToInspectParentBlock(ParserSeparateVHDX * pSeparateVHDX, LONG BlockIndex);
	LONG FindEndIndex(LONG BlockIndex);

	HRESULT SeparateVhdxWriteMaxBlocK( ParserSeparateVHDX * pSeparateVHDX, IN LONG BlockIndex, CONST BYTE * pBuffer, IN LONG BufferLen );
};