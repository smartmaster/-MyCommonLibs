
#include "stdafx.h"
#include "CacheIO.h"
#include "VhdxHelper.h"


CacheBuffer::CacheBuffer( LONG BlockSize ) :
	m_DataSize(0),
	m_BlockSize(BlockSize),
	m_bNeedToFlush(0),
	m_Reserved(0),
	m_Offset(-1),
	m_Buffer(NULL)
{

}

HRESULT CacheBuffer::Allocate()
{
	HRESULT hr = S_OK;
	CVMemPtr<BYTE> apBuffer;
	apBuffer.Allocate(m_BlockSize);
	if (NULL == apBuffer.m_pData)
	{
		hr = E_OUTOFMEMORY;
	}
	else
	{
		m_Buffer = apBuffer.Detach();
	}
	return hr;
}

HRESULT CacheBuffer::ResetCache()
{
	m_DataSize = 0;
	//m_BlockSize;
	m_bNeedToFlush = FALSE; //to support cache read / write at same time
	m_Reserved = 0;
	m_Offset = -1LL;
	//m_Buffer;
	return S_OK;
}

CacheBuffer::~CacheBuffer()
{
	CVMemPtr<BYTE> apBuffer;
	apBuffer.Attach(m_Buffer);
	m_Buffer = NULL;
}

CCacheIO::CCacheIO(IFileDevice * pFileDevice, BOOL bAsOwner, LONG BlockSize, BOOL bWriteNewAndSequential) :
	m_BlockSize(BlockSize),
	//m_pFileDevice(pFileDevice),
	m_bAsOwner(bAsOwner), //<sonmi01>2015-11-11 ###???
	m_HitCount(0),
	m_MissedCount(0),
	m_bFlushNeeded(FALSE),
	m_CurrentPointer(0),
	m_MaxWritePos(0),
	m_MaxReadPos(0),
	m_bWriteNewAndSequential(bWriteNewAndSequential), //<sonmi01>2015-5-19 ###???
	m_refCount(0)
{
	if (m_bAsOwner)
	{
		m_pFileDevice = pFileDevice;
	}
	else
	{
		m_pFileDevice.Attach(pFileDevice);
	}
	m_bBlockSize2N = (0 == (m_BlockSize & (m_BlockSize - 1)));
}

#if 0
CCacheIO::~CCacheIO()
{
	Flush();

	for (size_t ii = 0; ii < m_Caches.size(); ++ ii)
	{
		delete m_Caches[ii];
		m_Caches[ii] = NULL;
	}
	m_Caches.clear();

	CONST LONG DECIMAL_PLACE = 10000;
	LONG Percentage = 100 * DECIMAL_PLACE;
	if (m_HitCount || m_MissedCount)
	{
		Percentage = Percentage * m_HitCount / (m_HitCount + m_MissedCount);
		D_INFO(0, TEXT("HitCount=%I64d, MissedCount=%I64d, HitPercentage=%d.%04d%%"), m_HitCount, m_MissedCount, Percentage/DECIMAL_PLACE, Percentage%DECIMAL_PLACE);
	}
	
}
#endif

VOID CCacheIO::SortCacheBuffers()
{
	struct CacheBufferCompare
	{
		bool operator()(CONST CacheBuffer* c1, CONST CacheBuffer* c2) CONST
		{
			return c1->m_Offset < c2->m_Offset;
		}
	};

	CONST CacheBufferCompare comp;
	std::sort(m_Caches.begin(), m_Caches.end(), comp);
}

//VOID CCacheIO::ClearDataSizes()
//{
//	for (size_t ii = 0; ii < m_Caches.size(); ++ ii)
//	{
//		m_Caches[ii]->m_DataSize = 0;
//	}
//}

