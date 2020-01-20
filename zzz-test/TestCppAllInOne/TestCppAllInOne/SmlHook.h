#pragma once

#include <Windows.h>

#include "CodeBlock.h"
#include "log.h"
#include "SmlBlockAllocator.h"

#include "./external/distorm/include/distorm.h"


namespace SmartLib
{

	class SmlHook
	{
	private:
#include <pshpack1.h>
#if defined(_WIN64) //64bit
		static const int go_back_offset = 0xc;
		static const int min_instruction_boudary = 0xd;
		struct Jmp2Target
		{
			/*
			00007FF7416D2130 48 B8 89 67 45 23 01 EF CD AB 	mov         rax,0ABCDEF0123456789h
			00007FF7416D213A FF E0                					jmp         rax
			00007FF7416D213C 58                   					pop         rax
			00007FF7416D213D 90                   					nop
			00007FF7416D213E 90                   					nop
			*/

			//00007FF7416D2130 48 B8 89 67 45 23 01 EF CD AB 	mov         rax,0ABCDEF0123456789h
			BYTE bb100{ 0x48 };
			BYTE bb200{ 0xb8 };
			LPVOID target{ nullptr };


			//00007FF7416D213A FF E0                					jmp         rax
			BYTE bb300{ 0xff };
			BYTE bb400{ 0xe0 };


			//00007FF7416D213C 58                   					pop         rax
			BYTE bb500{ 0x58 };

			//00007FF7416D213D 90                   					nop
			//00007FF7416D213E 90                   					nop
			BYTE nop[32];


			Jmp2Target(LPVOID myfunc)
			{
				target = myfunc;
				memset(nop, 0x90, sizeof(nop));
			}

		};


		struct Jmp2Src
		{
			/*
			00007FF7416D213F 50                   					push        rax
			00007FF7416D2140 48 B8 89 67 45 23 01 EF CD AB 			mov         rax,0ABCDEF0123456789h
			00007FF7416D214A FF E0               					jmp         rax
			*/

			//00007FF7416D213F 50                   					push        rax
			BYTE bb100{ 0x50 };

			//00007FF7416D2140 48 B8 89 67 45 23 01 EF CD AB 			mov         rax,0ABCDEF0123456789h
			BYTE bb200{ 0x48 };
			BYTE bb300{ 0xb8 };
			LPVOID src{ nullptr }; //orginal src + 12

			//00007FF7416D214A FF E0               					jmp         rax
			BYTE bb400{ 0xff };
			BYTE bb500{ 0xe0 };


			Jmp2Src(LPVOID originalSrc)
			{
				src = (BYTE*)originalSrc + go_back_offset;
			}
		};
#else //32bit
		static const int go_back_offset = 0x7;
		static const int min_instruction_boudary = 0x8;
		struct Jmp2Target
		{
			/*
			0133F700 B8 12 EF CD AB       mov         eax,0ABCDEF12h
			0133F705 FF E0                jmp         eax
			0133F707 58                   pop         eax
			0133F708 90                   nop
			0133F709 90                   nop
			*/

			//0133F700 B8 12 EF CD AB       mov         eax,0ABCDEF12h
			BYTE bb100{ 0xB8 };
			LPVOID target{ nullptr };


			//0133F705 FF E0                jmp         eax
			BYTE bb300{ 0xff };
			BYTE bb400{ 0xe0 };


			//0133F707 58                   pop         eax
			BYTE bb500{ 0x58 };

			//00007FF7416D213D 90                   					nop
			//00007FF7416D213E 90                   					nop
			BYTE nop[16];


			Jmp2Target(LPVOID myfunc)
			{
				target = myfunc;
				memset(nop, 0x90, sizeof(nop));
			}

		};


		struct Jmp2Src
		{
			/*
			0133F70A 50                   push        eax
			0133F70B B8 12 EF CD AB       mov         eax,0ABCDEF12h
			0133F710 FF E0                jmp         eax
			*/

			//0133F70A 50                   push        eax
			BYTE bb100{ 0x50 };

			//0133F70B B8 12 EF CD AB       mov         eax,0ABCDEF12h
			BYTE bb300{ 0xb8 };
			LPVOID src{ nullptr }; //orginal src + 12

			//00007FF7416D214A FF E0               					jmp         rax
			BYTE bb400{ 0xff };
			BYTE bb500{ 0xe0 };


