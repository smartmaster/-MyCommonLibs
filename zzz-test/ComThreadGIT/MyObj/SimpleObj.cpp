// SimpleObj.cpp : Implementation of CSimpleObj

#include "stdafx.h"
#include "SimpleObj.h"


// CSimpleObj

#define PRINT_LINE(sss, ...) _ftprintf_s(stdout, sss TEXT("\r\n"), __VA_ARGS__)

STDMETHODIMP CSimpleObj::Method()
{
	// TODO: Add your implementation code here
	
	for (int ii = 0; ii < 5; ++ ii)
	{
		{
			ObjectLock objLock(this); //protecting shared data among threads
			++m_ProtectedData;
			m_ProtectedStr.Empty();
			for (size_t jj = 0; jj < 5; ++ jj)
			{
				Sleep(200);
				m_ProtectedStr.AppendFormat(TEXT("%d "), m_ProtectedData);
			}
			PRINT_LINE(TEXT("GetCurrentThreadId=%d, m_ProtectedData=%d, m_ProtectedStr=%s"), GetCurrentThreadId(), m_ProtectedData, m_ProtectedStr.GetString());
		}
	}
	return S_OK;
}
