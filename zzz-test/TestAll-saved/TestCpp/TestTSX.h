#pragma once

/*
Copyright (c) 2012, Intel Corporation

Intel Sample Source Code License Agreement

This license governs use of the accompanying software. By installing or copying all or any part of the software components in this package, you (“you?or “Licensee? agree to the terms of this agreement.  Do not install or copy the software until you have carefully read and agreed to the following terms and conditions.  If you do not agree to the terms of this agreement, promptly return the software to Intel Corporation (“Intel?.

1.      Definitions:
A.    “Materials" are defined as the software (including the Redistributables and Sample Source as defined herein), documentation, and other materials, including any updates and upgrade thereto, that are provided to you under this Agreement.
B.    "Redistributables" are the files listed in the "redist.txt" file that is included in the Materials or are otherwise clearly identified as redistributable files by Intel.
C.    “Sample Source?is the source code file(s) that: (i) demonstrate(s) certain functions for particular purposes; (ii) are identified as sample source code; and (iii) are provided hereunder in source code form.
D.    “Intel’s Licensed Patent Claims?means those claims of Intel’s patents that (a) are infringed by the Sample Source or Redistributables, alone and not in combination, in their unmodified form, as furnished by Intel to Licensee and (b) Intel has the right to license.
2.      License Grant:  Subject to all of the terms and conditions of this Agreement:
A.   Intel grants to you a non-exclusive, non-assignable, copyright license to use the Material for your internal development purposes only.
B.   Intel grants to you a non-exclusive, non-assignable copyright license to reproduce the Sample Source, prepare derivative works of the Sample Source and distribute the Sample Source or any derivative works thereof that you create, as part of the product or application you develop using the Materials.
C.    Intel grants to you a non-exclusive, non-assignable copyright license to distribute the Redistributables, or any portions thereof, as part of the product or application you develop using the Materials.
D.   Intel grants Licensee a non-transferable, non-exclusive, worldwide, non-sublicenseable license under Intel’s Licensed Patent Claims to make, use, sell, and import the Sample Source and the Redistributables.
3.      Conditions and Limitations:
A.   This license does not grant you any rights to use Intel’s name, logo or trademarks.
B.   Title to the Materials and all copies thereof remain with Intel.  The Materials are copyrighted and are protected by United States copyright laws.  You will not remove any copyright notice from the Materials.  You agree to prevent any unauthorized copying of the Materials.  Except as expressly provided herein, Intel does not grant any express or implied right to you under Intel patents, copyrights, trademarks, or trade secret information.
C.   You may NOT:  (i) use or copy the Materials except as provided in this Agreement; (ii) rent or lease the Materials to any third party; (iii) assign this Agreement or transfer the Materials without the express written consent of Intel; (iv) modify, adapt, or translate the Materials in whole or in part except as provided in this Agreement; (v) reverse engineer, decompile, or disassemble the Materials not provided to you in source code form; or (vii) distribute, sublicense or transfer the source code form of any components of the Materials and derivatives thereof to any third party except as provided in this Agreement.

4.            No Warranty:
THE MATERIALS ARE PROVIDED “AS IS?  INTEL DISCLAIMS ALL EXPRESS OR IMPLIED WARRANTIES WITH RESPECT TO THEM, INCLUDING ANY IMPLIED WARRANTIES OF MERCHANTABILITY, NON-INFRINGEMENT, AND FITNESS FOR ANY PARTICULAR PURPOSE.
5.                Limitation of Liability:  NEITHER INTEL NOR ITS SUPPLIERS SHALL BE LIABLE FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR OTHER LOSS) ARISING OUT OF THE USE OF OR INABILITY TO USE THE SOFTWARE, EVEN IF INTEL HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.  BECAUSE SOME JURISDICTIONS PROHIBIT THE EXCLUSION OR LIMITATION OF LIABILITY FOR CONSEQUENTIAL OR INCIDENTAL DAMAGES, THE ABOVE LIMITATION MAY NOT APPLY TO YOU.
6.                 USER SUBMISSIONS:  You agree that any material, information or other communication, including all data, images, sounds, text, and other things embodied therein, you transmit or post to an Intel website or provide to Intel under this Agreement will be considered non-confidential ("Communications").  Intel will have no confidentiality obligations with respect to the Communications.  You agree that Intel and its designees will be free to copy, modify, create derivative works, publicly display, disclose, distribute, license and sublicense through multiple tiers of distribution and licensees, incorporate and otherwise use the Communications, including derivative works thereto, for any and all commercial or non-commercial purposes
7.                 TERMINATION OF THIS LICENSE: This Agreement becomes effective on the date you accept this Agreement and will continue until terminated as provided for in this Agreement.  Intel may terminate this license at any time if you are in breach of any of its terms and conditions.  Upon termination, you will immediately return to Intel or destroy the Materials and all copies thereof.
8.                 U.S. GOVERNMENT RESTRICTED RIGHTS: The Materials are provided with "RESTRICTED RIGHTS". Use, duplication or disclosure by the Government is subject to restrictions set forth in FAR52.227-14 and DFAR252.227-7013 et seq. or its successor.  Use of the Materials by the Government constitutes acknowledgment of Intel's rights in them.
9.                 APPLICABLE LAWS: Any claim arising under or relating to this Agreement shall be governed by the internal substantive laws of the State of Delaware, without regard to principles of conflict of laws.  You may not export the Materials in violation of applicable export laws.

*/