			Jmp2Src(LPVOID originalSrc)
			{
				src = (BYTE*)originalSrc + go_back_offset;
			}
		};
#endif
#include <poppack.h>

	private:

		static BlockAllocator* AllocatorInstance()
		{
			const int BLOCK_SIZE = 64;
			static BlockAllocator allocator{ BLOCK_SIZE, PAGE_EXECUTE_READWRITE };
			return &allocator;
		}
		

	public:
		static int GetInstrctionBounry(LPVOID code, int codeSize, int instructionBoundary)
		{
			const int MAX_INSTRUCTIONS = 64;
			_DecodedInst decodedInstructions[MAX_INSTRUCTIONS];
			_DecodeType dt =
				sizeof(void*) == 8 ? Decode64Bits :
				sizeof(void*) == 4 ? Decode32Bits :
				Decode16Bits; //not possible to be 16 bit code
			unsigned int decodedInstructionsCount = 0;
			_DecodeResult res = distorm_decode(0, (const unsigned char*)code, codeSize, dt, decodedInstructions, MAX_INSTRUCTIONS, &decodedInstructionsCount);
			for (int jj = 0; jj < decodedInstructionsCount; jj++) 
			{
#ifdef SUPPORT_64BIT_OFFSET
				SML_LOG_LINE(L"%0*I64x (%02d) %-24S %S%S%S", 
					dt != Decode64Bits ? 8 : 16, 
					decodedInstructions[jj].offset, 
					decodedInstructions[jj].size, 
					(char*)decodedInstructions[jj].instructionHex.p, 
					(char*)decodedInstructions[jj].mnemonic.p, 
					decodedInstructions[jj].operands.length != 0 ? " " : "", 
					(char*)decodedInstructions[jj].operands.p);
#else
				SML_LOG_LINE(L"%08x (%02d) %-24S %S%S%S", 
					decodedInstructions[jj].offset, 
					decodedInstructions[jj].size, 
					(char*)decodedInstructions[jj].instructionHex.p, 
					(char*)decodedInstructions[jj].mnemonic.p, 
					decodedInstructions[jj].operands.length != 0 ? " " : "", 
					(char*)decodedInstructions[jj].operands.p);
#endif
			}

			int boundary = 0;
			for (int ii = 0; ii < decodedInstructionsCount; ii++)
			{
				if (boundary >= instructionBoundary)
				{
					break;
				}
				boundary += decodedInstructions[ii].size;
			}
			return boundary;
		}

		//if instructionBoundary < 0, we will use disam to find the boundary, or
		//dasm mannually and find a minimal instructionBoundary which must be >= min_instruction_boudary (0xd on 64bit, 0x8 on 32bit)
		//caution: within instructionBoundary, requires no jmp instruction
		static HRESULT Hook(LPVOID src, LPVOID target, int instructionBoundary, LPVOID* savedOriginal)
		{
			DWORD lastError = 0;
			HRESULT hr = S_OK;

			BEGIN_BLOCK(0);
			////////////////////////////////////////////////////////////////
			DWORD OldProtect = 0;
			DWORD NewProtect = PAGE_EXECUTE_READWRITE;
			BOOL ok = ::VirtualProtect(
				src,//_In_   LPVOID lpAddress,
				sizeof(Jmp2Target),//_In_   SIZE_T dwSize,
				NewProtect,//_In_   DWORD flNewProtect,
				&OldProtect//,//_Out_  PDWORD lpflOldProtect
			);
			if (!ok)
			{
				lastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(lastError);
				SML_LOG_LINE(TEXT("API [%s] failed with [%d, 0x%08x]"), TEXT("VirtualProtect"), lastError, hr);
				LEAVE_BLOCK(0);
			}

			////////////////////////////////////////////////////////////////
			Jmp2Target* pattern1 = (Jmp2Target*)(src);
			Jmp2Target pattern2(pattern1->target);
			if (0 == memcmp(pattern1, &pattern2, FIELD_OFFSET(Jmp2Target, nop)))
			{
				SML_LOG_LINE(TEXT("the src function is already hooked, skipping"));
				LEAVE_BLOCK(0);
			}

			////////////////////////////////////////////////////////////////
			if (instructionBoundary < 0)
			{
				const int CODE_SIZE = 64;
				instructionBoundary = GetInstrctionBounry(src, CODE_SIZE, min_instruction_boudary);
				SML_LOG_LINE(TEXT("dasm and find a minimal instructionBoundary %d, required min_instruction_boudary is %d"), instructionBoundary, min_instruction_boudary);
			}

			if (instructionBoundary < min_instruction_boudary)
			{
				SML_LOG_LINE(TEXT("please mannually dasm and find a minimal instructionBoundary which must be >= %d (0xd on 64bit, 0x8 on 32bit)"), min_instruction_boudary);
				hr = E_INVALIDARG;
				LEAVE_BLOCK(0);
			}

			////////////////////////////////////////////////////////////////
			auto allocator = AllocatorInstance();
			LPVOID buffer = allocator->Alloc();
			if (nullptr == buffer)
			{
				lastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(lastError);
				SML_LOG_LINE(TEXT("API [%s] failed with [%d, 0x%08x]"), TEXT("Alloc"), lastError, hr);
				LEAVE_BLOCK(0);
			}
			memset(buffer, 0xcc, allocator->BlockSize()); //fill will "int 3"



			////////////////////////////////////////////////////////////////
			CopyMemory(buffer, src, instructionBoundary);
			Jmp2Src jmp2src(src);
			CopyMemory((BYTE*)buffer + instructionBoundary, &jmp2src, sizeof(Jmp2Src));


			////////////////////////////////////////////////////////////////
			Jmp2Target jmp2target(target);
			CopyMemory(src, &jmp2target, instructionBoundary);

			///////////////////////////////////////////////////////////////
			*savedOriginal = buffer;

			END_BLOCK(0);

			return hr;
		}

