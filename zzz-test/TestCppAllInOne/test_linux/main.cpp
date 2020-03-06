

#include "EnumDir.h"
#include "EnumDir.test.h"

int main(int argc, const char** argv)
{
	int testCase = -1;
	if (argc > 1)
	{
		testCase = strtol(argv[1], nullptr, 10);
	}

	switch (testCase)
	{
	case 1:
	{
		SmartLib::CEnumDirTest::Case0();
	}
	break;
	case 0:
	{
		if (argc > 2)
		{
			SmartLib::CEnumDir::EnumDir(argv[2]);
		}
	}
	break;
	default:
	{

		std::cout << "Usage:" << std::endl;
		std::cout << "\tEnum Dir: this.exe 0 <dir name>" << std::endl;
	}
	break;
	}

	return 0;
}