// written by Roman Dementiev

#include "stdafx.h"
#include <windows.h>
#include <process.h>
#include <immintrin.h>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <random>

namespace TestCpp
{
	class TestTsx
	{

	private:
		class SimpleSpinLock
		{
			volatile unsigned int state;

			enum { Free = 0, Busy = 1 };

		public:
			SimpleSpinLock() : state(Free) {}

			void lock()
			{
				while (InterlockedCompareExchange(&state, Busy, Free) == Busy)
				{
					do { _mm_pause(); } while (state == Busy);
				}
			}

			void unlock() { InterlockedExchange(&state, Free); }

			bool isLocked() const { return state == Busy; }

		} globalFallBackLock;

#if 0
		class TransactionScope // implementation using a legacy lock
		{
			SimpleSpinLock & lock;
			TransactionScope(); // forbidden
		public:
			TransactionScope(SimpleSpinLock & lock_) : lock(lock_) { lock.lock(); }

			~TransactionScope() { lock.unlock(); }
		};
#endif


		

	private:
		class TransactionScope
		{
			SimpleSpinLock & fallBackLock;

			TransactionScope(); // forbidden
		public:
			TransactionScope(SimpleSpinLock & fallBackLock_, int max_retries = 3) : fallBackLock(fallBackLock_)
			{
#if 1
				int nretries = 0;

				while (1)
				{
					++nretries;
					unsigned status = _xbegin();

					if (status == _XBEGIN_STARTED)
					{
						if (!fallBackLock.isLocked()) return; // successfully started transaction
															  // started transaction but someone executes the transaction section 
															  // non-speculatively (acquired the fall-back lock)
						_xabort(0xff); // abort with code 0xff
					}
					// abort handler

					InterlockedIncrement64(&(NAborted())); // do abort statistics

					std::cout << "DEBUG: Transaction aborted " << nretries << " time(s) with the status " << status << std::endl;

					// handle _xabort(0xff) from above
					if ((status & _XABORT_EXPLICIT) && _XABORT_CODE(status) == 0xff && !(status & _XABORT_NESTED))
					{
						while (fallBackLock.isLocked()) _mm_pause(); // wait until lock is free

					}
					else if (!(status & _XABORT_RETRY)) break; // take the fall-back lock if the retry abort flag is not set

					if (nretries >= max_retries) break; // too many retries, take the fall-back lock
				}

				fallBackLock.lock();

#else // the naive version goes here
				int nretries = 0;

				while (1)
				{
					++nretries;
					unsigned status = _xbegin();

					if (status == _XBEGIN_STARTED) return;

					std::cout << "DEBUG: Transaction aborted " << nretries << " time(s) with the status " << status << std::endl;
				}
#endif
			}

