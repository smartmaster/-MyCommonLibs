#include "StdAfx.h"
#include "VHDDisk.h"

CVHDDisk::CVHDDisk(void)
: m_pParent(NULL)
, m_pInnerFile(NULL)
{
}

CVHDDisk::~CVHDDisk(void)
{
}


void
CVHDDisk::Release()
{
    delete this;
};

IVHDDisk* 
CVHDDisk::GetParentDisk() const
{
    return m_pParent;
}

int     
CVHDDisk::QueryRange(const LOCAL_ALLOCATED_RANGE* pRangeToQuery, LOCAL_ALLOCATED_RANGE* pRange, int* pnRangeCnt)
{
    return m_pInnerFile->QueryRange(pRangeToQuery, pRange, pnRangeCnt);
}

//
//Read the LocalAllocatedData
//return 0 success, others fail
//if return 0 and returned pRange->len == 0 && pRange->offset == -1, have read to the end of file.
//@pllStartOffset - the logical offset from where to start read, if NULL, read from current file's pointer
//@pRange - return the buffer range we read.
//
int     
CVHDDisk::Read(char* pBuf, unsigned long cbToRead, LOCAL_ALLOCATED_RANGE* pRange, const __int64* pllStartOffset)
{
    return m_pInnerFile->Read(pBuf, cbToRead, pRange, pllStartOffset);
}

BOOL 
CVHDDisk::Seek(LARGE_INTEGER i64DistanceToMove, PLARGE_INTEGER pi64NewPosition, DWORD nSeekMethod)
{
    return FALSE; //E_NOTIMPL
}

BOOL 
CVHDDisk::Read(LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead)
{
    //if in range, use current's read
    //if read other disk's data, use m_pParent's read

    return FALSE; //E_NOTIMPL
}
