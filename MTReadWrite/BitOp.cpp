#include "stdafx.h"

#include <assert.h>

#include "..\PublicHeader\PublicDTraceLib.h"

#include "BitOp.h"


int main_test_all(int argc, TCHAR * argv[])
{
#define VALUE_TYPE ULONGLONG

	VALUE_TYPE myvar = 0;

	//////////////////////////////////////////////////////////////////////////
//#define SET_BIT(Var, BitIndex)												((Var) |= (ONE_LL << (BitIndex)))
//#define SET_BIT_CONST(Var, BitIndex)										((Var) | (ONE_LL << (BitIndex)))
//	//////////////////////////////////////////////////////////////////////////
//#define CLEAR_BIT(Var, BitIndex)											((Var) &= (~(ONE_LL << (BitIndex))))
//#define CLEAR_BIT_CONST(Var, BitIndex)										((Var) & (~(ONE_LL << (BitIndex))))
//	//////////////////////////////////////////////////////////////////////////
//#define TEST_BIT(Var, BitIndex)												((Var) & (ONE_LL << (BitIndex)))

	CLEAR_ALL_BITS(myvar);
	for (LONG ii = 0; ii < BIT_COUNT_OF(myvar); ++ ii)
	{
		VALUE_TYPE temp = SET_BIT_CONST(myvar, ii);
		SET_BIT(myvar, ii);
		assert(temp == myvar);
		assert(TEST_BIT(myvar, ii));
	}

	for (LONG ii = 0; ii < BIT_COUNT_OF(myvar); ++ii)
	{
		VALUE_TYPE temp = CLEAR_BIT_CONST(myvar, ii);
		CLEAR_BIT(myvar, ii);
		assert(temp == myvar);
		assert(0 == TEST_BIT(myvar, ii));
	}


//	//////////////////////////////////////////////////////////////////////////
//#define SET_LOWER_BITS(Var, BitCount)										((BitCount)? ((Var) |= (U_NEGTIVE_ONE_LL >> (BIT_COUNT_OF(U_NEGTIVE_ONE_LL) - (BitCount)))) : (Var))
//#define SET_LOWER_BITS_CONST(Var, BitCount)									((BitCount)? ((Var) | (U_NEGTIVE_ONE_LL >> (BIT_COUNT_OF(U_NEGTIVE_ONE_LL) - (BitCount)))) : (Var))
//	//////////////////////////////////////////////////////////////////////////
//#define CLEAR_LOWER_BITS(Var, BitCount)										(((BitCount) < BIT_COUNT_OF(U_NEGTIVE_ONE_LL))? ((Var) &= (U_NEGTIVE_ONE_LL << (BitCount))) : ((Var) = ZERO_LL))
//#define CLEAR_LOWER_BITS_CONST(Var, BitCount)								(((BitCount) < BIT_COUNT_OF(U_NEGTIVE_ONE_LL))? ((Var) & (U_NEGTIVE_ONE_LL << (BitCount))) : (ZERO_LL))
//	//////////////////////////////////////////////////////////////////////////
//#define GET_LOWER_BITS(Var, BitCount)										(((BitCount) < BIT_COUNT_OF(U_NEGTIVE_ONE_LL))? ((Var) & (U_NEGTIVE_ONE_LL >> (BIT_COUNT_OF(U_NEGTIVE_ONE_LL) - (BitCount)))) : (Var))
	CLEAR_ALL_BITS(myvar);
	VALUE_TYPE rrr = 0;
	for (LONG ii = 0; ii < BIT_COUNT_OF(myvar); ++ii)
	{
		VALUE_TYPE temp = SET_LOWER_BITS_CONST(myvar, ii + 1);
		SET_LOWER_BITS(myvar, ii + 1);
		assert(temp == myvar);
		assert(TEST_BIT(myvar, ii));
		rrr = GET_LOWER_BITS(myvar, ii + 1);
	}


	for (LONG ii = 0; ii < BIT_COUNT_OF(myvar); ++ii)
	{
		VALUE_TYPE temp = CLEAR_LOWER_BITS_CONST(myvar, ii + 1);
		CLEAR_LOWER_BITS(myvar, ii + 1);
		assert(temp == myvar);
		assert(!TEST_BIT(myvar, ii));
		rrr = GET_LOWER_BITS(myvar, ii + 1);
	}



//	//////////////////////////////////////////////////////////////////////////
//#define SET_HIGHER_BITS(Var, BitCount)										((Var) |= (U_NEGTIVE_ONE_LL << (BIT_COUNT_OF(Var) - (BitCount))))
//#define SET_HIGHER_BITS_CONST(Var, BitCount)								((Var) | (U_NEGTIVE_ONE_LL << (BIT_COUNT_OF(Var) - (BitCount))))
//	//////////////////////////////////////////////////////////////////////////
//#define CLEAR_HIGHER_BITS(Var, BitCount)									((Var) &= (U_NEGTIVE_ONE_LL >> (BIT_COUNT_OF(U_NEGTIVE_ONE_LL) - BIT_COUNT_OF(Var) + (BitCount))))	
//#define CLEAR_HIGHER_BITS_CONST(Var, BitCount)								((Var) & (U_NEGTIVE_ONE_LL >> (BIT_COUNT_OF(U_NEGTIVE_ONE_LL) - BIT_COUNT_OF(Var) + (BitCount))))
//	//////////////////////////////////////////////////////////////////////////
//#define GET_HIGHER_BITS(Var, BitCount)										(((Var) & (U_NEGTIVE_ONE_LL << (BIT_COUNT_OF(Var) - (BitCount)))) >> (BIT_COUNT_OF(Var) - (BitCount)))
	CLEAR_ALL_BITS(myvar);
	rrr = 0;
	for (LONG ii = 0; ii < BIT_COUNT_OF(myvar); ++ii)
	{
		VALUE_TYPE temp = SET_HIGHER_BITS_CONST(myvar, ii + 1);
		SET_HIGHER_BITS(myvar, ii + 1);
		assert(temp == myvar);
		assert(TEST_BIT(myvar, BIT_COUNT_OF(myvar) - ii - 1));
		rrr = GET_HIGHER_BITS(myvar, ii + 1);
	}


	for (LONG ii = 0; ii < BIT_COUNT_OF(myvar); ++ii)
	{
		VALUE_TYPE temp = CLEAR_HIGHER_BITS_CONST(myvar, ii + 1);
		CLEAR_HIGHER_BITS(myvar, ii + 1);
		assert(temp == myvar);
		assert(!TEST_BIT(myvar, BIT_COUNT_OF(myvar) - ii - 1));
		rrr = GET_HIGHER_BITS(myvar, ii + 1);
	}



	//////////////////////////////////////////////////////////////////////////
//#define SET_BITS_RANGE(Var, BitIndex, BitCount)								((Var) |= (CLEAR_HIGHER_BITS_CONST(U_NEGTIVE_ONE_LL, (BIT_COUNT_OF(U_NEGTIVE_ONE_LL) - ((BitIndex) + (BitCount)))) & CLEAR_LOWER_BITS_CONST(U_NEGTIVE_ONE_LL, (BitIndex))))
//#define SET_BITS_RANGE_CONST(Var, BitIndex, BitCount)						((Var) | (CLEAR_HIGHER_BITS_CONST(U_NEGTIVE_ONE_LL, (BIT_COUNT_OF(U_NEGTIVE_ONE_LL) - ((BitIndex) + (BitCount)))) & CLEAR_LOWER_BITS_CONST(U_NEGTIVE_ONE_LL, (BitIndex))))
//	//////////////////////////////////////////////////////////////////////////
//#define CLEAR_BITS_RANGE(Var, BitIndex, BitCount)							((Var) &= (SET_HIGHER_BITS_CONST(ZERO_LL, (BIT_COUNT_OF(ZERO_LL) - ((BitIndex) + (BitCount)))) | SET_LOWER_BITS_CONST(ZERO_LL, (BitIndex))))
//#define CLEAR_BITS_RANGE_CONST(Var, BitIndex, BitCount)						((Var) & (SET_HIGHER_BITS_CONST(ZERO_LL, (BIT_COUNT_OF(ZERO_LL) - ((BitIndex) + (BitCount)))) | SET_LOWER_BITS_CONST(ZERO_LL, (BitIndex))))
//	//////////////////////////////////////////////////////////////////////////
//#define ASSIGN_BITS_RANGE_VALUE(Var, BitIndex, BitCount, Value)				((Var) = (CLEAR_BITS_RANGE_CONST((Var), (BitIndex), (BitCount)) | (GET_LOWER_BITS((Value), (BitCount)) << (BitIndex))))
//#define ASSIGN_BITS_RANGE_VALUE_CONST(Var, BitIndex, BitCount, Value)		(CLEAR_BITS_RANGE_CONST((Var), (BitIndex), (BitCount)) | (GET_LOWER_BITS((Value), (BitCount)) << (BitIndex)))
//	//////////////////////////////////////////////////////////////////////////
//#define GET_BITS_RANGE(Var, BitIndex, BitCount)								(CLEAR_HIGHER_BITS_CONST(Var, (BIT_COUNT_OF(Var) - ((BitIndex) + (BitCount)))) >> (BitIndex))
	CLEAR_ALL_BITS(myvar);
	rrr = 0;
	for (LONG ii = 0; ii < BIT_COUNT_OF(myvar); ++ii)
	{
		VALUE_TYPE temp = SET_BITS_RANGE_CONST(myvar, ii, 1);
		SET_BITS_RANGE(myvar, ii, 1);
		assert(temp == myvar);
		rrr = GET_BITS_RANGE(myvar, ii, 1);
		assert(1 == rrr);
	}

	for (LONG ii = 0; ii < BIT_COUNT_OF(myvar); ++ii)
	{
		VALUE_TYPE temp = CLEAR_BITS_RANGE_CONST(myvar, ii, 1);
		CLEAR_BITS_RANGE(myvar, ii, 1);
		assert(temp == myvar);
		rrr = GET_BITS_RANGE(myvar, ii, 1);
		assert(0 == rrr);
	}

	//////////////////////////////////////////////////////////////////////////
	CLEAR_ALL_BITS(myvar);
	rrr = 0;
	for (LONG ii = 0; ii < BIT_COUNT_OF(myvar); ++ii)
	{
		VALUE_TYPE temp = ASSIGN_BITS_RANGE_VALUE_CONST(myvar, ii, 1, 1);
		ASSIGN_BITS_RANGE_VALUE(myvar, ii, 1, 1);
		assert(temp == myvar);
		rrr = GET_BITS_RANGE(myvar, ii, 1);
		assert(1 == rrr);
	}

	SET_ALL_BITS(myvar);
	rrr = 0;
	for (LONG ii = 0; ii < BIT_COUNT_OF(myvar); ++ii)
	{
		VALUE_TYPE temp = ASSIGN_BITS_RANGE_VALUE_CONST(myvar, ii, 1, 0);
		ASSIGN_BITS_RANGE_VALUE(myvar, ii, 1, 0);
		assert(temp == myvar);
		rrr = GET_BITS_RANGE(myvar, ii, 1);
		assert(0 == rrr);
	}

	//////////////////////////////////////////////////////////////////////////
	const LONG BIT_COUNT = 3;
	const BYTE chPattern = 0x5;
	LONG Index = 0;
	for (LONG ii = 0; ii < BIT_COUNT_OF(myvar) / BIT_COUNT; ++ii)
	{
		ASSIGN_BITS_RANGE_VALUE(myvar, Index, BIT_COUNT, chPattern);
		rrr = GET_BITS_RANGE(myvar, Index, BIT_COUNT);
		ATLASSERT(chPattern == rrr);
		Index += BIT_COUNT;
	}

	Index = 0;
	for (LONG ii = 0; ii < BIT_COUNT_OF(myvar) / BIT_COUNT; ++ii)
	{
		rrr = GET_BITS_RANGE(myvar, Index, BIT_COUNT);
		ATLASSERT(chPattern == rrr);
		Index += BIT_COUNT;
	}

	const LONG BIT_COUNT_2 = 17;
	const LONG chPattern2 = 0xffffaa55;
	Index = 0;
	const VALUE_TYPE datapattern = GET_LOWER_BITS(chPattern2, BIT_COUNT_2);
	for (LONG ii = 0; ii < BIT_COUNT_OF(myvar) / BIT_COUNT_2; ++ii)
	{
		ASSIGN_BITS_RANGE_VALUE(myvar, Index, BIT_COUNT_2, chPattern2);
		rrr = GET_BITS_RANGE(myvar, Index, BIT_COUNT_2);
		ATLASSERT(datapattern == rrr);
		Index += BIT_COUNT_2;
	}

	Index = 0;
	for (LONG ii = 0; ii < BIT_COUNT_OF(myvar) / BIT_COUNT_2; ++ii)
	{
		rrr = GET_BITS_RANGE(myvar, Index, BIT_COUNT_2);
		ATLASSERT(datapattern == rrr);
		Index += BIT_COUNT_2;
	}

	return 0;
}


