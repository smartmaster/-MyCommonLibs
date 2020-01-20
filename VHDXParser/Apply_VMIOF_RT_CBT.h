#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include "..\PublicHeader\PublicDTraceLib.h"
#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicFileDeviceInterface.h"
#include "..\PublicHeader\PublicVMemPtr.h"


#include "DiskImageInterface.h"

namespace SmartLib
{

#ifndef SML_NAME_OF
#define SML_RAW_NAME_OF(var) # var
#define SML_NAME_OF(var) SML_RAW_NAME_OF(var)
#define SML_WNAME_OF(var) L ## SML_NAME_OF(var)
#define SML_TNAME_OF(var) TEXT(SML_NAME_OF(var))
#endif


#ifndef PRINT_LINE
#define PRINT_LINE(fmt, ...)  _ftprintf_s(stdout, __VA_ARGS__)
#endif

	struct CBT_SOL
	{
		ULONGLONG _seq;
		ULONGLONG _offset;
		ULONGLONG _length;
	};

	class Apply_VMIOF_RT_CBT
	{
	public:
		// parser S#0000000000000958.O#0000000000017408.L#0000000000008192.vmiofdata
		static BOOL ParseFileName(IN LPCTSTR fileName, OUT CBT_SOL & sol)
		{
			TCHAR * endPtr = nullptr;

			//////////////////////////////////////////////////////////////////////////
			if ((fileName[0] == L'S' || fileName[0] == L's') && fileName[1] == L'#')
			{
				fileName += 2;
				sol._seq = _tcstoull(fileName, &endPtr, 10);
			}
			else
			{
				return FALSE;
			}

			if (endPtr[0] == L'.' && (endPtr[1] == L'O' || endPtr[1] == L'o') && endPtr[2] == L'#')
			{
				fileName = endPtr + 3;
				sol._offset = _tcstoull(fileName, &endPtr, 10);
			}
			else
			{
				return FALSE;
			}

			if (endPtr[0] == L'.' && (endPtr[1] == L'L' || endPtr[1] == L'l') && endPtr[2] == L'#')
			{
				fileName = endPtr + 3;
				sol._length = _tcstoull(fileName, &endPtr, 10);
			}
			else
			{
				return FALSE;
			}

			return TRUE;
		}

		static HRESULT ApplyCbtFile(IFileDevice * dest, LPCTSTR dirName, LPCTSTR fileName)
		{
		

			HRESULT hr = S_OK;
			DWORD LastError = 0;
			
			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			CString fullPath = dirName;
			fullPath += TEXT("\\");
			fullPath += fileName;

			CBT_SOL sol = { 0 };
			BOOL bpf = ParseFileName(fileName, sol);
			if (!bpf)
			{
				hr = E_FAIL;
				LEAVE_BLOCK(0);
			}


			//////////////////////////////////////////////////////////////////////////
			CVMemPtr<BYTE> buffer;
			buffer.Allocate(sol._length);
			if (nullptr == buffer.m_pData)
			{
				hr = E_OUTOFMEMORY;
				LEAVE_BLOCK(0);
			}

			//////////////////////////////////////////////////////////////////////////
			HANDLE hcf = ::CreateFile(
				fullPath.GetString(),//_In_     LPCTSTR               lpFileName,
				GENERIC_READ,//_In_     DWORD                 dwDesiredAccess,
				FILE_SHARE_READ,//_In_     DWORD                 dwShareMode,
				nullptr,//_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
				OPEN_EXISTING,//_In_     DWORD                 dwCreationDisposition,
				0,//_In_     DWORD                 dwFlagsAndAttributes,
				nullptr//_In_opt_ HANDLE                hTemplateFile
			);
			if (INVALID_HANDLE_VALUE == hcf)
			{
				LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				LEAVE_BLOCK(0);
			}

			CHandle hfile(hcf); hcf = INVALID_HANDLE_VALUE;
			//////////////////////////////////////////////////////////////////////////

			DWORD      NumberOfBytesRead = 0;
			BOOL brf = ::ReadFile(
				hfile,//_In_        HANDLE       hFile,
				buffer.m_pData,//_Out_       LPVOID       lpBuffer,
				sol._length,//_In_        DWORD        nNumberOfBytesToRead,
				&NumberOfBytesRead,//_Out_opt_   LPDWORD      lpNumberOfBytesRead,
				nullptr//_Inout_opt_ LPOVERLAPPED lpOverlapped
			);
			if (!brf)
			{
				LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				LEAVE_BLOCK(0);
			}


			//////////////////////////////////////////////////////////////////////////
			ULONG  NumberOfBytesWritten = 0;
			hr = ::WriteAtOffsetHelper(dest, sol._offset, sol._length, &NumberOfBytesWritten, buffer.m_pData);
			if (FAILED(hr))
			{
				LEAVE_BLOCK(0);
			}


			
			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			
			return hr;
		}