HRESULT CCacheIO::IncreaseCacheBuffers( LONG NewCount )
{
	ATLASSERT(m_Caches.size() < NewCount);

	HRESULT hr = S_OK;

	size_t BeginIndex = m_Caches.size();
	m_Caches.resize(NewCount);
	for (size_t ii = BeginIndex; ii < m_Caches.size(); ++ ii)
	{
		m_Caches[ii] = new CacheBuffer(m_BlockSize);
		if (NULL == m_Caches[ii])
		{
			hr = E_OUTOFMEMORY;
			D_API_ERR(0, TEXT("new"), hr, TEXT(""));
			break;
		}

		hr = m_Caches[ii]->Allocate();
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("Allocate"), hr, TEXT(""));
			break;
		}
	}
	return hr;
}

HRESULT CCacheIO::CacheReadBlocks( LONGLONG StartOffset, LONG BlockCount, BOOL bForCacheWrite, CONST vector<LONGLONG> & NotAlignedOffsetTemp )
{
	ATLASSERT(NotAlignedOffsetTemp.size() <= 2);

	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	SortCacheBuffers();

	if (m_Caches.size() < MIN_CACHE_BUFFER_COUNT)
	{
		hr = IncreaseCacheBuffers(MIN_CACHE_BUFFER_COUNT);
		if (FAILED(hr))
		{
			LEAVE_BLOCK(0);
		}
	}

	if (m_Caches.size() < BlockCount)
	{
		hr = IncreaseCacheBuffers(BlockCount);
		if (FAILED(hr))
		{
			LEAVE_BLOCK(0);
		}
		SortCacheBuffers();
	}

	//////////////////////////////////////////////////////////////////////////
	m_hitOffsetsTemp.resize(0);
	m_missedCacheBufferIndexTemp.resize(0);

	LONGLONG OffsetUpperBound = StartOffset + BlockCount * m_BlockSize;
	for (size_t ii = 0; ii < m_Caches.size(); ++ ii)
	{
		if ((m_Caches[ii]->m_Offset >= StartOffset) && (m_Caches[ii]->m_Offset < OffsetUpperBound))
		{
			m_hitOffsetsTemp.push_back(m_Caches[ii]->m_Offset);
		}
		else
		{
			m_missedCacheBufferIndexTemp.push_back(ii);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	size_t missdeCacheIndex = 0;
	LONG ReusedCacheIndex = -1;
	for (LONGLONG offset = StartOffset; offset < OffsetUpperBound; offset += m_BlockSize)
	{
		if (m_hitOffsetsTemp.end() != std::find(m_hitOffsetsTemp.begin(), m_hitOffsetsTemp.end(), offset))
		{
			//////////////////////////////////////////////////////////////////////////
			++ m_HitCount;
		}
		else
		{
			//////////////////////////////////////////////////////////////////////////
			++ m_MissedCount;

			//////////////////////////////////////////////////////////////////////////
			ReusedCacheIndex = m_missedCacheBufferIndexTemp[missdeCacheIndex];
			if (m_Caches[ReusedCacheIndex]->m_Offset >= 0 && m_Caches[ReusedCacheIndex]->m_bNeedToFlush)
			{
				ULONG NumberOfBytesWritten = 0;
				hr = WriteAtOffsetHelper(m_pFileDevice, m_Caches[ReusedCacheIndex]->m_Offset, m_BlockSize, &NumberOfBytesWritten, m_Caches[ReusedCacheIndex]->m_Buffer);
				if (FAILED(hr))
				{
					D_API_ERR(0, TEXT("WriteAtOffsetHelper"), hr, TEXT(""));
					break;
				}
				
				m_Caches[ReusedCacheIndex]->m_Offset = -1;
				m_Caches[ReusedCacheIndex]->m_bNeedToFlush = FALSE;
			}
		
			
			//////////////////////////////////////////////////////////////////////////
			m_Caches[ReusedCacheIndex]->m_Offset = offset; //update offset

			if (bForCacheWrite) //<sonmi01>2013-8-26 ###???
			{
				if (NotAlignedOffsetTemp.end() != std::find(NotAlignedOffsetTemp.begin(), NotAlignedOffsetTemp.end(), m_Caches[ReusedCacheIndex]->m_Offset))
				{
					ZeroMemory(m_Caches[ReusedCacheIndex]->m_Buffer, m_BlockSize);
					if (m_bWriteNewAndSequential) //<sonmi01>2015-5-19 ###???
					{
						m_Caches[ReusedCacheIndex]->m_DataSize = m_BlockSize; //no need to rmw
					}
					else
					{
						m_Caches[ReusedCacheIndex]->m_DataSize = 0;
						hr = ReadAtOffsetHelper(m_pFileDevice, m_Caches[ReusedCacheIndex]->m_Offset, m_BlockSize, (ULONG*)&(m_Caches[ReusedCacheIndex]->m_DataSize), m_Caches[ReusedCacheIndex]->m_Buffer);
						if (FAILED(hr))
						{
							m_Caches[ReusedCacheIndex]->m_DataSize = 0; //<sonmi01>2014-8-29 ###???
							m_Caches[ReusedCacheIndex]->m_Offset = -1;
							m_Caches[ReusedCacheIndex]->m_bNeedToFlush = FALSE;
							D_API_ERR(0, TEXT("ReadAtOffsetHelper"), hr, TEXT(""));
							break;
						}
					}
				}
			}
			else
			{
				ZeroMemory(m_Caches[ReusedCacheIndex]->m_Buffer, m_BlockSize);
				m_Caches[ReusedCacheIndex]->m_DataSize = 0;
				hr = ReadAtOffsetHelper(m_pFileDevice, m_Caches[ReusedCacheIndex]->m_Offset, m_BlockSize, (ULONG*)&(m_Caches[ReusedCacheIndex]->m_DataSize), m_Caches[ReusedCacheIndex]->m_Buffer);
				if (FAILED(hr))
				{
					m_Caches[ReusedCacheIndex]->m_DataSize = 0; //<sonmi01>2014-8-29 ###???
					m_Caches[ReusedCacheIndex]->m_Offset = -1;
					m_Caches[ReusedCacheIndex]->m_bNeedToFlush = FALSE;
					D_API_ERR(0, TEXT("ReadAtOffsetHelper"), hr, TEXT(""));
					break;
				}

				//if (NumberOfBytesRead) //for safety. in case interface implementation return zero bytes even non-zero bytes read
				//{
				//	ZeroMemory(m_Caches[idxReusedCache]->m_Buffer + NumberOfBytesRead, m_BlockSize - NumberOfBytesRead);
				//}
			}

			//////////////////////////////////////////////////////////////////////////
			++ missdeCacheIndex;
		} //end else
	} //end for
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	SortCacheBuffers();

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CCacheIO::ReadAtOffset( LONGLONG StartOffset, LONG len, BYTE * Buffer)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	LONGLONG LowerBound;
	LONGLONG UpperBound;
	if (m_bBlockSize2N)
	{
		LowerBound = AtlAlignDown(StartOffset, m_BlockSize);
		UpperBound = AtlAlignUp(StartOffset + len, m_BlockSize);
	}
	else
	{
		LowerBound = ArithmeticAlignDown(StartOffset, m_BlockSize);
		UpperBound = ArithmeticAlignUp(StartOffset + len, m_BlockSize);
	}
	LONG BlockCount = (UpperBound - LowerBound) / m_BlockSize;

	m_NotAlignedOffsetTemp.resize(0);
	hr = CacheReadBlocks(LowerBound, BlockCount, FALSE, m_NotAlignedOffsetTemp);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CacheReadBlocks"), hr, TEXT("LowerBound=%I64d, BlockCount=%d"), LowerBound, BlockCount);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	size_t Index = FindOffset(LowerBound);
	ATLASSERT(-1 != Index);
	

	LONG TotalCopy = 0;

	LONG Diff = StartOffset - LowerBound;
	if (1 == BlockCount)
	{
		CopyMemory(Buffer, m_Caches[Index]->m_Buffer + Diff, len);
		TotalCopy += len;
	}
	else
	{
		//////////////////////////////////////////////////////////////////////////
		LONG HeaderSize = m_BlockSize - Diff;
		CopyMemory(Buffer + TotalCopy, m_Caches[Index]->m_Buffer + Diff, HeaderSize);
		TotalCopy += HeaderSize;
		++ Index;

		//////////////////////////////////////////////////////////////////////////
		for (LONG ii = 0; ii < BlockCount - 2; ++ ii)
		{
			CopyMemory(Buffer + TotalCopy, m_Caches[Index]->m_Buffer, m_BlockSize);
			TotalCopy += m_BlockSize;
			++Index;
		}

		//////////////////////////////////////////////////////////////////////////
		LONG TailSize = UpperBound - (StartOffset + len);
		LONG Remain = m_BlockSize - TailSize;

		CopyMemory(Buffer + TotalCopy, m_Caches[Index]->m_Buffer, Remain);
		TotalCopy += Remain;
		++ Index;
	}

	ATLASSERT(TotalCopy == len);
	m_CurrentPointer = StartOffset + len;
	if (m_CurrentPointer > m_MaxReadPos)
	{
		m_MaxReadPos = m_CurrentPointer;
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return hr;
}

size_t CCacheIO::FindOffset( LONGLONG Offset )
{
	size_t index = -1;
	for (size_t ii = 0; ii < m_Caches.size(); ++ ii)
	{
		if (Offset == m_Caches[ii]->m_Offset)
		{
			index = ii;
			break;
		}
	}
	return index;
}

VOID CCacheIO::InitParams( IFileDevice * pFileDevice, LONG BlockSize )
{
	ATLASSERT(NULL == m_pFileDevice);
	ATLASSERT(0 == m_BlockSize);
	if (m_bAsOwner) //<sonmi01>2015-12-29 ###???
	{
		m_pFileDevice = pFileDevice;
	}
	else
	{
		m_pFileDevice.Attach(pFileDevice);
	}
	m_BlockSize = BlockSize;
	m_bBlockSize2N = (0 == (m_BlockSize & (m_BlockSize - 1)));
}

HRESULT CCacheIO::WriteAtOffset( LONGLONG StartOffset, LONG len, CONST BYTE * Buffer )
{
	//////////////////////////////////////////////////////////////////////////
	m_bFlushNeeded = TRUE; //once WRITE is called, need to flush cache

	//////////////////////////////////////////////////////////////////////////
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	m_NotAlignedOffsetTemp.resize(0); //<sonmi01>2013-8-26 ###???

	//////////////////////////////////////////////////////////////////////////

	LONGLONG LowerBound;
	LONGLONG UpperBound;
	if (m_bBlockSize2N)
	{
		LowerBound = AtlAlignDown(StartOffset, m_BlockSize);
		UpperBound = AtlAlignUp(StartOffset + len, m_BlockSize);
	}
	else
	{
		LowerBound = ArithmeticAlignDown(StartOffset, m_BlockSize);
		UpperBound = ArithmeticAlignUp(StartOffset + len, m_BlockSize);
	}

	if (LowerBound != StartOffset) //<sonmi01>2013-8-26 ###???
	{
		m_NotAlignedOffsetTemp.push_back(LowerBound);
	}
	if (UpperBound != (StartOffset + len)) //<sonmi01>2013-8-26 ###???
	{
		m_NotAlignedOffsetTemp.push_back(UpperBound - m_BlockSize);
	}

	LONG BlockCount = (UpperBound - LowerBound) / m_BlockSize;
	hr = CacheReadBlocks(LowerBound, BlockCount, TRUE, m_NotAlignedOffsetTemp); //<sonmi01>2013-8-26 ###???
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CacheReadBlocks"), hr, TEXT("LowerBound=%I64d, BlockCount=%d"), LowerBound, BlockCount);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	size_t Index = FindOffset(LowerBound);
	ATLASSERT(-1 != Index);


	LONG TotalCopy = 0;

	LONG Diff = StartOffset - LowerBound;
	if (1 == BlockCount)
	{
		m_Caches[Index]->m_bNeedToFlush = TRUE;
		CopyMemory(m_Caches[Index]->m_Buffer + Diff, Buffer, len);

		TotalCopy += len;
	}
	else
	{
		//////////////////////////////////////////////////////////////////////////
		LONG HeaderSize = m_BlockSize - Diff;

		m_Caches[Index]->m_bNeedToFlush = TRUE;
		CopyMemory(m_Caches[Index]->m_Buffer + Diff, Buffer + TotalCopy, HeaderSize);

		TotalCopy += HeaderSize;
		++ Index;

		//////////////////////////////////////////////////////////////////////////
		for (LONG ii = 0; ii < BlockCount - 2; ++ ii)
		{
			m_Caches[Index]->m_bNeedToFlush = TRUE;
			CopyMemory(m_Caches[Index]->m_Buffer, Buffer + TotalCopy, m_BlockSize);

			TotalCopy += m_BlockSize;
			++Index;
		}

		//////////////////////////////////////////////////////////////////////////
		LONG TailSize = UpperBound - (StartOffset + len);
		LONG Remain = m_BlockSize - TailSize;

		m_Caches[Index]->m_bNeedToFlush = TRUE;
		CopyMemory(m_Caches[Index]->m_Buffer, Buffer + TotalCopy, Remain);

		TotalCopy += Remain;
		++ Index;
	}

	ATLASSERT(TotalCopy == len);
	m_CurrentPointer = StartOffset + len;
	if (m_CurrentPointer > m_MaxWritePos)
	{
		m_MaxWritePos = m_CurrentPointer;
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CCacheIO::Flush()
{
	if (!m_bFlushNeeded)
	{
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	//////////////////////////////////////////////////////////////////////////
	for (size_t ii = 0; ii < m_Caches.size(); ++ ii)
	{
		if (m_Caches[ii]->m_Offset >= 0 && m_Caches[ii]->m_bNeedToFlush)
		{
			ULONG NumberOfBytesWritten = 0;
			hr = WriteAtOffsetHelper(m_pFileDevice, m_Caches[ii]->m_Offset, m_BlockSize, &NumberOfBytesWritten, m_Caches[ii]->m_Buffer);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteAtOffsetHelper"), hr, TEXT(""));
				break;
			}
			m_Caches[ii]->m_bNeedToFlush = FALSE;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	if (SUCCEEDED(hr))
	{
		m_bFlushNeeded = FALSE;
	}
	//////////////////////////////////////////////////////////////////////////

	return hr;
}

HRESULT CCacheIO::SetPointerEx(LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	if (FILE_BEGIN != ulMoveMethod)
	{
		hr = E_INVALIDARG;
		D_API_ERR(0, TEXT("SetPointerEx"), hr, TEXT("must be FILE_BEGIN"));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	hr = Seek(llDistanceToMove);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("Seek"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}


	*lpNewFilePointer = m_CurrentPointer;

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CCacheIO::CreateMember()
{
	return S_OK;
}

HRESULT CCacheIO::DestroyMember()
{
	if (m_Caches.size() > 0)
	{
		Flush();

		for (size_t ii = 0; ii < m_Caches.size(); ++ii)
		{
			delete m_Caches[ii];
			m_Caches[ii] = NULL;
		}
		m_Caches.clear();

		CONST LONG DECIMAL_PLACE = 10000;
		LONG Percentage = 100 * DECIMAL_PLACE;
		if (m_HitCount || m_MissedCount)
		{
			Percentage = Percentage * m_HitCount / (m_HitCount + m_MissedCount);
			D_INFO(0, TEXT("HitCount=%I64d, MissedCount=%I64d, HitPercentage=%d.%04d%%"), m_HitCount, m_MissedCount, Percentage / DECIMAL_PLACE, Percentage%DECIMAL_PLACE);
		}
	}

	if (m_bAsOwner)
	{
		m_pFileDevice.Release();
	}
	else
	{
		m_pFileDevice.Detach();
	}

	return S_OK;
}

HRESULT CCacheIO::Seek(LONGLONG FilePointer)
{
	m_CurrentPointer = FilePointer;
	return S_OK;
}

HRESULT CCacheIO::Read( LONG len, BYTE * Buffer )
{
	return ReadAtOffset(m_CurrentPointer, len, Buffer);
}

HRESULT CCacheIO::Read(ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	hr = Read(nNumberOfBytesToRead, pBuffer);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("Read"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	*lpNumberOfBytesRead = nNumberOfBytesToRead;
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CCacheIO::Write(LONG len, CONST BYTE * Buffer)
{
	return WriteAtOffset(m_CurrentPointer, len, Buffer);
}

HRESULT CCacheIO::Write(ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	hr = Write(nNumberOfBytesToWrite, pBuffer);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("Write"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	*lpNumberOfBytesWritten = nNumberOfBytesToWrite;
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CCacheIO::GetFileSize(LONGLONG * pFileSize)
{
	return m_pFileDevice->GetFileSize(pFileSize);
}

HANDLE CCacheIO::GetRawHandle()
{
	return m_pFileDevice->GetRawHandle();
}

LONGLONG CCacheIO::GetFileSizeWrite()
{
	return m_MaxWritePos;
}

LONGLONG CCacheIO::GetFileSizeRead()
{
	return m_MaxReadPos;
}

CBulkWrite::CBulkWrite(IFileDevice * pFileDevice, BOOL bAsOwner, LONG bufferSize) :
//m_pFileDevice(pFileDevice),
m_bAsOwner(bAsOwner),
m_cache(bufferSize),
m_refCount(0)
{
	if (bAsOwner)
	{
		m_pFileDevice = pFileDevice;
	}
	else
	{
		m_pFileDevice.Attach(pFileDevice);
	}

}

HRESULT CBulkWrite::WriteAtOffset(LONGLONG StartOffset, LONG len, CONST BYTE * Buffer)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	if (nullptr == m_cache.m_Buffer)
	{
		hr = m_cache.Allocate();
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			// LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("Allocate"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	if ((StartOffset != m_cache.m_Offset + m_cache.m_DataSize) ||
		(-1LL == m_cache.m_Offset || 0 == m_cache.m_DataSize ) 
		) //offset skipped
	{
		hr = WriteAndUpdateCache(StartOffset, len, Buffer);
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("CacheWrite"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
	}
	else
	{
		//////////////////////////////////////////////////////////////////////////
		if (len <= m_cache.m_BlockSize - m_cache.m_DataSize)
		{
			//////////////////////////////////////////////////////////////////////////
			CopyMemory(m_cache.m_Buffer + m_cache.m_DataSize, Buffer, len);
			m_cache.m_DataSize += len;
			m_cache.m_bNeedToFlush = TRUE;
		}
		else
		{
			//////////////////////////////////////////////////////////////////////////
			LONG copyLen = m_cache.m_BlockSize - m_cache.m_DataSize;
			CopyMemory(m_cache.m_Buffer + m_cache.m_DataSize, Buffer, copyLen);
			m_cache.m_DataSize += copyLen;
			m_cache.m_bNeedToFlush = TRUE;
			hr = WriteAndUpdateCache(StartOffset + copyLen, len - copyLen, Buffer + copyLen);
			if (FAILED(hr))
			{
				//LastError = WSAGetLastError();
				//LastError = GetLastError();
				//hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("CacheWrite"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}
		}
	}

	m_CurrentPointer = StartOffset + len;
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CBulkWrite::Flush()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	if (m_cache.m_bNeedToFlush  && m_cache.m_Offset >= 0 && m_cache.m_DataSize > 0)
	{
		ULONG NumberOfBytesWritten = 0;
		hr = WriteAtOffsetHelper(m_pFileDevice, m_cache.m_Offset, m_cache.m_DataSize, &NumberOfBytesWritten, m_cache.m_Buffer);
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("WriteAtOffsetHelper"), hr, TEXT(""));
			//LEAVE_BLOCK(0);
		}
		else
		{
			m_cache.ResetCache();
		}
	}

	return hr;
}

HRESULT CBulkWrite::SetPointerEx(LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	if (FILE_BEGIN != ulMoveMethod)
	{
		hr = E_INVALIDARG;
		D_API_ERR(0, TEXT("SetPointerEx"), hr, TEXT("must be FILE_BEGIN"));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	hr = Seek(llDistanceToMove);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("Seek"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}


	*lpNewFilePointer = m_CurrentPointer;

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

#if 0
CBulkWrite::~CBulkWrite()
{
	Flush();
}
#endif

HRESULT CBulkWrite::WriteAndUpdateCache(CONST LONGLONG StartOffset, CONST LONG len, CONST BYTE * Buffer)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	ULONG NumberOfBytesWritten = 0;
	//////////////////////////////////////////////////////////////////////////
	if (m_cache.m_Offset >= 0 && m_cache.m_DataSize > 0)
	{
		NumberOfBytesWritten = 0;
		hr = WriteAtOffsetHelper(m_pFileDevice, m_cache.m_Offset, m_cache.m_DataSize, &NumberOfBytesWritten, m_cache.m_Buffer);
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("WriteAtOffsetHelper"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
		m_cache.ResetCache();
	}

	//////////////////////////////////////////////////////////////////////////
	LONG CurrentWriteLen = AtlAlignDown(len, m_cache.m_BlockSize);
	if (CurrentWriteLen > 0)
	{
		NumberOfBytesWritten = 0;
		hr = WriteAtOffsetHelper(m_pFileDevice, StartOffset, CurrentWriteLen, &NumberOfBytesWritten, Buffer);
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("WriteAtOffsetHelper"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	LONG remainLen = len - CurrentWriteLen;
	if (remainLen > 0)
	{
		m_cache.m_DataSize = remainLen;
		//m_cache.m_BlockSize;
		m_cache.m_bNeedToFlush = TRUE; //to support cache read / write at same time
		//m_Reserved = 0;
		m_cache.m_Offset = StartOffset + CurrentWriteLen;
		CopyMemory(m_cache.m_Buffer, Buffer + CurrentWriteLen, remainLen);
	}
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CBulkWrite::CreateMember()
{
	return S_OK;
}

HRESULT CBulkWrite::DestroyMember()
{
	HRESULT hr = Flush();
	if (m_bAsOwner)
	{
		m_pFileDevice.Release();
	}
	else
	{
		m_pFileDevice.Detach();
	}
	return hr;
}

HRESULT CBulkWrite::Seek(LONGLONG FilePointer)
{
	m_CurrentPointer = FilePointer;
	return S_OK;
}

HRESULT CBulkWrite::Read(LONG len, BYTE * Buffer)
{
	throw E_NOTIMPL;
	return E_NOTIMPL;
}

HRESULT CBulkWrite::Read(ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	hr = Read(nNumberOfBytesToRead, pBuffer);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("Read"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	*lpNumberOfBytesRead = nNumberOfBytesToRead;
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CBulkWrite::Write(LONG len, CONST BYTE * Buffer)
{
	return WriteAtOffset(m_CurrentPointer, len, Buffer);
}

HRESULT CBulkWrite::Write(ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	hr = Write(nNumberOfBytesToWrite, pBuffer);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("Write"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	*lpNumberOfBytesWritten = nNumberOfBytesToWrite;
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CBulkWrite::GetFileSize(LONGLONG * pFileSize)
{
	return m_pFileDevice->GetFileSize(pFileSize);
}

HANDLE CBulkWrite::GetRawHandle()
{
	return m_pFileDevice->GetRawHandle();
}

HRESULT CBulkWrite::ReadAtOffset(LONGLONG StartOffset, LONG len, BYTE * Buffer)
{
	throw E_NOTIMPL;
	return E_NOTIMPL;
}

HRESULT CreateInstanceCCacheIO(IFileDevice * pFileDevice, BOOL bAsOwner, LONG BlockSize, BOOL bWriteNewAndSequential, OUT ICCacheIO ** ppobj, BOOL bCreateMember)
{
	return CREATE_INSTCANCE_COBJECT(new CCacheIO(pFileDevice, bAsOwner, BlockSize, bWriteNewAndSequential), ppobj, bCreateMember);
}

HRESULT CreateInstanceCBulkWrite(IFileDevice * pFileDevice, BOOL bAsOwner, LONG bufferSize, OUT ICCacheIO ** ppobj, BOOL bCreateMember)
{
	return CREATE_INSTCANCE_COBJECT(new CBulkWrite(pFileDevice, bAsOwner, bufferSize), ppobj, bCreateMember);
}
