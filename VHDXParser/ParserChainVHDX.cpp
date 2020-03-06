#include "stdafx.h"
#include "ParserChainVHDX.h"




HRESULT ParserChainVHDXBase::FindMaxBlockSizeAndAllocateBuffer()
{
	ATLASSERT(NULL == m_HelperBufferData.m_pData && NULL == m_HelperBufferBitmap.m_pData);
	ATLASSERT(m_Chain.size());

	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	m_MaxBlockSize = 0;
	LONG MinBlockSize = 1024 * 1024 * 1024;
	for (size_t ii = 0; ii < m_Chain.size(); ++ ii)
	{
		if (m_MaxBlockSize < m_Chain[ii]->m_FileParameters.m_BlockSize)
		{
			m_MaxBlockSize = m_Chain[ii]->m_FileParameters.m_BlockSize;
			m_MaxBlockSizeVhdxIndex = ii;
		}

		if (MinBlockSize > m_Chain[ii]->m_FileParameters.m_BlockSize)
		{
			MinBlockSize = m_Chain[ii]->m_FileParameters.m_BlockSize;
		}
	}
	D_INFO(0, TEXT("MaxBlockSize=%d, MinBlockSize=%d"), m_MaxBlockSize, MinBlockSize);

	//////////////////////////////////////////////////////////////////////////
	m_MaxBitmapSize = m_MaxBlockSize / m_Chain[0]->m_LogicalSectorSize.m_LogicalSectorSize / 8;
	
	//////////////////////////////////////////////////////////////////////////
	m_ReadStartIndex = 0;
	m_ReadCount = m_Chain.size();

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT ParserChainVHDXBase::SeparateVhdxReadMaxBlocK( ParserSeparateVHDX * pSeparateVHDX, IN BOOL bFillZero, IN LONG BlockIndex, OUT BYTE * pBuffer, IN LONG BufferLen )
{
	ATLASSERT(BlockIndex < m_Chain[m_MaxBlockSizeVhdxIndex]->m_DataBlocksCount);
	ATLASSERT(BufferLen >= m_Chain[m_MaxBlockSizeVhdxIndex]->m_FileParameters.m_BlockSize);

	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	LONG CurrentBlockSize = pSeparateVHDX->m_FileParameters.m_BlockSize;
	ATLASSERT(0 == m_MaxBlockSize % CurrentBlockSize);

	LONG Scalar = m_MaxBlockSize / CurrentBlockSize;
	LONG CurrentBlockIndex = BlockIndex * Scalar;

	for (LONG ii = 0; ii < Scalar; ++ ii)
	{
		hr = pSeparateVHDX->ReadBlocK(bFillZero, CurrentBlockIndex, pBuffer, CurrentBlockSize);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("ReadBlocK"), hr, TEXT(""));
			break;
		}

		++ CurrentBlockIndex;
		pBuffer += CurrentBlockSize;
	}
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT ParserChainVHDXBase::SeparateVhdxReadMaxBitmap( ParserSeparateVHDX * pSeparateVHDX, LONG BlockIndex, IN LONG Len, BYTE * Buffer )
{
	ATLASSERT(BlockIndex < m_Chain[m_MaxBlockSizeVhdxIndex]->m_DataBlocksCount);

	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	LONG CurrentBlockBitmapBufferSize = pSeparateVHDX->m_BlockBitmapBufferSize;
	ATLASSERT(0 == m_Chain[m_MaxBlockSizeVhdxIndex]->m_BlockBitmapBufferSize % CurrentBlockBitmapBufferSize);

	LONG Scalar = m_Chain[m_MaxBlockSizeVhdxIndex]->m_BlockBitmapBufferSize / CurrentBlockBitmapBufferSize;
	ATLASSERT(Len >= CurrentBlockBitmapBufferSize * Scalar);

	LONG CurrentBlockIndex = BlockIndex * Scalar;

	LONG CurrentLen = 0;
	for (LONG ii = 0; ii < Scalar; ++ ii)
	{
		CurrentLen = CurrentBlockBitmapBufferSize;
		hr = pSeparateVHDX->ReadBlocKBitmap(CurrentBlockIndex, CurrentLen, Buffer);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("ReadBlocKBitmap"), hr, TEXT(""));
			break;
		}

		++ CurrentBlockIndex;
		Buffer += CurrentBlockBitmapBufferSize;
	}
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

