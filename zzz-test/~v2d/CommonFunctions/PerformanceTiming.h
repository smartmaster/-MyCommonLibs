#pragma once 

#include "stdafx.h"

class CStopTimer
{
private: //********************************************************************
	LARGE_INTEGER m_Start;
	LARGE_INTEGER m_End;
	LARGE_INTEGER m_Frequency;
	BOOL m_UseHighResolution;

public:	//********************************************************************
	CStopTimer(BOOL bHighResolution);
	~CStopTimer();

	//********************************************************************
	LONGLONG Start();
	LONGLONG End();
	LONGLONG Timing(); //in MilSec
};