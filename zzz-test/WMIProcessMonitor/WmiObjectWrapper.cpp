#include "stdafx.h"

#include "WmiObjectWrapper.h"



CWmiObjectWrapper::CWmiObjectWrapper(IWbemClassObject* pWbemClassObject) : 
m_pWbemClassObject(pWbemClassObject)
{
}



CWmiObjectWrapper::~CWmiObjectWrapper()
{
}

CWmiObjectWrapper::operator IWbemClassObject* () const
{ 
	return m_pWbemClassObject;
}

CComBSTR CWmiObjectWrapper::GetPath() const
{
	CComBSTR	path;
	CComVariant	var;
	
	if (GetProperty(L"__PATH", var) == S_OK )
		path = var.bstrVal;
	return path;
}

long CWmiObjectWrapper::GetLongProperty(const wstring& strName, long lDefaultValue) const
{
	long lValue = lDefaultValue;
	
	VARIANT	v;
	VariantInit(&v);

	if( GetProperty(strName, v) == S_OK )
	{
		if( v.vt == VT_I4 || v.vt == VT_UI4 ) //<sonmi01>2014-2-17 ###???
		{
			lValue = v.lVal;
		}
	}

	VariantClear(&v);
	
	return lValue;
}

bool CWmiObjectWrapper::GetBoolProperty(const wstring& strName, bool bDefault)	const
{
	bool bValue = bDefault;

	VARIANT	v;
	VariantInit(&v);

	if( GetProperty(strName, v) == S_OK )
	{
		if( v.vt == VT_BOOL )
		{
			if( !v.boolVal )
			{
				bValue = false;
			}
			else
			{
				bValue = true;
			}
		}
	}

	VariantClear(&v);
	return bValue;
}

BYTE CWmiObjectWrapper::GetByteProperty(const wstring& strName, BYTE bDefault) const
{
	BYTE bValue = bDefault;

	VARIANT	v;
	VariantInit(&v);

	if( GetProperty(strName, v) == S_OK )
	{
		if( v.vt == VT_UI1 )
		{
			bValue = v.bVal;
		}
	}

	VariantClear(&v);
	return bValue;
}

wstring CWmiObjectWrapper::GetStringProperty(const wstring& strName) const
{
	wstring strValue;
	
	VARIANT	v;
	VariantInit(&v);

	if( GetProperty(strName, v) == S_OK )
	{
		if( v.vt == VT_BSTR )
		{
			strValue = v.bstrVal;
		}
	}

	VariantClear(&v);
	
	return strValue;
}

vector<wstring> CWmiObjectWrapper::GetMultiValueProperty(const wstring& strName) const
{
	vector<wstring> vProperty;

	VARIANT	v;
	VariantInit(&v);

	if( GetProperty(strName, v) == S_OK )
	if( v.vt == (VT_BSTR | VT_ARRAY) )
	{
		SAFEARRAYBOUND sab = v.parray->rgsabound[0];
		for (LONG i = sab.lLbound; i < (LONG)sab.cElements; ++i)
		{
			VARIANT Value;
			long Index[1];
			Index[0] = i;
			SafeArrayPtrOfIndex(v.parray, Index, &Value.byref);
			vProperty.push_back((V_BSTRREF(&Value))[0]);
		}
	}

	VariantClear(&v);
	return vProperty;
}

vector<unsigned short> CWmiObjectWrapper::GetMultiIntValueProperty(const wstring& strName) const
{
	vector<unsigned short> vProperty;

	VARIANT	v;
	VariantInit(&v);

	if( GetProperty(strName, v) == S_OK )
	if( v.vt == (VT_UI2 | VT_ARRAY) )
	{
		SAFEARRAYBOUND sab = v.parray->rgsabound[0];
		for (LONG i = sab.lLbound; i < (LONG)sab.cElements; ++i)
		{
			VARIANT Value;
			long Index[1];
			Index[0] = i;
			SafeArrayPtrOfIndex(v.parray, Index, &Value.byref);
			vProperty.push_back((V_I2REF(&Value))[0]);
		}
	}

	VariantClear(&v);
	return vProperty;
}

vector<long> CWmiObjectWrapper::GetMultiLongValueProperty(const wstring& strName) const //<huvfe01>2012-12-7 #VM operational status
{
    vector<long> vProperty;

    VARIANT	v;
    VariantInit(&v);

    if(GetProperty(strName, v) == S_OK)
    {
        if(v.vt == (VT_I4 | VT_ARRAY) || v.vt == (VT_UI4 | VT_ARRAY)) //<sonmi01>2014-2-17 ###???
        {
            SAFEARRAYBOUND sab = v.parray->rgsabound[0];
            for (LONG i = sab.lLbound; i < (LONG)sab.cElements; ++i)
            {
                VARIANT Value;
                long Index[1];
                Index[0] = i;
                SafeArrayPtrOfIndex(v.parray, Index, &Value.byref);
                vProperty.push_back((V_I4REF(&Value))[0]);
            }
        }
    }

    VariantClear(&v);

    return vProperty;        
}

DWORD CWmiObjectWrapper::GetProperty(const wstring& strName, VARIANT& var) const
{
	if (!m_pWbemClassObject)  return WBEM_E_INVALID_OBJECT;
	HRESULT hr = m_pWbemClassObject->Get(strName.c_str(), 0, &var, 0, 0);
	return hr;
}