int main_test_bit(int argc, TCHAR ** argv)
{
	ULONGLONG MyVar = 0;

	SET_ALL_BITS(MyVar);
	CLEAR_ALL_BITS(MyVar);
	SET_ALL_BITS(MyVar);
	CLEAR_ALL_BITS(MyVar);

	D_INFO(0, TEXT("set all bits..."));
	for (int ii = 0; ii < BIT_COUNT_OF(MyVar); ++ ii )
	{
		SET_BIT(MyVar, ii);
		if (TEST_BIT(MyVar, ii))
		{
			D_INFO(0, TEXT("OK %d"), ii);
		}
		else
		{
			D_INFO(0, TEXT("Error %d"), ii);
		}
	}

	if (TEST_ALL_BITS_SET(MyVar))
	{
		D_INFO(0, TEXT("OK %d"), MyVar);
	}
	else
	{
		D_INFO(0, TEXT("Error %d"), MyVar);
	}


	D_INFO(0, TEXT("clear all bits..."));
	for (int ii = 0; ii < BIT_COUNT_OF(MyVar); ++ ii)
	{
		CLEAR_BIT(MyVar, ii);
		if (TEST_BIT(MyVar, ii))
		{
			D_INFO(0, TEXT("Error %d"), ii);
		}
		else
		{
			D_INFO(0, TEXT("OK %d"), ii);
		}
	}

	if (TEST_ALL_BITS_CLEARED(MyVar))
	{
		D_INFO(0, TEXT("OK %d"), MyVar);
	}
	else
	{
		D_INFO(0, TEXT("Error %d"), MyVar);
	}


	D_INFO(0, TEXT("set all bits..."));
	for (int ii = 0; ii < BIT_COUNT_OF(MyVar); ++ii)
	{
		SET_BIT(MyVar, ii);
		if (TEST_BIT(MyVar, ii))
		{
			D_INFO(0, TEXT("OK %d"), ii);
		}
		else
		{
			D_INFO(0, TEXT("Error %d"), ii);
		}
	}

	if (TEST_ALL_BITS_SET(MyVar))
	{
		D_INFO(0, TEXT("OK %d"), MyVar);
	}
	else
	{
		D_INFO(0, TEXT("Error %d"), MyVar);
	}


	D_INFO(0, TEXT("clear all bits..."));
	for (int ii = 0; ii < BIT_COUNT_OF(MyVar); ++ii)
	{
		CLEAR_BIT(MyVar, ii);
		if (TEST_BIT(MyVar, ii))
		{
			D_INFO(0, TEXT("Error %d"), ii);
		}
		else
		{
			D_INFO(0, TEXT("OK %d"), ii);
		}
	}

	if (TEST_ALL_BITS_CLEARED(MyVar))
	{
		D_INFO(0, TEXT("OK %d"), MyVar);
	}
	else
	{
		D_INFO(0, TEXT("Error %d"), MyVar);
	}

	long bit_count_of_ll = BIT_COUNT_OF(U_NEGTIVE_ONE_LL);

	MyVar = -1;
	CLEAR_LOWER_BITS(MyVar, 3);
	BYTE temp = GET_LOWER_BITS(MyVar, 3);
	SET_LOWER_BITS(MyVar, 3);
	temp = GET_LOWER_BITS(MyVar, 3);


	MyVar = 0;
	SET_HIGHER_BITS(MyVar, 3);
	temp = GET_HIGHER_BITS(MyVar, 3);
	CLEAR_HIGHER_BITS(MyVar, 3);
	temp = GET_HIGHER_BITS(MyVar, 3);

	return 0;
}



