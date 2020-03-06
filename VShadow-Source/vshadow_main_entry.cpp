#include "stdafx.h"
#include "shadow.h"

#include "..\PublicHeader\PublicDTraceLib.h"

extern "C" int __cdecl wmain(__in int argc, __in_ecount(argc) WCHAR ** argv)
{
	D_SETTINGS(999, FALSE, nullptr, nullptr);
	return vshadow_wmain(argc, argv);
}