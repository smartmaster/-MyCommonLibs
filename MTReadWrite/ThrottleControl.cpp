#include "stdafx.h"
#include "ThrottleControl.h"

ThrottleControl_NS::CThrottleControl::CThrottleControl(LONGLONG limitedSpeed, /*bytes/sec*/ LONGLONG sampleTimeSec, /*sec*/ LONGLONG sampleSize /*bytes*/) :
m_limitedSpeed(limitedSpeed),
m_sampleTimeSec(sampleTimeSec),
m_sampleSize(sampleSize),
m_sampleTimeMS(sampleTimeSec * 1000),
m_startTime(-1),
m_transfferedSize(0)
{

}

LONG ThrottleControl_NS::CThrottleControl::Throttle(LONGLONG deltaSize /*bytes*/)
{
	if (-1 == m_startTime)
	{
		m_startTime = GetTickCount();
	}

	LONG sleepTime = 0;
	m_transfferedSize += deltaSize;
	LONG deltaTime = GetTickCount() - m_startTime;
	if (deltaTime >= m_sampleTimeMS || m_transfferedSize >= m_sampleSize)
	{
		sleepTime = m_transfferedSize * 1000 / m_limitedSpeed - deltaTime;
		if (sleepTime > 0)
		{
			::Sleep(sleepTime);
		}

		if (deltaTime >= m_sampleTimeMS)
		{
			m_transfferedSize = 0;
			m_startTime = GetTickCount();
		}
	}
	return sleepTime > 0 ? sleepTime : 0;
}
