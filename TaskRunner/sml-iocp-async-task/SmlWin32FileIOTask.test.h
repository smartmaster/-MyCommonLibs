#pragma once
#include "SmlWin32FileIOTask.h"
#include "SmlCommon.h"

namespace SmartLib
{
	class SmlWin32FileIOTaskTest
	{
	public:
		//////////////////////////////////////////////////////////////////////////
		static void Case2()
		{
			CString _srcName = TEXT("f:\\sss.zip");
			CString _destName = TEXT("f:\\ddd.zip");
			Case2_CopyFile(_srcName, _destName);
		}

		static HRESULT Case2_CopyFile(const CString& _srcName, const CString& _destName)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			RefPtr<TaskRunner> sptr = TaskRunner::Instance();
		
			//////////////////////////////////////////////////////////////////////////
			HANDLE _src = ::CreateFile(
				_srcName.GetString(),//_In_      LPCTSTR lpFileName,
				GENERIC_READ,//_In_      DWORD dwDesiredAccess,
				FILE_SHARE_READ,//_In_      DWORD dwShareMode,
				nullptr,//_In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
				OPEN_EXISTING,//_In_      DWORD dwCreationDisposition,
				FILE_FLAG_OVERLAPPED,//_In_      DWORD dwFlagsAndAttributes,
				nullptr//,//_In_opt_  HANDLE hTemplateFile
			);
			if (INVALID_HANDLE_VALUE == _src)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				SML_LOG_LINE(TEXT("api=[%s], file=[%s], error=[%d], hresult=[0x%08x]"), TEXT("CreateFile"), _srcName.GetString(), LastError, hr);
				LEAVE_BLOCK(0);
			}
			RefPtr<IFile*> spsrc = RefPtr<IFile*>::Make(new Win32File{ _src });
			spsrc.SetDispose(Disposer::DeletePtr<IFile>);
			

