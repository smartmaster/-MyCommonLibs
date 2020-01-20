#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>

#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicDTraceLib.h"

//////////////////////////////////////////////////////////////////////////
//can be used by ONE provider thread and ONE consumer thread
//does not support multiple provider threads or multiple consumer threads
//////////////////////////////////////////////////////////////////////////
class CCircularBuffer
{
private:
	BYTE * m_Buffer; /***INCOMMING***/
	LONG  m_BufferLen; /***INCOMMING***/

	LONG m_StartOffset; /***INTERNAL***/
	LONG m_EndBoud; /***INTERNAL***/
	CRITICAL_SECTION m_Lock; /***INTERNAL***/ /***TO CLEANUP***/
	HANDLE m_hSpaceAvailable; /***INTERNAL***/ /***TO CLEANUP***/
	HANDLE m_hDataAvaible; /***INTERNAL***/ /***TO CLEANUP***/
	HANDLE m_hStop; /***INTERNAL***/


	//used by application in case multiple threads R/W circular buffer. Ideally circular buffer is used by one READER and one WRITER thread
	CRITICAL_SECTION m_WriterCS; /***INTERNAL***//***TO CLEANUP***/ 
	CRITICAL_SECTION m_ReaderCS; /***INTERNAL***//***TO CLEANUP***/ 

	DWORD m_tidProvider; //<sonmi01>2015-3-12 ###???
	DWORD m_tidConsumer;

public:
	CCircularBuffer(BYTE * Buffer, LONG BufferLen);
	virtual ~CCircularBuffer();

	HRESULT CreateMember();
	HRESULT DestroyMember();

	//if stop event is received and no space is available, ProcessedLen is zero; else ProcessedLen is non-zero
	//return S_OK always
	HRESULT Provide(CONST BYTE * pData, LONG Len, LONG & ProcessedLen);

	//if stop event is received and no data is available, ProcessedLen is zero; else ProcessedLen is non-zero
	//return S_OK always
	HRESULT Consume(BYTE * pData, LONG Len, LONG & ProcessedLen);
	HRESULT PeekNoLocked(BYTE * pData, LONG Len, LONG & ProcessedLen); //must be called in the same thread as consumer

	//if stop event is received, return S_FALSE and TotalProcessedLen < Len; else return S_OK and TotalProcessedLen == Len
	//return S_OK or S_FALSE
	HRESULT ProvideAll(CONST BYTE * pData, LONG Len, LONG & TotalProcessedLen);

	//if stop event is received, return S_FALSE and TotalProcessedLen < Len; else return S_OK and TotalProcessedLen == Len
	//return S_OK or S_FALSE
	HRESULT ConsumeAll(BYTE * pData, LONG Len, LONG & TotalProcessedLen);

	//return S_OK always
	HRESULT Stop();


	HRESULT WriterLock();
	HRESULT WriterUnlock();


	HRESULT ReaderLock();

	HRESULT ReaderUnlock();

private:
	VOID GetDataBufferLocked(LONG & offset, LONG & len);  //S_OK always;
	VOID GetSpaceBufferLocked(LONG & offset, LONG & len); 	//S_OK always
	VOID UpdateStartOffsetLocked(LONG delta);  //S_OK always;
	VOID UpdateEndBoundLocked(LONG delta);  //S_OK always;

	VOID CircularCopyIn(LONG Offset, CONST BYTE * pData, LONG DataLen);
	VOID CircularCopyOut( LONG Offset, BYTE * pData, LONG DataLen );

private:
	CCircularBuffer(CONST CCircularBuffer &);
	CONST CCircularBuffer & operator=(CONST CCircularBuffer &);
};

//////////////////////////////////////////////////////////////////////////
class CAutoWriterLock
{
	CCircularBuffer * m_pCircularBuffer;

public:
	CAutoWriterLock(CCircularBuffer * pCircularBuffer);
	~CAutoWriterLock();
};


class CAutoReaderLock
{
	CCircularBuffer * m_pCircularBuffer;

public:
	CAutoReaderLock(CCircularBuffer * pCircularBuffer);
	~CAutoReaderLock();
};
