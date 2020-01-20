#pragma once

#include "stdafx.h"

#include <Wbemidl.h>

#include <vector>
using namespace std;

HRESULT PrepareLocatorAndService(IWbemLocator ** ppLocator,
								 IWbemServices ** ppService);

HRESULT ExecQuery(IWbemServices *pService, 
				  LPCTSTR pQuery, 
				  vector<IWbemClassObject*> & VecObj);


HRESULT GetObjProp(IWbemClassObject * pclsObj, 
				   LPCTSTR pName, 
				   CComVariant & VarProp);


HRESULT ExecMethod(IWbemServices * pService, 
				   IWbemClassObject * pclsObj, 				
				   LPCTSTR pClassName,
				   LPCTSTR pMethodName,
				   vector<CString> & ParamNames,
				   vector<CComVariant> & ParamValues,
				   CComVariant & ReturnValue);