			//////////////////////////////////////////////////////////////////////////
			HANDLE _dest = ::CreateFile(
				_destName.GetString(),//_In_      LPCTSTR lpFileName,
				GENERIC_WRITE,//_In_      DWORD dwDesiredAccess,
				0,//_In_      DWORD dwShareMode,
				nullptr,//_In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
				CREATE_ALWAYS,//_In_      DWORD dwCreationDisposition,
				FILE_FLAG_OVERLAPPED,//_In_      DWORD dwFlagsAndAttributes,
				nullptr//,//_In_opt_  HANDLE hTemplateFile
			);
			if (INVALID_HANDLE_VALUE == _dest)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				SML_LOG_LINE(TEXT("api=[%s], file=[%s], error=[%d], hresult=[0x%08x]"), TEXT("CreateFile"), _destName.GetString(), LastError, hr);
				LEAVE_BLOCK(0);
			}
			RefPtr<IFile*> spdest = RefPtr<IFile*>::Make(new Win32File{ _dest });
			spdest.SetDispose(Disposer::DeletePtr<IFile>);
			

			//////////////////////////////////////////////////////////////////////////
			RefPtr<CircularBuffer> spcb = RefPtr<CircularBuffer>::Make(BUFFER_SIZE::_CIRCULAR_BUFFER_SIZE);

			//////////////////////////////////////////////////////////////////////////
			RefPtr<IAsyncTask*> reader = RefPtr<IAsyncTask*>::Make(new Win32FileReadAsyncTask{ spsrc, spcb, sptr });
			reader.SetDispose(Disposer::DeletePtr<IAsyncTask>);;
			sptr->AddRunning(reader);
			//ScopeResource<> sr_reader{ [&sptr, &reader]()
			//{
			//	if (sptr->TestSetCleanupFlag(reader.Ref()))
			//	{
			//		sptr->CleanupTask(reader.Ref());
			//	}
			//} };
			//IAsyncTask::RefSelf(reader);
			


			
			RefPtr<IAsyncTask*> writer = RefPtr<IAsyncTask*>::Make(new Win32FileWriteAsyncTask{ spdest, spcb, sptr });
			writer.SetDispose(Disposer::DeletePtr<IAsyncTask>);
			sptr->AddRunning(writer);
			//ScopeResource<> sr_writer{ [&sptr, &writer]()
			//{
			//	if (sptr->TestSetCleanupFlag(writer.Ref()))
			//	{
			//		sptr->CleanupTask(writer.Ref());
			//	}
			//} };
			//IAsyncTask::RefSelf(writer);

			(*reader)->AddRelatedTasks(writer.Ref());

			(*reader)->Create();
			(*writer)->Create();


			(*reader)->Wait();
			(*writer)->Wait();


			//////////////////////////////////////////////////////////////////////////
			//this_thread::sleep_for(chrono::seconds(10));
			//sptr->StopThreads();


			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			/***CLEANUPS***/
			return hr;
		}

		//////////////////////////////////////////////////////////////////////////
		static void Case1()
		{
			CString _srcName = TEXT("f:\\sss.zip");
			CString _destName = TEXT("f:\\ddd.zip");
			Case1_CopyFile(_srcName, _destName);
		}


		static HRESULT Case1_CopyFile(const CString& _srcName, const CString& _destName)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////


			//////////////////////////////////////////////////////////////////////////
			RefPtr<TaskRunner> sptr = RefPtr<TaskRunner>::Make();
			TaskRunner* sptrShadow = sptr.Ptr();
			sptr->Create();
			sptr->StartThreads();

			//////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////
			HANDLE _src = ::CreateFile(
				_srcName.GetString(),//_In_      LPCTSTR lpFileName,
				GENERIC_READ,//_In_      DWORD dwDesiredAccess,
				FILE_SHARE_READ,//_In_      DWORD dwShareMode,
				nullptr,//_In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
				OPEN_EXISTING,//_In_      DWORD dwCreationDisposition,
				FILE_FLAG_OVERLAPPED,//_In_      DWORD dwFlagsAndAttributes,
				nullptr//,//_In_opt_  HANDLE hTemplateFile
			);
			if (INVALID_HANDLE_VALUE == _src)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				SML_LOG_LINE(TEXT("api=[%s], file=[%s], error=[%d], hresult=[0x%08x]"), TEXT("CreateFile"), _srcName.GetString(), LastError, hr);
				LEAVE_BLOCK(0);
			}
			RefPtr<IFile*> spsrc = RefPtr<IFile*>::Make(new Win32File{ _src });
			spsrc.SetDispose(Disposer::DeletePtr<IFile>);

			//////////////////////////////////////////////////////////////////////////
			HANDLE _dest = ::CreateFile(
				_destName.GetString(),//_In_      LPCTSTR lpFileName,
				GENERIC_WRITE,//_In_      DWORD dwDesiredAccess,
				0,//_In_      DWORD dwShareMode,
				nullptr,//_In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
				CREATE_ALWAYS,//_In_      DWORD dwCreationDisposition,
				FILE_FLAG_OVERLAPPED,//_In_      DWORD dwFlagsAndAttributes,
				nullptr//,//_In_opt_  HANDLE hTemplateFile
			);
			if (INVALID_HANDLE_VALUE == _dest)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				SML_LOG_LINE(TEXT("api=[%s], file=[%s], error=[%d], hresult=[0x%08x]"), TEXT("CreateFile"), _destName.GetString(), LastError, hr);
				LEAVE_BLOCK(0);
			}
			RefPtr<IFile*> spdest = RefPtr<IFile*>::Make(new Win32File{ _dest });
			spdest.SetDispose(Disposer::DeletePtr<IFile>);

			//////////////////////////////////////////////////////////////////////////
			RefPtr<CircularBuffer> spcb = RefPtr<CircularBuffer>::Make(BUFFER_SIZE::_CIRCULAR_BUFFER_SIZE);

			//////////////////////////////////////////////////////////////////////////
			RefPtr<IAsyncTask*> reader = RefPtr<IAsyncTask*>::Make(new Win32FileReadAsyncTask{ spsrc, spcb, sptr });
			reader.SetDispose(Disposer::DeletePtr<IAsyncTask>);
			sptr->AddRunning(reader);
			//{
			//	//auto temp{reader};
			//	//(*reader)->RefSelf(temp.Detach());
			//}
			

			RefPtr<IAsyncTask*> writer = RefPtr<IAsyncTask*>::Make(new Win32FileWriteAsyncTask{spdest, spcb, sptr});
			writer.SetDispose(Disposer::DeletePtr<IAsyncTask>);
			sptr->AddRunning(writer);
			//{
			//	auto temp{ reader };
			//	(*writer)->RefSelf(temp.Detach());
			//}

			(*reader)->Create();
			(*writer)->Create();


			(*reader)->Wait();
			(*writer)->Wait();

			//////////////////////////////////////////////////////////////////////////
			this_thread::sleep_for(chrono::seconds(10));
			sptr->StopThreads();


			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			/***CLEANUPS***/
			return hr;
		}

		//////////////////////////////////////////////////////////////////////////
		static void Case0()
		{
			CString _srcName = TEXT("f:\\sss.zip");
			CString _destName = TEXT("f:\\ddd.zip");
			Case0_CopyFile(_srcName, _destName);
		}


		static HRESULT Case0_CopyFile(const CString& _srcName, const CString& _destName)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;
			
			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			
			
			//////////////////////////////////////////////////////////////////////////
			RefPtr<TaskRunner> sptr = RefPtr<TaskRunner>::Make();
			TaskRunner* sptrShadow = sptr.Ptr();
			sptr->Create();
			sptr->StartThreads();

			//////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////
			HANDLE _src = ::CreateFile(
				_srcName.GetString(),//_In_      LPCTSTR lpFileName,
				GENERIC_READ,//_In_      DWORD dwDesiredAccess,
				FILE_SHARE_READ,//_In_      DWORD dwShareMode,
				nullptr,//_In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
				OPEN_EXISTING,//_In_      DWORD dwCreationDisposition,
				FILE_FLAG_OVERLAPPED,//_In_      DWORD dwFlagsAndAttributes,
				nullptr//,//_In_opt_  HANDLE hTemplateFile
			);
			if (INVALID_HANDLE_VALUE == _src)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				SML_LOG_LINE(TEXT("api=[%s], file=[%s], error=[%d], hresult=[0x%08x]"), TEXT("CreateFile"), _srcName.GetString(), LastError, hr);
				LEAVE_BLOCK(0);
			}
			RefPtr<IFile*> spsrc = RefPtr<IFile*>::Make(new Win32File{ _src });
			spsrc.SetDispose(Disposer::DeletePtr<IFile>);

			//////////////////////////////////////////////////////////////////////////
			HANDLE _dest = ::CreateFile(
				_destName.GetString(),//_In_      LPCTSTR lpFileName,
				GENERIC_WRITE,//_In_      DWORD dwDesiredAccess,
				0,//_In_      DWORD dwShareMode,
				nullptr,//_In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
				CREATE_ALWAYS,//_In_      DWORD dwCreationDisposition,
				FILE_FLAG_OVERLAPPED,//_In_      DWORD dwFlagsAndAttributes,
				nullptr//,//_In_opt_  HANDLE hTemplateFile
			);
			if (INVALID_HANDLE_VALUE == _dest)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				SML_LOG_LINE(TEXT("api=[%s], file=[%s], error=[%d], hresult=[0x%08x]"), TEXT("CreateFile"), _destName.GetString(), LastError, hr);
				LEAVE_BLOCK(0);
			}
			RefPtr<IFile*> spdest = RefPtr<IFile*>::Make(new Win32File{ _dest });
			spdest.SetDispose(Disposer::DeletePtr<IFile>);

			//////////////////////////////////////////////////////////////////////////
			RefPtr<CircularBuffer> spcb = RefPtr<CircularBuffer>::Make(BUFFER_SIZE::_CIRCULAR_BUFFER_SIZE);

			//////////////////////////////////////////////////////////////////////////
			RefPtr<Win32FileReadAsyncTask> reader = RefPtr<Win32FileReadAsyncTask>::Make(spsrc, spcb, sptr);
			RefPtr<Win32FileWriteAsyncTask> writer = RefPtr<Win32FileWriteAsyncTask>::Make(spdest, spcb, sptr);

			reader->Create();
			writer->Create();


			reader->Wait();
			writer->Wait();

			//////////////////////////////////////////////////////////////////////////
			sptr->StopThreads();


			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			/***CLEANUPS***/
			return hr;
		}
	};
}