#if 1
HRESULT ParserChainVHDXBase::ReadBlocK(IN BOOL bFillZero, IN LONG BlockIndex, OUT BYTE * pBuffer, IN LONG BufferLen) //<sonmi01>2013-10-29 ###??? //data from parent to child
{
	//where is the bug???
	ATLASSERT(BlockIndex < m_Chain[m_MaxBlockSizeVhdxIndex]->m_DataBlocksCount);
	ATLASSERT(BufferLen >= m_MaxBlockSize);

	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	if (0 == m_ReadCount)
	{
		hr = E_INVALIDARG;
		D_API_ERR(0, TEXT("ReadCount"), hr, TEXT("ReadCount ZERO"));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	hr = SeparateVhdxReadMaxBlocK(m_Chain[m_ReadStartIndex], bFillZero, BlockIndex, pBuffer, m_MaxBlockSize);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("SeparateVhdxReadBlocKs"), hr, TEXT("Index=%d"), m_ReadStartIndex);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	BOOL bContainOne = FALSE;
	BOOL bAllOne = FALSE;

	//<sonmi01>2014-1-14 ###???
	LONG MaxIndex = FindEndIndex(BlockIndex); //Index have to be signed integer
	LONG CurrentReadCount = MaxIndex - m_ReadStartIndex + 1;

	if (CurrentReadCount > 1)
	{
		if (NULL == m_HelperBufferBitmapChild.m_pData)
		{
			m_HelperBufferBitmapChild.Allocate(m_MaxBitmapSize);
			if (NULL == m_HelperBufferBitmapChild.m_pData)
			{
				hr = E_OUTOFMEMORY;
				D_API_ERR(0, TEXT("Allocate"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}
		}

		hr = SeparateVhdxReadMaxBitmap(m_Chain[m_ReadStartIndex], BlockIndex, m_MaxBitmapSize, m_HelperBufferBitmapChild.m_pData);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("SeparateVhdxReadBitmaps"), hr, TEXT("Index=%d"), m_ReadStartIndex);
			LEAVE_BLOCK(0);
		}
		bAllOne = IsAllOne(m_HelperBufferBitmapChild.m_pData, m_MaxBitmapSize);
	}

	//////////////////////////////////////////////////////////////////////////
	LONG LogicalSectorSize = m_Chain[0]->m_LogicalSectorSize.m_LogicalSectorSize;

	for (LONG ii = 1; ii < CurrentReadCount; ++ ii)
	{
		//////////////////////////////////////////////////////////////////////////
		if (bAllOne /*IsAllOne(m_HelperBufferBitmapChild.m_pData, m_MaxBitmapSize)*/)
		{
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		if (NULL == m_HelperBufferData.m_pData)
		{
			m_HelperBufferData.Allocate(m_MaxBlockSize);
			if (NULL == m_HelperBufferData.m_pData)
			{
				hr = E_OUTOFMEMORY;
				D_API_ERR(0, TEXT("Allocate"), hr, TEXT(""));
				break;
			}
		}

		if (NULL == m_HelperBufferBitmap.m_pData)
		{
			m_HelperBufferBitmap.Allocate(m_MaxBitmapSize);
			if (NULL == m_HelperBufferBitmap.m_pData)
			{
				hr = E_OUTOFMEMORY;
				D_API_ERR(0, TEXT("Allocate"), hr, TEXT(""));
				break;
			}
		}


		//////////////////////////////////////////////////////////////////////////
		hr = SeparateVhdxReadMaxBlocK(m_Chain[m_ReadStartIndex + ii], bFillZero, BlockIndex, m_HelperBufferData.m_pData, m_MaxBlockSize);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("SeparateVhdxReadBlocKs"), hr, TEXT("Index=%d"), ii);
			break;
		}

		hr = SeparateVhdxReadMaxBitmap(m_Chain[m_ReadStartIndex + ii], BlockIndex, m_MaxBitmapSize, m_HelperBufferBitmap.m_pData);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("SeparateVhdxReadBitmaps"), hr, TEXT("Index=%d"), ii);
			break;
		}

		//////////////////////////////////////////////////////////////////////////
