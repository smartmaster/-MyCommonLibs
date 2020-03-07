
#include <stdio.h>
#include <string.h>

extern void CLangRaii();
extern void CLangRaii2();
extern void CLangRaii3();

int main(int argc, char** argv)
{
	long testCase = -1;
	if (argc > 1)
	{
		testCase = strtol(argv[1], NULL, 10);
	}

	switch (testCase)
	{
	case 3:
	{
		CLangRaii4();
		CLangRaii4();
	}
	break;
	case 2:
	{
		CLangRaii3();
		CLangRaii3();
	}
	break;
	case 1:
	{
		CLangRaii2();
		CLangRaii2();
	}
	break;
	case 0:
	{
		CLangRaii();
		CLangRaii();
	}
	break;
	}


	return 0;
}