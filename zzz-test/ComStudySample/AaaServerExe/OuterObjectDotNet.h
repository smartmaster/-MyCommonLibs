// OuterObjectDotNet.h : Declaration of the COuterObjectDotNet

#pragma once
#include "resource.h"       // main symbols



#include "AaaServerExe_i.h"

#import "../AaaServerDllCs/bin/Debug/AaaServerDllCs.tlb" raw_interfaces_only

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;


static LONGLONG s_index = 0;

// COuterObjectDotNet

class ATL_NO_VTABLE COuterObjectDotNet :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<COuterObjectDotNet, &CLSID_OuterObjectDotNet>,
	public IDispatchImpl<IOuterObjectDotNet, &IID_IOuterObjectDotNet, &LIBID_AaaServerExeLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
private:
	LONGLONG m_index{ 0 };

public:
	COuterObjectDotNet()
	{
		m_index = InterlockedIncrement64(&s_index);
	}

DECLARE_REGISTRY_RESOURCEID(IDR_OUTEROBJECTDOTNET)


BEGIN_COM_MAP(COuterObjectDotNet)
	COM_INTERFACE_ENTRY(IOuterObjectDotNet)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY_AGGREGATE(__uuidof(AaaServerDllCs::IHello), m_pIUnknownIHello) //<sonmi01>2016-1-4 ###???
	COM_INTERFACE_ENTRY_AGGREGATE(__uuidof(AaaServerDllCs::IParam), m_pIUnknownIParam) //<sonmi01>2016-1-4 ###???

END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	DECLARE_GET_CONTROLLING_UNKNOWN() 

	HRESULT FinalConstruct()
	{
		HRESULT hr = S_OK;
		BEGIN_BLOCK(0); 

		//<sonmi01>2016-1-4 ###???
		hr = CreateAggregatedIUnknown(__uuidof(AaaServerDllCs::CHello), &m_pIUnknownIHello);
		if (FAILED(hr))
		{
			//_OLD_DSETLASTSTATUS(hr);
			//_OLD_DAPIERR((0, DRNPOS, TEXT("CoCreateInstance"), hr));
			break;
		}

		//<sonmi01>2016-1-4 ###???
		hr = CreateAggregatedIUnknown(__uuidof(AaaServerDllCs::CParam), &m_pIUnknownIParam);
		if (FAILED(hr))
		{
			//_OLD_DSETLASTSTATUS(hr);
			//_OLD_DAPIERR((0, DRNPOS, TEXT("CoCreateInstance"), hr));
			break;
		}

		END_BLOCK(0);
		return hr;
	}

	void FinalRelease()
	{

		//<sonmi01>2016-1-4 ###???
		if (m_pIUnknownIHello)
		{
			CComPtr<IUnknown> spTemp;
			spTemp.Attach(m_pIUnknownIHello);
			m_pIUnknownIHello = NULL;
		}

		//<sonmi01>2016-1-4 ###???
		if (m_pIUnknownIParam)
		{
			CComPtr<IUnknown> spTemp;
			spTemp.Attach(m_pIUnknownIParam);
			m_pIUnknownIParam = NULL;
		}


		_ftprintf(stdout, TEXT("%I64d %I64d \r\n"), s_index, m_index);
	}

public:


private:
	IUnknown * m_pIUnknownIHello{ nullptr }; //<sonmi01>2016-1-4 ###???
	IUnknown * m_pIUnknownIParam{ nullptr }; //<sonmi01>2016-1-4 ###???


private:
	HRESULT CreateAggregatedIUnknown(REFCLSID clsId, IUnknown ** ppobj)
	{
		CComPtr<IUnknown> spIUnknown;
		HRESULT hr = spIUnknown.CoCreateInstance(clsId, GetControllingUnknown(), CLSCTX_INPROC);
		if (FAILED(hr))
		{
			//_OLD_DSETLASTSTATUS(hr);
			//_OLD_DAPIERR((0, DRNPOS, TEXT("CoCreateInstance"), hr));
			//break;
		}
		else
		{
			*ppobj = spIUnknown.Detach();
		}
		return hr;
	}
public:
	STDMETHOD(Nop)();
};

OBJECT_ENTRY_AUTO(__uuidof(OuterObjectDotNet), COuterObjectDotNet)
