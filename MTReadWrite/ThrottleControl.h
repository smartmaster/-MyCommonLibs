#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>


namespace ThrottleControl_NS
{
	class CThrottleControl
	{
	private:
		CONST LONGLONG	m_limitedSpeed; /***INCOMMING***/ /*bytes/sec*/
		CONST LONGLONG	m_sampleTimeSec; /***INCOMMING***/ /*sec*/
		CONST LONGLONG	m_sampleSize;		/***INCOMMING***/ /*bytes*/

		CONST LONGLONG	m_sampleTimeMS; /***INTERNAL***/
		LONGLONG	m_startTime; /***INTERNAL***/
		LONGLONG	m_transfferedSize; /***INTERNAL***/

	public:
		CThrottleControl(
			LONGLONG limitedSpeed, /*bytes/sec*/
			LONGLONG sampleTimeSec,		/*sec*/
			LONGLONG sampleSize		/*bytes*/
			);

		LONG Throttle(LONGLONG deltaSize /*bytes*/);
	};
} //end namespace ThrottleControl_NS