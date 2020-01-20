#pragma once 

#include <stdio.h>
#include <tchar.h>
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit
#include <atlbase.h>
#include <atlstr.h>
#include <windows.h>

class CStopTimer
{
private: //////////////////////////////////////////////////////////////////////////
	LARGE_INTEGER m_Start;
	LARGE_INTEGER m_End;
	LARGE_INTEGER m_Frequency;
	BOOL m_UseHighResolution;

public:	//////////////////////////////////////////////////////////////////////////
	CStopTimer(BOOL UseHighResolution);
	~CStopTimer();

	//////////////////////////////////////////////////////////////////////////
	LONGLONG Start();
	LONGLONG End();
	LONGLONG Timing(); //in MilSec
};