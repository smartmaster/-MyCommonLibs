#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>


#include "../SmlRefPtr.h"


namespace SmartLib
{

	class IAsyncTask
	{
	public:
		virtual ~IAsyncTask() {};


		//virtual void RefSelf(IAsyncTask** self) = 0;
		virtual HRESULT Create() = 0;
		virtual HRESULT OnNext(long transffered, LPOVERLAPPED lpoverlapped) = 0;
		virtual HRESULT OnError(long error, LPOVERLAPPED lpoverlapped) = 0;
		virtual HRESULT Wait() = 0;
		virtual void AddRelatedTasks(IAsyncTask* task) = 0;

	//public:
	//	static void RefSelf(RefPtr<IAsyncTask*> sp)
	//	{
	//		IAsyncTask** pptask = sp.Detach();
	//		(*pptask)->RefSelf(pptask);
	//	}
	};
}