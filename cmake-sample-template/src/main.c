#include <stdio.h>

#include "feature_def.h"

int main_c(int argc, char** argv)
{
	for (long ii = 0; ii < argc; ++ ii)
	{
		fprintf(stdout, "%s" END_LINE, argv[ii]);
	}
	return 0;
}