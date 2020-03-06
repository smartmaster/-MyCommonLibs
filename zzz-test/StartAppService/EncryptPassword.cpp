#include "stdafx.h"
#include "EncryptString.h"

static INT MyHash(INT ii)
{
	return ((ii+13)%11 + (ii+5)*(ii+23)%17);
}

LONG GetEncPassword(OUT CString & StrPassword)
{
	TCHAR ThisIsMe[] = TEXT("ILIKEMATHButmuchtostudyMATHEMAticsIthoughTAFTERGRADUATINGfromcollegeaNDLESSWORKINVolvedinmatheMATICS");
	StrPassword = ThisIsMe;
	SecureZeroMemory((LPVOID)ThisIsMe, sizeof(ThisIsMe));
	StrPassword.MakeReverse();
	LPTSTR pBuffer = StrPassword.GetBuffer();
	for (INT ii = 0; ii < StrPassword.GetLength(); ++ii)
	{
		pBuffer[ii] +=  MyHash(ii);
	}

	return 0;
}