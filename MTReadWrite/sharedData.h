#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>
#include <atlfile.h>

#include <algorithm>

template<class T>
class CSharedData
{
private:
	HRESULT m_result;
	CAtlFileMapping<T> m_fileMapping;

public:
	//name - sample TEXT("Global\\MysharedDataXXXXXXXX");
	CSharedData(LPCTSTR name, LONG size) :
		m_result(E_FAIL)
	{
		if (nullptr != name && 0 != name[0])
		{
			Create(name, size);
		}
	}

	HRESULT Create(LPCTSTR name, LONG size)
	{
		ATLASSERT(nullptr != name && 0 != name[0]);
		ATLASSERT(FAILED(m_result));

		m_result = m_fileMapping.OpenMapping(name, 0);
		if (FAILED(m_result))
		{
			BOOL bAlreadyExisted = FALSE;
			m_result = m_fileMapping.MapSharedMem(
				std::max<LONG>(sizeof(T), size),//SIZE_T nMappingSize,
				name,//LPCTSTR szName,
				&bAlreadyExisted,//BOOL* pbAlreadyExisted = NULL,
				nullptr,//LPSECURITY_ATTRIBUTES lpsa = NULL,
				PAGE_READWRITE,//DWORD dwMappingProtection = PAGE_READWRITE,
				FILE_MAP_ALL_ACCESS//DWORD dwViewDesiredAccess = FILE_MAP_ALL_ACCESS
				);
			if (FAILED(m_result))
			{
				//LastError = WSAGetLastError();
				//LastError = GetLastError();
				//hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("MapSharedMem"), m_result, TEXT(""));
				//LEAVE_BLOCK(0);
			}
		}
		return m_result;
	}

	HRESULT Result()
	{
		return (m_result);
	}

	operator T&()
	{
		return *((T*)(m_fileMapping));
	}
};