		static HRESULT Unhook(LPVOID savedOriginal)
		{
			DWORD lastError = 0;
			HRESULT hr = S_OK;

			BEGIN_BLOCK(0);
			////////////////////////////////////////////////////////////////
			auto allocator = AllocatorInstance();

			//find 0xcc "int 3" 
			BYTE * ptr = (BYTE*)savedOriginal;
			bool found = false;
			int ii = 0;
			for (; ii < allocator->BlockSize(); ++ii)
			{
				if (0xcc == ptr[ii])
				{
					break;
				}
			}
			if (ii == allocator->BlockSize())
			{
				SML_LOG_LINE(TEXT("cannot find 'int 3' in the block"));
				LEAVE_BLOCK(0);
			}
			
			////////////////////////////////////////////////////////////////
			ptr += ii;
			ptr -= sizeof(Jmp2Src);
			Jmp2Src* j2s = (Jmp2Src*)(ptr);
			Jmp2Src patetern((BYTE*)j2s->src - go_back_offset);
			if (0 != memcmp(j2s, &patetern, sizeof(Jmp2Src)))
			{
				SML_LOG_LINE(TEXT("cannot find 'jump back to source' in the block"));
				LEAVE_BLOCK(0);
			}

			////////////////////////////////////////////////////////////////
			int copySize = ptr - (BYTE*)savedOriginal;
			if (copySize < 0)
			{
				SML_LOG_LINE(TEXT("copySize is unexpected %d"), copySize);
				LEAVE_BLOCK(0);
			}

			int boundary = GetInstrctionBounry(savedOriginal, allocator->BlockSize(), min_instruction_boudary);
			if (copySize != boundary)
			{
				SML_LOG_LINE(TEXT("THe instructions boundary not match [%d] vs. [%d]"), copySize, boundary);
				LEAVE_BLOCK(0);
			}

			////////////////////////////////////////////////////////////////
			LPVOID src = (BYTE*)(j2s->src) - go_back_offset;
			Jmp2Target* j2t = (Jmp2Target*)(src);
			Jmp2Target pattern(j2t->target);
			if (0 != memcmp(j2t, &pattern, copySize))
			{
				SML_LOG_LINE(TEXT("cannot find 'jump to target'"));
				LEAVE_BLOCK(0);
			}

			SML_LOG_LINE(TEXT("all verification donw, unhook now"));
			////////////////////////////////////////////////////////////////
			CopyMemory(src, savedOriginal, copySize);
			ZeroMemory(savedOriginal, allocator->BlockSize());
			allocator->Free(savedOriginal);

			END_BLOCK(0);

			return hr;
		}

	};


	class MyAPIHooks
	{
	
	public:
		typedef HANDLE (WINAPI *pfnCreateFile)(
			_In_      LPCTSTR lpFileName,
			_In_      DWORD dwDesiredAccess,
			_In_      DWORD dwShareMode,
			_In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
			_In_      DWORD dwCreationDisposition,
			_In_      DWORD dwFlagsAndAttributes,
			_In_opt_  HANDLE hTemplateFile
		);
		pfnCreateFile pOriginalCreateFile{ nullptr };
		
