#pragma once
#include "InnerInterface.h"


//
//Represent a logical disk
//
class IVHDDisk : public IReader, public ILocalAllocReader
{
public:
    virtual IVHDDisk* GetParentDisk() const = 0;
};

class CVHDDisk : public IVHDDisk
{
public:
    CVHDDisk(void);
protected:
    virtual ~CVHDDisk(void);
public:
    virtual void Release();

    virtual IVHDDisk* GetParentDisk() const;

    /*
    methods of ILocalAllocReader
    */
    virtual int     QueryRange(const LOCAL_ALLOCATED_RANGE* pRangeToQuery, LOCAL_ALLOCATED_RANGE* pRange, int* pnRangeCnt);
    //
    //Read the LocalAllocatedData
    //return 0 success, others fail
    //if return 0 and returned pRange->len == 0 && pRange->offset == -1, have read to the end of file.
    //@pllStartOffset - the logical offset from where to start read, if NULL, read from current file's pointer
    //@pRange - return the buffer range we read.
    //
    virtual int     Read(char* pBuf, unsigned long cbToRead, LOCAL_ALLOCATED_RANGE* pRange, const __int64* pllStartOffset);

    /*
    Methods of IReader
    */
    virtual BOOL Seek(LARGE_INTEGER i64DistanceToMove, PLARGE_INTEGER pi64NewPosition, DWORD nSeekMethod);
    virtual BOOL Read(LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead);
protected:
    IVHDDisk* m_pParent;
    IVHDFile* m_pInnerFile;
};
