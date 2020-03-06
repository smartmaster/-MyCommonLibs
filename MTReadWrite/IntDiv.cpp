#include "stdafx.h"

#include "IntDiv.h"

static  CONST LONGLONG _pow10[10] =
{
	1LL,
	10LL,
	100LL,
	1000LL,
	10000LL,
	100000LL,
	1000000LL,
	10000000LL,
	100000000LL,
	1000000000LL,
};


static CONST LPCTSTR _format[10] =
{
	TEXT("%I64d"),
	TEXT("%I64d.%I64d"),
	TEXT("%I64d.%02I64d"),
	TEXT("%I64d.%03I64d"),
	TEXT("%I64d.%04I64d"),
	TEXT("%I64d.%05I64d"),
	TEXT("%I64d.%06I64d"),
	TEXT("%I64d.%07I64d"),
	TEXT("%I64d.%08I64d"),
	TEXT(""),
};

VOID IntDiv::Div(LONGLONG x, LONGLONG y, LONG p, LONGLONG & n, LONGLONG & d)
{
	ATLASSERT(p >= 1 && p <= _countof(_pow10) - 2); //p >= 1 && p <= 8
	n = (_pow10[p + 1] * x / y + 5 ) / 10;
	d = n % _pow10[p];
	n /= _pow10[p];
}


VOID IntDiv::Div(LONGLONG x, LONGLONG y, LONG p, CString & str)
{
	ATLASSERT(p >= 1 && p <= _countof(_pow10) - 2); //p >= 1 && p <= 8

	LONGLONG n = 0;
	LONGLONG d = 0;
	Div(x, y, p, n, d);
	str.Format(_format[p], n, d);
}

INT IntDiv::TestCases()
{
	CString str;
	IntDiv::Div(10002, 8569, 8, str);
	IntDiv::Div(10002, 8569, 7, str);
	IntDiv::Div(10002, 8569, 6, str);
	IntDiv::Div(10002, 8569, 5, str);
	IntDiv::Div(10002, 8569, 4, str);
	IntDiv::Div(10002, 8569, 3, str);
	IntDiv::Div(10002, 8569, 2, str);
	IntDiv::Div(10002, 8569, 1, str);
	IntDiv::Div(10002, 8569, 0, str);

	IntDiv::Div(77777, 8569, 3, str);
	IntDiv::Div(33333, 8569, 3, str);
	IntDiv::Div(22222, 8569, 3, str);


	IntDiv::Div(999, 1000, 2, str);
	IntDiv::Div(555, 1000, 2, str);
	IntDiv::Div(333, 1000, 2, str);


	IntDiv::Div(999, 100, 1, str);
	IntDiv::Div(555, 100, 1, str);
	IntDiv::Div(333, 100, 1, str);


	IntDiv::Div(999, 100, 0, str);
	IntDiv::Div(555, 100, 0, str);
	IntDiv::Div(333, 100, 0, str);
	return 0;
}