#if 0
		//if (NULL == m_HelperBufferBitmapDiff.m_pData)
		//{
		//	m_HelperBufferBitmapDiff.Allocate(m_MaxBitmapSize);
		//	if (NULL == m_HelperBufferBitmapDiff.m_pData)
		//	{
		//		hr = E_OUTOFMEMORY;
		//		D_API_ERR(0, TEXT("Allocate"), hr, TEXT(""));
		//		break;
		//	}
		//}

		//CopyMemory(m_HelperBufferBitmapDiff.m_pData, m_HelperBufferBitmap.m_pData, m_MaxBitmapSize);
		//DiffBitmapBuffer(m_HelperBufferBitmapDiff.m_pData, m_HelperBufferBitmapChild.m_pData, m_MaxBitmapSize, bContainOne, bAllOne);
		//MergeBuffer(pBuffer, m_HelperBufferData.m_pData, m_HelperBufferBitmapDiff.m_pData, m_MaxBitmapSize * 8LL, LogicalSectorSize);
		//OrBitmapBuffer(m_HelperBufferBitmapChild.m_pData, m_HelperBufferBitmapDiff.m_pData, m_MaxBitmapSize, bContainOne, bAllOne);
#else
		DiffBitmapBuffer(m_HelperBufferBitmap.m_pData, m_HelperBufferBitmapChild.m_pData, m_MaxBitmapSize, bContainOne, bAllOne);
		MergeBuffer(pBuffer, m_HelperBufferData.m_pData, m_HelperBufferBitmap.m_pData, m_MaxBitmapSize * 8LL, LogicalSectorSize);
		OrBitmapBuffer(m_HelperBufferBitmapChild.m_pData, m_HelperBufferBitmap.m_pData, m_MaxBitmapSize, bContainOne, bAllOne);
#endif
	}
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}
	
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return hr;
}
#else
HRESULT ParserChainVHDXBase::ReadBlocK( IN BOOL bFillZero, IN LONG BlockIndex, OUT BYTE * pBuffer, IN LONG BufferLen )  //data from child to parent
{
	ATLASSERT(BlockIndex < m_Chain[m_MaxBlockSizeVhdxIndex]->m_DataBlocksCount);
	ATLASSERT(BufferLen >= m_MaxBlockSize);

	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	if (0 == m_ReadCount)
	{
		hr = E_INVALIDARG;
		D_API_ERR(0, TEXT("ReadCount"), hr, TEXT("ReadCount ZERO"));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	//LONG UpperBound = m_ReadStartIndex + m_ReadCount;
	LONG MaxIndex = FindEndIndex(BlockIndex); //Index have to be signed integer
	hr = SeparateVhdxReadMaxBlocK(m_Chain[MaxIndex], bFillZero, BlockIndex, pBuffer, m_MaxBlockSize);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("SeparateVhdxReadBlocKs"), hr, TEXT("Index=%d"), MaxIndex);
		LEAVE_BLOCK(0);
	}

	LONG LogicalSectorSize = m_Chain[0]->m_LogicalSectorSize.m_LogicalSectorSize;

	//////////////////////////////////////////////////////////////////////////
	for (LONG ii = MaxIndex - 1; ii >= m_ReadStartIndex; --ii)
	{
		if (NULL == m_HelperBufferData.m_pData)
		{
			m_HelperBufferData.Allocate(m_MaxBlockSize);
			if (NULL == m_HelperBufferData.m_pData)
			{
				hr = E_OUTOFMEMORY;
				D_API_ERR(0, TEXT("Allocate"), hr, TEXT(""));
				break;
			}
		}

		if (NULL == m_HelperBufferBitmap.m_pData)
		{
			m_HelperBufferBitmap.Allocate(m_MaxBitmapSize);
			if (NULL == m_HelperBufferBitmap.m_pData)
			{
				hr = E_OUTOFMEMORY;
				D_API_ERR(0, TEXT("Allocate"), hr, TEXT(""));
				break;
			}
		}


		hr = SeparateVhdxReadMaxBlocK(m_Chain[ii], bFillZero, BlockIndex, m_HelperBufferData.m_pData, m_MaxBlockSize);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("SeparateVhdxReadBlocKs"), hr, TEXT("Index=%d"), ii);
			LEAVE_BLOCK(0);
		}

		hr = SeparateVhdxReadMaxBitmap(m_Chain[ii], BlockIndex, m_MaxBitmapSize, m_HelperBufferBitmap.m_pData);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("SeparateVhdxReadBitmaps"), hr, TEXT("Index=%d"), ii);
			LEAVE_BLOCK(0);
		}

		MergeBuffer(pBuffer, m_HelperBufferData.m_pData, m_HelperBufferBitmap.m_pData, m_MaxBitmapSize * 8LL, LogicalSectorSize);
	}
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}
#endif