int main_test_ut(int argc, TCHAR * argv[])
{
#define VALUE_TYPE SHORT

	CONST LONG BIT_INDEX = 2;
	CONST LONG BIT_COUNT = 3;

	CONST VALUE_TYPE myvar_const = 0;
	VALUE_TYPE myvar = myvar_const;

	SET_BITS_RANGE(myvar, BIT_INDEX, BIT_COUNT);
	VALUE_TYPE xxx = SET_BITS_RANGE_CONST(myvar_const, BIT_INDEX, BIT_COUNT);
	VALUE_TYPE gValue = GET_BITS_RANGE(myvar, BIT_INDEX, BIT_COUNT);

	CONST VALUE_TYPE myvar_const_1 = myvar;
	CLEAR_BITS_RANGE(myvar, BIT_INDEX, BIT_COUNT);
	xxx = CLEAR_BITS_RANGE_CONST(myvar_const_1, BIT_INDEX, BIT_COUNT);

	LONG val = 0xffabcdef;
	ULONGLONG ll_var = 0;
	ASSIGN_BITS_RANGE_VALUE(ll_var, 7, 12, val);
	ULONGLONG ll_r = GET_BITS_RANGE(ll_var, 7, 12);
	CLEAR_BITS_RANGE(ll_var, 7, 12);


	ULONGLONG ll_r2 = ASSIGN_BITS_RANGE_VALUE_CONST(0LL, 7, 12, val);
	ll_r = GET_BITS_RANGE(ll_r2, 7, 12);
	CLEAR_BITS_RANGE(ll_r2, 7, 12);

	//////////////////////////////////////////////////////////////////////////
	VALUE_TYPE myvarvar = 0;
	for (LONG ii = 0; ii < BIT_COUNT_OF(myvarvar); ++ii)
	{
		ASSIGN_BITS_RANGE_VALUE(myvarvar, ii, 1, 1);
	}

	for (LONG ii = 0; ii < BIT_COUNT_OF(myvarvar); ++ii)
	{
		ASSIGN_BITS_RANGE_VALUE(myvarvar, ii, 1, 0);
	}

	SET_ALL_BITS(myvarvar);
	long inedex = 0;
	ASSIGN_BITS_RANGE_VALUE(myvarvar, inedex, 1, 0);
	ASSIGN_BITS_RANGE_VALUE(myvarvar, 1, 1, 0);


	const long count = 0;
	USHORT xxxx = SET_LOWER_BITS_CONST(0, count);

	ULONGLONG ull = -1;
	ULONGLONG xx = (ull >> 64);
	xx = (ull << 64);

	//////////////////////////////////////////////////////////////////////////



	return 0;
}



int test_compiler()
{
	{
		USHORT var = -1;
		const long index = 0; //const long
		ASSIGN_BITS_RANGE_VALUE(var, index, 1, 0);
		assert(0xfffe == var);
	}


	{
	USHORT var = -1;
	/*const*/ long index = 0; //non-const long
	ASSIGN_BITS_RANGE_VALUE(var, index, 1, 0);
	assert(0xfffe == var);
}

	return 0;
}