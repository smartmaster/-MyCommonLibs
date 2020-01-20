#pragma once

#include <functional>
#include <memory>

//used for scope resource management
#define RESOURCE_NEW(res)
#define RESOURCE_DELETE(res)
#define RESOURCE_AUTO_DELETE(res)
#define RESOURCE_CHANGE_OWNER(res)
#define RESOURCE_NEW_OWER(res)

//used for class members declaration
#define RESOURCE_INTERNAL(res)
#define RESOURCE_INCOMMING(res)
#define RESOURCE_TO_CLEANUP(res)

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//helper macros for internal use
#define auto_deleter_func_name(var)											auto_deleter_func_name ## __ ## var
#define auto_deleter_unique_ptr_name(var)									auto_deleter_unique_ptr_name ## __ ## var
#define auto_delete_type_obj(type, var)										std::unique_ptr<type, decltype(auto_deleter_func_name(var))> auto_deleter_unique_ptr_name(var)(&var, auto_deleter_func_name(var))
#define auto_delete_type_obj_reuse_deleter(type, var, reuse_deleter)		std::unique_ptr<type, decltype(auto_deleter_func_name(reuse_deleter))> auto_deleter_unique_ptr_name(var)(&var, auto_deleter_func_name(reuse_deleter))

//////////////////////////////////////////////////////////////////////////
//macros to use
#define auto_deleter_func(var)												auto auto_deleter_func_name(var)
#define auto_delete_obj(var)												auto_delete_type_obj(decltype(var), var)
#define auto_delete_obj_reuse_deleter(var, reuse_deleter)					auto_delete_type_obj_reuse_deleter(decltype(var), var, reuse_deleter)
#define mannual_delete_obj(var)												auto_deleter_unique_ptr_name(var).reset(nullptr)

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
class CScopeCleanup
{
	std::function<void()> m_cleanup_fn;
public:
	CScopeCleanup(std::function<void()> cleanup_fn) :
		m_cleanup_fn(cleanup_fn)
	{
		int size = sizeof(m_cleanup_fn);
	}

	void Cleanup()
	{
		if (m_cleanup_fn)
		{
			m_cleanup_fn();
			m_cleanup_fn = nullptr;
		}
	}

	void Detach()
	{
		m_cleanup_fn = nullptr;
	}

	~CScopeCleanup()
	{
		if (m_cleanup_fn)
		{
			m_cleanup_fn();
			m_cleanup_fn = nullptr;
		}
	}
};


#define SCOPE_AUTO_CLEANUP(name)				CScopeCleanup MyScopeCleanup_##name
#define SCOPE_EXPLICIT_CLEANUP(name)			MyScopeCleanup_##name.Cleanup()
#define SCOPE_DETACH(name)						MyScopeCleanup_##name.Detach()
//////////////////////////////////////////////////////////////////////////

#if 0 //unit test
int _tmain_auto_delete(int argc, _TCHAR* argv[])
{
	using namespace std;

	bool b_mannual_delete = true;

	//////////////////////////////////////////////////////////////////////////
	HANDLE hFile = /*HANDLE WINAPI */CreateFile(
		TEXT("\\\\?\\\\C:"),//_In_      LPCTSTR lpFileName,
		GENERIC_READ,//_In_      DWORD dwDesiredAccess,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,//_In_      DWORD dwShareMode,
		nullptr,//_In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		OPEN_EXISTING,//_In_      DWORD dwCreationDisposition,
		0,//_In_      DWORD dwFlagsAndAttributes,
		nullptr//_In_opt_  HANDLE hTemplateFile
		);

	HANDLE win32_file_handle = INVALID_HANDLE_VALUE;
	auto_deleter_func(win32_file_handle) = [](HANDLE * phFile)
	{
		if (phFile && *phFile != INVALID_HANDLE_VALUE)
		{
			CloseHandle(*phFile);
			*phFile = INVALID_HANDLE_VALUE;
		}
	};
	auto_delete_obj_reuse_deleter(hFile, win32_file_handle);
	//use hfile
	//read file
	if (b_mannual_delete)
	{
		mannual_delete_obj(hFile);
	}


	//////////////////////////////////////////////////////////////////////////
	LONGLONG * pll = new LONGLONG[100];
	auto_deleter_func(pll) = [](LONGLONG ** ppll)
	{
		if (ppll && *ppll)
		{
			delete[] * ppll;
			*ppll = nullptr;
		}
	};
	auto_delete_obj(pll);

	//////////////////////////////////////////////////////////////////////////
	//use pll
	pll[0] = 0;
	pll[1] = 1;
	for (int ii = 2; ii < 100; ++ii)
	{
		pll[ii] = pll[ii - 1] + pll[ii - 2];
	}

	if (b_mannual_delete)
	{
		mannual_delete_obj(pll);
	}

	//////////////////////////////////////////////////////////////////////////
	vector<BYTE*> vec;
	auto_deleter_func(vec) = [](vector<BYTE*> * pvec)
	{
		for (auto & var : *pvec)
		{
			free(var);
			var = nullptr;
		}
		pvec->clear();
	};
	auto_delete_obj(vec);

	// use vec
	for (size_t ii = 0; ii < 7; ++ii)
	{
		vec.push_back((BYTE*)malloc(64));
	}

	if (b_mannual_delete)
	{
		mannual_delete_obj(vec);
	}

	//////////////////////////////////////////////////////////////////////////
	HANDLE m_handle_sdfl = INVALID_HANDLE_VALUE;
	SCOPE_AUTO_CLEANUP(m_handle_sdfl)([&m_handle_sdfl]()
	{
		if (m_handle_sdfl != INVALID_HANDLE_VALUE && m_handle_sdfl != nullptr)
		{
			::CloseHandle(m_handle_sdfl);
			m_handle_sdfl = INVALID_HANDLE_VALUE;
		}
	});
	SCOPE_EXPLICIT_CLEANUP(m_handle_sdfl);

	//////////////////////////////////////////////////////////////////////////
	int * ptr_to_uvhahsf = new int[100];
	SCOPE_AUTO_CLEANUP(ptr_to_uvhahsf)([&ptr_to_uvhahsf]()
	{
		if (ptr_to_uvhahsf != nullptr)
		{
			delete[] ptr_to_uvhahsf;
			ptr_to_uvhahsf = nullptr;
		}
	});
	SCOPE_DETACH(ptr_to_uvhahsf); //memory leak for test purpose
	SCOPE_EXPLICIT_CLEANUP(ptr_to_uvhahsf);
	delete[] ptr_to_uvhahsf; ptr_to_uvhahsf = nullptr;

	return 0;

	//////////////////////////////////////////////////////////////////////////
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	_tmain_auto_delete(argc, argv);
	return 0;
}
#endif