#include <iostream>
#include "feature_def.h"

int main_cxx(int argc, char** argv)
{
	using namespace ::std;
	for (long ii = 0; ii < argc; ++ii)
	{
		cout << argv[ii] << endl;
	}
	return 0;
}