#pragma once

#include "stdafx.h"

#include <vector>
using namespace std;

#include <atlsafe.h>

#include "CodeBlockDef.h"

HRESULT StringVectorToSafeArray(CONST vector<CString> & strs, SAFEARRAY ** ppSa);
HRESULT StringVectorToVar(CONST vector<CString> & strs, CComVariant & var);

template<class T>
HRESULT IntegerVectorToSafeArray(CONST vector<T> & VecInt, SAFEARRAY ** ppSa)
{
	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CComSafeArray<T> sa;
	hr = sa.Create(VecInt.size());
	if (FAILED(hr))
	{
		break;
	}

	//////////////////////////////////////////////////////////////////////////
	for (LONG ii = 0; ii < VecInt.size(); ++ ii)
	{
		hr = sa.SetAt(ii, VecInt[ii]);
		if (FAILED(hr))
		{
			break;
		}
	}
	if (FAILED(hr))
	{
		break;
	}

	//////////////////////////////////////////////////////////////////////////
	*ppSa = sa.Detach();

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

template <class T>
HRESULT IntegerVectorToVar(CONST vector<T> & VecInt, CComVariant & var)
{
	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	SAFEARRAY * psa = NULL;
	hr = IntegerVectorToSafeArray<T>(VecInt, &psa);
	if (FAILED(hr))
	{
		break;
	}

	//////////////////////////////////////////////////////////////////////////
	var = psa;

	//////////////////////////////////////////////////////////////////////////
	CComSafeArray<T> ssa; //destroy psa
	ssa.Attach(psa);
	END_BLOCK(0);

	return hr;
}