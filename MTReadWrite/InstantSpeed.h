#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>

class CIOInstantSpeed
{
private:
	CONST LONGLONG m_DATA_DELTA; /***INCOMMING***/ //in bytes
	CONST LONGLONG m_TIME_DELTA; /***INCOMMING***/ // in millSec

	LONGLONG m_size1; /***INTERNAL***/
	LONGLONG m_size2; /***INTERNAL***/
	LONGLONG m_t0; /***INTERNAL***/
	LONGLONG m_t1; /***INTERNAL***/
	LONGLONG m_t2; /***INTERNAL***/

public:
	CIOInstantSpeed(LONGLONG data_delta_bytes, LONGLONG time_delta_millsec);

	VOID Start();
	LONGLONG Speed(LONG dataDelta); //return Bytes/sec;
	LONGLONG AverageSpeed(LONGLONG totalDataSize); //return Bytes/sec;
};