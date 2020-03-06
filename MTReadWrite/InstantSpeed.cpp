#include "stdafx.h"
#include "InstantSpeed.h"




CIOInstantSpeed::CIOInstantSpeed( LONGLONG data_delta_bytes, LONGLONG time_delta_millsec ) :
	m_DATA_DELTA(data_delta_bytes),
	m_TIME_DELTA(time_delta_millsec),
	m_size1(0),
	m_size2(0),
	m_t0(-1),
	m_t1(-1),
	m_t2(-1)
{

}

VOID CIOInstantSpeed::Start()
{
	m_size1 = 0;
	m_size2 = 0;
	m_t0 = m_t1 = m_t2 = GetTickCount();
}

LONGLONG CIOInstantSpeed::Speed( LONG dataDelta ) //return Bytes/Sec
{
	//if (m_t0 < 0)
	//{
	//	ATLASSERT(-1 == m_t1);
	//	m_t0 = m_t1 = m_t2 = GetTickCount();
	//}

	LONGLONG SpeedValue = -1;

	m_size2 += dataDelta;

	LONGLONG dataSize = m_size2 - m_size1;
	if (dataSize >= m_DATA_DELTA)
	{
		m_t2 = GetTickCount();

		LONGLONG elapsedTime = m_t2 - m_t1;
		if (elapsedTime >= m_TIME_DELTA)
		{
			SpeedValue = (((dataSize << 24) * 1000 / elapsedTime) >> 24);
			m_t1 = m_t2;
			m_size1 = m_size2;
		}
	}

	return SpeedValue;
}

LONGLONG CIOInstantSpeed::AverageSpeed(LONGLONG totalDataSize) //return Bytes/Sec
{
	LONGLONG elapsedTime = GetTickCount() - m_t0;
	if (0 == elapsedTime)
	{
		elapsedTime = 1;
	}

	if (totalDataSize <= 0)
	{
		totalDataSize = m_size2;
	}

	return (((totalDataSize << 16) * 1000 / elapsedTime) >> 16);
}
