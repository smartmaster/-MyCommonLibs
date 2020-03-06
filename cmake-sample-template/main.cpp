#include <iostream>
#include "feature_def.h"

#ifdef _WINDLL
#define  MAIN_START main_cpp
#else
#define  MAIN_START main
#endif

int MAIN_START(int argc, char** argv)
{
	using namespace ::std;
	for (long ii = 0; ii < argc; ++ii)
	{
		cout << argv[ii] << endl;
	}
	return 0;
}