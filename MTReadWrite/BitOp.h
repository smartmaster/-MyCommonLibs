#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>

//////////////////////////////////////////////////////////////////////////
CONST ULONGLONG ZERO_LL = (0LL);
CONST ULONGLONG ONE_LL = (1LL);
CONST ULONGLONG U_NEGTIVE_ONE_LL = ((ULONGLONG)(-1LL));

//////////////////////////////////////////////////////////////////////////
#define BIT_COUNT_OF(Var)													(sizeof(Var) << 3)

//////////////////////////////////////////////////////////////////////////
#define SET_BIT(Var, BitIndex)												((Var) |= (ONE_LL << (BitIndex)))
#define SET_BIT_CONST(Var, BitIndex)										((Var) | (ONE_LL << (BitIndex)))
//////////////////////////////////////////////////////////////////////////
#define CLEAR_BIT(Var, BitIndex)											((Var) &= (~(ONE_LL << (BitIndex))))
#define CLEAR_BIT_CONST(Var, BitIndex)										((Var) & (~(ONE_LL << (BitIndex))))
//////////////////////////////////////////////////////////////////////////
#define TEST_BIT(Var, BitIndex)												((Var) & (ONE_LL << (BitIndex)))

//////////////////////////////////////////////////////////////////////////
#define SET_LOWER_BITS(Var, BitCount)										((BitCount)? ((Var) |= (U_NEGTIVE_ONE_LL >> (BIT_COUNT_OF(U_NEGTIVE_ONE_LL) - (BitCount)))) : (Var))
#define SET_LOWER_BITS_CONST(Var, BitCount)									((BitCount)? ((Var) | (U_NEGTIVE_ONE_LL >> (BIT_COUNT_OF(U_NEGTIVE_ONE_LL) - (BitCount)))) : (Var))
//////////////////////////////////////////////////////////////////////////
#define CLEAR_LOWER_BITS(Var, BitCount)										(((BitCount) < BIT_COUNT_OF(U_NEGTIVE_ONE_LL))? ((Var) &= (U_NEGTIVE_ONE_LL << (BitCount))) : ((Var) = ZERO_LL))
#define CLEAR_LOWER_BITS_CONST(Var, BitCount)								(((BitCount) < BIT_COUNT_OF(U_NEGTIVE_ONE_LL))? ((Var) & (U_NEGTIVE_ONE_LL << (BitCount))) : (ZERO_LL))
//////////////////////////////////////////////////////////////////////////
#define GET_LOWER_BITS(Var, BitCount)										(((BitCount) < BIT_COUNT_OF(U_NEGTIVE_ONE_LL))? ((Var) & (U_NEGTIVE_ONE_LL >> (BIT_COUNT_OF(U_NEGTIVE_ONE_LL) - (BitCount)))) : (Var))

//////////////////////////////////////////////////////////////////////////
#define SET_HIGHER_BITS(Var, BitCount)										((BitCount)? ((Var) |= (U_NEGTIVE_ONE_LL << (BIT_COUNT_OF(Var) - (BitCount)))) : (Var))
#define SET_HIGHER_BITS_CONST(Var, BitCount)								((BitCount)? ((Var) | (U_NEGTIVE_ONE_LL << (BIT_COUNT_OF(Var) - (BitCount)))) : (Var))
//////////////////////////////////////////////////////////////////////////
#define CLEAR_HIGHER_BITS(Var, BitCount)									(((BitCount) < BIT_COUNT_OF(U_NEGTIVE_ONE_LL))? ((Var) &= (U_NEGTIVE_ONE_LL >> (BIT_COUNT_OF(U_NEGTIVE_ONE_LL) - BIT_COUNT_OF(Var) + (BitCount)))) : ((Var) = ZERO_LL))
#define CLEAR_HIGHER_BITS_CONST(Var, BitCount)								(((BitCount) < BIT_COUNT_OF(U_NEGTIVE_ONE_LL))? ((Var) & (U_NEGTIVE_ONE_LL >> (BIT_COUNT_OF(U_NEGTIVE_ONE_LL) - BIT_COUNT_OF(Var) + (BitCount)))) : (ZERO_LL))
//////////////////////////////////////////////////////////////////////////
#define GET_HIGHER_BITS(Var, BitCount)										(((BitCount) < BIT_COUNT_OF(U_NEGTIVE_ONE_LL))? (((Var) & (U_NEGTIVE_ONE_LL << (BIT_COUNT_OF(Var) - (BitCount)))) >> (BIT_COUNT_OF(Var) - (BitCount))) : (Var))

//////////////////////////////////////////////////////////////////////////
#define SET_BITS_RANGE(Var, BitIndex, BitCount)								((Var) |= (CLEAR_HIGHER_BITS_CONST(U_NEGTIVE_ONE_LL, (BIT_COUNT_OF(U_NEGTIVE_ONE_LL) - ((BitIndex) + (BitCount)))) & CLEAR_LOWER_BITS_CONST(U_NEGTIVE_ONE_LL, (BitIndex))))
#define SET_BITS_RANGE_CONST(Var, BitIndex, BitCount)						((Var) | (CLEAR_HIGHER_BITS_CONST(U_NEGTIVE_ONE_LL, (BIT_COUNT_OF(U_NEGTIVE_ONE_LL) - ((BitIndex) + (BitCount)))) & CLEAR_LOWER_BITS_CONST(U_NEGTIVE_ONE_LL, (BitIndex))))
//////////////////////////////////////////////////////////////////////////
#define CLEAR_BITS_RANGE(Var, BitIndex, BitCount)							((Var) &= (SET_HIGHER_BITS_CONST(ZERO_LL, (BIT_COUNT_OF(ZERO_LL) - ((BitIndex) + (BitCount)))) | SET_LOWER_BITS_CONST(ZERO_LL, (BitIndex))))
#define CLEAR_BITS_RANGE_CONST(Var, BitIndex, BitCount)						((Var) & (SET_HIGHER_BITS_CONST(ZERO_LL, (BIT_COUNT_OF(ZERO_LL) - ((BitIndex) + (BitCount)))) | SET_LOWER_BITS_CONST(ZERO_LL, (BitIndex))))
//////////////////////////////////////////////////////////////////////////
#define ASSIGN_BITS_RANGE_VALUE(Var, BitIndex, BitCount, Value)				((Var) = (CLEAR_BITS_RANGE_CONST((Var), (BitIndex), (BitCount)) | (GET_LOWER_BITS((Value), (BitCount)) << (BitIndex))))
#define ASSIGN_BITS_RANGE_VALUE_CONST(Var, BitIndex, BitCount, Value)		(CLEAR_BITS_RANGE_CONST((Var), (BitIndex), (BitCount)) | (GET_LOWER_BITS((Value), (BitCount)) << (BitIndex)))
//////////////////////////////////////////////////////////////////////////
#define GET_BITS_RANGE(Var, BitIndex, BitCount)								(CLEAR_HIGHER_BITS_CONST(Var, (BIT_COUNT_OF(Var) - ((BitIndex) + (BitCount)))) >> (BitIndex))

//////////////////////////////////////////////////////////////////////////
#define SET_ALL_BITS(Var)													((Var) = U_NEGTIVE_ONE_LL)
#define CLEAR_ALL_BITS(Var)													((Var) = ZERO_LL)
#define TEST_ALL_BITS_SET(Var)												((Var) == U_NEGTIVE_ONE_LL)
#define TEST_ALL_BITS_CLEARED(Var)											((Var) == ZERO_LL)


