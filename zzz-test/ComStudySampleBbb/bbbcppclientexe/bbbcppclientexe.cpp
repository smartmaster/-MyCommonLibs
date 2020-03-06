// bbbcppclientexe.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <atlsafe.h>

#include <vector>

#import "..\bbbcomnet\bin\Debug\bbbcomnet.tlb" raw_interfaces_only

namespace
{
#define GET_INTEGER(name) { \
	pobj->get_ ## name (&data. ## name ); \
	}

#define GET_BSTR(name) { \
	CComBSTR bstrTmp; \
	pobj->get_ ## name (&bstrTmp); \
	data. ## name  = bstrTmp; \
	}

#define  GET_ARRAY(name, com_type) { \
	SAFEARRAY * psatmp = nullptr; \
	pobj->get_ ## name(&psatmp); \
	CComSafeArray<com_type> satemp; \
	satemp.Attach(psatmp); psatmp = nullptr; \
	SafeArrayToVector(satemp, data.## name); \
	}

#define GET_COMPLEX(name, interface_type, complex_type) { \
	CComPtr<interface_type> spIComplexData; \
	pobj->get_ ## name(&spIComplexData); \
	complex_type::InterfaceToData(spIComplexData, data. ## name); \
	}

#define GET_COMPLEX_ARRAY(name, interface_type, complex_type) { \
	SAFEARRAY * psacomplex = nullptr; \
	pobj->get_ ## name(&psacomplex); \
	CComSafeArray<IUnknown*> sacomplex; \
	sacomplex.Attach(psacomplex); psacomplex = nullptr; \
	SafeArrayIUnknownToVector<interface_type, complex_type>(sacomplex, data. ## name); \
	}


	template<typename T1, typename T2 = T1>
	void SafeArrayToVector(CONST CComSafeArray<T1> & sa, std::vector<T2> & vec)
	{
		vec.resize(sa.GetCount());
		for (size_t ii = 0; ii < vec.size(); ++ ii)
		{
			vec[ii] = sa[(int)ii];
		}
	}

	template<typename T1, typename T2>
	void SafeArrayIUnknownToVector(CONST CComSafeArray<IUnknown*> & sa, std::vector<T2> & vec)
	{
		vec.resize(sa.GetCount());
		for (int ii = 0; ii < sa.GetCount(); ++ii)
		{
			CComPtr<IUnknown> spIUnknown = sa[ii];
			CComPtr<T1> spT1;
			spIUnknown.QueryInterface(&spT1);
			T2::InterfaceToData(spT1, vec[ii]);
		}
	}

	struct CComplexData
	{
		LONGLONG lval;
		CString sval;
		std::vector<LONGLONG> larr;
		std::vector<CString> sarr;


		static void InterfaceToData(bbbcomnet::IComplexData * pobj, CComplexData & data)
		{
			GET_INTEGER(lval);
			GET_BSTR(sval);
			GET_ARRAY(larr, LONGLONG);
			GET_ARRAY(sarr, BSTR);
		}

	};
	
	struct CMoreComplexData
	{
		LONGLONG lval;
		CString sval;
		std::vector<LONGLONG> larr;
		std::vector<CString> sarr;
		CComplexData complex;
		std::vector<CComplexData> arrcomplex;

		static void InterfaceToData(bbbcomnet::IMoreComplexData * pobj, CMoreComplexData & data)
		{
			GET_INTEGER(lval);
			GET_BSTR(sval);
			GET_ARRAY(larr, LONGLONG);
			GET_ARRAY(sarr, BSTR);
			GET_COMPLEX(complex, bbbcomnet::IComplexData, CComplexData);
			GET_COMPLEX_ARRAY(arrcomplex, bbbcomnet::IComplexData, CComplexData);
		}
	};
}

int _tmain(int arc, TCHAR ** argv)
{
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	CComSafeArray<IUnknown*> arrIMoreComplexData;
	for (int ii = 0; ii < 3; ++ii)
	{
		CComPtr<bbbcomnet::IMoreComplexData> spIMoreComplexData;
		spIMoreComplexData.CoCreateInstance(__uuidof(bbbcomnet::CMoreComplexData));
		spIMoreComplexData->Init();
		arrIMoreComplexData.Add(spIMoreComplexData);
	}

	CComPtr<bbbcomnet::ItestList> spItestList;
	spItestList.CoCreateInstance(__uuidof(bbbcomnet::CtestList));

	SAFEARRAY * psa = nullptr;
	spItestList->Mmm3(arrIMoreComplexData, &psa);

	CComSafeArray<IUnknown*> saout;
	saout.Attach(psa); psa = nullptr;
	std::vector<CMoreComplexData> vec;
	SafeArrayIUnknownToVector<bbbcomnet::IMoreComplexData, CMoreComplexData>(saout, vec);

	CoUninitialize();
    return 0;
}