HRESULT ParserChainVHDXBase::ReadBlocKBitmap(LONG BlockIndex, IN LONG Len, BYTE * Buffer, BOOL & bContainOne, BOOL & bAllOne) //<sonmi01>2013-10-29 ###???
{
	ATLASSERT(BlockIndex < m_Chain[m_MaxBlockSizeVhdxIndex]->m_DataBlocksCount);
	ATLASSERT(Len >= m_MaxBitmapSize);

	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	if (0 == m_ReadCount)
	{
		hr = E_INVALIDARG;
		D_API_ERR(0, TEXT("ReadCount"), hr, TEXT("ReadCount ZERO"));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	//LONG UpperBound = m_ReadStartIndex + m_ReadCount;
	LONG MaxIndex = FindEndIndex( BlockIndex ); //Index have to be signed integer
	hr = SeparateVhdxReadMaxBitmap(m_Chain[MaxIndex], BlockIndex, m_MaxBitmapSize, Buffer);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("SeparateVhdxReadMaxBitmap"), hr, TEXT("Index=%d"),MaxIndex);
		LEAVE_BLOCK(0);
	}
	bContainOne = ContainsOne(Buffer, m_MaxBitmapSize);
	bAllOne = IsAllOne(Buffer, m_MaxBitmapSize); //<sonmi01>2013-10-29 ###???

	//////////////////////////////////////////////////////////////////////////
	for (LONG ii = MaxIndex - 1; ii >= m_ReadStartIndex; -- ii)
	{
		if (NULL == m_HelperBufferBitmap.m_pData)
		{
			m_HelperBufferBitmap.Allocate(m_MaxBitmapSize);
			if (NULL == m_HelperBufferBitmap.m_pData)
			{
				hr = E_OUTOFMEMORY;
				D_API_ERR(0, TEXT("Allocate"), hr, TEXT(""));
				break;
			}
		}

		hr = SeparateVhdxReadMaxBitmap(m_Chain[ii], BlockIndex, m_MaxBitmapSize, m_HelperBufferBitmap.m_pData);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("SeparateVhdxReadBlocKs"), hr, TEXT("Index=%d"),ii);
			LEAVE_BLOCK(0);
		}

		//BOOL bAllOne = FALSE; //<sonmi01>2013-10-29 ###??? //<sonmi01>2013-9-2 ###??? 
		OrBitmapBuffer(Buffer, m_HelperBufferBitmap.m_pData, m_MaxBitmapSize, bContainOne, bAllOne);
		if (bAllOne)
		{
			ATLASSERT(bContainOne);
			break;
		}
	}
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

ParserChainVHDXBase::ParserChainVHDXBase() :
	m_MaxBlockSize(0),
	m_MaxBitmapSize(0),
	m_MaxBlockSizeVhdxIndex(-1),
	m_ReadStartIndex(0),
	m_ReadCount(0)
{

}

ParserChainVHDXBase::~ParserChainVHDXBase()
{
	for (size_t ii = 0; ii < m_Chain.size(); ++ ii)
	{
		if (m_Chain[ii])
		{
			delete m_Chain[ii];
			m_Chain[ii] = NULL;
		}
	}
	m_Chain.clear();
}

