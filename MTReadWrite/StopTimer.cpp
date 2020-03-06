#include "stdafx.h"
#include "StopTimer.h"

CStopTimer::CStopTimer( BOOL UseHighResolution )
{
	m_Start.QuadPart = 0;
	m_End.QuadPart = 0;
	m_Frequency.QuadPart = 0;
	m_UseHighResolution = FALSE;

	if (UseHighResolution)
	{
		m_UseHighResolution = QueryPerformanceFrequency(&m_Frequency);
	}
}

CStopTimer::~CStopTimer()
{
	m_Start.QuadPart = 0;
	m_End.QuadPart = 0;
	m_Frequency.QuadPart = 0;
	m_UseHighResolution = FALSE;
}

LONGLONG CStopTimer::Start()
{
	if (m_UseHighResolution)
	{
		QueryPerformanceCounter(&m_Start);
	}
	else
	{
		m_Start.QuadPart = GetTickCount();
	}

	return m_Start.QuadPart;
}

LONGLONG CStopTimer::End()
{
	if (m_UseHighResolution)
	{
		QueryPerformanceCounter(&m_End);
	}
	else
	{
		m_End.QuadPart = GetTickCount();
	}

	return m_End.QuadPart;
}

LONGLONG CStopTimer::Timing()
{
	LONGLONG llMilSecond = 0;
	if (m_UseHighResolution)
	{
		llMilSecond = (m_End.QuadPart - m_Start.QuadPart) * 1000 / m_Frequency.QuadPart;
	}
	else
	{
		llMilSecond = (m_End.QuadPart - m_Start.QuadPart);
	}
	return llMilSecond;
}

