// aaa-io-client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "..\aaa-io-server\MyParamsAuto.h"
#include "..\aaa-io-server\aaaioserver_i.h"

int _tmain(int argc, _TCHAR* argv[])
{
	ComInitAuto comInitAuto(COINIT_MULTITHREADED);

	CComPtr<IMyObj> spIMyObj;
	HRESULT hr = spIMyObj.CoCreateInstance(__uuidof(MyObj), nullptr, CLSCTX_ALL);


	{
		MyParamsAuto in, out;
		in.Code = 0;
		spIMyObj->IO(&in, &out);
	}

	{
		MyParamsAuto in, out;
		in.Code = 1;
		in.Set(TEXT("this is from client"));
		BYTE buffer[] = "this is from client";
		in.Set(buffer, sizeof(buffer));
		spIMyObj->IO(&in, &out);
	}

	{
		MyParamsAuto in, out;
		in.Code = 2;
		in.Set(TEXT("this is from client"));
		BYTE buffer[] = "this is from client";
		in.Set(buffer, sizeof(buffer));
		spIMyObj->IO(&in, &out);
	}
		
	return 0;
}