VOID ParserChainVHDXBase::SetReadRange( LONG VDiskStartIndex, LONG VDiskCount )
{
	//////////////////////////////////////////////////////////////////////////
	m_ReadStartIndex = VDiskStartIndex;
	if (m_ReadStartIndex < 0)
	{
		m_ReadStartIndex = 0;
	}
	if (m_ReadStartIndex > m_Chain.size())
	{
		m_ReadStartIndex = m_Chain.size();
	}

	//////////////////////////////////////////////////////////////////////////
	m_ReadCount = VDiskCount;
	if (m_ReadCount < 0 || m_ReadCount > m_Chain.size())
	{
		m_ReadCount = m_Chain.size();
	}

	if (m_ReadStartIndex + m_ReadCount > m_Chain.size())
	{
		m_ReadCount = m_Chain.size() - m_ReadStartIndex;
	}
}

BOOL ParserChainVHDXBase::SeparateVhdxNeedToInspectParentBlock( ParserSeparateVHDX * pSeparateVHDX, LONG BlockIndex )
{
	ATLASSERT(BlockIndex < m_Chain[m_MaxBlockSizeVhdxIndex]->m_DataBlocksCount);

	//////////////////////////////////////////////////////////////////////////
	LONG CurrentBlockSize = pSeparateVHDX->m_FileParameters.m_BlockSize;
	ATLASSERT(0 == m_MaxBlockSize % CurrentBlockSize);

	LONG Scalar = m_MaxBlockSize / CurrentBlockSize;
	LONG CurrentBlockIndex = BlockIndex * Scalar;

	BOOL NeedToInspectParentBlock = FALSE;
	for (LONG ii = 0; ii < Scalar; ++ ii)
	{
		NeedToInspectParentBlock = pSeparateVHDX->NeedToInspectParentBlock(CurrentBlockIndex);
		if (NeedToInspectParentBlock)
		{
			break;
		}

		++ CurrentBlockIndex;
	}

	return NeedToInspectParentBlock;
}

LONG ParserChainVHDXBase::FindEndIndex( LONG BlockIndex )
{
	LONG EndIndex = m_ReadStartIndex + m_ReadCount - 1;
	for (LONG ii = m_ReadStartIndex; ii < m_ReadStartIndex + m_ReadCount; ++ ii)
	{
		if (!SeparateVhdxNeedToInspectParentBlock(m_Chain[ii], BlockIndex))
		{
			EndIndex = ii;
			break;
		}
	}

	return EndIndex;
}

HRESULT ParserChainVHDXBase::SeparateVhdxWriteMaxBlocK( ParserSeparateVHDX * pSeparateVHDX, IN LONG BlockIndex, CONST BYTE * pBuffer, IN LONG BufferLen )
{
	ATLASSERT(BlockIndex < m_Chain[m_MaxBlockSizeVhdxIndex]->m_DataBlocksCount);
	ATLASSERT(BufferLen >= m_Chain[m_MaxBlockSizeVhdxIndex]->m_FileParameters.m_BlockSize);

	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	LONG CurrentBlockSize = pSeparateVHDX->m_FileParameters.m_BlockSize;
	ATLASSERT(0 == m_MaxBlockSize % CurrentBlockSize);

	LONG Scalar = m_MaxBlockSize / CurrentBlockSize;
	LONG CurrentBlockIndex = BlockIndex * Scalar;

	for (LONG ii = 0; ii < Scalar; ++ ii)
	{
		hr = pSeparateVHDX->WriteBlocK(CurrentBlockIndex, pBuffer, CurrentBlockSize);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("WriteBlocK"), hr, TEXT(""));
			break;
		}

		++ CurrentBlockIndex;
		pBuffer += CurrentBlockSize;
	}
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT ParserChainVHDXBase::WriteBlocK( IN LONG BlockIndex, CONST BYTE * pBuffer, IN LONG BufferLen )
{
	return SeparateVhdxWriteMaxBlocK( m_Chain[0], BlockIndex, pBuffer, BufferLen ); //write can only happen on the top child disk
}

HRESULT ParserChainVHDXBase::UpdateBAT()
{
	return m_Chain[0]->UpdateBAT();
}
