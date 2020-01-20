#pragma once 

#include <Windows.h>
#include "MiscHelper.h"
//#include <atlbase.h>

//#define VALID_HANLE(h) ((NULL != h) && (INVALID_HANDLE_VALUE != h))
typedef BOOL (WINAPI * pfn_CAutoHandleEx_CloseHandle)( HANDLE hObject );

template< pfn_CAutoHandleEx_CloseHandle pfnCMyAutoHandleCloseHandle = ::CloseHandle >
class CAutoHandleEx
{
public:
	inline CAutoHandleEx() throw() :
	m_h( NULL )
	{
	}

	inline CAutoHandleEx(_Inout_ CAutoHandleEx& h) throw() :
	m_h( NULL )
	{
		Attach( h.Detach() );
	}

	inline explicit CAutoHandleEx(_In_ HANDLE h) throw() :
	m_h( h )
	{
	}

	inline ~CAutoHandleEx() throw()
	{
		Close();
	}

	inline CAutoHandleEx& operator=(_Inout_ CAutoHandleEx& h) throw()
	{
		if( this != &h )
		{
			if( VALID_HANLE(m_h) )
			{
				Close();
			}
			Attach( h.Detach() );
		}

		return( *this );
	}

	inline operator HANDLE() const throw()
	{
		return( m_h );
	}

	// Attach to an existing handle (takes ownership).
	inline void Attach(_In_ HANDLE h) throw()
	{
		ATLASSUME( m_h == NULL );
		m_h = h;  // Take ownership
	}

	// Detach the handle from the object (releases ownership).
	inline HANDLE Detach() throw()
	{
		HANDLE h;

		h = m_h;  // Release ownership
		m_h = NULL;

		return( h );
	}

	// Close the handle.
	inline void Close() throw()
	{
		if( VALID_HANLE(m_h) )
		{
			pfnCMyAutoHandleCloseHandle( m_h );
			m_h = NULL;
		}
	}

public:
	HANDLE m_h;
};