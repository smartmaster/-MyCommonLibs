#pragma once

#include <Windows.h>
#include <set>
#include <vector>

#include "log.h"
#include "CodeBlock.h"

namespace SmartLib
{
	class BlockAllocator
	{
	private:
		std::set<LPVOID> _usedBlocks;
		std::vector<LPVOID> _freeBlocks;
		std::vector<LPVOID> _pages;

		int _blockSize{ 0 };
		DWORD _flAllocationType{ 0 };
		DWORD _flProtect{ 0 };

		int _pageSize{ 0 };
		CRITICAL_SECTION _cs{ 0 };

		class AutoLocker
		{
			CRITICAL_SECTION * _cs;
		public:
			AutoLocker(CRITICAL_SECTION * cs) :
				_cs(cs)
			{
				::EnterCriticalSection(_cs);
			}

			~AutoLocker()
			{
				::LeaveCriticalSection(_cs);
			}
		};


	private:
		HRESULT AllocNewPage()
		{
			DWORD lastError = 0;
			HRESULT hr = S_OK;

			BEGIN_BLOCK(0);
			///////////////////////////////////////////////////////////////////////
			LPVOID page = ::VirtualAlloc(
				nullptr,//_In_opt_  LPVOID lpAddress,
				_pageSize,//_In_      SIZE_T dwSize,
				_flAllocationType,//_In_      DWORD flAllocationType,
				_flProtect//,//_In_      DWORD flProtect
			);
			if (nullptr == page)
			{
				lastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(lastError);
				SML_LOG_LINE(TEXT("API [%s] failed with [%d, 0x%08x]"), TEXT("VirtualProtect"), lastError, hr);
				LEAVE_BLOCK(0);
			}

			///////////////////////////////////////////////////////////////////////
			_pages.push_back(page);
			int jj = _freeBlocks.size();
			_freeBlocks.resize(jj + _pageSize / _blockSize);
			for (int ii = 0; ii < _pageSize; ii += _blockSize)
			{
				_freeBlocks[jj++] = (BYTE*)page + ii;
			}
			END_BLOCK(0);

			return hr;
		}

	public:
		BlockAllocator(int blockSize, DWORD flProtect):
			_blockSize(blockSize),
			_flAllocationType(MEM_COMMIT | MEM_RESERVE),
			_flProtect(flProtect)
		{
			SYSTEM_INFO si{ 0 };
			::GetNativeSystemInfo(&si);
			_pageSize = si.dwAllocationGranularity > si.dwPageSize ? si.dwAllocationGranularity : si.dwPageSize;
			(VOID)::InitializeCriticalSectionAndSpinCount(&_cs, 1024);
		}

		~BlockAllocator()
		{
			::DeleteCriticalSection(&_cs);
		}


		LPVOID Alloc()
		{
			bool found = false;
			LPVOID ret = nullptr;
			{
				AutoLocker lock(&_cs);

				if (0 == _freeBlocks.size())
				{
					AllocNewPage();
				}

				if (_freeBlocks.size())
				{
					ret = _freeBlocks.back();
					_usedBlocks.insert(ret);
					_freeBlocks.pop_back();
				}
			}
			
			return ret;
		}


		void Free(LPVOID buffer)
		{
			AutoLocker lock(&_cs);
			auto iter = _usedBlocks.find(buffer);
			if (iter != _usedBlocks.end())
			{
				_freeBlocks.push_back(*iter);
				_usedBlocks.erase(iter);
			}
		}
		

		int BlockSize()
		{
			return _blockSize;
		}
	};


	class BlockAllocatorTest
	{
	public:
		static void Case0()
		{
			BlockAllocator allocator(1024, PAGE_EXECUTE_READWRITE);
			std::vector<LPVOID> vec(128);
			for (int ii = 0; ii < vec.size(); ++ii)
			{
				vec[ii] = allocator.Alloc();
			}

			for (auto buffer : vec)
			{
				allocator.Free(buffer);
			}
		}
	};
}