		static HANDLE WINAPI MyCreateFile(
			_In_      LPCTSTR lpFileName,
			_In_      DWORD dwDesiredAccess,
			_In_      DWORD dwShareMode,
			_In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
			_In_      DWORD dwCreationDisposition,
			_In_      DWORD dwFlagsAndAttributes,
			_In_opt_  HANDLE hTemplateFile
		)
		{
			auto instance = MyAPIHooks::GetInstance();
			HANDLE hfile = instance->pOriginalCreateFile(
				lpFileName,//_In_      LPCTSTR lpFileName,
				dwDesiredAccess,//_In_      DWORD dwDesiredAccess,
				dwShareMode,//_In_      DWORD dwShareMode,
				lpSecurityAttributes,//_In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
				dwCreationDisposition,//_In_      DWORD dwCreationDisposition,
				dwFlagsAndAttributes,//_In_      DWORD dwFlagsAndAttributes,
				hTemplateFile//,//_In_opt_  HANDLE hTemplateFile
			);
			return hfile;
		
		}


	public:
		typedef int (WINAPI *pfnconnect)(
			_In_  SOCKET s,
			_In_  const struct sockaddr* name,
			_In_  int namelen
			);
		pfnconnect pOriginalconnect{ nullptr };
		static int WINAPI Myconnect(
			_In_  SOCKET s,
			_In_  const struct sockaddr* name,
			_In_  int namelen
			)
		{
			auto instance = MyAPIHooks::GetInstance();
			int ret = instance->pOriginalconnect(
				s,//	_In_  SOCKET s,
				name,//	_In_  const struct sockaddr* name,
				namelen//,//	_In_  int namelen
			);
			return ret;
		}


	public:

	
		static MyAPIHooks* GetInstance()
		{
			static MyAPIHooks mh;
			return &mh;
		}


	public:
		static void Case1()
		{
#if 1
			auto instance = MyAPIHooks::GetInstance();
			WSADATA wsadata{ 0 };
			WSAStartup(MAKEWORD(2, 2), &wsadata);
			HMODULE  mod = ::GetModuleHandle(L"ws2_32.dll");
			LPVOID proc = ::GetProcAddress(mod, "connect");
			//int boundary = sizeof(void*) == 8 ? 0xf : 0x8;
			int boundary = -1;
			for (int ii = 0; ii < 3; ++ii)
			{
				SmlHook::Hook(proc, MyAPIHooks::Myconnect, boundary, (LPVOID*)& instance->pOriginalconnect);
				int ret = ::connect(SOCKET_ERROR, nullptr, 0);

				SmlHook::Unhook(instance->pOriginalconnect);
				ret = ::connect(SOCKET_ERROR, nullptr, 0);
			}
			

			WSACleanup();
#endif
		}

		static void Case0()
		{
			auto instance = MyAPIHooks::GetInstance();
			HMODULE  mod = ::GetModuleHandle(L"KernelBase.dll");
			LPVOID proc = ::GetProcAddress(mod, "CreateFileW");
			//int boundary = sizeof(void*) == 8 ? 0xf : 0x8;
			int boundary = -1;
			for (int ii = 0; ii < 3; ++ii)
			{
				SmlHook::Hook(proc, MyAPIHooks::MyCreateFile, boundary, (LPVOID*)& instance->pOriginalCreateFile);
				SmlHook::Hook(proc, MyAPIHooks::MyCreateFile, boundary, (LPVOID*)& instance->pOriginalCreateFile);
				HANDLE hfile = ::CreateFileW(L"f:\\111.txt", GENERIC_ALL, 0, nullptr, CREATE_ALWAYS, 0, INVALID_HANDLE_VALUE);
				if (INVALID_HANDLE_VALUE != hfile)
				{
					::CloseHandle(hfile);
				}

				SmlHook::Unhook(instance->pOriginalCreateFile);
				SmlHook::Unhook(instance->pOriginalCreateFile);
				hfile = ::CreateFileW(L"f:\\111.txt", GENERIC_ALL, 0, nullptr, CREATE_ALWAYS, 0, INVALID_HANDLE_VALUE);
				if (INVALID_HANDLE_VALUE != hfile)
				{
					::CloseHandle(hfile);
				}
			}
			
			return;
		}
	};
	

	
}