			~TransactionScope()
			{
				if (fallBackLock.isLocked())
					fallBackLock.unlock();
				else
					_xend();
			}
		};


	private:
		std::vector<int> Accounts;
		//static LONGLONG naborted;
		static LONGLONG& NAborted()
		{
			static LONGLONG naborted = 0;
			return naborted;
		}

		int global = 0;

		unsigned __stdcall thread_worker_method(int arg)
		{
			int thread_nr = (int)arg;
			std::cout << "Thread " << thread_nr << " started." << std::endl;
			std::tr1::minstd_rand myRand(thread_nr);
			long int loops = 10000;

			while (--loops)
			{
				{
					TransactionScope guard(globalFallBackLock); // protect everything in this scope with RTM
																// put 100 units into a random account atomically
					Accounts[myRand() % Accounts.size()] += 100;
				}

				{
					TransactionScope guard(globalFallBackLock); // protect everything in this scope with RTM
																// transfer 100 units between random accounts (if there is enough money) atomically
					int a = myRand() % Accounts.size(), b = myRand() % Accounts.size();
					if (Accounts[a] >= 100)
					{
						Accounts[a] -= 100;
						Accounts[b] += 100;
					}
				}
			}

			std::cout << "Thread " << thread_nr << " finished." << std::endl;

			return 0;
		}


		struct ThreadArgs
		{
			TestTsx * _p{ nullptr };
			int _tnr{0};
		};


		static unsigned __stdcall thread_worker_proc(void * arg)
		{
			ThreadArgs * args = (ThreadArgs*)(arg);
			return args->_p->thread_worker_method(args->_tnr);
		}

		int Case0(int argc, _TCHAR* argv[])
		{
			{
				std::cout << "open new account" << std::endl;
				TransactionScope guard(globalFallBackLock); // protect everything in this scope with RTM
				Accounts.push_back(0);
			}
			{
				std::cout << "open new account" << std::endl;
				TransactionScope guard(globalFallBackLock); // protect everything in this scope with RTM
				Accounts.push_back(0);
			}
			{
				std::cout << "put 100 units into account 0" << std::endl;
				TransactionScope guard(globalFallBackLock); // protect everything in this scope with RTM
				Accounts[0] += 100; // atomic update due to RTM
			}
			{
				std::cout << "transfer 10 units from account 0 to account 1 atomically!" << std::endl;
				TransactionScope guard(globalFallBackLock); // protect everything in this scope with RTM
				Accounts[0] -= 10;
				Accounts[1] += 10;
			}
			{
				std::cout << "atomically draw 10 units from account 0 if there is enough money" << std::endl;
				TransactionScope guard(globalFallBackLock); // protect everything in this scope with RTM
				if (Accounts[0] >= 10) Accounts[0] -= 10;
			}
			{
				std::cout << "add 1000 empty accounts atomically" << std::endl;
				TransactionScope guard(globalFallBackLock); // protect everything in this scope with RTM
				Accounts.resize(Accounts.size() + 1000, 0);
			}


			ThreadArgs arg1
			{
				this,
				1
			};

			ThreadArgs arg2
			{
				this,
				2
			};
			HANDLE thread1 = (HANDLE)_beginthreadex(NULL, 0, &thread_worker_proc, (void *)&arg1, 0, NULL);
			HANDLE thread2 = (HANDLE)_beginthreadex(NULL, 0, &thread_worker_proc, (void *)&arg2, 0, NULL);

			WaitForSingleObject(thread1, INFINITE);
			WaitForSingleObject(thread2, INFINITE);

			std::cout << "Total number of transaction aborts: " << NAborted() << std::endl;

			return 0;
		}


		public:
			static void Case0()
			{
				TestTsx tsx;
				tsx.Case0(0, nullptr);
			}

	}; //class xxxx

} //namespace TestCpp