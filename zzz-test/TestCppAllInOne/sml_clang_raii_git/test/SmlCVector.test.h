#pragma once

#include "../src/SmlCVector.h"
#include "log.h"

namespace SmartLib
{
	class SmlvecTest
	{
	public:
		static void Case0()
		{
			SmlCVector vec;
			SmlCVector_Ctor(&vec);

			for (int ii = 0; ii < 80; ++ii)
			{
				SmlCVector_Push(&vec, (SML_DATA_PTR)(ii));
			}

			while (SmlCVector_Size(&vec))
			{
				SML_DATA_PTR data = SmlCVector_Pop(&vec);
				SML_LOG_LINE(TEXT("%p"), data);
			}

			SmlCVector_Dtor(&vec);
		}
	};
}