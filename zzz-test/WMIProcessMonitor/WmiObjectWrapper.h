#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <Wbemidl.h>

#include <string>
#include <vector>
using namespace std;


class CWmiObjectWrapper
{
public:
	CWmiObjectWrapper(IWbemClassObject* pWbemClassObject);
	~CWmiObjectWrapper();

public:

	operator IWbemClassObject* () const;


	CComBSTR GetPath() const;

	DWORD	GetProperty(const wstring& strName, VARIANT& var) const;

	wstring	GetStringProperty(const wstring& strName) const;

	long	GetLongProperty(const wstring& strName, long lDefaultValue = -1)	const;
	bool	GetBoolProperty(const wstring& strName, bool bDefault = false)	const;
	BYTE	GetByteProperty(const wstring& strName, BYTE bDefault = 0xFF) const;

	vector<wstring> GetMultiValueProperty(const wstring& strName) const;
	vector<unsigned short> GetMultiIntValueProperty(const wstring& strName) const;
    vector<long> GetMultiLongValueProperty(const wstring& strName) const; //<huvfe01>2012-12-7 #VM operational status

private:
	IWbemClassObject*		m_pWbemClassObject;
	CWmiObjectWrapper();
};