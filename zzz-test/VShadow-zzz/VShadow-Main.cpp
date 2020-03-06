// VShadow.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "..\PublicHeader\PublicDTraceLib.h"


int VShadowMain(__in int argc, __in_ecount(argc) WCHAR ** argv);

int _tmain(int argc, _TCHAR* argv[])
{
	D_SETTINGS(99999, FALSE, TEXT("vshadow-"), NULL);
	return VShadowMain(argc, argv);
}

