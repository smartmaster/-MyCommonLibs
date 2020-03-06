// AutoInputKey.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "AutoInputKey.h"


int _tmain(int argc, TCHAR ** argv)
{
	int testCase = 0;
	switch (testCase)
	{
	case 0:
	{
		if (argc < 3)
		{
			SmartLib::CPrint::Print(L"Usage: this.exe className WindowName");
			break;
		}
		SmartLib::CAutoKeyInput::WorkerProc(
			argv[1], 
			argv[2], 
			argc > 3? argv[3] : nullptr,
			argc > 4? argv[4] : nullptr
			);
	}
	default:
		break;
	}
    return 0;
}

