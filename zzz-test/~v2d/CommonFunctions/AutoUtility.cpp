#include "stdafx.h"

#include "CodeBlockDef.h"
#include "AutoUtility.h"

HRESULT StringVectorToSafeArray( CONST vector<CString> & strs, SAFEARRAY ** ppSa )
{
	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CComSafeArray<BSTR> sa;
	hr = sa.Create(strs.size());
	if (FAILED(hr))
	{
		break;
	}

	//////////////////////////////////////////////////////////////////////////
	for (LONG ii = 0; ii < strs.size(); ++ ii)
	{
		hr = sa.SetAt(ii, strs[ii].AllocSysString(), FALSE);
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

HRESULT StringVectorToVar(CONST vector<CString> & strs, CComVariant & var)
{
	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	SAFEARRAY * psa = NULL;
	hr = StringVectorToSafeArray(strs, &psa);
	if (FAILED(hr))
	{
		break;
	}

	//////////////////////////////////////////////////////////////////////////
	var = psa;

	//////////////////////////////////////////////////////////////////////////
	CComSafeArray<BSTR> ssa; //destroy psa
	ssa.Attach(psa);
	END_BLOCK(0);

	return hr;
}
