#pragma once


#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>
#include <lm.h>

#include "CommonDef.h"

class CListNetSession
{
public:
	static HRESULT List()
	{
		HRESULT hr = S_OK;
		DWORD LastError = 0;


		auto p1 = SML_EXPAND_MACRO(BEGIN_BLOCK(888));
		auto p2 = SML_EXPAND_MACRO(LEAVE_BLOCK(888));
		auto p3 = SML_EXPAND_MACRO(END_BLOCK(888));

		auto p4 = SML_MAKE_STRING((
			int x = 100; 
			int y = 200; 
			int z = (x, y, x + y);
			x >>= 1; 
			y >>= 1;  
			_ftprintf_s(stdout, TEXT("%d %d" "\r\n"), x, y);
			));
		
		
		BEGIN_BLOCK(0);
		//////////////////////////////////////////////////////////////////////////
		CONST DWORD LEVEL = 502;

		DWORD resume_handle = 0;
		NET_API_STATUS status = NERR_Success;
		SESSION_INFO_502 * data = nullptr;
		do 
		{
			DWORD entriesread = 0;
			DWORD totalentries = 0;
			data = nullptr;
			status = NetSessionEnum(
				nullptr,//_In_     LPWSTR servername,
				nullptr,//_In_     LPWSTR UncClientName,
				nullptr,//_In_     LPWSTR username,
				LEVEL,//_In_     DWORD level,
				(LPBYTE *)(&data),//_Out_    LPBYTE *bufptr,
				MAX_PREFERRED_LENGTH,//_In_     DWORD prefmaxlen,
				&entriesread,//_Out_    LPDWORD entriesread,
				&totalentries,//_Out_    LPDWORD totalentries,
				&resume_handle//_Inout_  LPDWORD resume_handle
				);

			
			for (int ii = 0; ii < entriesread; ++ii)
			{
				_ftprintf_s(stdout, TEXT("cname=%s,\tusername=%s,\tnum_opens=%d,\ttime=%d,\tidle_time=%d,\tuser_flags=0x%08x,\tcltype_name=%s,\ttransport=%s") TEXT("\r\n"),
					data[ii].sesi502_cname,
					data[ii].sesi502_username,
					data[ii].sesi502_num_opens,
					data[ii].sesi502_time,
					data[ii].sesi502_idle_time,
					data[ii].sesi502_user_flags,
					data[ii].sesi502_cltype_name,
					data[ii].sesi502_transport);
			}

			

			NetApiBufferFree(data);


		} while (status == NERR_Success && nullptr != data );
		

		
		//////////////////////////////////////////////////////////////////////////
		END_BLOCK(0);
		
		return hr;
	}
};