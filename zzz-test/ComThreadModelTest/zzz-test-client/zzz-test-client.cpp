// zzz-test-client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "..\Single\Single_i.h"
#include "..\Apartment\Apartment_i.h"
#include "..\Both\Both_i.h"
#include "..\Free\Free_i.h"
#include "..\Neutral\Neutral_i.h"

#define test_thread_model(model) \
CComPtr<IObj##model> spIObj##model; \
hr = spIObj##model.CoCreateInstance(__uuidof(Obj##model)); \
hr = spIObj##model->Print(); \
spIObj##model.Release();

HRESULT TestThreadModue(DWORD dwCoInit)
{
	CoInitializeEx(NULL, dwCoInit);
	if (COINIT_MULTITHREADED == dwCoInit)
	{
		_tprintf_s(TEXT("Enter MULTITHREADED apartment %d") TEXT("\r\n"), GetCurrentThreadId());
	}
	else if (COINIT_APARTMENTTHREADED == dwCoInit)
	{
		_tprintf_s(TEXT("Enter APARTMENTTHREADED apartment %d") TEXT("\r\n"), GetCurrentThreadId());
	}

	{
		HRESULT hr = S_OK;

		//CComPtr<IObjSingle> spIObjSingle;
		//hr = spIObjSingle.CoCreateInstance(__uuidof(ObjSingle));
		//hr = spIObjSingle->Print();
		//spIObjSingle.Release();

		test_thread_model(Single);
		test_thread_model(Apartment);
		test_thread_model(Both);
		test_thread_model(Free);
		test_thread_model(Neutral);
	}

	if (COINIT_MULTITHREADED == dwCoInit)
	{
		_tprintf_s(TEXT("Leave MULTITHREADED apartment %d") TEXT("\r\n"), GetCurrentThreadId());
	}
	else if (COINIT_APARTMENTTHREADED == dwCoInit)
	{
		_tprintf_s(TEXT("Leave APARTMENTTHREADED apartment %d") TEXT("\r\n"), GetCurrentThreadId());
	}
	CoUninitialize();


	return S_OK;
}

int _tmain(int argc, _TCHAR* argv[])
{
	TestThreadModue(COINIT_APARTMENTTHREADED);
	TestThreadModue(COINIT_MULTITHREADED);
	return 0;
}

