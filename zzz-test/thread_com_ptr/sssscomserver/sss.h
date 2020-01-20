// sss.h : Declaration of the Csss

#pragma once
#include "resource.h"       // main symbols



#include "sssscomserver_i.h"

#include <fstream>



using namespace ATL;


// Csss

class ATL_NO_VTABLE Csss :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<Csss, &CLSID_sss>,
	public Isss
{
private:
	std::ofstream m_out;
	LONG m_index{ 0 };
	LONG m_funId{ 0 };
public:
	Csss()
	{
		m_out.open("c:\\some_test.txt");
	}

DECLARE_REGISTRY_RESOURCEID(IDR_SSS)


BEGIN_COM_MAP(Csss)
	COM_INTERFACE_ENTRY(Isss)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

	virtual HRESULT STDMETHODCALLTYPE Do(void)
	{
		LONG funid = 0;
		{
			ObjectLock lock(this);
			funid = ++m_funId;
		}
		
		for (long ii = 0; ii < 3; ++ ii)
		{
			//_ftprintf_s(stdout, TEXT("running in tid %d"), GetCurrentThreadId());
			{
				ObjectLock lock(this);
				m_out << ++m_index << ", " << funid << ", " << " running in tid " << GetCurrentThreadId() << std::endl;
			}
			
			Sleep(300);
		}
		return S_OK;
	}

	~Csss()
	{
		/*_ftprintf_s(stdout, TEXT("dtor called in tid %d"), GetCurrentThreadId());*/
		m_out << "dtor called in tid " << GetCurrentThreadId() << std::endl;
		m_out.close();
	}

public:



};

OBJECT_ENTRY_AUTO(__uuidof(sss), Csss)