		static HRESULT ApplyCBTFiles(IFileDevice * dest, LPCTSTR dirName,LONGLONG first, LONGLONG last, BOOL deleteCBT)
		{

			HRESULT hr = S_OK;
			DWORD LastError = 0;
			
			
			CString str;
			while (first < last + 1)
			{
				str = dirName;
				str.AppendFormat(TEXT("\\S#%016I64d.O#*.L#*.vmiofdata"), first);
				WIN32_FIND_DATA FindFileData = { 0 };
				HANDLE hff = ::FindFirstFile(
					str.GetString(),//_In_  LPCTSTR           lpFileName,
					&FindFileData//_Out_ LPWIN32_FIND_DATA lpFindFileData
				);
				if (INVALID_HANDLE_VALUE == hff)
				{
					LastError = GetLastError();
					hr = HRESULT_FROM_WIN32(LastError);
					D_API_ERR(0, TEXT("FindFirstFile"), hr, TEXT(""));
				}
				else //if (INVALID_HANDLE_VALUE != hff)
				{
					::FindClose(hff); hff = INVALID_HANDLE_VALUE;
					hr = ApplyCbtFile(dest, dirName, FindFileData.cFileName);
					if (FAILED(hr))
					{
						D_API_ERR(0, TEXT("ApplyCbtFile"), hr, TEXT("failed to appply [%s]"), FindFileData.cFileName);
					}
					else
					{
						if (deleteCBT)
						{
							CString fullFileName = dirName;
							fullFileName += TEXT("\\");
							fullFileName += FindFileData.cFileName;
							::DeleteFile(fullFileName.GetString());
						}
						
					}
				}

				++first;
			}

			return hr;
		}
		

		static HRESULT ApplyCBTFilesToVHDX(LPCTSTR vhdx, LONGLONG sizeInMB, LPCTSTR dirName, LONGLONG first, LONGLONG last, BOOL deleteCBT)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;
			
			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			CComPtr<IDiskImageVirtual> spIDiskDest;
			CreateInstanceCDiskImageVhdxParams paramsWrite =
			{
				NULL,//LPCTSTR parentVhdxFileName;
				vhdx,//LPCTSTR vhdxFileName;
				GENERIC_WRITE,//DWORD DesiredAccess;
				0,//DWORD CreationDisposition;
				sizeInMB << 20,//LONGLONG VirtualDiskSize;
				512,//LONG LogicalSectorSize;
				4096,//LONG PhysicalSectorSize;
				2 * 1024 * 1024,//LONG BlockSize;
				VHDX_DISK_TYPE_ENUM::DYNAMIC
			};
			hr = CreateInstanceCDiskImageVhdx(paramsWrite, &spIDiskDest, TRUE);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("CreateInstanceCDiskImageVhdx"), hr, TEXT("destFile"));
				LEAVE_BLOCK(0);
			}


			//////////////////////////////////////////////////////////////////////////
			hr = ApplyCBTFiles(spIDiskDest, dirName, first, last, deleteCBT);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("CreateInstanceCDiskImageVhdx"), hr, TEXT("destFile"));
				LEAVE_BLOCK(0);
			}

			spIDiskDest->FlushData();
			spIDiskDest->FlushMetadata();
			

			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			
			return hr;
		}


		class CmdLine
		{
		public:
			CString vhdx;
			LONGLONG sizeInMB;
			CString dirName;
			LONGLONG first;
			LONGLONG last;
			BOOL deleteCBT;

			bool Parse(int argc, TCHAR ** argv)
			{

				int parsedCount = 0;
				for (int ii = 1; ii < argc; ++ ii)
				{
					if (0 == _tcsicmp(argv[ii], SML_TNAME_OF(vhdx)))
					{
						++ii;
						if (ii < argc)
						{
							vhdx = argv[ii];
							++parsedCount;
						}
					}
					else if (0 == _tcsicmp(argv[ii], SML_TNAME_OF(sizeInMB)))
					{
						++ii;
						if (ii < argc)
						{
							sizeInMB = _tcstoll(argv[ii], nullptr, 10);
							++parsedCount;
						}
					}
					else if (0 == _tcsicmp(argv[ii], SML_TNAME_OF(dirName)))
					{
						++ii;
						if (ii < argc)
						{
							dirName = argv[ii];
							++parsedCount;
						}
					}
					else if (0 == _tcsicmp(argv[ii], SML_TNAME_OF(first)))
					{
						++ii;
						if (ii < argc)
						{
							first = _tcstoll(argv[ii], nullptr, 10);
							++parsedCount;
						}
					}

					else if (0 == _tcsicmp(argv[ii], SML_TNAME_OF(last)))
					{
						++ii;
						if (ii < argc)
						{
							last = _tcstoll(argv[ii], nullptr, 10);
							++parsedCount;
						}
					}
					else if (0 == _tcsicmp(argv[ii], SML_TNAME_OF(deleteCBT)))
					{
						++ii;
						if (ii < argc)
						{
							deleteCBT = _tcstoll(argv[ii], nullptr, 10);
							++parsedCount;
						}
					}

				}


				bool ok = (6 == parsedCount);
				if (!ok)
				{
					PRINT_LINE(TEXT("%s vhdx <vhdx> sizeInMB <sizeInMB> dirName <dirName> first <first> last <last> deleteCBT <deleteCBT> -case 20"), argv[0]);
					
				}


				return ok;
			}
		};

		static void Main(int argc, TCHAR ** argv)
		{
			CmdLine cmd;
			if (cmd.Parse(argc, argv))
			{
				ApplyCBTFilesToVHDX(cmd.vhdx, cmd.sizeInMB, cmd.dirName, cmd.first, cmd.last, cmd.deleteCBT);
			}
		}

	};
}//namespace SmartLib
