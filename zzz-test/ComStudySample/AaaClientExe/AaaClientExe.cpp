// AaaClientExe.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <atlsafe.h>

#include "..\AaaServerDll\AaaServerDll_i.h"
#include "..\AaaServerExe\AaaServerExe_i.h"

#import "../AaaServerDllCs/bin/Debug/AaaServerDllCs.tlb" raw_interfaces_only

template<class ITF>
HRESULT CallMethod(ITF * pItf)
{
	//D_START(0);
	LONG Length = 0;
	WCHAR * pBuffer = NULL;
	HRESULT hr = pItf->OutDynamicArray(
		&Length,//[out] LONG* pLength, 
		&pBuffer//[out, size_is(, * pLength)] WCHAR ** ppBuffer
		);
	//_OLD_DINFO((0, DRNPOS, TEXT("String [%s]"), pBuffer));

	BEGIN_BLOCK(999);
	CComHeapPtr<WCHAR> apTemp(pBuffer);
	pBuffer = NULL;
	END_BLOCK(999);
	
	return hr;
}

int _tmain_client(int argc, _TCHAR* argv[])
{
	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);

	CComPtr<IOuterObject> spIOuterObject;
	hr = spIOuterObject.CoCreateInstance(__uuidof(OuterObject));
	if (FAILED(hr))
	{
		//_OLD_DSETLASTSTATUS(hr);
		//_OLD_DAPIERR((0, DRNPOS, TEXT("CoCreateInstance"), hr));
		break;
	}

	hr = CallMethod(spIOuterObject.p);
	if (FAILED(hr))
	{
		//_OLD_DSETLASTSTATUS(hr);
		//_OLD_DAPIERR((0, DRNPOS, TEXT("CallMethod"), hr));
		break;
	}

	CComPtr<IInnerObject> spIInnerObject;
	hr = spIOuterObject.QueryInterface(&spIInnerObject);
	if (FAILED(hr))
	{
		//_OLD_DSETLASTSTATUS(hr);
		//_OLD_DAPIERR((0, DRNPOS, TEXT("QueryInterface"), hr));
		break;
	}

	hr = CallMethod(spIInnerObject.p);
	if (FAILED(hr))
	{
		//_OLD_DSETLASTSTATUS(hr);
		//_OLD_DAPIERR((0, DRNPOS, TEXT("CallMethod"), hr));
		break;
	}

	CComPtr<IOuterObject> spIOuterObject2;
	spIInnerObject.QueryInterface(&spIOuterObject2);
	
	spIOuterObject2.Release();
	spIInnerObject.Release();
	spIOuterObject.Release();

	///////////////////////////////////////////////////////////////////////////////////////////////////
	if (1)
	{
		//////////////////////////////////////////////////////////////////////////
		CComPtr<IOuterObjectDotNet> spIOuterObjectDotNet;
		hr = spIOuterObjectDotNet.CoCreateInstance(__uuidof(OuterObjectDotNet));

		CComPtr<AaaServerDllCs::IParam> spIParam;
		hr = spIOuterObjectDotNet.QueryInterface(&spIParam);

		CComSafeArray<LONGLONG> sall;
		sall.Create(3);
		for (int ii = 0; ii < sall.GetCount(); ++ii)
		{
			sall[ii] = (ii + 1) * 100;
		}
		spIParam->put_ArrayData(sall);
		spIParam->put_Message(CComBSTR(TEXT("hello cpp")));
		spIParam->put_Value(100LL);

		CComPtr<AaaServerDllCs::IHello> spIHello;
		hr = spIOuterObjectDotNet.QueryInterface(&spIHello);
		long oval = 0;
		CComBSTR ostr;

		CComSafeArray<LONGLONG> sa;
		sa.Create(5);
		for (int ii = 0; ii < sa.GetCount(); ++ii) { sa[ii] = (ii + 1) * 10000; }
		SAFEARRAY * osaTmp = nullptr;
		CComPtr<AaaServerDllCs::IParam> oparam;
		hr = spIHello->Say(
			999,//	/*[in]*/ long val,
			&oval,//	/*[out]*/ long * oval,
			CComBSTR(TEXT("this is some text")),//	/*[in]*/ BSTR str,
			&ostr,//	/*[out]*/ BSTR * ostr,
			sa,
			&osaTmp,
			spIParam,///*[in]*/ struct IParam * param,
			&oparam//,///*[out]*/ struct IParam * * oparam
			);
		CComSafeArray<LONGLONG> ossa;
		ossa.Attach(osaTmp); osaTmp = nullptr;

		SAFEARRAY * psa = nullptr;
		oparam->get_ArrayData(&psa);
		CComSafeArray<LONGLONG> osa;
		osa.Attach(psa); psa = nullptr;

		CComBSTR omessage;
		oparam->get_Message(&omessage);

		LONGLONG ollval;
		oparam->get_Value(&ollval);

		oparam.Release();

		spIHello.Release();
	}
	
	END_BLOCK(0);

	return hr;
}

int _tmain(int argc, _TCHAR* argv[])
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	_tmain_client(argc, argv);
	CoUninitialize();
	return 0;
}

